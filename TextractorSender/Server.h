#pragma once

#include <time.h>
#include <iostream>
#include <uwebsockets/App.h>

using namespace std;
using namespace literals::chrono_literals;

struct PerSocketData {
    wstring process_path;
    wstring sentence;
};

void StartServer();
void CloseServer();

void BroadcastData(string data);
