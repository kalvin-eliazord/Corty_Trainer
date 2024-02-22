#pragma once
#include "header.h"
#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, GameOffset) struct {unsigned char MAKE_PAD(GameOffset); type name;}

struct Entity
{
	union
	{
		//              Type     Name    GameOffset
		DEFINE_MEMBER_N(int32_t, health, 0x334);
		DEFINE_MEMBER_N(int32_t, team_variable, 0x3C8);
		DEFINE_MEMBER_N(Vector3, body_pos, 0xD60);
		DEFINE_MEMBER_N(Vector3, angles, 0x11A4);
	};
};