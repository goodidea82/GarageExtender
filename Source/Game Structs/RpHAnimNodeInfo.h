// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct RpHAnimNodeInfo	// sizeof = 0x10
{
	DWORD nodeID;
	DWORD nodeIndex;
	DWORD flags;
	DWORD pFrame;
};
#pragma pack(pop)

static_assert(sizeof(RpHAnimNodeInfo) == 0x10, "Incorrect struct size: RpHAnimNodeInfo");