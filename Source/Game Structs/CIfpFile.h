// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct CIfpFile	// sizeof = 0x20
{
	BYTE name[16];
	BYTE loaded;
	BYTE field_11;
	WORD usageCount;
	DWORD startAnimation;
	DWORD animationCount;
	DWORD animationStyle;
};
#pragma pack(pop)

static_assert(sizeof(CIfpFile) == 0x20, "Incorrect struct size: CIfpFile");