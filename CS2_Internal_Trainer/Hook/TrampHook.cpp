#include "TrampHook.h"

TrampHook::TrampHook(intptr_t* pSrcAddr, intptr_t* pDstAddr, int pStolenBytesSize)
	: srcAddr{ pSrcAddr }
	, stolenBSize{pStolenBytesSize}
	, dstAddr{ pDstAddr }
{
	if (InitGateway())
		if (HookSource()) bHooked = true;
}
void TrampHook::Unhook()
{
	if (bHooked)
	{
		DWORD oldProtect{};
		VirtualProtect(srcAddr, jmpSize, PAGE_EXECUTE_READWRITE, &oldProtect);

		memcpy(srcAddr, &stolenBytes, jmpSize);

		VirtualFree(gatewayAddr, 0, MEM_RELEASE);

		VirtualProtect(srcAddr, jmpSize, oldProtect, &oldProtect);
	}
}

TrampHook::~TrampHook()
{
	Unhook();
}

bool TrampHook::InitGateway()
{
	DWORD oldProtect{};
	VirtualProtect(srcAddr, jmpSize, PAGE_EXECUTE_READWRITE, &oldProtect);
	// save stb
	memcpy(&stolenBytes, srcAddr, jmpSize);
	memset(srcAddr, 0x90, 5);
	VirtualProtect(srcAddr, jmpSize, oldProtect, &oldProtect);

	gatewayAddr = static_cast<intptr_t*>(VirtualAlloc(
		NULL,
		jmpSize*2,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE));

	if (!gatewayAddr) return false;

	// Put stolen bytes into gateway
	memcpy(gatewayAddr, srcAddr, jmpSize);

	BYTE jmpSrc[14]
	{
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,				// JMP QWORD PTR instruction
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // 8 BYTES PTR to relative destination
	};

	// Set JMP to source
	const intptr_t srcJmpBackAddr{ reinterpret_cast<intptr_t>(srcAddr) + jmpSize };
	memcpy(jmpSrc + 6, &srcJmpBackAddr, 8);

	// Put JMP in gateway
	memcpy(
		reinterpret_cast<intptr_t*>(reinterpret_cast<intptr_t>(gatewayAddr) + jmpSize)
		, &jmpSrc
		, jmpSize);

	return true;
}

bool TrampHook::HookSource()
{
	DWORD oldProtect{};
	VirtualProtect(srcAddr, jmpSize, PAGE_EXECUTE_READWRITE, &oldProtect);

	BYTE jmpDst[14]
	{
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,				// JMP QWORD PTR instruction
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // 8 BYTES PTR to relative destination
	};

	// Set JMP to destination
	memcpy(jmpDst + 6, &dstAddr, 8);

	// Nop source stolen bytes
	memset(srcAddr, 0x90, jmpSize);

	// Put JMP in source
	memcpy(srcAddr, jmpDst, jmpSize);

	VirtualProtect(srcAddr, jmpSize, oldProtect, &oldProtect);

	return true;
}

intptr_t* TrampHook::GetGateway()
{
	return gatewayAddr;
}