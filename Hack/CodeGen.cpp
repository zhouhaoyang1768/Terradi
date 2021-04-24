#include "codeGen.h"

//private
void CodeGen::OneOpcode(BYTE opcode) {
	*pCur = opcode;
	++pCur;
}
void CodeGen::OneOpcodeOneMEM(BYTE opcode, DataAddress address) {
	*pCur = opcode;
	++pCur;
	*((DWORD*)pCur) = (DWORD)address;
	pCur += ADDRESS_LENGTH;
}
void CodeGen::OneOpcodeOneDirect(BYTE opcode, BYTE direct) {
	*pCur = opcode;
	++pCur;
	*pCur = direct;
	++pCur;
}
void CodeGen::OneOpcodeOneDirect(BYTE opcode, WORD direct) {
	*pCur = opcode;
	++pCur;
	*((WORD*)pCur) = direct;
	pCur += WORD_LENGTH;
}
//same as OneOpcodeOneMEM
void CodeGen::OneOpcodeOneDirect(BYTE opcode, DWORD direct) {
	*pCur = opcode;
	++pCur;
	*((DWORD*)pCur) = direct;
	pCur += DWORD_LENGTH;
}

void CodeGen::TwoOpcode(BYTE opcode, BYTE opcode2) {
	*pCur = opcode;
	++pCur;
	*pCur = opcode2;
	++pCur;
}
void CodeGen::TwoOpcodeOneDirect(BYTE opcode, BYTE opcode2, BYTE direct) {
	*pCur = opcode;
	++pCur;
	*pCur = opcode2;
	++pCur;
	*pCur = direct;
	++pCur;
}

void CodeGen::TwoOpcodeOneDirect(BYTE opcode, BYTE opcode2, DWORD direct) {
	*pCur = opcode;
	++pCur;
	*pCur = opcode2;
	++pCur;
	*((DWORD*)pCur) = direct;
	pCur += DWORD_LENGTH;

}
void CodeGen::TwoOpcodeOneMEM(BYTE opcode, BYTE opcode2, DataAddress address) {
	*pCur = opcode;
	++pCur;
	*pCur = opcode2;
	++pCur;
	*((DWORD*)pCur) = (DWORD)address;
	pCur += ADDRESS_LENGTH;
}
void CodeGen::TwoOpcodeOneMEMOneDirect(BYTE opcode, BYTE opcode2, DataAddress address, BYTE direct) {
	*pCur = opcode;
	++pCur;
	*pCur = opcode2;
	++pCur;
	*((DWORD*)pCur) = (DWORD)address;
	pCur += ADDRESS_LENGTH;
	*pCur = direct;
	++pCur;
}
void CodeGen::TwoOpcodeTwoDirect(BYTE opcode, BYTE opcode2, BYTE direct, DWORD direct2) {
	*pCur = opcode;
	++pCur;
	*pCur = opcode2;
	++pCur;
	*pCur = direct;
	++pCur;
	*((DWORD*)pCur) = direct2;
	pCur += DWORD_LENGTH;
}

void CodeGen::ThreeOpcode(BYTE opcode, BYTE opcode2, BYTE opcode3) {
	*pCur = opcode;
	++pCur;
	*pCur = opcode2;
	++pCur;
	*pCur = opcode3;
	++pCur;
}
void CodeGen::ThreeOpcodeOneDirect(BYTE opcode, BYTE opcode2, BYTE opcode3, BYTE direct) {
	*pCur = opcode;
	++pCur;
	*pCur = opcode2;
	++pCur;
	*pCur = opcode3;
	++pCur;
	*pCur = direct;
	++pCur;
}

void CodeGen::ThreeOpcodeTwoDirect(BYTE opcode, BYTE opcode2, BYTE opcode3, BYTE direct, BYTE direct2) {
	*pCur = opcode;
	++pCur;
	*pCur = opcode2;
	++pCur;
	*pCur = opcode3;
	++pCur;
	*pCur = direct;
	++pCur;
	*pCur = direct2;
	++pCur;
}


//public
CodeGen::CodeGen(CodeAddress from) {
	this->from = (BYTE*)from;
	to = nullptr;
	pCur = (BYTE*)this->from;
	setNOPToEnd = false;
}
CodeGen::CodeGen(CodeAddress from, CodeAddress to, bool setNOPToEnd) {
	this->from = (BYTE*)from;
	this->to = (BYTE*)to;
	pCur = (BYTE*)this->from;
	this->setNOPToEnd = setNOPToEnd;
}
void CodeGen::GenStart() {
}
void CodeGen::GenStart(CodeAddress from) {
	this->from = (BYTE*)from;
	pCur = (BYTE*)from;
}
void CodeGen::GenStart(CodeAddress from, CodeAddress to, bool setNOPToEnd) {
	this->from = (BYTE*)from;
	this->to = (BYTE*)to;
	pCur = (BYTE*)this->from;
	this->setNOPToEnd = setNOPToEnd;
}

CodeAddress CodeGen::GenEnd() {
	if ((to != nullptr) && setNOPToEnd) {
		while (pCur <= to) {
			nop();
		}
	}
	return (CodeAddress)pCur;
}
CodeAddress CodeGen::GenEnd(bool setNOPToEnd) {
	this->setNOPToEnd = setNOPToEnd;
	return GenEnd();
}

void CodeGen::nop() {
	OneOpcode(0x90);
}
void CodeGen::push_ad() {
	OneOpcode(0x60);
}
void CodeGen::push_fd() {
	OneOpcode(0x9c);
}
void CodeGen::push_eax() {
	OneOpcode(0x50);
}
void CodeGen::push_ebx() {
	OneOpcode(0x53);
}
void CodeGen::push_ecx() {
	OneOpcode(0x51);
}
void CodeGen::push_edx() {
	OneOpcode(0x52);
}
void CodeGen::push_esi() {
	OneOpcode(0x56);
}
void CodeGen::push_edi() {
	OneOpcode(0x57);
}
void CodeGen::push_ebp() {
	OneOpcode(0x55);
}
void CodeGen::push_direct(BYTE direct) {
	OneOpcodeOneDirect(0x6a, direct);
}
void CodeGen::push_direct(DWORD direct) {
	OneOpcodeOneDirect(0x68, direct);
}

void CodeGen::push_mem(DataAddress address) {
	CodeGen::TwoOpcodeOneMEM(0xff, 0x35, address);
}



void CodeGen::pop_ad() {
	OneOpcode(0x61);
}
void CodeGen::pop_fd() {
	OneOpcode(0x9d);
}
void CodeGen::pop_eax() {
	OneOpcode(0x58);
}
void CodeGen::pop_ebx() {
	OneOpcode(0x5b);
}
void CodeGen::pop_ecx() {
	OneOpcode(0x59);
}
void CodeGen::pop_esi() {
	OneOpcode(0x5e);
}
void CodeGen::pop_edi() {
	OneOpcode(0x5f);
}
void CodeGen::pop_ebp() {
	OneOpcode(0x5d);
}

void CodeGen::mov_eax_to_edx() {
	TwoOpcode(0x8b, 0xd0);
}
void CodeGen::mov_eax_to_esi() {
	TwoOpcode(0x8b, 0xf0);
}
void CodeGen::mov_eax_to_edi() {
	TwoOpcode(0x8b, 0xf8);
}

void CodeGen::mov_ebx_to_eax() {
	TwoOpcode(0x8b, 0xc3);
}

//void CodeGen::mov_ebx_to_edi() {

//}

void CodeGen::mov_esi_to_eax() {
	TwoOpcode(0x8b, 0xc6);
}
void CodeGen::mov_esi_to_ecx() {
	TwoOpcode(0x8b, 0xce);
}
void CodeGen::mov_edi_to_ebx() {
	TwoOpcode(0x8b, 0xdf);
}

void CodeGen::mov_ebp_to_esp() {
	TwoOpcode(0x8b, 0xe5);
}

void CodeGen::mov_esp_to_ebp() {
	TwoOpcode(0x8b, 0xec);
}

void CodeGen::mov_eax_to_mem(DataAddress address) {
	OneOpcodeOneMEM(0xA3, address);
}
void CodeGen::mov_ebx_to_mem(DataAddress address) {
	TwoOpcodeOneMEM(0x89, 0x1d, address);
}
void CodeGen::mov_ecx_to_mem(DataAddress address) {
	TwoOpcodeOneMEM(0x89, 0x0d, address);
}
void CodeGen::mov_edx_to_mem(DataAddress address) {
	TwoOpcodeOneMEM(0x89, 0x15, address);
}
void CodeGen::mov_esi_to_mem(DataAddress address) {
	TwoOpcodeOneMEM(0x89, 0x35, address);
}
void CodeGen::mov_edi_to_mem(DataAddress address) {
	TwoOpcodeOneMEM(0x89, 0x3d, address);
}
void CodeGen::mov_esp_to_mem(DataAddress address) {
	TwoOpcodeOneMEM(0x89, 0x25, address);
}
void CodeGen::mov_ebp_to_mem(DataAddress address) {
	TwoOpcodeOneMEM(0x89, 0x2d, address);
}

void CodeGen::mov_eax_to_eax_pointto() {
	TwoOpcode(0x89, 0x00);
}
void CodeGen::mov_eax_to_ecx_pointto() {
	TwoOpcode(0x89, 0x01);
}
void CodeGen::mov_eax_to_edx_pointto() {
	TwoOpcode(0x89, 0x02);
}
void CodeGen::mov_ecx_to_eax_pointto() {
	TwoOpcode(0x89, 0x08);
}
void CodeGen::mov_edx_to_ecx_pointto() {
	TwoOpcode(0x89, 0x11);
}
void CodeGen::mov_edx_to_edx_pointto() {
	TwoOpcode(0x89, 0x12);
}


void CodeGen::mov_eax_to_eax_plus(BYTE offset) {
	TwoOpcodeOneDirect(0x89, 0x40, offset);
}
void CodeGen::mov_eax_to_edx_plus(BYTE offset) {
	TwoOpcodeOneDirect(0x89, 0x42, offset);
}
void CodeGen::mov_eax_to_esp_plus(BYTE offset) {
	ThreeOpcodeOneDirect(0x89, 0x44, 0x24, offset);
}
void CodeGen::mov_ecx_to_edx_plus(BYTE offset) {
	ThreeOpcodeOneDirect(0x89, 0x4a, 0x24, offset);
}
void CodeGen::mov_edx_to_eax_plus(BYTE offset) {
	TwoOpcodeOneDirect(0x89, 0x50, offset);
}
void CodeGen::mov_edx_to_edx_plus(BYTE offset) {
	TwoOpcodeOneDirect(0x89, 0x52, offset);
}


void CodeGen::mov_eax_to_ebp_minus(BYTE offset) {
	WORD result = 256 - offset;
	TwoOpcodeOneDirect(0x89, 0x45, (BYTE)result);
}
/*
void CodeGen::mov_ecx_to_ebp_minus(BYTE offset) {
	WORD result = 256 - offset;
	TwoOpcodeOneDirect(0x89, 0x4d, (BYTE)result);
}
*/
void CodeGen::mov_edx_to_ebp_minus(BYTE offset) {
	WORD result = 256 - offset;
	TwoOpcodeOneDirect(0x89, 0x55, (BYTE)result);
}

void CodeGen::mov_ecx_pointto_to_eax() {
	TwoOpcode(0x8b, 0x01);
}
void CodeGen::mov_edi_pointto_to_eax() {
	TwoOpcode(0x8b, 0x07);
}

void CodeGen::mov_eax_plus_to_ecx(BYTE offset) {
	TwoOpcodeOneDirect(0x8b, 0x48, offset);
}
void CodeGen::mov_eax_plus_to_esi(BYTE offset) {
	TwoOpcodeOneDirect(0x8b, 0x70, offset);
}

void CodeGen::mov_edx_plus_to_eax(BYTE offset) {
	TwoOpcodeOneDirect(0x8b, 0x42, offset);
}

void CodeGen::mov_edi_plus_to_eax(BYTE offset) {
	TwoOpcodeOneDirect(0x8b, 0x47, offset);
}

void CodeGen::mov_esp_plus_to_ecx(BYTE offset) {
	ThreeOpcodeOneDirect(0x8b, 0x4c, 0x24, offset);
}

void CodeGen::mov_ebp_plus_to_eax(BYTE offset) {
	TwoOpcodeOneDirect(0x8b, 0x45, offset);
}
void CodeGen::mov_ebp_plus_to_ebx(BYTE offset) {
	TwoOpcodeOneDirect(0x8b, 0x5d, offset);
}
void CodeGen::mov_ebp_plus_to_ecx(BYTE offset) {
	TwoOpcodeOneDirect(0x8b, 0x4d, offset);
}
void CodeGen::mov_ebp_plus_to_edx(BYTE offset) {
	TwoOpcodeOneDirect(0x8b, 0x55, offset);
}

void CodeGen::mov_ebp_minus_to_eax(BYTE offset) {
	WORD result = 256 - offset;
	TwoOpcodeOneDirect(0x8b, 0x45, (BYTE)result);
}
void CodeGen::mov_ebp_minus_to_ecx(BYTE offset) {
	WORD result = 256 - offset;
	TwoOpcodeOneDirect(0x8b, 0x4d, (BYTE)result);
}
/*
void CodeGen::mov_ebp_minus_to_edx(BYTE offset) {
	WORD result = 256 - offset;
	TwoOpcodeOneDirect(0x8b, 0x55, result);
}
*/

void CodeGen::mov_direct_to_eax(DWORD direct) {
	OneOpcodeOneDirect(0xB8, direct);
}
void CodeGen::mov_direct_to_ecx(DWORD direct) {
	OneOpcodeOneDirect(0xb9, direct);
}
void CodeGen::mov_direct_to_edx(DWORD direct) {
	OneOpcodeOneDirect(0xba, direct);
}
void CodeGen::mov_direct_to_edi(DWORD direct) {
	OneOpcodeOneDirect(0xbf, direct);
}

void CodeGen::mov_direct_to_ebx_pointto(DWORD direct) {
	TwoOpcodeOneDirect(0xc7, 0x03, direct);
}
void CodeGen::mov_direct_to_ecx_pointto(DWORD direct) {
	TwoOpcodeOneDirect(0xc7, 0x01, direct);
}


void CodeGen::mov_direct_to_ebp_minus(DWORD direct, BYTE offset) {
	WORD result = 256 - offset;
	TwoOpcodeTwoDirect(0xc7, 0x45, (BYTE)result, direct);
}

void CodeGen::mov_mem_to_eax(DataAddress address) {
	OneOpcodeOneMEM(0xA1, address);
}
void CodeGen::mov_mem_to_ebx(DataAddress address) {
	TwoOpcodeOneMEM(0x8b, 0x1d, address);
}
void CodeGen::mov_mem_to_ecx(DataAddress address) {
	TwoOpcodeOneMEM(0x8b, 0x0d, address);
}
void CodeGen::mov_mem_to_edx(DataAddress address) {
	TwoOpcodeOneMEM(0x8b, 0x15, address);
}
void CodeGen::mov_mem_to_esi(DataAddress address) {
	TwoOpcodeOneMEM(0x8b, 0x35, address);
}
void CodeGen::mov_mem_to_edi(DataAddress address) {
	TwoOpcodeOneMEM(0x8b, 0x3d, address);
}
void CodeGen::mov_mem_to_esp(DataAddress address) {
	TwoOpcodeOneMEM(0x8b, 0x25, address);
}
void CodeGen::mov_mem_to_ebp(DataAddress address) {
	TwoOpcodeOneMEM(0x8b, 0x2d, address);
}

void CodeGen::movzx_ax_to_eax() {
	ThreeOpcode(0x0f, 0xb7, 0xc0);
}
void CodeGen::movzx_ax_to_edi() {
	ThreeOpcode(0x0f, 0xb7, 0xf8);
}
void CodeGen::movzx_al_to_eax() {
	ThreeOpcode(0x0f, 0xb6, 0xc0);
}
void CodeGen::movzx_si_to_esi() {
	ThreeOpcode(0x0f, 0xb7, 0xf6);
}
void CodeGen::movzx_di_to_edi() {
	ThreeOpcode(0x0f, 0xb7, 0xff);
}



void CodeGen::cmp_eax_by_ebx() {
	TwoOpcode(0x3b, 0xc3);
}
void CodeGen::cmp_eax_by_ecx() {
	TwoOpcode(0x3b, 0xc1);
}
void CodeGen::cmp_eax_by_esi() {
	TwoOpcode(0x3b, 0xc6);
}
void CodeGen::cmp_eax_by_edi() {
	TwoOpcode(0x3b, 0xc7);
}
void CodeGen::cmp_ebx_by_eax() {
	TwoOpcode(0x3b, 0xd8);
}
void CodeGen::cmp_ecx_by_eax() {
	TwoOpcode(0x3b, 0xc8);
}
void CodeGen::cmp_edx_by_eax() {
	TwoOpcode(0x3b, 0xd0);
}
void CodeGen::cmp_esi_by_eax() {
	TwoOpcode(0x3b, 0xf0);
}

void CodeGen::cmp_eax_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0x83, 0xf8, direct);
}
void CodeGen::cmp_ecx_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0x83, 0xf9, direct);
}

void CodeGen::cmp_esi_by_direct(DWORD direct) {
	TwoOpcodeOneDirect(0x81, 0xfe, direct);
}

void CodeGen::cmp_edi_by_direct(DWORD direct) {
	TwoOpcodeOneDirect(0x81, 0xff, direct);
}

void CodeGen::cmp_ecx_pointto_by_eax() {
	TwoOpcode(0x39, 0x01);
}

void CodeGen::cmp_edx_pointto_by_direct(DWORD direct) {
	TwoOpcodeOneDirect(0x81, 0x3a, direct);
}

void CodeGen::cmp_esp_plus_by_byte(BYTE offset, BYTE value) {
	ThreeOpcodeTwoDirect(0x80, 0x7c, 0x24, offset, value);
}

void CodeGen::cmp_eax_by_mem(DataAddress address) {
	TwoOpcodeOneMEM(0x3b, 0x05, address);
}

void CodeGen::cmp_mem_by_byte(DataAddress address, BYTE value) {
	TwoOpcodeOneMEMOneDirect(0x80, 0x3d, address, value);
}


void CodeGen::test_eax_by_eax() {
	TwoOpcode(0x85, 0xc0);
}
void CodeGen::test_ebx_by_ebx() {
	TwoOpcode(0x85, 0xdb);
}
void CodeGen::test_ecx_by_ecx() {
	TwoOpcode(0x85, 0xc9);
}
void CodeGen::test_edx_by_edx() {
	TwoOpcode(0x85, 0xd2);
}
void CodeGen::test_esi_by_esi() {
	TwoOpcode(0x85, 0xf6);
}
void CodeGen::test_edi_by_edi() {
	TwoOpcode(0x85, 0xff);
}

void CodeGen::test_al_by_direct(BYTE direct) {
	OneOpcodeOneDirect(0xa8, direct);
}

void CodeGen::ja(BYTE offset) {
	OneOpcodeOneDirect(0x77, offset);
}
void CodeGen::jae(BYTE offset) {
	OneOpcodeOneDirect(0x73, offset);
}
void CodeGen::jb(BYTE offset) {
	OneOpcodeOneDirect(0x72, offset);
}
void CodeGen::jbe(BYTE offset) {
	OneOpcodeOneDirect(0x76, offset);
}
void CodeGen::je(BYTE offset) {
	OneOpcodeOneDirect(0x74, offset);
}
void CodeGen::jg(BYTE offset) {
	OneOpcodeOneDirect(0x7f, offset);
}
void CodeGen::jl(BYTE offset) {
	OneOpcodeOneDirect(0x7c, offset);
}
void CodeGen::jle(BYTE offset) {
	OneOpcodeOneDirect(0x7e, offset);
}
void CodeGen::jmp(BYTE offset) {
	OneOpcodeOneDirect(0xeb, offset);
}
void CodeGen::jne(BYTE offset) {
	OneOpcodeOneDirect(0x75, offset);
}
void CodeGen::jns(BYTE offset) {
	OneOpcodeOneDirect(0x79, offset);
}
void CodeGen::js(BYTE offset) {
	OneOpcodeOneDirect(0x78, offset);
}

void CodeGen::ja(DWORD offset) {
	TwoOpcodeOneDirect(0x0f, 0x87, offset);
}
void CodeGen::jae(DWORD offset) {
	TwoOpcodeOneDirect(0x0f, 0x83, offset);
}
void CodeGen::jb(DWORD offset) {
	TwoOpcodeOneDirect(0x0f, 0x82, offset);
}
void CodeGen::je(DWORD offset) {
	TwoOpcodeOneDirect(0x0f, 0x84, offset);
}
void CodeGen::jg(DWORD offset) {
	TwoOpcodeOneDirect(0x0f, 0x8f, offset);
}
void CodeGen::jl(DWORD offset) {
	TwoOpcodeOneDirect(0x0f, 0x8c, offset);
}
void CodeGen::jle(DWORD offset) {
	TwoOpcodeOneDirect(0x0f, 0x8e, offset);
}
void CodeGen::jmp(DWORD offset) {
	OneOpcodeOneDirect(0xe9, offset);
}
void CodeGen::jne(DWORD offset) {
	TwoOpcodeOneDirect(0x0f, 0x85, offset);
}
void CodeGen::jns(DWORD offset) {
	TwoOpcodeOneDirect(0x0f, 0x89, offset);
}
void CodeGen::js(DWORD offset) {
	TwoOpcodeOneDirect(0x0f, 0x88, offset);
}

void CodeGen::jmp(CodeAddress jumpTo) {
	TwoOpcode(0xff, 0x25);
	*((DWORD*)pCur) = (DWORD)(((ULONG_PTR)pCur) + ADDRESS_LENGTH);
	pCur += ADDRESS_LENGTH;
	*((DWORD*)pCur) = (DWORD)jumpTo;
	pCur += DWORD_LENGTH;
}

void CodeGen::inc_eax() {
	OneOpcode(0x40);
}
void CodeGen::inc_ecx() {
	OneOpcode(0x40);
}
void CodeGen::inc_edx() {
	OneOpcode(0x42);
}
void CodeGen::inc_esi() {
	OneOpcode(0x46);
}
void CodeGen::inc_edi() {
	OneOpcode(0x47);
}

void CodeGen::dec_eax() {
	OneOpcode(0x48);
}
void CodeGen::dec_ebx() {
	OneOpcode(0x4b);
}
void CodeGen::dec_edx() {
	OneOpcode(0x4a);
}
void CodeGen::dec_esi() {
	OneOpcode(0x4e);
}
void CodeGen::dec_edi() {
	OneOpcode(0x4f);
}

void CodeGen::add_eax_by_eax() {
	TwoOpcode(0x03, 0xc0);
}
void CodeGen::add_eax_by_ecx() {
	TwoOpcode(0x03, 0xc1);
}
void CodeGen::add_eax_by_edx() {
	TwoOpcode(0x03, 0xc2);
}

void CodeGen::add_eax_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0x83, 0xc0, direct);
}
void CodeGen::add_ecx_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0x83, 0xc1, direct);
}
void CodeGen::add_edx_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0x83, 0xc2, direct);
}
void CodeGen::add_esp_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0x83, 0xc4, direct);
}

void CodeGen::add_eax_by_direct(DWORD direct) {
	OneOpcodeOneDirect(0x05, direct);
}

void CodeGen::add_edi_by_direct(DWORD direct) {
	TwoOpcodeOneDirect(0x81, 0xc7, direct);
}

//void CodeGen::add_eax_by_mem(DataAddress address);

void CodeGen::sub_eax_by_ecx() {
	TwoOpcode(0x2b, 0xc1);
}
void CodeGen::sub_eax_by_edx() {
	TwoOpcode(0x2b, 0xc2);
}
void CodeGen::sub_eax_by_esi() {
	TwoOpcode(0x2b, 0xc6);
}

void CodeGen::sub_ecx_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0x83, 0xe9, direct);
}
void CodeGen::sub_esi_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0x83, 0xee, direct);
}
void CodeGen::sub_esp_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0x83, 0xec, direct);
}

void CodeGen::sar_eax_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0xc1, 0xf8, direct);
}

void CodeGen::shr_eax_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0xc1, 0xe8, direct);
}
void CodeGen::shr_ecx_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0xc1, 0xe9, direct);
}


void CodeGen::shl_eax_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0xc1, 0xe0, direct);
}

void CodeGen::call_eax() {
	TwoOpcode(0xFF, 0xD0);
}

void CodeGen::call_esi() {
	TwoOpcode(0xFF, 0xD6);
}
void CodeGen::call_edi() {
	TwoOpcode(0xFF, 0xD7);
}


void CodeGen::call_esi_pointto() {
	TwoOpcode(0xFF, 0x16);
}

void CodeGen::call_esp_plus(BYTE offset) {
	ThreeOpcodeOneDirect(0xff, 0x54, 0x24, offset);
}

void CodeGen::call_mem(DataAddress addressAt) {
	TwoOpcodeOneMEM(0xFF, 0x15, addressAt);
}

void CodeGen::call_relative(DWORD offset) {
	OneOpcodeOneDirect(0xe8, offset);
}

void CodeGen::and_ebx_by_eax() {
	TwoOpcode(0x23, 0xd8);
}
void CodeGen::and_ecx_by_eax() {
	TwoOpcode(0x23, 0xc8);
}
void CodeGen::and_edx_by_ecx() {
	TwoOpcode(0x23, 0xd1);
}
void CodeGen::and_edi_by_eax() {
	TwoOpcode(0x23, 0xf8);
}

void CodeGen::and_eax_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0x83, 0xe0, direct);
}
void CodeGen::and_ebx_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0x83, 0xe3, direct);
}
void CodeGen::and_ecx_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0x83, 0xe1, direct);
}

void CodeGen::and_eax_by_direct(DWORD direct) {
	OneOpcodeOneDirect(0x25, direct);
}
void CodeGen::and_ebx_by_direct(DWORD direct) {
	TwoOpcodeOneDirect(0x81, 0xe3, direct);
}
void CodeGen::and_edi_by_direct(DWORD direct) {
	TwoOpcodeOneDirect(0x81, 0xe7, direct);
}

void CodeGen::and_edi_pointto_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0x83, 0x27, direct);
}

void CodeGen::or_ebx_by_eax() {
	TwoOpcode(0x0b, 0xd8);
}
void CodeGen::or_ebx_by_ecx() {
	TwoOpcode(0x0b, 0xd9);
}


void CodeGen::or_eax_by_direct(BYTE direct) {
	TwoOpcodeOneDirect(0x83, 0xc8, direct);
}

void CodeGen::or_eax_by_direct(DWORD direct) {
	OneOpcodeOneDirect(0x0d, direct);
}
void CodeGen::or_edx_by_direct(DWORD direct) {
	TwoOpcodeOneDirect(0x81, 0xca, direct);
}
void CodeGen::or_esi_by_direct(DWORD direct) {
	TwoOpcodeOneDirect(0x81, 0xce, direct);
}
void CodeGen::or_edi_by_direct(DWORD direct) {
	TwoOpcodeOneDirect(0x81, 0xcf, direct);
}

void CodeGen::xor_eax_by_eax() {
	TwoOpcode(0x33, 0xc0);
}
void CodeGen::xor_ebx_by_ebx() {
	TwoOpcode(0x33, 0xdb);
}
void CodeGen::xor_ecx_by_esp() {
	TwoOpcode(0x33, 0xcc);
}
void CodeGen::xor_edx_by_edx() {
	TwoOpcode(0x33, 0xd2);
}
void CodeGen::xor_esi_by_esi() {
	TwoOpcode(0x33, 0xf6);
}
void CodeGen::xor_edi_by_edi() {
	TwoOpcode(0x33, 0xff);
}

void CodeGen::lea_eax_plus_to_edi(BYTE direct) {
	TwoOpcodeOneDirect(0x8d, 0x78, direct);
}
void CodeGen::lea_ebx_plus_to_eax(BYTE direct) {
	TwoOpcodeOneDirect(0x8d, 0x43, direct);
}
void CodeGen::lea_edx_plus_to_eax(BYTE direct) {
	TwoOpcodeOneDirect(0x8d, 0x42, direct);
}
void CodeGen::lea_edx_plus_to_ecx(BYTE direct) {
	TwoOpcodeOneDirect(0x8d, 0x4a, direct);
}
void CodeGen::lea_edi_plus_to_esi(BYTE direct) {
	TwoOpcodeOneDirect(0x81, 0xcf, direct);
}

void CodeGen::lea_ebp_minus_to_eax(BYTE direct) {
	WORD result = 256 - direct;
	TwoOpcodeOneDirect(0x8d, 0x45, (BYTE)result);
}
void CodeGen::lea_ebp_minus_to_ecx(BYTE direct) {
	WORD result = 256 - direct;
	TwoOpcodeOneDirect(0x8d, 0x4d, (BYTE)result);
}

void CodeGen::ret(WORD size) {
	OneOpcodeOneDirect(0xc2, size);
}

void CodeGen::int_3() {
	OneOpcode(0xcc);
}

void CodeGen::int_29() {
	TwoOpcode(0xcd, 0x29);
}

