// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct PsGlobalType	// sizeof = 0x28
{
	DWORD window;
	DWORD instance;
	DWORD fullscreen;
	DWORD lastMousePos_X;
	DWORD lastMousePos_Y;
	DWORD field_14;
	DWORD diInterface;
	DWORD diMouse;
	DWORD diDevice1;
	DWORD diDevice2;
};
#pragma pack(pop)

static_assert(sizeof(PsGlobalType) == 0x28, "Incorrect struct size: PsGlobalType");