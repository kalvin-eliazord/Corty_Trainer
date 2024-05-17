#pragma once
#include <Windows.h>
#include <cstdint>

class TrampHook
{
private:
	const int jmpSize{ 14 };
	bool bHooked{false};

	intptr_t* srcAddr{};
	intptr_t stolenBytes;

	intptr_t* dstAddr{};
	intptr_t* gatewayAddr{};

	bool InitGateway(intptr_t* pSrcAddr, intptr_t* pDstAddr, int pStolenBSize);
	bool HookSource(intptr_t* pSrcAddr, intptr_t* pDstAddr, int pStolenBSize);
public:
	const intptr_t stolenBSize;

	TrampHook(intptr_t* pSrcAddr, intptr_t* pDstAddr, int pStolenBSize);
	void Unhook();
	bool IsHooked();
	~TrampHook();
	intptr_t* GetGateway();
};