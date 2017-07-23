// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "RwObject.h"
#include "RwLLLink.h"
#include "RwLinkList.h"


#pragma pack(push, 1)
struct RpClump	// sizeof = 0x2C
{
	RwObject object;
	RwLinkList atomicList;
	RwLinkList lightList;
	RwLinkList cameraList;
	RwLLLink inWorldLink;
	DWORD callback;
};
#pragma pack(pop)

static_assert(sizeof(RpClump) == 0x2C, "Incorrect struct size: RpClump");