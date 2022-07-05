#include "Utilities.h"

wstring ProcessIdToName(DWORD processId) {
	HANDLE Handle = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE,
		processId
	);
	wstring ret;

	if (Handle)
	{
		DWORD buffSize = MAX_PATH;
		WCHAR buffer[MAX_PATH];
		if (QueryFullProcessImageNameW(Handle, 0, buffer, &buffSize))
		{
			ret = buffer;
		}

		CloseHandle(Handle);
	}

	return ret;
}

// Sourced from https://stackoverflow.com/a/69410299
string ToString(const wstring& wide_string)
{
    if (wide_string.empty())
    {
        return "";
    }

    const auto size_needed = WideCharToMultiByte(CP_UTF8, 0, &wide_string.at(0), (int)wide_string.size(), nullptr, 0, nullptr, nullptr);
    if (size_needed <= 0)
    {
        throw std::runtime_error("WideCharToMultiByte() failed: " + std::to_string(size_needed));
    }

    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wide_string.at(0), (int)wide_string.size(), &result.at(0), size_needed, nullptr, nullptr);
    return result;
}