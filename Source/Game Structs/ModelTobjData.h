// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "CModelTobj.h"


#pragma pack(push, 1)
struct ModelTobjData	// sizeof = 0x17C8
{
	DWORD count;
	CModelTobj objects[169];
};
#pragma pack(pop)

static_assert(sizeof(ModelTobjData) == 0x17C8, "Incorrect struct size: ModelTobjData");