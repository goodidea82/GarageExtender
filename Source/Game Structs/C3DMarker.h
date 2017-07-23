// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "RwV3D.h"
#include "RwRGBA.h"
#include "CMatrix_Padded.h"


#pragma pack(push, 1)
struct C3DMarker	// sizeof = 0xA0
{
	CMatrix_Padded m_mat;
	DWORD dwPad;
	DWORD dwPad2;
	DWORD m_pRwObject;
	DWORD m_pMaterial;
	WORD m_nType;
	BYTE m_bIsUsed;
	BYTE _undefined_0;
	DWORD m_nIdentifier;
	RwRGBA rwColour;
	WORD m_nPulsePeriod;
	WORD m_nRotateRate;
	DWORD m_nStartTime;
	DWORD m_fPulseFraction;
	DWORD m_fStdSize;
	DWORD m_fSize;
	DWORD m_fBrightness;
	DWORD m_fCameraRange;
	RwV3D m_normal;
	WORD m_LastMapReadX;
	WORD m_LastMapReadY;
	DWORD m_LastMapReadResultZ;
	DWORD m_roofHeight;
	RwV3D m_lastPosition;
	DWORD m_OnScreenTestTime;
};
#pragma pack(pop)

static_assert(sizeof(C3DMarker) == 0xA0, "Incorrect struct size: C3DMarker");