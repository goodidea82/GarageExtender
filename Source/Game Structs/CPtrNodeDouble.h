// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct CPtrNodeDouble	// sizeof = 0xC
{
	DWORD ptr;
	DWORD next;
	DWORD prev;
};
#pragma pack(pop)

static_assert(sizeof(CPtrNodeDouble) == 0xC, "Incorrect struct size: CPtrNodeDouble");