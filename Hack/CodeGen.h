#pragma once

#include <Windows.h>

#define ADDRESS_LENGTH 4
#define DWORD_LENGTH 4
#define WORD_LENGTH 2

typedef void* CodeAddress;
typedef void* BridgeAddress;
typedef void* DataAddress;
typedef void* Address;

class CodeGen {
private:
	BYTE* from;
	BYTE* to;
	BYTE* pCur;
	bool setNOPToEnd;

private:
	void OneOpcode(BYTE opcode);
	void OneOpcodeOneMEM(BYTE opcode, DataAddress address);
	void OneOpcodeOneDirect(BYTE opcode, BYTE direct);
	void OneOpcodeOneDirect(BYTE opcode, WORD direct);
	void OneOpcodeOneDirect(BYTE opcode, DWORD direct);

	void TwoOpcode(BYTE opcode, BYTE opcode2);
	void TwoOpcodeOneMEM(BYTE opcode, BYTE opcode2, DataAddress address);
	void TwoOpcodeOneDirect(BYTE opcode, BYTE opcode2, BYTE direct);
	void TwoOpcodeOneDirect(BYTE opcode, BYTE opcode2, DWORD direct);
	void TwoOpcodeOneMEMOneDirect(BYTE opcode, BYTE opcode2, DataAddress address, BYTE direct);
	void TwoOpcodeTwoDirect(BYTE opcode, BYTE opcode2, BYTE direct, DWORD direct2);

	void ThreeOpcode(BYTE opcode, BYTE opcode2, BYTE opcode3);
	void ThreeOpcodeOneDirect(BYTE opcode, BYTE opcode2, BYTE opcode3, BYTE direct);
	void ThreeOpcodeTwoDirect(BYTE opcode, BYTE opcode2, BYTE opcode3, BYTE direct, BYTE direct2);

public:
	CodeGen(CodeAddress from);
	CodeGen(CodeAddress from, CodeAddress to, bool setNOPToEnd = false);
	void GenStart();
	void GenStart(CodeAddress from);
	void GenStart(CodeAddress from, CodeAddress to, bool setNOPToEnd = false);
	CodeAddress GenEnd();
	CodeAddress GenEnd(bool setNOPToEnd);

	void nop();
	void push_ad();
	void push_fd();
	void push_eax();
	void push_ebx();
	void push_ecx();
	void push_edx();
	void push_esi();
	void push_edi();
	void push_ebp();
	void push_direct(BYTE direct);
	void push_direct(DWORD direct);
	void push_mem(DataAddress address);

	void pop_ad();
	void pop_fd();
	void pop_eax();
	void pop_ebx();
	void pop_ecx();
	//void pop_edx();
	void pop_esi();
	void pop_edi();
	void pop_ebp();

	void mov_eax_to_edx();
	void mov_eax_to_esi();
	void mov_eax_to_edi();
	void mov_ebx_to_eax();
	//void mov_ebx_to_edi();
	void mov_esi_to_eax();
	void mov_esi_to_ecx();
	void mov_edi_to_ebx();
	void mov_esp_to_ebp();
	void mov_ebp_to_esp();

	void mov_eax_to_mem(DataAddress address);
	void mov_ebx_to_mem(DataAddress address);
	void mov_ecx_to_mem(DataAddress address);
	void mov_edx_to_mem(DataAddress address);
	void mov_esi_to_mem(DataAddress address);
	void mov_edi_to_mem(DataAddress address);
	void mov_esp_to_mem(DataAddress address);
	void mov_ebp_to_mem(DataAddress address);

	void mov_eax_to_eax_pointto();
	void mov_eax_to_ecx_pointto();
	void mov_eax_to_edx_pointto();
	void mov_ecx_to_eax_pointto();
	void mov_edx_to_ecx_pointto();
	void mov_edx_to_edx_pointto();

	void mov_eax_to_eax_plus(BYTE offset);
	void mov_eax_to_edx_plus(BYTE offset);
	void mov_eax_to_esp_plus(BYTE offset);
	void mov_ecx_to_edx_plus(BYTE offset);
	void mov_edx_to_eax_plus(BYTE offset);
	void mov_edx_to_edx_plus(BYTE offset);


	void mov_eax_to_ebp_minus(BYTE offset);
	/*
	void mov_ecx_to_ebp_minus(BYTE offset);
	*/
	void mov_edx_to_ebp_minus(BYTE offset);

	void mov_ecx_pointto_to_eax();
	void mov_edi_pointto_to_eax();

	void mov_eax_plus_to_ecx(BYTE offset);
	void mov_eax_plus_to_esi(BYTE offset);
	void mov_edx_plus_to_eax(BYTE offset);
	void mov_edi_plus_to_eax(BYTE offset);
	//mov     ecx,dword ptr [esp+64h]
	void mov_esp_plus_to_ecx(BYTE offset);
	void mov_ebp_plus_to_eax(BYTE offset);
	void mov_ebp_plus_to_ebx(BYTE offset);
	void mov_ebp_plus_to_ecx(BYTE offset);


	//75d63e89 8b55e0          mov     edx,dword ptr [ebp-20h]
	//75d63d20 8b5508          mov     edx,dword ptr [ebp+8]
	void mov_ebp_plus_to_edx(BYTE offset);
	void mov_ebp_minus_to_eax(BYTE offset);
	void mov_ebp_minus_to_ecx(BYTE offset);
	/*
	void mov_ebp_minus_to_edx(BYTE offset);
	*/


	void mov_direct_to_eax(DWORD direct);
	//void mov_direct_to_ebx(DWORD direct);
	void mov_direct_to_ecx(DWORD direct);
	void mov_direct_to_edx(DWORD direct);
	void mov_direct_to_edi(DWORD direct);

	void mov_direct_to_ebx_pointto(DWORD direct);
	void mov_direct_to_ecx_pointto(DWORD direct);

	void mov_direct_to_ebp_minus(DWORD direct, BYTE offset);

	void mov_mem_to_eax(DataAddress address);
	void mov_mem_to_ebx(DataAddress address);
	void mov_mem_to_ecx(DataAddress address);
	void mov_mem_to_edx(DataAddress address);
	void mov_mem_to_esi(DataAddress address);
	void mov_mem_to_edi(DataAddress address);
	void mov_mem_to_esp(DataAddress address);
	void mov_mem_to_ebp(DataAddress address);

	void movzx_ax_to_eax();
	void movzx_ax_to_edi();
	void movzx_al_to_eax();
	void movzx_di_to_edi();
	void movzx_si_to_esi();


	void cmp_eax_by_ebx();
	void cmp_eax_by_ecx();
	void cmp_eax_by_esi();
	void cmp_eax_by_edi();
	void cmp_ebx_by_eax();
	void cmp_ecx_by_eax();
	void cmp_edx_by_eax();
	void cmp_esi_by_eax();

	void cmp_eax_by_direct(BYTE direct);
	void cmp_ecx_by_direct(BYTE direct);

	void cmp_esi_by_direct(DWORD direct);
	void cmp_edi_by_direct(DWORD direct);

	void cmp_ecx_pointto_by_eax();

	void cmp_edx_pointto_by_direct(DWORD direct);

	void cmp_esp_plus_by_byte(BYTE offset, BYTE value);

	void cmp_eax_by_mem(DataAddress address);

	void cmp_mem_by_byte(DataAddress address, BYTE value);

	void test_eax_by_eax();
	void test_ebx_by_ebx();
	void test_ecx_by_ecx();
	void test_edx_by_edx();
	void test_esi_by_esi();
	void test_edi_by_edi();

	void test_al_by_direct(BYTE direct);

	void ja(BYTE offset);
	void jae(BYTE offset);
	void jb(BYTE offset);
	void jbe(BYTE offset);
	void je(BYTE offset);
	void jg(BYTE offset);
	void jl(BYTE offset);
	void jle(BYTE offset);
	void jmp(BYTE offset);
	void jne(BYTE offset);
	void jns(BYTE offset);
	void js(BYTE offset);

	void ja(DWORD offset);
	void jae(DWORD offset);
	void jb(DWORD offset);
	void je(DWORD offset);
	void jg(DWORD offset);
	void jl(DWORD offset);
	void jle(DWORD offset);
	void jmp(DWORD offset);
	void jne(DWORD offset);
	void jns(DWORD offset);
	void js(DWORD offset);


	void jmp(CodeAddress jumpTo);

	void inc_eax();
	void inc_ecx();
	void inc_edx();
	void inc_esi();
	void inc_edi();

	void dec_eax();
	void dec_ebx();
	void dec_edx();
	void dec_esi();
	void dec_edi();

	void add_eax_by_eax();
	void add_eax_by_ecx();
	void add_eax_by_edx();

	void add_eax_by_direct(BYTE direct);
	void add_ecx_by_direct(BYTE direct);
	void add_edx_by_direct(BYTE direct);
	void add_esp_by_direct(BYTE direct);

	void add_eax_by_direct(DWORD direct);
	void add_edi_by_direct(DWORD direct);
	//void add_eax_by_mem(DataAddress address);

	void sub_eax_by_ecx();
	void sub_eax_by_edx();
	void sub_eax_by_esi();

	void sub_ecx_by_direct(BYTE direct);
	void sub_esi_by_direct(BYTE direct);
	void sub_esp_by_direct(BYTE direct);


	void sar_eax_by_direct(BYTE direct);

	void shr_eax_by_direct(BYTE direct);
	void shr_ecx_by_direct(BYTE direct);

	void shl_eax_by_direct(BYTE direct);

	void call_eax();
	void call_esi();
	void call_edi();

	void call_esi_pointto();

	void call_esp_plus(BYTE offset);

	void call_mem(DataAddress addressAt);
	void call_relative(DWORD offset);

	void and_ebx_by_eax();
	void and_ecx_by_eax();
	void and_edx_by_ecx();
	void and_edi_by_eax();

	void and_eax_by_direct(BYTE direct);
	void and_ebx_by_direct(BYTE direct);
	void and_ecx_by_direct(BYTE direct);

	void and_eax_by_direct(DWORD direct);
	void and_ebx_by_direct(DWORD direct);
	void and_edi_by_direct(DWORD direct);

	void and_edi_pointto_by_direct(BYTE direct);

	void or_ebx_by_eax();
	void or_ebx_by_ecx();

	void or_eax_by_direct(BYTE direct);

	void or_eax_by_direct(DWORD direct);
	void or_edx_by_direct(DWORD direct);
	void or_esi_by_direct(DWORD direct);
	void or_edi_by_direct(DWORD direct);

	void xor_eax_by_eax();
	void xor_ebx_by_ebx();
	void xor_ecx_by_esp();
	void xor_edx_by_edx();
	void xor_esi_by_esi();
	void xor_edi_by_edi();

	void lea_eax_plus_to_edi(BYTE direct);
	void lea_ebx_plus_to_eax(BYTE direct);
	void lea_edx_plus_to_eax(BYTE direct);
	void lea_edx_plus_to_ecx(BYTE direct);
	void lea_edi_plus_to_esi(BYTE direct);

	void lea_ebp_minus_to_eax(BYTE direct);
	void lea_ebp_minus_to_ecx(BYTE direct);

	void ret(WORD size);
	void int_3();
	void int_29();

};