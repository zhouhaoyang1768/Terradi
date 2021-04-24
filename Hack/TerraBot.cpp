#include "TerraBot.h"
#include "CommandManager.h"











bool TerraBot::Init() {


	CommandManager::registerCmd("pvp", [](Command cmd) -> void {
		Toggle();
	});
	return true;
}

bool TerraBot::Toggle() {


	return true;
}

void TerraBot::Dispose() {

}