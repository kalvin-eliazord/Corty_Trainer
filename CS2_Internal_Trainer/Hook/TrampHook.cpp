#include "TrampHook.h"

TrampHook::TrampHook(intptr_t* pSrcAddr, intptr_t* pDstAddr)
	: srcAddr{ pSrcAddr }
	, dstAddr{ pDstAddr }
{
	bHooked = InitGateway(pSrcAddr, pDstAddr);
}

bool TrampHook::InitGateway(intptr_t* pSrcAddr, intptr_t* pDstAddr)
{
	// Save source stolen bytes (for unhook later)
	DWORD oldSrcProtect{};
	VirtualProtect(pSrcAddr, jmpSize, PAGE_EXECUTE_READWRITE, &oldSrcProtect);
	memcpy(&stolenBytes, pSrcAddr, jmpSize);

	// NOP original Steam Hook instruction
	memset(pSrcAddr, 0x90, 5);
	VirtualProtect(pSrcAddr, jmpSize, oldSrcProtect, &oldSrcProtect);

	// Alloc Gateway into the game memory
	gatewayAddr = static_cast<intptr_t*>(VirtualAlloc(
		NULL,
		(jmpSize * 2) + sizeof(bool),
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE));

	if (!gatewayAddr) return false;

	// Put stolen bytes into gateway
	memcpy(gatewayAddr, pSrcAddr, jmpSize);

	BYTE jmpSrc[14]
	{
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,				// JMP QWORD PTR instruction
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // 8 BYTES PTR to relative destination
	};

	// Set JMP Relative Address
	const intptr_t srcJmpBackAddr{ reinterpret_cast<intptr_t>(pSrcAddr) + jmpSize };
	memcpy(jmpSrc + 6, &srcJmpBackAddr, 8);

	// Put JMP in gateway
	memcpy(
		reinterpret_cast<intptr_t*>(reinterpret_cast<intptr_t>(gatewayAddr) + jmpSize)
		, &jmpSrc
		, jmpSize);

	return HookSource(pSrcAddr, pDstAddr);
}

bool TrampHook::HookSource(intptr_t* pSrcAddr, intptr_t* pDstAddr)
{
	DWORD oldProtect{};
	VirtualProtect(pSrcAddr, jmpSize, PAGE_EXECUTE_READWRITE, &oldProtect);

	BYTE jmpDst[14]
	{
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,				// JMP QWORD PTR instruction
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // 8 BYTES PTR to relative destination
	};

	// Set JMP relative Addr to destination
	memcpy(jmpDst + 6, &pDstAddr, 8);

	// NOP stolen bytes
	memset(pSrcAddr, 0x90, jmpSize);

	// Put JMP in source
	memcpy(pSrcAddr, jmpDst, jmpSize);

	VirtualProtect(pSrcAddr, jmpSize, oldProtect, &oldProtect);

	return true;
}

bool TrampHook::IsHooked()
{
	return bHooked;
}

intptr_t* TrampHook::GetGateway()
{
	return gatewayAddr;
}

void TrampHook::Unhook()
{
	if (bHooked)
	{
		DWORD oldProtect{};
		VirtualProtect(srcAddr, jmpSize, PAGE_EXECUTE_READWRITE, &oldProtect);

		memcpy(srcAddr, &stolenBytes, jmpSize);

		VirtualFree(gatewayAddr, 0, MEM_RELEASE); //TODO

		VirtualProtect(srcAddr, jmpSize, oldProtect, &oldProtect);

		bHooked = false;
	}
}

TrampHook::~TrampHook()
{
	Unhook();
}