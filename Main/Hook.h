#pragma once

#include <windows.h>
#include "CodeGen.h"
#include <vector>


typedef DWORD SkipOffset;

typedef struct _Register_Info {
	DWORD eax;
	DWORD ebx;
	DWORD ecx;
	DWORD edx;
	DWORD esi;
	DWORD edi;
	DWORD esp;
	DWORD ebp;
}Register_Info, * PRegister_Info;

class Hook {
#define RS_EAX (BYTE)1
#define RS_EBX (BYTE)2
#define RS_ECX (BYTE)4
#define RS_EDX (BYTE)8
#define RS_ESI (BYTE)16
#define RS_EDI (BYTE)32
#define RS_ESP (BYTE)64
#define RS_EBP (BYTE)128
#define RS_ALL (BYTE)255

#define REGISTER_VALUE_LENGTH 4
#define REGISTER_SET_INDICATOR_LENGTH 1
#define REGISTER_NUM 8


#define PUSH_INSTRUCTION_LENGTH 1
#define POP_INSTRUCTION_LENGTH 1
#define MOVE_REGISTER_TO_MEM_INSTRUCTION_LENGTH 5
#define MOVE_MEM_TO_REGISTER_INSTRUCTION_LENGTH 5
#define CALL_FROM_EAX_INSTRUCTION_LENGTH 2

#define JUMP_INSTRUCTION_LENGTH 10

#define HOOK_INSTRUCTION_LENGTH2  ( 5+6*7 +2 +5+2 +7+2+5+6+2+2+5+2+5+5 +2 +7+2+5 +(7+2+6)*7 )
#define JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH 10
	//content length
#define JUMP_TO_ORIGINAL_INSTRUCTION_LENGTH 10
#define REGISTER_VALUE_FOR_GET_LENGTH 32
#define REGISTER_VALUE_FOR_SET_INDICATOR_LENGTH 8
#define REGISTER_VALUE_FOR_SET_LENGTH 32
#define CONTENT_SIZE_LENTH 4
#define SKIP_OFFSET_LENTH 4
#define SKIP_OFFSET_BY_RETURN_ENABLED_LENTH 1

	typedef void (*CodeGenerator)(CodeGen& cg);
	typedef void (*CodeGeneratorWithTarget)(CodeGen& cg, CodeAddress target);

	typedef SkipOffset(*HookBody)(void);

	static std::vector<Hook*> hooks;
	
	
	SRWLOCK srwLock;

	void* from;	// Address of the original function
	void* hookBody;		// Address of the function to call 
	unsigned int contentSize;
	bool jumpAtEnd;

	bool hooked;
	//UINT hookType;

	void* bridgePtr;   // Address of the instruction bridge
	void* hookInstructionPtr;
	void* jumpToSkipOffsetPtr;
	void* contentPtr;
	void* jumpToOriginalPtr;
	void* registerInfoToGetPtr;
	void* registerInfoToSetIndicatorPtr;
	void* registerInfoToSetPtr;
	void* contentSizePtr;
	void* skipOffsetPtr;
	void* skipOffsetByReturnEnabledPtr;
	UINT bridgeSize;

	void* CreateBridge(void* from, void* hookBody, unsigned int contentSize, bool jumpAtEnd);

	bool Set(void* baseAddress, DWORD offset, DWORD value);
	bool Set(void* address, DWORD value);

	static void SetJump(CodeGen& cg, CodeAddress jumpTo);

	void SetHook(void* originalFunction, void* bridgeAddress, void* newFunction, UINT contentSize);
	//void SetJump(void* jumpTo);
	void SetContents(void* baseAddress, void* from, UINT contentSize);
	void SetDefaultRegisterIndicator(void* baseAddress);
	void SetObjectAddress(void* originalPlaceToHoldObjectAddress, void* newObjectAddress);
	void SetCode(void* originalAddress, UINT contentSize, CodeGenerator codeGenerator, bool setNOPToEnd);
	void SetCode(void* originalAddress, UINT contentSize, CodeGeneratorWithTarget codeGenerator, void* target, bool setNOPToEnd);

public:
	Hook();
	Hook(void* from, void* hookFunction, unsigned int contentSize, bool jumpAtEnd = false);

	bool hook();
	bool unhook();
	bool isHooked();
	bool getRegisterInfo(Register_Info& rInfo);

	static void Dispose();
};