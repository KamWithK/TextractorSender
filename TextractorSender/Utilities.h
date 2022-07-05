#pragma once

#include <string>
#include <stdexcept>
#include <iostream>
#include <time.h>
#include <uwebsockets/App.h>

using namespace std;

wstring ProcessIdToName(DWORD processId);
string ToString(const wstring& wide_string);
