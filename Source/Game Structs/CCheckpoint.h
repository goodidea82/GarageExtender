// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "RwRGBA.h"
#include "RwV3D.h"


#pragma pack(push, 1)
struct CCheckpoint	// sizeof = 0x38
{
	WORD m_nType;
	BYTE m_bIsUsed;
	BYTE m_rotFlag;
	DWORD m_nIdentifier;
	RwRGBA rwColour;
	WORD m_nPulsePeriod;
	WORD m_nRotateRate;
	RwV3D m_pos;
	RwV3D m_pointDir;
	DWORD m_fPulseFraction;
	DWORD m_fSize;
	DWORD m_fCameraRange;
	DWORD m_multiSize;
};
#pragma pack(pop)

static_assert(sizeof(CCheckpoint) == 0x38, "Incorrect struct size: CCheckpoint");