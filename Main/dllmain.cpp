#include "dllMain.h"
#include "Main.h"


HMODULE hackHModule;


DWORD __stdcall EjectThread(LPVOID lpParameter) {
	FreeLibraryAndExitThread(hackHModule, 0);
}

void ExitProgram() {
	CreateThread(0, 0, EjectThread, 0, 0, 0);
}

DWORD WINAPI run() {
	Main::Init();
	Main::MainLoop();
	Main::Dispose();
	ExitProgram();
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, 	DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			hackHModule = hModule;
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)run, NULL, 0, NULL);
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
		default:
			break;
	}
	return TRUE;
}

