#pragma once
#include "Windows.h"


//class _dllmain {
//public:
//	static HMODULE csBridgeModule;
//};
DWORD __stdcall EjectThread(LPVOID lpParameter);
DWORD WINAPI run();
void ExitProgram();

