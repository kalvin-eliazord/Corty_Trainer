#pragma once
#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

struct Controller
{
	union
	{
		DEFINE_MEMBER_N(char*, name, 0x638); 
		DEFINE_MEMBER_N(int, pawnOffset, 0x7e4);
		//DEFINE_MEMBER_N(int, isAliveStatus, 0x7ec);
	};
};