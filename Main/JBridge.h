#pragma once

#include <thread>
#include <string>
//#include <vector>

#include "winsock.h"

class JBridge {

	static bool running;
	static bool connected;
	static std::thread socketThread;
	//static std::thread taskThread;

	static SOCKET s_server;
	static SOCKET s_accept;

	static void Loop();
	static void process(const std::string& s);

	JBridge() {

	}

public:

	static bool Send(std::string s);
	
	static bool Init();
	static void Dispose();
};