#pragma once

#include <string>
#include <map>
#include <vector>
#include <thread>
#include "Windows.h"
#include "Hook.h"
#include "DataManager.h"


struct Command {
	wstring originalCmdStr;
	wstring type;
	wstring argFullText;
	std::vector<wstring> args;
	int argsLen;

	Command(wstring cmd);
};



class CommandManager {
	CommandManager() {

	}
	
	static DWORD createOutGoingMessageAddress;
	static Hook outMessageHook;

	static bool running;
	static vector<wstring> messagesTyped;
	static vector<Command> commandWaitList;
	static int messageCount;
	static wstring currentMessageTyping;
	static thread* commandManagerMainThread;
	static thread* taskThread;

	
	static wchar_t CommandPrefix;
	static map<wstring, void(*)(Command)> commands;

	static void handleChatRestore();

	static void CommandManagerMainThread();
	static void TaskThread();
	static SkipOffset PreProcessCmd();

public:
	static bool Init();
	static void Dispose();
	static bool ProcessCommand(wstring& cmd);

	static bool registerCmd(string identifier, void(*cmdProcessor)(Command));
	static bool registerCmd(wstring identifier, void(*cmdProcessor)(Command));
};