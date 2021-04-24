#pragma once

#include <Windows.h>
#include <vector>
#include <string>

#define STRING_LENGTH_OFFSET 4
#define STRING_VALUE_OFFSET 8

using namespace std;

class data {
public:

	static bool set(void* baseAddress, DWORD offset, BYTE value);
	static bool set(void* address, BYTE value);
	static bool get(void* baseAddress, DWORD offset, BYTE& value);
	static bool get(void* address, BYTE& value);

	static bool set(void* baseAddress, DWORD offset, bool value);
	static bool set(void* address, bool value);
	static bool get(void* baseAddress, DWORD offset, bool& value);
	static bool get(void* address, bool& value);

	static bool set(void* baseAddress, DWORD offset, CHAR value);
	static bool set(void* address, CHAR value);
	static bool get(void* baseAddress, DWORD offset, CHAR& value);
	static bool get(void* address, CHAR& value);

	static bool set(void* baseAddress, DWORD offset, DWORD value);
	static bool set(void* address, DWORD value);
	static bool get(void* baseAddress, DWORD offset, DWORD& value);
	static bool get(void* address, DWORD& value);

	static bool set(void* baseAddress, DWORD offset, int value);
	static bool set(void* address, int value);
	static bool get(void* baseAddress, DWORD offset, int& value);
	static bool get(void* address, int& value);

	static bool set(void* baseAddress, DWORD offset, FLOAT value);
	static bool set(void* address, FLOAT value);
	static bool get(void* baseAddress, DWORD offset, FLOAT& value);
	static bool get(void* address, FLOAT& value);

	static bool set(void* baseAddress, DWORD offset, short value);
	static bool set(void* address, short value);
	static bool get(void* baseAddress, DWORD offset, short& value);
	static bool get(void* address, short& value);

	static bool set(void* baseAddress, DWORD offset, unsigned short value);
	static bool set(void* address, unsigned short value);
	static bool get(void* baseAddress, DWORD offset, unsigned short& value);
	static bool get(void* address, unsigned short& value);

	static bool set(void* baseAddress, DWORD offset, wchar_t value);
	static bool set(void* address, wchar_t value);
	static bool get(void* baseAddress, DWORD offset, wchar_t& value);
	static bool get(void* address, wchar_t& value);
	/*
	static bool get(void* baseAddress, DWORD offset, string& value);
	static bool get(void* address, string& value);
	*/
	static bool get(void* baseAddress, DWORD offset, wstring& value);
	static bool get(void* address, wstring& value);



	static DWORD GetAddressFromSignature(vector<int> signature, DWORD startaddress = 0, DWORD endaddress = 0);
};