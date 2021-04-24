#pragma once
#include "Windows.h"
#include "Hook.h"


class ScreenTextManager {


	static DWORD DrawNetplayStatusTextAddress;
	static Hook screenTextHook;

	static void OnScreenTextHook();

public:
	static bool Init();
	static void Dispose();
	static bool add();
};