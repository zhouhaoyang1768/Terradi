#include <iostream>
#include <Windows.h>

#include <string>
#include <psapi.h>
#include <stdlib.h>
#include <vector>
#include <filesystem>

#pragma warning(disable:4996)

using namespace std;

#define PROCESS_NUM 2048

//DWORD GetProcID(const char* procName);
bool IsTargetProcess(DWORD dwProcessId, string targetProcess);
int GetProcessID(string& targetProcess, vector<DWORD>& processArray);

vector<const char*> paths = { "\\Terradi.dll", "\\CsBridge.dll" };

int main()
{
	string procName = "Terraria";

	vector<DWORD> processes;
	DWORD procID = GetProcessID(procName, processes);

	if (1 == procID) {
		procID = processes[0];
	}

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procID);

	if (!hProc || hProc == INVALID_HANDLE_VALUE) {
		CloseHandle(hProc);
		return 0;
	}

	for (const char* c : paths) {
		string _path = std::filesystem::current_path().u8string() + c;
		const char* path = _path.c_str();

		void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		WriteProcessMemory(hProc, loc, path, strlen(path) + 1, 0);
		HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
		if (hThread) {
			CloseHandle(hThread);
		}
	}

	

	return 0;
}

int GetProcessID(string& targetProcess, vector<DWORD>& processArray)
{
	DWORD dwProcess[PROCESS_NUM], dwNeeded;
	EnumProcesses(dwProcess, PROCESS_NUM, &dwNeeded);

	for (int i = 0; i < dwNeeded / sizeof(DWORD); i++)
	{
		if (0 != dwProcess[i])
		{
			if (IsTargetProcess(dwProcess[i], targetProcess))
			{
				processArray.push_back(dwProcess[i]);
				//cout << "dwProcessId:" << dwProcessId << endl;
			}
		}
	}

	return processArray.size();
}

bool IsTargetProcess(DWORD dwProcessId, string targetProcess)
{
	HANDLE hProcess;

	hProcess = NULL;

	hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, dwProcessId);
	if (NULL == hProcess)
	{
		//OutputErrorMessage("IsTargetProcess - OpenProcess Failed , Error Code Is %d , Error Message Is %s !");
		return FALSE;
	}

	DWORD dwNameLen = 0;
	CHAR pathArray[MAX_PATH];
	ZeroMemory(pathArray, MAX_PATH);

	dwNameLen = GetModuleFileNameExA(hProcess, NULL, pathArray, MAX_PATH);
	if (dwNameLen == 0)
	{
		CloseHandle(hProcess);
		return FALSE;
	}

	CHAR exeNameArray[MAX_PATH];
	ZeroMemory(exeNameArray, MAX_PATH);
	_splitpath(pathArray, NULL, NULL, exeNameArray, NULL);

	if (!targetProcess.compare(exeNameArray))
	{
		CloseHandle(hProcess);
		return TRUE;
	}

	return FALSE;
}