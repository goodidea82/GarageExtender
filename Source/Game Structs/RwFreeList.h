// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "RwLLLink.h"
#include "RwLinkList.h"


#pragma pack(push, 1)
struct RwFreeList	// sizeof = 0x24
{
	DWORD entrySize;
	DWORD entriesPerBlock;
	DWORD heapSize;
	DWORD alignment;
	RwLinkList blockList;
	DWORD flags;
	RwLLLink link;
};
#pragma pack(pop)

static_assert(sizeof(RwFreeList) == 0x24, "Incorrect struct size: RwFreeList");