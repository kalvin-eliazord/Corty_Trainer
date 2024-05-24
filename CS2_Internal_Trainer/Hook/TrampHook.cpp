#include "TrampHook.h"

TrampHook::TrampHook(intptr_t* pSrcAddr, intptr_t* pDstAddr, int pStolenBSize)
	: srcAddr{ pSrcAddr }
	, dstAddr{ pDstAddr }
	, stolenBSize{ pStolenBSize }
{
	bHooked = InitGateway(pSrcAddr, pDstAddr, pStolenBSize);
}

TrampHook::TrampHook(int pStolenBSize)
: stolenBSize{ pStolenBSize }
{
}

bool TrampHook::InitGateway(intptr_t* pSrcAddr, intptr_t* pDstAddr, int pStolenBSize)
{
	// Save source stolen bytes (for unhook later)
	DWORD oldSrcProtect{};
	VirtualProtect(pSrcAddr, pStolenBSize, PAGE_EXECUTE_READWRITE, &oldSrcProtect);
	memcpy(&stolenBytes, pSrcAddr, pStolenBSize);

	// NOP original Steam Hook instruction
	memset(pSrcAddr, 0x90, 5);
	VirtualProtect(pSrcAddr, pStolenBSize, oldSrcProtect, &oldSrcProtect);

	// Alloc Gateway into the game memory
	gatewayAddr = static_cast<intptr_t*>(VirtualAlloc(
		NULL,
		pStolenBSize + jmpSize,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE));

	if (!gatewayAddr) return false;

	// Put stolen bytes into gateway
	memcpy(gatewayAddr, pSrcAddr, pStolenBSize);

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
		reinterpret_cast<intptr_t*>(reinterpret_cast<intptr_t>(gatewayAddr) + pStolenBSize)
		, &jmpSrc
		, jmpSize);

	return HookSource(pSrcAddr, pDstAddr, pStolenBSize);
}

bool TrampHook::HookSource(intptr_t* pSrcAddr, intptr_t* pDstAddr, int pStolenBSize)
{
	DWORD oldProtect{};
	VirtualProtect(pSrcAddr, pStolenBSize, PAGE_EXECUTE_READWRITE, &oldProtect);

	BYTE jmpDst[14]
	{
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,				// JMP QWORD PTR instruction
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // 8 BYTES PTR to relative destination
	};

	// Set JMP relative Addr to destination
	memcpy(jmpDst + 6, &pDstAddr, 8);

	// NOP stolen bytes
	memset(pSrcAddr, 0x90, pStolenBSize);

	// Put JMP in source
	memcpy(pSrcAddr, jmpDst, jmpSize);

	VirtualProtect(pSrcAddr, pStolenBSize, oldProtect, &oldProtect);

	return true;
}

const bool& TrampHook::GetbHookRef()
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
		bHooked = false;

		DWORD oldProtect{};
		VirtualProtect(srcAddr, stolenBSize, PAGE_EXECUTE_READWRITE, &oldProtect);
		memcpy(srcAddr, &stolenBytes, stolenBSize);
		VirtualProtect(srcAddr, stolenBSize, oldProtect, &oldProtect);
	}
}

TrampHook::~TrampHook()
{
	Unhook();
}