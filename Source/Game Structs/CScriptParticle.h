// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct CScriptParticle	// sizeof = 0x8
{
	BYTE bUsed;
	BYTE _undefined_0;
	WORD wUniqueID;
	DWORD field_4;
};
#pragma pack(pop)

static_assert(sizeof(CScriptParticle) == 0x8, "Incorrect struct size: CScriptParticle");