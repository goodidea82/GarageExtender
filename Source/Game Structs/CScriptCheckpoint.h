// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct CScriptCheckpoint	// sizeof = 0x8
{
	BYTE bUsed;
	BYTE field_1;
	WORD wUniqueID;
	DWORD field_4;
};
#pragma pack(pop)

static_assert(sizeof(CScriptCheckpoint) == 0x8, "Incorrect struct size: CScriptCheckpoint");