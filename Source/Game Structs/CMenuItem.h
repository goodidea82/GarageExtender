// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "CEntryData.h"


#pragma pack(push, 1)
struct CMenuItem	// sizeof = 0xE2
{
	BYTE name[8];
	BYTE field_8;
	BYTE field_9;
	CEntryData entryList[12];
};
#pragma pack(pop)

static_assert(sizeof(CMenuItem) == 0xE2, "Incorrect struct size: CMenuItem");