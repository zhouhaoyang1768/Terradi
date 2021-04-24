#include "CommandManager.h"
#include "Util.h"
#include "../CsBridge/TerraBridge.h"












Command::Command(wstring cmd) {
	originalCmdStr = cmd;
	wchar_t space(L' ');
	if (originalCmdStr.find(space) != wstring::npos) {
		type = originalCmdStr.substr(0, originalCmdStr.find(space));

		argFullText = originalCmdStr.substr(originalCmdStr.find(space) + 1);

		bool hasQuotation = false;
		wstring arg(L"");
		for (int i = 0; i < argFullText.length(); i++) {
			if (argFullText[i] == L' ') {
				if (!hasQuotation || argFullText[i - 1] == L'\"') {
					if (arg != L"") args.push_back(arg);
					arg = L"";
					hasQuotation = false;
				}
				else arg += L' ';
			}
			else {
				if (argFullText[i] == L'\"' && (i == 0 || argFullText[i - 1] == L' '))
					hasQuotation = true;
				else if (argFullText[i] != L'\"' || (i < argFullText.length() - 1 && argFullText[i + 1] != L' '))
					arg += argFullText[i];
			}
		}
		if (arg != L"") args.push_back(arg);
		argsLen = args.size();
		/**/
	}
	else {
		type = originalCmdStr;
		args = {};
		argsLen = 0;
	}
}

SkipOffset CommandManager::PreProcessCmd() {
	Register_Info rInfo;
	if (!outMessageHook.getRegisterInfo(rInfo)) return 0;
	wstring chatMessage;
	if (!data::get((void*)rInfo.edx, chatMessage)) return 0;

	if (messagesTyped.size() == 0 || messagesTyped.at(messagesTyped.size() - 1) != chatMessage)
		messagesTyped.push_back(chatMessage);

	if (chatMessage[0] != CommandPrefix) {
		return 0;
	}

	chatMessage = chatMessage.substr(1);
	if (ProcessCommand(chatMessage)) {
		tout << L"Command Executed [" + chatMessage + L"]" << tendl;
	}
	else {
		tout << L"Unknown command entered [" + chatMessage + L"]" << tendl;
		tout << "Type [.help] to get a list of avaliable commands" << tendl;
	}
	
	return 0x38;
}



bool CommandManager::ProcessCommand(wstring& cmd) {
	Command command(cmd);

	if (commands.count(command.type)) {
		commandWaitList.push_back(command);
		return true;
	}

	return false;
}

void CommandManager::handleChatRestore() {
	if (!TerraBridge::IsDrawingPlayerChat()) {
		messageCount = messagesTyped.size();
		currentMessageTyping = L"";
	}
	if (TerraBridge::IsDrawingPlayerChat() && running && GetAsyncKeyState(VK_RIGHT)) {
		if (messageCount + 1 <= (int)messagesTyped.size())
			messageCount++;
		if (messageCount == messagesTyped.size()) {
			TerraBridge::SetChatAPIText(currentMessageTyping);
		}
		else {
			TerraBridge::SetChatAPIText(messagesTyped.at(messageCount));
		}
		while (TerraBridge::IsDrawingPlayerChat() && running && GetAsyncKeyState(VK_RIGHT)) {
			Sleep(2);
		}
	}
	if (TerraBridge::IsDrawingPlayerChat() && running && GetAsyncKeyState(VK_LEFT)) {
		if (messageCount == messagesTyped.size()) {
			TerraBridge::GetCurrentChapAPIText(currentMessageTyping);
		}
		if (messageCount - 1 >= 0)
			messageCount--;
		try {
			TerraBridge::SetChatAPIText(messagesTyped.at(messageCount));
		}
		catch (out_of_range e) {
			//print(L"out of range");
		}
		while (TerraBridge::IsDrawingPlayerChat() && running && GetAsyncKeyState(VK_LEFT)) {
			Sleep(2);
		}
	}
}

void CommandManager::CommandManagerMainThread() {
	while (running) {
		handleChatRestore();
		

		Sleep(2);
	}
}

void CommandManager::TaskThread() {
	while (running) {
		if (commandWaitList.size() != 0) {
			Command c = commandWaitList.at(0);
			commands[c.type](c);
			commandWaitList.erase(commandWaitList.begin());
		}
		Sleep(2);
	}
}

DWORD CommandManager::createOutGoingMessageAddress = 0;
Hook CommandManager::outMessageHook;


bool CommandManager::running = true;
vector<wstring> CommandManager::messagesTyped = {};
int CommandManager::messageCount = 0;
wstring CommandManager::currentMessageTyping = L"";
thread* CommandManager::commandManagerMainThread;
thread* CommandManager::taskThread;

wchar_t CommandManager::CommandPrefix = L'.';
map<wstring, void(*)(Command)> CommandManager::commands = {};
vector<Command> CommandManager::commandWaitList = {};

bool CommandManager::Init() {
	vector <int> sig = { 0xFF, 0x15, -1, -1, -1, -1, 0x8B, 0xD0, 0x83, 0x3D, -1, -1, -1, -1, 0x01, 0x75, 0x0A, 0x8B, 0xCA, 0xFF, 0x15, -1, -1, -1, -1, 0xEB, 0x1D, 0x83, 0x3D, -1, -1, -1, -1 , 0x00, 0x75, 0x14, 0xFF, 0x35, -1, -1, -1, -1, 0x8B, 0x0D };
	createOutGoingMessageAddress = data::GetAddressFromSignature(sig);

	if (!createOutGoingMessageAddress) {
		tout << "Unable to find the address of the function <createOutGoingMessageAddress>." << tendl;
		return false;
	}

	outMessageHook = Hook((void*)(createOutGoingMessageAddress), &(CommandManager::PreProcessCmd), 15, true);
	if (!outMessageHook.hook()) {
		tout << "Unable to hook <createOutGoingMessageAddress>." << tendl;
		return false;
	}

	running = true;
	commandManagerMainThread = new thread(CommandManagerMainThread);
	taskThread = new thread(TaskThread);
	return true;
}

void CommandManager::Dispose() {
	running = false;
	commandManagerMainThread->join();
	taskThread->join();
	outMessageHook.unhook();
}


bool CommandManager::registerCmd(string identifier, void(*cmdProcessor)(Command)) {
	return registerCmd(to_wstr(identifier), cmdProcessor);
}
bool CommandManager::registerCmd(wstring identifier, void(*cmdProcessor)(Command)) {
	if (commands.count(identifier)) {
		tout << "Command already registered: " << identifier << tendl;
		return false;
	}
	commands.insert({ identifier, cmdProcessor });
	tout << "Registered command: " << identifier << tendl;
	return true;
}