
#include <string>
#include <wsserver/server.h>
#include <wsvideo-ffmpeg/middlewares/callback.h>
#include <wsvideo-ffmpeg/middlewares/ffmpeg.h>
#include <wsvideo-ffmpeg/subprocess.h>

WebSocketServer server;
VideoBroadcaster broadcaster(nullptr, "");

CallbackMiddleware libcamera;
FFMPEGMiddleware   ffmpeg;

void onCameraData (std::string data) {
    broadcaster.sendPacket(data);
}

int main () {
    server.init(5420);
    broadcaster = VideoBroadcaster(&server, "flux.mp4");

    while (!server.listen()) continue ;

    auto u = onCameraData;

    libcamera = CallbackMiddleware();
    libcamera.init( subprocess( "sh", "sh", "-c", "libcamera-vid -t 0 -o - | ffmpeg -re -i - -g 50 -movflags frag_keyframe+empty_moov+default_base_moof -f mp4 output.mp4" ), u );

    while (true) {
        libcamera.tick();

        broadcaster.tick();
    }

    return 0;
}
