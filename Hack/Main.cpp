#include <cstdlib>

#include "Main.h"
#include "Windows.h"
#include "Util.h"
#include "CommandManager.h"
#include "TerraBot.h"
#include "CursorTail.h"
#include "ScreenText.h"

#include "JBridge.h"

bool Main::running = false;
bool Main::initialized = false;
const std::wstring Main::name = L"TerraBot";




















bool Main::Init() {
	tout << "Initializing" << tendl;

	if (!CommandManager::Init()) {
		tout << "Start up abolished: Failed to initialize Command Manager." << tendl;
		return false;
	}
	if (!ScreenTextManager::Init()) {
		tout << "Failed to initialize Screen Text Manager." << tendl;
	}
	if (!CursorTail::Init()) {
		tout << "Failed to initialize Curse Tail." << tendl;
	}
	if (!JBridge::Init()) {
		tout << "Failed to initialize Java Bridge." << tendl;
	}
	if (!TerraBot::Init()) {
		tout << "Failed to initialize TerraBot." << tendl;
	}

	CommandManager::registerCmd("exit", [](Command cmd)->void {
		Terminate();
	});
	CommandManager::registerCmd("test", [](Command cmd)->void {
		tout << "test cmd" << tendl;
	});

	initialized = true;
	tout << name <<" is ready." << tendl;

	return true;
}

void Main::MainLoop() {
	if (running || !initialized) return;

	running = true;
	while (running) {
		Sleep(10);
	}
}

void Main::Dispose() {
	tout << "Disposing, no more commands accepting." << tendl;
	//Hook::Dispose();
	CommandManager::Dispose();
	ScreenTextManager::Dispose();
	CursorTail::Dispose();

	JBridge::Dispose();

	tout << name << " detached from Terraria." << tendl;
}

void Main::Terminate() {
	running = false;
}