#pragma once

#include <time.h>
#include <iostream>
#include "Utilities.h"
#include "uwebsockets/App.h"

using namespace std;
using namespace std::literals::chrono_literals;

std::wstring const START_BRACKET = L"{";
std::wstring const END_BRACKET = L"}";

struct PerSocketData {
    std::wstring process_path;
    std::wstring sentence;

    std::wstring JsonString()
    {
        return START_BRACKET + L"process_path:" + process_path + L"," + L"sentence:" + sentence + END_BRACKET;
    }
};

void StartServer();
void CloseServer();

void BroadcastData(PerSocketData data);
