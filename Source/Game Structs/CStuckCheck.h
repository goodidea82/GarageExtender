// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "RwV3D.h"


#pragma pack(push, 1)
struct CStuckCheck	// sizeof = 0x24
{
	DWORD field_0;
	RwV3D field_4;
	DWORD field_10;
	DWORD field_14;
	DWORD field_18;
	BYTE field_1C;
	BYTE field_1D;
	BYTE field_1E;
	BYTE field_1F;
	BYTE field_20;
	BYTE field_21;
	BYTE field_22;
	BYTE field_23;
};
#pragma pack(pop)

static_assert(sizeof(CStuckCheck) == 0x24, "Incorrect struct size: CStuckCheck");