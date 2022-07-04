#include "Server.h"

uWS::App* app;

void StartServer() {
    uWS::App* app = new uWS::App();

    app->ws<PerSocketData>("/*", {
        /* Settings */
        .compression = uWS::SHARED_COMPRESSOR,
        .maxPayloadLength = 16 * 1024 * 1024,
        .idleTimeout = 60,
        .maxBackpressure = 16 * 1024 * 1024,
        .closeOnBackpressureLimit = false,
        .resetIdleTimeoutOnSend = true,
        .sendPingsAutomatically = true
        }).listen(9001, [](auto* listen_socket) {
            if (listen_socket) {
                // MessageBoxW(NULL, L"Listening on port 9001", L"TextractorSender", MB_OK);
            }
            });

    app->run();
}

void CloseServer() {
    delete app;
    uWS::Loop::get() -> free();
}

void CloseServerWrapper() {
    uWS::Loop::get() -> defer(CloseServer);
}

void BroadcastData(std::string topic, PerSocketData data) {
    wstring json_string = data.JsonString();
    wstring_view data_view{ json_string };
    app->publish(topic, *((string_view*)(&data_view)), uWS::OpCode::TEXT, false);
}

void BroadcastDataWrapper(std::string topic, PerSocketData data) {
    uWS::Loop::get() -> defer([=] { BroadcastData(topic, data); });
}
