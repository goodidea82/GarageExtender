// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct RwObject	// sizeof = 0x8
{
	BYTE type;
	BYTE subType;
	BYTE flags;
	BYTE privateFlags;
	DWORD parent;
};
#pragma pack(pop)

static_assert(sizeof(RwObject) == 0x8, "Incorrect struct size: RwObject");