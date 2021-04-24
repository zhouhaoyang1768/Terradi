#include "DataManager.h"

using namespace std;

#define BYTELENGTH 8


//BYTE
bool data::set(void* baseAddress, DWORD offset, BYTE value) {
	if (baseAddress == nullptr) return false;
	else return set((void*)((DWORD)baseAddress + offset), value);
}

bool data::set(void* address, BYTE value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;

	DWORD addressAligned = ((DWORD)address % 4);
	DWORD* targetpt = (DWORD*)((DWORD)address - addressAligned);

	DWORD flushZero = (0xFF << (addressAligned * BYTELENGTH)) ^ 0xFFFFFFFF;
	*(DWORD*)(targetpt) = *(DWORD*)(targetpt)&flushZero;

	DWORD newValue = value << (addressAligned * BYTELENGTH);
	*(DWORD*)(targetpt) = *(DWORD*)(targetpt) | newValue;

	//wcout << "Result: " << hex << (int)newValue << endl;
	return true;
}

bool data::get(void* baseAddress, DWORD offset, BYTE& value) {
	if (baseAddress == nullptr) return false;
	else return get((void*)((DWORD)baseAddress + offset), value);
}

bool data::get(void* address, BYTE& value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;

	DWORD addressAligned = ((DWORD)address % 4);
	DWORD* targetpt = (DWORD*)((DWORD)address - addressAligned);
	//wcout << "dataHack::get address: " << hex << (DWORD)address << " |targetPt: " << hex << (DWORD)targetpt << endl;

	DWORD targetValue = *targetpt;
	targetValue = targetValue << ((3 - addressAligned) * BYTELENGTH);
	targetValue = targetValue >> (3 * BYTELENGTH);

	value = (BYTE)targetValue;
	//wcout << "dataHack::get return value: " << hex << (DWORD)value << endl;
	return true;
}


//BOOL
bool data::set(void* baseAddress, DWORD offset, bool value) {
	if (baseAddress == nullptr) return false;
	else return set((void*)((DWORD)baseAddress + offset), value);
}

bool data::set(void* address, bool value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	return set(address, (BYTE)value);
}

bool data::get(void* baseAddress, DWORD offset, bool& value) {
	if (baseAddress == nullptr) return false;
	else return get((void*)((DWORD)baseAddress + offset), value);
}

bool data::get(void* address, bool& value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	return get(address, (BYTE&)value);
}


//CHAR
bool data::set(void* baseAddress, DWORD offset, CHAR value) {
	if (baseAddress == nullptr) return false;
	else return set((void*)((DWORD)baseAddress + offset), value);
}

bool data::set(void* address, CHAR value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	return set(address, (BYTE)value);
}

bool data::get(void* baseAddress, DWORD offset, CHAR& value) {
	if (baseAddress == nullptr) return false;
	else return get((void*)((DWORD)baseAddress + offset), value);
}

bool data::get(void* address, CHAR& value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	return get(address, (BYTE&)value);
}


//DWORD
bool data::set(void* baseAddress, DWORD offset, DWORD value) {
	if (baseAddress == nullptr) return false;
	else return set((void*)((DWORD)baseAddress + offset), value);
}

bool data::set(void* address, DWORD value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	*(DWORD*)(address) = value;
	return true;
}

bool data::get(void* baseAddress, DWORD offset, DWORD& value) {
	if (baseAddress == nullptr) return false;
	else return get((void*)((DWORD)baseAddress + offset), value);
}

bool data::get(void* address, DWORD& value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	value = *(DWORD*)address;
	//wcout << "dataHack::get value: " << value << endl;
	return true;
}

//int
bool data::set(void* baseAddress, DWORD offset, int value) {
	if (baseAddress == nullptr) return false;
	else return set((void*)((DWORD)baseAddress + offset), value);
}
bool data::set(void* address, int value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	*(int*)(address) = value;
	return true;
}
bool data::get(void* baseAddress, DWORD offset, int& value) {
	if (baseAddress == nullptr) return false;
	else return get((void*)((DWORD)baseAddress + offset), value);
}
bool data::get(void* address, int& value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	value = *(int*)address;
	return true;
}

//float
bool data::set(void* baseAddress, DWORD offset, FLOAT value) {
	if (baseAddress == nullptr) return false;
	else return set((void*)((DWORD)baseAddress + offset), value);
}

bool data::set(void* address, FLOAT value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	return set(address, *(DWORD*)&value);
}

bool data::get(void* baseAddress, DWORD offset, FLOAT& value) {
	if (baseAddress == nullptr) return false;
	else return get((void*)((DWORD)baseAddress + offset), value);
}

bool data::get(void* address, FLOAT& value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	value = *(FLOAT*)address;
	//wcout << "dataHack::get value: " << value << endl;
	return true;
}



bool data::set(void* baseAddress, DWORD offset, short value) {
	if (baseAddress == nullptr) return false;
	else return set((void*)((DWORD)baseAddress + offset), value);
}

bool data::set(void* address, short value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	DWORD addressAligned = ((DWORD)address % 4);
	DWORD* targetpt = (DWORD*)((DWORD)address - addressAligned);

	//addressAligned 2 or 0
	DWORD flushZero = (0xFFFF << (addressAligned * BYTELENGTH)) ^ 0xFFFFFFFF;
	*(DWORD*)(targetpt) = *(DWORD*)(targetpt)&flushZero;

	DWORD newValue = value << (addressAligned * BYTELENGTH);
	*(DWORD*)(targetpt) = *(DWORD*)(targetpt) | newValue;

	//wcout << "Result: " << hex << (int)newValue << endl;
	return true;
}

bool data::get(void* baseAddress, DWORD offset, short& value) {
	//wcout << "checkpoint get 01" << endl;
	if (baseAddress == nullptr) return false;
	else return get((void*)((DWORD)baseAddress + offset), value);
}

bool data::get(void* address, short& value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	DWORD addressAligned = ((DWORD)address % 4);
	DWORD* targetpt = (DWORD*)((DWORD)address - addressAligned);
	DWORD targetValue = *targetpt;
	targetValue = targetValue >> ((addressAligned)*BYTELENGTH);
	targetValue = targetValue & 0xFFFF;
	value = (short)targetValue;
	return true;
}


bool data::set(void* baseAddress, DWORD offset, unsigned short value) {
	if (baseAddress == nullptr) return false;
	else return set((void*)((DWORD)baseAddress + offset), value);
}

bool data::set(void* address, unsigned short value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	DWORD addressAligned = ((DWORD)address % 4);
	DWORD* targetpt = (DWORD*)((DWORD)address - addressAligned);

	//addressAligned 2 or 0
	DWORD flushZero = (0xFFFF << (addressAligned * BYTELENGTH)) ^ 0xFFFFFFFF;
	*(DWORD*)(targetpt) = *(DWORD*)(targetpt)&flushZero;

	DWORD newValue = value << (addressAligned * BYTELENGTH);
	*(DWORD*)(targetpt) = *(DWORD*)(targetpt) | newValue;

	//wcout << "Result: " << hex << (int)newValue << endl;
	return true;
}

bool data::get(void* baseAddress, DWORD offset, unsigned short& value) {
	if (baseAddress == nullptr) return false;
	else return get((void*)((DWORD)baseAddress + offset), value);
}

bool data::get(void* address, unsigned short& value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	DWORD addressAligned = ((DWORD)address % 4);
	DWORD* targetpt = (DWORD*)((DWORD)address - addressAligned);
	DWORD targetValue = *targetpt;
	targetValue = targetValue >> ((addressAligned)*BYTELENGTH);
	targetValue = targetValue & 0xFFFF;
	value = (unsigned short)targetValue;
	return true;
}



bool data::set(void* baseAddress, DWORD offset, wchar_t value) {
	if (baseAddress == nullptr) return false;
	else return set((void*)((DWORD)baseAddress + offset), value);
}

bool data::set(void* address, wchar_t value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	DWORD addressAligned = ((DWORD)address % 4);
	DWORD* targetpt = (DWORD*)((DWORD)address - addressAligned);

	//addressAligned 2 or 0
	DWORD flushZero = (0xFFFF << (addressAligned * BYTELENGTH)) ^ 0xFFFFFFFF;
	*(DWORD*)(targetpt) = *(DWORD*)(targetpt)&flushZero;

	DWORD newValue = value << (addressAligned * BYTELENGTH);
	*(DWORD*)(targetpt) = *(DWORD*)(targetpt) | newValue;

	//wcout << "Result: " << hex << (int)newValue << endl;
	return true;
}
bool data::get(void* baseAddress, DWORD offset, wchar_t& value) {
	if (baseAddress == nullptr) return false;
	else return get((void*)((DWORD)baseAddress + offset), value);
}
bool data::get(void* address, wchar_t& value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	DWORD addressAligned = ((DWORD)address % 4);
	DWORD* targetpt = (DWORD*)((DWORD)address - addressAligned);
	DWORD targetValue = *targetpt;
	targetValue = targetValue >> ((addressAligned)*BYTELENGTH);
	targetValue = targetValue & 0xFFFF;
	value = (wchar_t)targetValue;
	return true;
}

/*
bool dataHack::get(void* baseAddress, DWORD offset, string& value) {
	if (baseAddress == nullptr) return false;
	DWORD stringAddress;
	if (!dataHack::get(baseAddress, offset, stringAddress)) return false;
	return dataHack::get((void*)stringAddress, value);
}

bool dataHack::get(void* address, string& value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	DWORD lengthValue;
	if (!dataHack::get(address, STRING_LENGTH_OFFSET, lengthValue)) return false;
	CHAR ch;
	value.clear();
	for (int i = 0; i < lengthValue; i++) {
		if (!dataHack::get((void*)address, STRING_VALUE_OFFSET + i * 2, ch)) return false;
		value += ch;
	}
	return true;
}
*/
bool data::get(void* baseAddress, DWORD offset, wstring& value) {
	if (IsBadReadPtr((const void*)baseAddress, NULL)) return false;
	DWORD stringAddress;
	if (!data::get(baseAddress, offset, stringAddress)) return false;
	return data::get((void*)stringAddress, value);
}
bool data::get(void* address, wstring& value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	DWORD lengthValue;
	if (!data::get(address, STRING_LENGTH_OFFSET, lengthValue)) return false;
	wchar_t wch;
	value.clear();
	for (int i = 0; i < lengthValue; i++) {
		if (!data::get((void*)address, STRING_VALUE_OFFSET + i * 2, wch)) return false;
		value += wch;
	}
	return true;
}


DWORD data::GetAddressFromSignature(vector<int> signature, DWORD startaddress, DWORD endaddress) {
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	if (0 == startaddress)
		startaddress = (DWORD)(si.lpMinimumApplicationAddress);
	if (0 == endaddress)
		endaddress = (DWORD)(si.lpMaximumApplicationAddress);

	MEMORY_BASIC_INFORMATION mbi{ 0 };
	DWORD protectflags = (PAGE_GUARD | PAGE_NOCACHE | PAGE_NOACCESS);

	for (DWORD i = startaddress; i < endaddress - signature.size(); i++) {
		//wcout << "scanning: " << hex << i << endl;
		if (VirtualQuery((LPCVOID)i, &mbi, sizeof(mbi))) {
			if (mbi.Protect & protectflags || !(mbi.State & MEM_COMMIT)) {
				i += mbi.RegionSize;
				continue;
			}
			for (DWORD k = (DWORD)mbi.BaseAddress; k < (DWORD)mbi.BaseAddress + mbi.RegionSize - signature.size(); k++) {
				for (DWORD j = 0; j < signature.size(); j++) {
					if (signature.at(j) != -1 && signature.at(j) != *(BYTE*)(k + j))
						break;
					if (j + 1 == signature.size()) {
						return k;
					}
				}
			}
			i = (DWORD)mbi.BaseAddress + mbi.RegionSize;
		}

	}
	return NULL;
}