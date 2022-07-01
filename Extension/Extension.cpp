#include "Extension.h"
#include <tchar.h>
#include <psapi.h>

using namespace std;

const wstring DELIMITER = L";;;";
const wstring MESSAGE_START_SEQUENCE = L"TextractorSender";

BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// MessageBoxW(NULL, L"Extension Loaded", L"Example", MB_OK);
		break;
	case DLL_PROCESS_DETACH:
		// MessageBoxW(NULL, L"Extension Removed", L"Example", MB_OK);
		break;
	}
	return TRUE;
}

std::wstring ProcessIdToName(DWORD processId) {
    HANDLE Handle = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
        FALSE,
        processId
    );
    std::wstring ret;

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

/*
	Param sentence: sentence received by Textractor (UTF-16). Can be modified, Textractor will receive this modification only if true is returned.
	Param sentenceInfo: contains miscellaneous info about the sentence (see README).
	Return value: whether the sentence was modified.
	Textractor will display the sentence after all extensions have had a chance to process and/or modify it.
	The sentence will be destroyed if it is empty or if you call Skip().
	This function may be run concurrently with itself: please make sure it's thread safe.
	It will not be run concurrently with DllMain.
*/
bool ProcessSentence(std::wstring& sentence, SentenceInfo sentenceInfo)
{
	if (sentenceInfo["current select"])
	{
		wstring newSentence = MESSAGE_START_SEQUENCE + DELIMITER
			+ ProcessIdToName((DWORD) sentenceInfo["process id"]) + DELIMITER
			+ sentence;

		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (newSentence.size() + 2) * sizeof(wchar_t));
		memcpy(GlobalLock(hMem), newSentence.c_str(), (newSentence.size() + 2) * sizeof(wchar_t));
		GlobalUnlock(hMem);
		OpenClipboard(0);
		EmptyClipboard();
		SetClipboardData(CF_UNICODETEXT, hMem);
		CloseClipboard();
	}
	return false;
}
