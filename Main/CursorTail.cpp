#include "CursorTail.h"
#include "DataManager.h"
#include "CommandManager.h"


SkipOffset CursorTail::onDrawCursor() {
	//tout << "triggered" << tendl;
	CursorInfo p;
	TerraBridge::GetCursorPos(p.x, p.y, p.c);
	coords.push_back(p);

	for (int i = 0; i < coords.size(); i++) {
		CursorInfo p = coords.at(i);
		TerraBridge::DrawCursor(p.x, p.y, p.c, false);
	}
	
	while (coords.size() > maxCursors)
		coords.erase(coords.begin());
	return 27;
}

SkipOffset CursorTail::onDrawSmartCursor() {
	//tout << "triggered" << tendl;
	CursorInfo p;
	TerraBridge::GetCursorPos(p.x, p.y, p.c);
	coords.push_back(p);

	for (int i = 0; i < coords.size(); i++) {
		CursorInfo p = coords.at(i);
		TerraBridge::DrawCursor(p.x, p.y, p.c, true);
	}

	while (coords.size() > maxCursors)
		coords.erase(coords.begin());
	return 33;
}


int CursorTail::hookAddress;
int CursorTail::maxCursors = 10;
bool CursorTail::running = false;
Hook CursorTail::drawCursorHook;
Hook CursorTail::drawCursorHook2;

std::vector<CursorInfo> CursorTail::coords;

bool CursorTail::Init() {
	/**
	vector <int> sig = {
		0xFF, 0x15,   -1,   -1,   -1,   -1,
		0x8D, 0x85, 0x40, 0xF7, 0xff, 0xff,
		0x83, 0xec, 0x08,
		0xf3, 0x0f, 0x7e, 0x00,
		0x66, 0x0f, 0xd6, 0x04, 0x24,
		0x33, 0xc9,
		0xff, 0x15, -1, -1, -1, -1,
		0xff, 0x15, -1, -1, -1, -1,
		0x8b, 0x0d, -1, -1, -1, -1,
		0x39, 0x09
	};/**/
	vector <int> sig = {
		0x55,
		0x8b, 0xec,
		0x57,
		0x56,
		0x53,
		0x81, 0xec, 0x4c, 0x01,   00,   00,
		0x8d, 0xbd, 0xe8, 0xfe, 0xff, 0xff,
		0xb9, 0x43,   00,   00,   00,
		0x33, 0xc0
	};
	hookAddress = data::GetAddressFromSignature(sig);

	//tout << "drawCursor address:" << hex_str(hookAddress) << tendl;

	drawCursorHook = Hook((void*)(hookAddress + 0x625), &(onDrawCursor), 27, true);
	drawCursorHook2 = Hook((void*)(hookAddress + 0x5f7), &(onDrawSmartCursor), 33, true);

	if (!hookAddress) {
		return false;
	}

	CommandManager::registerCmd("ct", [](Command cmd)->void {
		CursorTail::Toggle();
	});

	return true;
}

void CursorTail::Toggle(){
	if (running) {
		drawCursorHook.unhook();
		drawCursorHook2.unhook();
		coords.clear();
		tout << "Cursor tail off!" << tendl;
	}
	else {
		drawCursorHook.hook();
		drawCursorHook2.hook();
		tout << "Cursor tail on!" << tendl;
	}
	running = !running;
}

void CursorTail::Dispose() {
	if (running)
		Toggle();
}