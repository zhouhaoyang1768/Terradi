#pragma once
#include <string>



class Main {
private:
	static bool running;
	static bool initialized;

	Main() {

	}

public:
	static const std::wstring name;

	static bool Init();

	static void MainLoop();

	static void Dispose();
	static void Terminate();
};