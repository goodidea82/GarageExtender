// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "CTaskComplex.h"


#pragma pack(push, 1)
struct CTaskComplexMoveSeekEntity	// sizeof = 0x4C
{
	CTaskComplex __parent;
	DWORD m_pEntity;
	DWORD m_nTime;
	DWORD field_14;
	DWORD m_nRadius;
	DWORD field_1C;
	DWORD field_20;
	DWORD field_24;
	DWORD field_28;
	DWORD field_2C;
	BYTE field_30;
	BYTE field_31;
	BYTE _undefined_0;
	BYTE _undefined_1;
	DWORD field_34;
	DWORD field_38;
	BYTE field_3C;
	BYTE field_3D;
	BYTE _undefined_2;
	BYTE _undefined_3;
	DWORD m_posCalc;
	DWORD field_44;
	BYTE field_48;
	BYTE field_49[3];
};
#pragma pack(pop)

static_assert(sizeof(CTaskComplexMoveSeekEntity) == 0x4C, "Incorrect struct size: CTaskComplexMoveSeekEntity");