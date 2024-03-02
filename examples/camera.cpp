
#include <string>
#include <wsserver/server.h>
#include <wsvideo-ffmpeg/middlewares/callback.h>

#include <wsvideo-ffmpeg/subprocess.h>

WebSocketServer  server;
VideoBroadcaster boardcaster;

CallbackMiddleware libcamera;
FFMPEGMiddleware   ffmpeg;

void onCameraData (std::string data) {
    ffmpeg.send(data);
}

int main () {
    server.init(5420);
    boardcaster = VideoBroadcaster(&server, "flux.mp4");

    while (!server.listen()) continue ;

    libcamera = CallbackMiddleware( subprocess( "libcamera-vid", "libcamera-vid", "-t", "0", "-o", "-" ), onCameraData );

    ffmpeg.init(&broadcaster, 25);
    ffmpeg.getPipe()->useBlocking(false);

    while (true) {
        libcamera.tick();
        ffmpeg   .tick();

        broadcaster.tick();
    }

    return 0;
}
