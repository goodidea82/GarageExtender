// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct CDecisionMakers	// sizeof = 0x54
{
	BYTE field_0[20];
	WORD m_awUniqueID[20];
	BYTE m_abUsed[20];
	DWORD m_pMgr;
};
#pragma pack(pop)

static_assert(sizeof(CDecisionMakers) == 0x54, "Incorrect struct size: CDecisionMakers");