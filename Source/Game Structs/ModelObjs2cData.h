// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "CModelObjs2c.h"


#pragma pack(push, 1)
struct ModelObjs2cData	// sizeof = 0x9DC
{
	DWORD count;
	CModelObjs2c objects[70];
};
#pragma pack(pop)

static_assert(sizeof(ModelObjs2cData) == 0x9DC, "Incorrect struct size: ModelObjs2cData");