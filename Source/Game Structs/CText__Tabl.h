// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "TablEntry.h"


#pragma pack(push, 1)
struct CText__Tabl	// sizeof = 0x964
{
	TablEntry data[200];
	WORD size;
	WORD __pad;
};
#pragma pack(pop)

static_assert(sizeof(CText__Tabl) == 0x964, "Incorrect struct size: CText__Tabl");