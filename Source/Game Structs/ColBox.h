// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "ColSurface.h"
#include "RwBBox.h"


#pragma pack(push, 1)
struct ColBox	// sizeof = 0x1C
{
	RwBBox boxes;
	ColSurface surface;
};
#pragma pack(pop)

static_assert(sizeof(ColBox) == 0x1C, "Incorrect struct size: ColBox");