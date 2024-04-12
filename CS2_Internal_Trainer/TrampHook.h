#pragma once
#include <Windows.h>
#include <cstdint>
class TrampHook
{
private:
	const int jmpSize{ 14 };
	bool bHooked{false};

	intptr_t* srcAddr{};
	intptr_t stolenBSize{};

	intptr_t* dstAddr{};
	intptr_t* gatewayAddr{};

	bool InitGateway();
	bool HookSource();

public:
	TrampHook(intptr_t* pSrcAddr, intptr_t pStolenBytesSize, intptr_t* pDstAddr);
	void Unhook();
	~TrampHook();
	intptr_t* GetGateway();
};