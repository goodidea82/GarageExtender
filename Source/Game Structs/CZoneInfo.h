// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "0x40.h"
#include "0x80.h"


#pragma pack(push, 1)
struct CZoneInfo	// sizeof = 0x11
{
	BYTE GangDensity[10];
	BYTE DrugDealerCounter;
	BYTE ZoneRGBA[4];
	BYTE Flags1;
	0x40 ;[=];
	0x80 ;[=];
	BYTE Flags2;
};
#pragma pack(pop)

static_assert(sizeof(CZoneInfo) == 0x11, "Incorrect struct size: CZoneInfo");