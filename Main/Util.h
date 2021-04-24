#pragma once

#include <string>

using namespace std;


const wstring tendl = L"\n";

struct point {
	int x, y;
};
 static class TerrariaOutStream {
	wstring content;
	

	void print(wstring s);


public:

	TerrariaOutStream& operator<<(string s);
	TerrariaOutStream& operator<<(wstring s);
	TerrariaOutStream& operator<<(const char* s);
	TerrariaOutStream& operator<<(const wchar_t* s);

} tout;

string hex_str(int num);
char hex_char(int num);

string to_str(wstring s);
wstring to_wstr(string s);

