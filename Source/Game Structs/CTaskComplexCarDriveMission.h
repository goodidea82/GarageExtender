// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "CTaskComplexCarDrive.h"


#pragma pack(push, 1)
struct CTaskComplexCarDriveMission	// sizeof = 0x2C
{
	CTaskComplexCarDrive __parent;
	DWORD m_pTargetVehicle;
	DWORD field_28;
};
#pragma pack(pop)

static_assert(sizeof(CTaskComplexCarDriveMission) == 0x2C, "Incorrect struct size: CTaskComplexCarDriveMission");