// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "CTaskSimpleAnim.h"


#pragma pack(push, 1)
struct CTaskSimpleHandsignalAnim	// sizeof = 0x24
{
	CTaskSimpleAnim __parent;
	DWORD field_10;
	DWORD field_14;
	BYTE field_18;
	BYTE _undefined_0;
	BYTE _undefined_1;
	BYTE _undefined_2;
	DWORD field_1C;
	DWORD field_20;
};
#pragma pack(pop)

static_assert(sizeof(CTaskSimpleHandsignalAnim) == 0x24, "Incorrect struct size: CTaskSimpleHandsignalAnim");