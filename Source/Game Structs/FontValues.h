// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>


#pragma pack(push, 1)
struct FontValues	// sizeof = 0xD2
{
	BYTE proportionalValues[208];
	BYTE replacementSpaceChar;
	BYTE unproportionalValue;
};
#pragma pack(pop)

static_assert(sizeof(FontValues) == 0xD2, "Incorrect struct size: FontValues");