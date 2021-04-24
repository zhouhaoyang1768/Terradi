#include "Util.h"
#include <locale>
#include <codecvt>
#include <string>
#include "../CsBridge/TerraBridge.h"
#include "Main.h"
//#include "../../Hack/CsBridge/TerrariaBridge.h"




std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

string hex_str(int num) {
	int digits = 1;
	while (pow(16, digits) < num)
		digits++;
	string hexStr = "";
	for (int i = digits; i >= 0; i--) {
		hexStr += hex_char((num / (int)pow(16, i)) % 16);
	}
	return hexStr;
}

char hex_char(int num) {
	switch (num) {
	case 0:
		return '0';
	case 1:
		return '1';
	case 2:
		return '2';
	case 3:
		return '3';
	case 4:
		return '4';
	case 5:
		return '5';
	case 6:
		return '6';
	case 7:
		return '7';
	case 8:
		return '8';
	case 9:
		return '9';
	case 10:
		return 'A';
	case 11:
		return 'B';
	case 12:
		return 'C';
	case 13:
		return 'D';
	case 14:
		return 'E';
	case 15:
		return 'F';
	default:
		return '?';
	}
}

wstring to_wstr(string s) {
	return converter.from_bytes(s);
}

string to_str(wstring s) {
	return converter.to_bytes(s);
}







void TerrariaOutStream::print(wstring s) {
	wstring message = Main::name + L" >> " + s;
	TerraBridge::LocalMessage(message, { 255, 255, 0 });
}

TerrariaOutStream& TerrariaOutStream::operator<<(string s) {
	*this = operator<< (to_wstr(s));
	return *this;
}
TerrariaOutStream& TerrariaOutStream::operator<<(wstring s) {
	if (s == tendl) {
		print(content);
		content = L"";
		return *this;
	}

	this->content += s;
	return *this;
}
TerrariaOutStream& TerrariaOutStream::operator<<(const char* s) {
	*this = operator<<(string(s));
	return *this;
}
TerrariaOutStream& TerrariaOutStream::operator<<(const wchar_t* s) {
	*this = operator<<(wstring(s));
	return *this;
}