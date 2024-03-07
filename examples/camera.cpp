
#include <string>
#include <wsserver/server.h>
#include <wsvideo-ffmpeg/middlewares/callback.h>
#include <wsvideo-ffmpeg/middlewares/ffmpeg.h>
#include <wsvideo-ffmpeg/subprocess.h>

WebSocketServer server;

CallbackMiddleware libcamera;
FFMPEGMiddleware   ffmpeg;

void onCameraData (std::string data) {
    ffmpeg.send(data);
}

int main () {
    server.init(5420);
    VideoBroadcaster broadcaster = VideoBroadcaster(&server, "flux.mp4");

    while (!server.listen()) continue ;

    auto u = onCameraData;

    libcamera = CallbackMiddleware();
    libcamera.init( subprocess( "libcamera-vid", "libcamera-vid", "-t", "0", "-o", "-" ), u );

    ffmpeg.init(&broadcaster);
    ffmpeg.getPipe()->useBlocking(false);

    while (true) {
        libcamera.tick();
        ffmpeg   .tick();

        broadcaster.tick();
    }

    return 0;
}
