#include "ScreenText.h"
#include "DataManager.h"
#include "CommandManager.h"
#include "../CsBridge/TerraBridge.h"
#include "Util.h"



DWORD ScreenTextManager::DrawNetplayStatusTextAddress = 0;
Hook ScreenTextManager::screenTextHook;

void ScreenTextManager::OnScreenTextHook() {
	wstring s = L"Robot Running...\n";

	
	s += L"Use '<-' and '->' keys to restore\n";
	s += L"previouse messages!\n\n";
	

	
	TerraBridge::ScreenText(s, TerraBridge::Color(255, 0, 0), 50, 400, DrawNetplayStatusTextAddress);
}

bool ScreenTextManager::Init() {
	vector<int> sig = {
		0x55,
		0x8B, 0xEC,
		0x57,
		0x56,
		0x53,
		0x81, 0xEC, 0x84, 0x00, 0x00, 0x00,
		0x33, 0xC0,
		0x89, 0x45, 0xF0,
		0x89, 0x45, 0xE8,
		0x89, 0x45, 0xEC,
		0x89, 0x45, 0xE4,
		0x83, 0x3D, -1, -1, -1, -1, 0x01,
		0x74, 0x08,
		0x8D, 0x65, 0xF4,
		0x5B,
		0x5E,
		0x5F,
		0x5D,
		0xC3 };

	if (!(DrawNetplayStatusTextAddress = data::GetAddressFromSignature(sig))) return false;

	//tout << "DrawNetplayStatusTextAddress: " << hex_str(DrawNetplayStatusTextAddress) << tendl;

	screenTextHook = Hook((void*)(DrawNetplayStatusTextAddress), &(OnScreenTextHook), 14, false);
	CommandManager::registerCmd("st", [](Command cmd) -> void {
		if (!screenTextHook.isHooked())
			screenTextHook.hook();
		else screenTextHook.unhook();
	});


	return true;
}

void ScreenTextManager::Dispose() {
	screenTextHook.unhook();

}