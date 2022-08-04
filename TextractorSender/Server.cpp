#include "Server.h"

const int BACKPRESSURE_LIMIT = 1024;

uWS::App* app;
uWS::WebSocket<false, true, PerSocketData>* gws = nullptr;
uWS::Loop* loop;
queue<string> send_queue;

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
        .drain = [](auto* /*ws*/) {
            /* Check ws->getBufferedAmount() here */
            /* If it is below threshold then send the next message on the queue */
            if (gws->getBufferedAmount() <= BACKPRESSURE_LIMIT) {
                string data = send_queue.front();
                auto result = gws->send(data, uWS::OpCode::TEXT, false);

                // If failed, then put back onto queue to try again next
                if (result == 2) {
                    send_queue.push(data);
                }
                else {
                    send_queue.pop();
                }
            }
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
            if (gws->getBufferedAmount() <= BACKPRESSURE_LIMIT) {
                auto result = gws->send(data, uWS::OpCode::TEXT, false);

                // If failed, then put back onto queue to try again next
                if (result == 2) {
                    send_queue.push(data);
                }
            }
            else {
                send_queue.push(data);
            }
        });
    }
}
