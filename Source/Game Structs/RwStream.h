// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct RwStream	// sizeof = 0x24
{
	DWORD type;
	DWORD accessType;
	DWORD position;
	DWORD field_C;
	DWORD field_10;
	DWORD field_14;
	DWORD field_18;
	DWORD field_1C;
	DWORD rwOwned;
};
#pragma pack(pop)

static_assert(sizeof(RwStream) == 0x24, "Incorrect struct size: RwStream");