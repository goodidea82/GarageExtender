// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "CAiTimer.h"
#include "CTaskSimpleDriveCar.h"


#pragma pack(push, 1)
struct CTaskSimpleCarDriveTimed	// sizeof = 0x70
{
	CTaskSimpleDriveCar __parent;
	BYTE _undefined_0;
	BYTE _undefined_1;
	BYTE _undefined_2;
	DWORD field_60;
	CAiTimer field_64;
};
#pragma pack(pop)

static_assert(sizeof(CTaskSimpleCarDriveTimed) == 0x70, "Incorrect struct size: CTaskSimpleCarDriveTimed");