// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "TransmissionGear.h"


#pragma pack(push, 1)
struct CTransmissionData	// sizeof = 0x68
{
	TransmissionGear gears[6];
	BYTE nDriveType;
	BYTE nEngineType;
	BYTE nNumberOfGears;
	BYTE field_4B;
	DWORD field_4C;
	DWORD fEngineAcceleration;
	DWORD fEngineInertia;
	DWORD fMaxVelocity;
	DWORD field_5C;
	DWORD field_60;
	DWORD field_64;
};
#pragma pack(pop)

static_assert(sizeof(CTransmissionData) == 0x68, "Incorrect struct size: CTransmissionData");