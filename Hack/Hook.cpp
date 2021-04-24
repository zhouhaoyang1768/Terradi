#include "Hook.h"
#include "Util.h"

bool Hook::Set(void* baseAddress, DWORD offset, DWORD value) {
	if (baseAddress == nullptr) return false;
	else return Set((void*)((DWORD)baseAddress + offset), value);
}
bool Hook::Set(void* address, DWORD value) {
	if (IsBadReadPtr((const void*)address, NULL)) return false;
	DWORD dwOldProtect = 0;
	bool vProtected = false;

	do {
		vProtected = VirtualProtect(address, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		Sleep(50);
	} while (!vProtected);
	*(DWORD*)(address) = value;
	DWORD dwBuf = 0;
	VirtualProtect(address, 4, dwOldProtect, &dwBuf);

	return true;
}

void* Hook::CreateBridge(void* from, void* hookBody, unsigned int contentSize, bool jumpAtEnd) {
	unsigned int bridgeSize = HOOK_INSTRUCTION_LENGTH2
		+ JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH
		+ contentSize
		+ JUMP_TO_ORIGINAL_INSTRUCTION_LENGTH
		+ REGISTER_VALUE_FOR_GET_LENGTH
		+ REGISTER_VALUE_FOR_SET_INDICATOR_LENGTH
		+ REGISTER_VALUE_FOR_SET_LENGTH
		+ CONTENT_SIZE_LENTH
		+ SKIP_OFFSET_LENTH
		+ SKIP_OFFSET_BY_RETURN_ENABLED_LENTH;

	void* pBridgeBuffer = VirtualAlloc(NULL, bridgeSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (pBridgeBuffer == NULL) return NULL;

	//set new function call
	SetHook(from, (BridgeAddress)pBridgeBuffer, hookBody, contentSize);

	//set bridge contents
	SetContents((CodeAddress)((BYTE*)pBridgeBuffer + HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH), (CodeAddress)from, contentSize);

	//back to original function call
	SetCode((CodeAddress)((BYTE*)pBridgeBuffer + HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH + contentSize), JUMP_INSTRUCTION_LENGTH, SetJump, (CodeAddress)(DWORD(from) + contentSize), false);

	//in case new function will not set any register
	SetDefaultRegisterIndicator((BYTE*)pBridgeBuffer + HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH + contentSize + JUMP_TO_ORIGINAL_INSTRUCTION_LENGTH + REGISTER_VALUE_FOR_GET_LENGTH);

	//set content size, to decide if need to skip in bridge or original function
	Set(pBridgeBuffer, HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH + contentSize + JUMP_TO_ORIGINAL_INSTRUCTION_LENGTH + REGISTER_VALUE_FOR_GET_LENGTH + REGISTER_VALUE_FOR_SET_INDICATOR_LENGTH + REGISTER_VALUE_FOR_SET_LENGTH, (DWORD)contentSize);

	//in case new function will not skip any instruction SetDefaultSkipOffset to 0
	Set(pBridgeBuffer, HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH + contentSize + JUMP_TO_ORIGINAL_INSTRUCTION_LENGTH + REGISTER_VALUE_FOR_GET_LENGTH + REGISTER_VALUE_FOR_SET_INDICATOR_LENGTH + REGISTER_VALUE_FOR_SET_LENGTH + CONTENT_SIZE_LENTH, 0);

	//set skipOffsetByReturnEnabled
	Set(pBridgeBuffer, HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH + contentSize + JUMP_TO_ORIGINAL_INSTRUCTION_LENGTH + REGISTER_VALUE_FOR_GET_LENGTH + REGISTER_VALUE_FOR_SET_INDICATOR_LENGTH + REGISTER_VALUE_FOR_SET_LENGTH + CONTENT_SIZE_LENTH + SKIP_OFFSET_LENTH, jumpAtEnd);

	return (void*)pBridgeBuffer;
}

void Hook::SetJump(CodeGen& cg, CodeAddress jumpTo) {
	cg.jmp(jumpTo);
}
void Hook::SetHook(CodeAddress originalFunction, BridgeAddress bridgeAddress, CodeAddress newFunction, UINT contentSize) {
	/*
500c0000 a3fc000c50      mov     dword ptr ds:[500C00FCh],eax
500c0005 891d00010c50    mov     dword ptr ds:[500C0100h],ebx
500c000b 890d04010c50    mov     dword ptr ds:[500C0104h],ecx
500c0011 891508010c50    mov     dword ptr ds:[500C0108h],edx
500c0017 89350c010c50    mov     dword ptr ds:[500C010Ch],esi
500c001d 893d10010c50    mov     dword ptr ds:[500C0110h],edi
500c0023 892514010c50    mov     dword ptr ds:[500C0114h],esp
500c0029 892d18010c50    mov     dword ptr ds:[500C0118h],ebp
500c002f 60              pushad
500c0030 9c              pushfd
500c0031 b87074b97b      mov     eax,offset Hack!CommandManager::TimeHook (7bb97470)
500c0036 ffd0            call    eax
500c0038 803d4c010c5001  cmp     byte ptr ds:[500C014Ch],1
500c003f 7405            je      500c0046
500c0041 a148010c50      mov     eax,dword ptr ds:[500C0148h]
500c0046 8b1d44010c50    mov     ebx,dword ptr ds:[500C0144h]
500c004c 3bc3            cmp     eax,ebx
500c004e 7f07            jg      500c0057
500c0050 05e4000c50      add     eax,500C00E4h
500c0055 eb05            jmp     500c005c
500c0057 0531207e23      add     eax,237E2031h
500c005c a3da000c50      mov     dword ptr ds:[500C00DAh],eax
500c0061 9d              popfd
500c0062 61              popad
500c0063 803d1c010c5001  cmp     byte ptr ds:[500C011Ch],0
500c006a 7405            je      500c0071
500c006c a124010c50      mov     eax,dword ptr ds:[500C0124h]
500c0071 803d1d010c5001  cmp     byte ptr ds:[500C011Dh],0
500c0078 7406            je      500c0080
500c007a 8b1d28010c50    mov     ebx,dword ptr ds:[500C0128h]
500c0080 803d1e010c5001  cmp     byte ptr ds:[500C011Eh],0
500c0087 7406            je      500c008f
500c0089 8b0d2c010c50    mov     ecx,dword ptr ds:[500C012Ch]
500c008f 803d1f010c5001  cmp     byte ptr ds:[500C011Fh],0
500c0096 7406            je      500c009e
500c0098 8b1530010c50    mov     edx,dword ptr ds:[500C0130h]
500c009e 803d20010c5001  cmp     byte ptr ds:[500C0120h],0
500c00a5 7406            je      500c00ad
500c00a7 8b3534010c50    mov     esi,dword ptr ds:[500C0134h]
500c00ad 803d21010c5001  cmp     byte ptr ds:[500C0121h],0
500c00b4 7406            je      500c00bc
500c00b6 8b3d38010c50    mov     edi,dword ptr ds:[500C0138h]
500c00bc 803d22010c5001  cmp     byte ptr ds:[500C0122h],0
500c00c3 7406            je      500c00cb
500c00c5 8b253c010c50    mov     esp,dword ptr ds:[500C013Ch]
500c00cb 803d23010c5001  cmp     byte ptr ds:[500C0123h],0
500c00d2 7406            je      500c00da
500c00d4 8b2d40010c50    mov     ebp,dword ptr ds:[500C0140h]
500c00da ff25e0000c50    jmp     dword ptr ds:[500C00E0h]
500c00e0 da00            fiadd   dword ptr [eax]
500c00e2 0c50            or      al,50h


		*/
		/*
		skipOffsetPtr = (DWORD*)((DWORD)baseAddress + HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH + contentSize + JUMP_TO_ORIGINAL_INSTRUCTION_LENGTH + REGISTER_VALUE_FOR_GET_LENGTH + REGISTER_VALUE_FOR_SET_INDICATOR_LENGTH + REGISTER_VALUE_FOR_SET_LENGTH + CONTENT_SIZE_LENTH);
		skipOffsetByReturnPtr = (BYTE*)((DWORD)baseAddress + HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH + contentSize + JUMP_TO_ORIGINAL_INSTRUCTION_LENGTH + REGISTER_VALUE_FOR_GET_LENGTH + REGISTER_VALUE_FOR_SET_INDICATOR_LENGTH + REGISTER_VALUE_FOR_SET_LENGTH + CONTENT_SIZE_LENTH + SKIP_OFFSET_LENTH);
			contentSizePtr = (DWORD*)((DWORD)baseAddress + HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH + contentSize + JUMP_INSTRUCTION_LENGTH + REGISTER_VALUE_FOR_GET_LENGTH);
			jumpToPtr = (DWORD*)((DWORD)baseAddress + HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH + contentSize + JUMP_INSTRUCTION_LENGTH + REGISTER_VALUE_FOR_GET_LENGTH + REGISTER_VALUE_FOR_SET_INDICATOR_LENGTH);
			orig = (DWORD*)originalFunction;
			base = (DWORD*)baseAddress;
			__asm {
				mov     dword ptr ds : [1D800FCh] , eax
				mov     dword ptr ds : [1D80100h] , ebx
				mov     eax, dword ptr ds : [1D80144h]
				mov     ebx, dword ptr ds : [1D80144h]
				mov eax, 20;
				cmp BYTE PTR [skipOffsetByReturnPtr], 1
				je tmp0
				mov eax, skipOffsetPtr
		tmp0:
				mov ebx, contentSizePtr
				cmp     eax, ebx
				jg tmp
				add eax, 0x007452CD
				jmp tmp2
		tmp:
				add eax, 0x88ff6600
		tmp2:
				mov dword ptr jumpToPtr, eax
				popfd
				popad
				jmp jumpToPtr


				mov al, 2
				mov dx, 0xD007
				out dx, al
			}
			/**/
	
	DWORD dwOldProtect = 0;
	bool vProtected = false;

	do {
		vProtected = VirtualProtect(bridgeAddress, HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		Sleep(50);
	} while (!vProtected);

	CodeGen CG(bridgeAddress);
	DataAddress rMem = (DWORD*)((DWORD)bridgeAddress + HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH + contentSize + JUMP_TO_ORIGINAL_INSTRUCTION_LENGTH);

	CG.GenStart();
	//save register info to mem 5+6*7
	CG.mov_eax_to_mem((DataAddress)((DWORD)rMem + REGISTER_VALUE_LENGTH * 0));
	CG.mov_ebx_to_mem((DataAddress)((DWORD)rMem + REGISTER_VALUE_LENGTH * 1));
	CG.mov_ecx_to_mem((DataAddress)((DWORD)rMem + REGISTER_VALUE_LENGTH * 2));
	CG.mov_edx_to_mem((DataAddress)((DWORD)rMem + REGISTER_VALUE_LENGTH * 3));
	CG.mov_esi_to_mem((DataAddress)((DWORD)rMem + REGISTER_VALUE_LENGTH * 4));
	CG.mov_edi_to_mem((DataAddress)((DWORD)rMem + REGISTER_VALUE_LENGTH * 5));
	CG.mov_esp_to_mem((DataAddress)((DWORD)rMem + REGISTER_VALUE_LENGTH * 6));
	CG.mov_ebp_to_mem((DataAddress)((DWORD)rMem + REGISTER_VALUE_LENGTH * 7));
	//save register info to stack 2
	CG.push_ad();
	CG.push_fd();
	//call new function 5+2
	CG.mov_direct_to_eax((DWORD)newFunction);
	CG.call_eax();
	//skipOffsetByReturn? 7
	CG.cmp_mem_by_byte((DWORD*)((DWORD)bridgeAddress + HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH + contentSize + JUMP_TO_ORIGINAL_INSTRUCTION_LENGTH + REGISTER_VALUE_FOR_GET_LENGTH + REGISTER_VALUE_FOR_SET_INDICATOR_LENGTH + REGISTER_VALUE_FOR_SET_LENGTH + CONTENT_SIZE_LENTH + SKIP_OFFSET_LENTH), 0x01);
	//if skipOffsetByReturn, goto //get contentSize 2
	CG.je((BYTE)0x05);
	//if not skipOffsetByReturn, check skipOffset by SetSkipOffset 5
	CG.mov_mem_to_eax((DWORD*)((DWORD)bridgeAddress + HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH + contentSize + JUMP_TO_ORIGINAL_INSTRUCTION_LENGTH + REGISTER_VALUE_FOR_GET_LENGTH + REGISTER_VALUE_FOR_SET_INDICATOR_LENGTH + REGISTER_VALUE_FOR_SET_LENGTH + CONTENT_SIZE_LENTH));
	//get contentSize 6
	CG.mov_mem_to_ebx((DWORD*)((DWORD)bridgeAddress + HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH + contentSize + JUMP_TO_ORIGINAL_INSTRUCTION_LENGTH + REGISTER_VALUE_FOR_GET_LENGTH + REGISTER_VALUE_FOR_SET_INDICATOR_LENGTH + REGISTER_VALUE_FOR_SET_LENGTH));
	//compare skipOffset with contentSize 2+2
	CG.cmp_eax_by_ebx();
	CG.jg((BYTE)0x07);
	//set skip in bridge 5+2
	CG.add_eax_by_direct((DWORD)bridgeAddress + HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH);
	CG.jmp((BYTE)0x05);
	//set skip in originalFunction 5
	CG.add_eax_by_direct((DWORD)originalFunction);
	//save target address to skip(jump) 5
	CG.mov_eax_to_mem((DWORD*)((DWORD)bridgeAddress + HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH - ADDRESS_LENGTH));

	//restore register info from stack 2
	CG.pop_fd();
	CG.pop_ad();

	BYTE* rIndicator = (BYTE*)((DWORD)bridgeAddress + HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH + contentSize + JUMP_TO_ORIGINAL_INSTRUCTION_LENGTH + REGISTER_VALUE_FOR_GET_LENGTH);
	BYTE* rToSet = (BYTE*)((DWORD)bridgeAddress + HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH + contentSize + JUMP_TO_ORIGINAL_INSTRUCTION_LENGTH + REGISTER_VALUE_FOR_GET_LENGTH + REGISTER_VALUE_FOR_SET_INDICATOR_LENGTH);

	//7+2+5
	CG.cmp_mem_by_byte(rIndicator + 0 * REGISTER_SET_INDICATOR_LENGTH, 0x00);
	CG.je((BYTE)0x05);
	CG.mov_mem_to_eax(rToSet + 0 * REGISTER_VALUE_LENGTH);

	//7+2+6
	CG.cmp_mem_by_byte(rIndicator + 1 * REGISTER_SET_INDICATOR_LENGTH, 0x00);
	CG.je((BYTE)0x06);
	CG.mov_mem_to_ebx(rToSet + 1 * REGISTER_VALUE_LENGTH);

	CG.cmp_mem_by_byte(rIndicator + 2 * REGISTER_SET_INDICATOR_LENGTH, 0x00);
	CG.je((BYTE)0x06);
	CG.mov_mem_to_ecx(rToSet + 2 * REGISTER_VALUE_LENGTH);

	CG.cmp_mem_by_byte(rIndicator + 3 * REGISTER_SET_INDICATOR_LENGTH, 0x00);
	CG.je((BYTE)0x06);
	CG.mov_mem_to_edx(rToSet + 3 * REGISTER_VALUE_LENGTH);

	CG.cmp_mem_by_byte(rIndicator + 4 * REGISTER_SET_INDICATOR_LENGTH, 0x00);
	CG.je((BYTE)0x06);
	CG.mov_mem_to_esi(rToSet + 4 * REGISTER_VALUE_LENGTH);

	CG.cmp_mem_by_byte(rIndicator + 5 * REGISTER_SET_INDICATOR_LENGTH, 0x00);
	CG.je((BYTE)0x06);
	CG.mov_mem_to_edi(rToSet + 5 * REGISTER_VALUE_LENGTH);

	CG.cmp_mem_by_byte(rIndicator + 6 * REGISTER_SET_INDICATOR_LENGTH, 0x00);
	CG.je((BYTE)0x06);
	CG.mov_mem_to_esp(rToSet + 6 * REGISTER_VALUE_LENGTH);

	CG.cmp_mem_by_byte(rIndicator + 7 * REGISTER_SET_INDICATOR_LENGTH, 0x00);
	CG.je((BYTE)0x06);
	CG.mov_mem_to_ebp(rToSet + 7 * REGISTER_VALUE_LENGTH);

	//jump/skip to target address
	CG.jmp((CodeAddress)((DWORD)bridgeAddress + HOOK_INSTRUCTION_LENGTH2));

	CG.GenEnd();

	DWORD dwBuf = 0;	// nessary othewrise the function fails
	VirtualProtect(bridgeAddress, HOOK_INSTRUCTION_LENGTH2 + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH, dwOldProtect, &dwBuf);

}
void Hook::SetContents(CodeAddress baseAddress, CodeAddress source, UINT contentSize) {

	DWORD dwOldProtect = 0;
	bool vProtected = false;
	do {
		vProtected = VirtualProtect(baseAddress, contentSize, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		Sleep(50);
	} while (!vProtected);

	BYTE* to = (BYTE*)baseAddress;
	BYTE* from = (BYTE*)source;

	for (UINT i = 0; i < contentSize; i++) {
		*to = *from;
		to++;
		from++;
	}

	DWORD dwBuf = 0;	// nessary othewrise the function fails
	VirtualProtect(baseAddress, contentSize, dwOldProtect, &dwBuf);
}
void Hook::SetDefaultRegisterIndicator(CodeAddress baseAddress) {
	DWORD dwOldProtect = 0;
	bool vProtected = false;
	do {
		vProtected = VirtualProtect(baseAddress, REGISTER_NUM * REGISTER_SET_INDICATOR_LENGTH, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		Sleep(50);
	} while (!vProtected);

	BYTE* to = (BYTE*)baseAddress;

	for (int i = 0; i < REGISTER_NUM; i++) {
		*to = false;
		to += REGISTER_SET_INDICATOR_LENGTH;
	}

	DWORD dwBuf = 0;	// nessary othewrise the function fails
	VirtualProtect(baseAddress, REGISTER_NUM * REGISTER_SET_INDICATOR_LENGTH, dwOldProtect, &dwBuf);
}

void Hook::SetObjectAddress(CodeAddress originalPlaceToHoldObjectAddress, Address newObjectAddress) {
	SetContents(originalPlaceToHoldObjectAddress, &newObjectAddress, ADDRESS_LENGTH);
}
void Hook::SetCode(CodeAddress originalAddress, UINT contentSize, CodeGenerator codeGenerator, bool setNOPToEnd) {

	DWORD dwOldProtect = 0;
	bool vProtected = false;
	do {
		vProtected = VirtualProtect(originalAddress, contentSize, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		Sleep(50);
	} while (!vProtected);

	CodeGen CG(originalAddress, (CodeAddress)((DWORD)originalAddress + contentSize - 1), setNOPToEnd);
	CG.GenStart();
	if (codeGenerator) codeGenerator(CG);
	CG.GenEnd();

	DWORD dwBuf = 0;	// nessary othewrise the function fails
	VirtualProtect(originalAddress, contentSize, dwOldProtect, &dwBuf);
}
void Hook::SetCode(CodeAddress originalAddress, UINT contentSize, CodeGeneratorWithTarget codeGenerator, CodeAddress target, bool setNOPToEnd) {

	DWORD dwOldProtect = 0;
	bool vProtected = false;
	do {
		vProtected = VirtualProtect(originalAddress, contentSize, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		Sleep(50);
	} while (!vProtected);

	CodeGen CG(originalAddress, (CodeAddress)((DWORD)originalAddress + contentSize - 1), setNOPToEnd);
	CG.GenStart();
	if (codeGenerator) codeGenerator(CG, target);
	CG.GenEnd();

	DWORD dwBuf = 0;	// nessary othewrise the function fails
	VirtualProtect(originalAddress, contentSize, dwOldProtect, &dwBuf);
}






std::vector<Hook*> Hook::hooks = {};

Hook::Hook() {

}

Hook::Hook(void* from, void* hookBody, unsigned int contentSize, bool jumpAtEnd) {
	hooked = false;

	this->from = from;
	this->hookBody = hookBody;
	this->contentSize = contentSize;
	this->jumpAtEnd = jumpAtEnd;

	hooks.push_back(this);

	InitializeSRWLock(&srwLock);
}

bool Hook::hook() {
	AcquireSRWLockExclusive(&srwLock);

	

	if (hooked) {
		ReleaseSRWLockExclusive(&srwLock);
		tout << "Hook failed 1" << tendl;
		return false;
	}
	

	
	void* pBridge = CreateBridge(from, hookBody, contentSize, jumpAtEnd);


	if (pBridge == NULL) {
		ReleaseSRWLockExclusive(&srwLock);
		tout << "Hook failed 2" << tendl;
		return false;
	}

	//HOOK_INFO hookInfo;
	//hookInfo.orignalAddress = originalFunction;
	//hookInfo.newAddress = newFunction;
	//hookInfo.contentSize = contentSize;
	//hookInfo.hookType = HT_FUNCTION;
	bridgePtr = pBridge;
	hookInstructionPtr = bridgePtr;
	jumpToSkipOffsetPtr = (void*)((DWORD)hookInstructionPtr + HOOK_INSTRUCTION_LENGTH2);
	contentPtr = (void*)((DWORD)jumpToSkipOffsetPtr + JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH);
	jumpToOriginalPtr = (void*)((DWORD)contentPtr + contentSize);
	registerInfoToGetPtr = (void*)((DWORD)jumpToOriginalPtr + JUMP_TO_ORIGINAL_INSTRUCTION_LENGTH);
	registerInfoToSetIndicatorPtr = (void*)((DWORD)registerInfoToGetPtr + REGISTER_VALUE_FOR_GET_LENGTH);
	registerInfoToSetPtr = (void*)((DWORD)registerInfoToSetIndicatorPtr + REGISTER_VALUE_FOR_SET_INDICATOR_LENGTH);
	contentSizePtr = (void*)((DWORD)registerInfoToSetPtr + REGISTER_VALUE_FOR_SET_LENGTH);
	skipOffsetPtr = (void*)((DWORD)contentSizePtr + CONTENT_SIZE_LENTH);
	skipOffsetByReturnEnabledPtr = (void*)((DWORD)skipOffsetPtr + SKIP_OFFSET_LENTH);
	bridgeSize = HOOK_INSTRUCTION_LENGTH2
		+ JUMP_SKIP_OFFSET_INSTRUCTION_LENGTH
		+ contentSize
		+ JUMP_TO_ORIGINAL_INSTRUCTION_LENGTH
		+ REGISTER_VALUE_FOR_GET_LENGTH
		+ REGISTER_VALUE_FOR_SET_INDICATOR_LENGTH
		+ REGISTER_VALUE_FOR_SET_LENGTH
		+ CONTENT_SIZE_LENTH
		+ SKIP_OFFSET_LENTH
		+ SKIP_OFFSET_BY_RETURN_ENABLED_LENTH;

	SetCode(from, contentSize, SetJump, pBridge, true);

	ReleaseSRWLockExclusive(&srwLock);

	hooked = true;
	return true;
}

bool Hook::unhook() {
	if (hooked)
	{
		AcquireSRWLockExclusive(&srwLock);
		SetContents(from, contentPtr, contentSize);
		VirtualFree((void*)bridgePtr, 0, MEM_RELEASE);
		ReleaseSRWLockExclusive(&srwLock);
	}

	hooked = false;
	return true;
}

bool Hook::isHooked() {
	return hooked;
}

bool Hook::getRegisterInfo(Register_Info& rInfo) {
	Address registerInfoToGetPtr;
	bool returnValue = false;

	AcquireSRWLockExclusive(&srwLock);
	if (this->registerInfoToGetPtr != nullptr) {
		registerInfoToGetPtr = this->registerInfoToGetPtr;
		rInfo.eax = *(DWORD*)((DWORD)registerInfoToGetPtr + REGISTER_VALUE_LENGTH * 0);
		rInfo.ebx = *(DWORD*)((DWORD)registerInfoToGetPtr + REGISTER_VALUE_LENGTH * 1);
		rInfo.ecx = *(DWORD*)((DWORD)registerInfoToGetPtr + REGISTER_VALUE_LENGTH * 2);
		rInfo.edx = *(DWORD*)((DWORD)registerInfoToGetPtr + REGISTER_VALUE_LENGTH * 3);
		rInfo.esi = *(DWORD*)((DWORD)registerInfoToGetPtr + REGISTER_VALUE_LENGTH * 4);
		rInfo.edi = *(DWORD*)((DWORD)registerInfoToGetPtr + REGISTER_VALUE_LENGTH * 5);
		rInfo.esp = *(DWORD*)((DWORD)registerInfoToGetPtr + REGISTER_VALUE_LENGTH * 6);
		rInfo.ebp = *(DWORD*)((DWORD)registerInfoToGetPtr + REGISTER_VALUE_LENGTH * 7);
		returnValue = true;
	}
	ReleaseSRWLockExclusive(&srwLock);

	return returnValue;
}

void Hook::Dispose() {
	for (int i = 0; i < hooks.size(); i++)
		hooks[i]->unhook();
}