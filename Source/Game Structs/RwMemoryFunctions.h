// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct RwMemoryFunctions	// sizeof = 0x10
{
	DWORD rwmalloc;
	DWORD rwfree;
	DWORD rwrealloc;
	DWORD rwcalloc;
};
#pragma pack(pop)

static_assert(sizeof(RwMemoryFunctions) == 0x10, "Incorrect struct size: RwMemoryFunctions");