// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct RpPlaneSector	// sizeof = 0x18
{
	DWORD type;
	DWORD value;
	DWORD leftSubTree;
	DWORD rightSubTree;
	DWORD leftValue;
	DWORD rightValue;
};
#pragma pack(pop)

static_assert(sizeof(RpPlaneSector) == 0x18, "Incorrect struct size: RpPlaneSector");