// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct CMouseState	// sizeof = 0x14
{
	BYTE lmb;
	BYTE rmb;
	BYTE mmb;
	BYTE wheelUp;
	BYTE wheelDown;
	BYTE bmx1;
	BYTE bmx2;
	BYTE __align;
	DWORD Z;
	DWORD X;
	DWORD Y;
};
#pragma pack(pop)

static_assert(sizeof(CMouseState) == 0x14, "Incorrect struct size: CMouseState");