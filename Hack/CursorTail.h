#pragma once

#include "Util.h"
#include "Hook.h"
#include <vector>
#include "../CsBridge/TerraBridge.h"

struct CursorInfo {
	CursorInfo() {
		c = TerraBridge::Color(0, 0, 0);
	}

	int x, y;
	TerraBridge::Color c;
};

class CursorTail {
	CursorTail() {

	}




	static int hookAddress;
	static int maxCursors;

	static bool running;
	static Hook drawCursorHook;
	static Hook drawCursorHook2;
	static std::vector<CursorInfo> coords;

	static SkipOffset onDrawCursor();
	static SkipOffset onDrawSmartCursor();

public: 
	static bool Init();
	static void Toggle();
	static void Dispose();
};