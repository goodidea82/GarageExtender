// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "CEntity_VTBL.h"


#pragma pack(push, 1)
struct CPhysical_VTBL	// sizeof = 0x5C
{
	CEntity_VTBL v;
	DWORD ProcessEntityCollision;
};
#pragma pack(pop)

static_assert(sizeof(CPhysical_VTBL) == 0x5C, "Incorrect struct size: CPhysical_VTBL");