// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct RsKeyStatus	// sizeof = 0x8
{
	DWORD keyScanCode;
	DWORD keyCharCode;
};
#pragma pack(pop)

static_assert(sizeof(RsKeyStatus) == 0x8, "Incorrect struct size: RsKeyStatus");