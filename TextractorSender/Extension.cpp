#include "Extension.h"

BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		std::thread(StartServer).detach();
	}

	if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		CloseServerWrapper();
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
		PerSocketData data = {
			sentence,
			ProcessIdToName((DWORD)sentenceInfo["process id"])
		};

		BroadcastDataWrapper("broadcast", data);
	}

	return false;
}
