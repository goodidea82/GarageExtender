// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "CSurfaceType.h"


#pragma pack(push, 1)
struct CSurface	// sizeof = 0x8F4
{
	DWORD surface[36];
	CSurfaceType surfType[179];
};
#pragma pack(pop)

static_assert(sizeof(CSurface) == 0x8F4, "Incorrect struct size: CSurface");