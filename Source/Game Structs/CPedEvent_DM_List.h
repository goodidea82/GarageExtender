// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "CPedEvent_DM_EventData.h"


#pragma pack(push, 1)
struct CPedEvent_DM_List	// sizeof = 0x99C
{
	CPedEvent_DM_EventData field_0[41];
};
#pragma pack(pop)

static_assert(sizeof(CPedEvent_DM_List) == 0x99C, "Incorrect struct size: CPedEvent_DM_List");