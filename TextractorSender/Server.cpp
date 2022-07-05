#include "Server.h"

uWS::App* app;
uWS::WebSocket<false, true, PerSocketData>* gws = nullptr;
uWS::Loop* loop;

void StartServer() {
    uWS::App* app = new uWS::App();
    loop = uWS::Loop::get();

    app->ws<PerSocketData>("/*", {
        /* Settings */
        .compression = uWS::SHARED_COMPRESSOR,
        .maxPayloadLength = 16 * 1024 * 1024,
        .idleTimeout = 60,
        .maxBackpressure = 16 * 1024 * 1024,
        .closeOnBackpressureLimit = false,
        .resetIdleTimeoutOnSend = true,
        .sendPingsAutomatically = true,
        /* Handlers */
        .open = [](auto* ws) {
                    gws = ws;
        },
        .close = [](auto*/*ws*/, int /*code*/, std::string_view /*message*/) {
                    gws = nullptr;
        }
    }).listen(9001, [](auto* listen_socket) {
            if (listen_socket) {
                std::cout << "Listening on port " << 9001 << std::endl;
            }
    });;

    app->run();
}

void CloseServer() {
    loop -> defer([] {
        delete app;
        loop -> free();
    });
}

void BroadcastData(string data) {
    if (gws) {
        loop -> defer([data] {
            gws -> send(data, uWS::OpCode::TEXT, false);
        });
    }
}
