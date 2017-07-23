// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "OVERLAPPED.h"


#pragma pack(push, 1)
struct GTA_STREAM	// sizeof = 0x30
{
	DWORD nSectorOffset;
	DWORD nSectorsToRead;
	DWORD lpBuffer;
	BYTE field_C;
	BYTE bLocked;
	BYTE bInUse;
	BYTE field_F;
	DWORD status;
	DWORD hSemaphore;
	DWORD hFile;
	OVERLAPPED overlapped;
};
#pragma pack(pop)

static_assert(sizeof(GTA_STREAM) == 0x30, "Incorrect struct size: GTA_STREAM");