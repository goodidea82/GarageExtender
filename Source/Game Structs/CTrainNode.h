// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct CTrainNode	// sizeof = 0xA
{
	WORD x;
	WORD y;
	WORD z;
	WORD RailDistance;
	BYTE field_8;
	BYTE field_9;
};
#pragma pack(pop)

static_assert(sizeof(CTrainNode) == 0xA, "Incorrect struct size: CTrainNode");