#pragma once
#include <cstdint>

struct Vector3 { float x, y, z; };

class Entity
{
public:
	char pad_0000[2664]; //0x0000
	int32_t N00000959; //0x0A68
	int32_t health; //0x0A6C
	char pad_0A70[152]; //0x0A70
	int64_t team_variable; //0x0B08
	char pad_0B10[456]; //0x0B10
	Vector3 body_pos; //0x0CD8
	char pad_0CE4[1128]; //0x0CE4
	Vector3 angles; //0x114C
	char pad_1158[2992]; //0x1158
};