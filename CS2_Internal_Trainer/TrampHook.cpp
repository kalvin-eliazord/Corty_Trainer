#include "TrampHook.h"

TrampHook::TrampHook(intptr_t* pSrcAddr, intptr_t pStolenBytesSize, intptr_t* pDstAddr)
	: srcAddr{ pSrcAddr }
	, stolenBSize{pStolenBytesSize}
	, dstAddr{ pDstAddr }
{
	stolenBSize = (stolenBSize < jmpSize ? jmpSize : stolenBSize);

	if (InitGateway())
		if (HookSource()) bHooked = true;
}
void TrampHook::Unhook()
{
	if (bHooked)
	{
		DWORD oldProtect{};
		VirtualProtect(srcAddr, stolenBSize, PAGE_EXECUTE_READWRITE, &oldProtect);

		memcpy(srcAddr, gatewayAddr, stolenBSize);

		VirtualProtect(srcAddr, stolenBSize, oldProtect, &oldProtect);
	}
}

TrampHook::~TrampHook()
{
	Unhook();
}

bool TrampHook::InitGateway()
{
	gatewayAddr = reinterpret_cast<intptr_t*>(VirtualAlloc(
		NULL,
		stolenBSize + jmpSize,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE));

	if (!gatewayAddr) return false;

	memcpy(gatewayAddr, srcAddr, stolenBSize);

	BYTE jmpSrc[14]
	{
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,				// JMP QWORD PTR instruction
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // 8 BYTES PTR to relative destination
	};

	// Set JMP to source
	const intptr_t srcJmpBackAddr{ reinterpret_cast<intptr_t>(srcAddr + stolenBSize) };
	memcpy(jmpSrc + 6, &srcJmpBackAddr, 8);

	// Put JMP in gateway
	memcpy(gatewayAddr + stolenBSize, jmpSrc, jmpSize);

	return true;
}

bool TrampHook::HookSource()
{
	DWORD oldProtect{};
	VirtualProtect(srcAddr, stolenBSize, PAGE_EXECUTE_READWRITE, &oldProtect);

	BYTE jmpDst[14]
	{
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,				// JMP QWORD PTR instruction
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // 8 BYTES PTR to relative destination
	};

	// Set JMP to destination
	memcpy(jmpDst + 6, &dstAddr, 8);

	// Nop source
	memset(srcAddr, 0x90, jmpSize);

	// Put JMP in source
	memcpy(srcAddr, jmpDst, jmpSize);

	VirtualProtect(srcAddr, stolenBSize, oldProtect, &oldProtect);

	return true;
}

intptr_t* TrampHook::GetGateway()
{
	return gatewayAddr;
}