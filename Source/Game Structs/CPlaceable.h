// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "Placement.h"


#pragma pack(push, 1)
struct CPlaceable	// sizeof = 0x18
{
	DWORD __vmt;
	Placement placement;
	DWORD m_pCoords;
};
#pragma pack(pop)

static_assert(sizeof(CPlaceable) == 0x18, "Incorrect struct size: CPlaceable");