#include "Extension.h"

thread server_thread;

BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		server_thread = thread(StartServer);
		server_thread.detach();
	}

	if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		CloseServer();
		server_thread.join();
	}

	return TRUE;
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
	if (sentenceInfo["current select"] && sentenceInfo["process id"] != 0)
	{
		json data;

		if (sentenceInfo["process id"])
		{
			data["process_path"] = ToString(ProcessIdToName((DWORD)sentenceInfo["process id"]));
		}
		data["sentence"] = ToString(sentence);

		BroadcastData(data.dump());
	}

	return false;
}
