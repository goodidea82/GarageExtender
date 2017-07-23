// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct RwPluginEntry	// sizeof = 0x3C
{
	DWORD offset;
	DWORD size;
	DWORD pluginID;
	DWORD readCB;
	DWORD writeCB;
	DWORD getSizeCB;
	DWORD alwaysCB;
	DWORD rightsCB;
	DWORD constructCB;
	DWORD destructCB;
	DWORD copyCB;
	DWORD errStrCB;
	DWORD nextRegEntry;
	DWORD prevRegEntry;
	DWORD parentRegistry;
};
#pragma pack(pop)

static_assert(sizeof(RwPluginEntry) == 0x3C, "Incorrect struct size: RwPluginEntry");