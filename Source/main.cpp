/**
	Garage eXtender
		by LINK/2012 (http://www.link2012world.blogspot.com/)

	Here is our entry point.
*/
#define _CRT_SECURE_NO_WARNINGS
#define IS_ADDRESSES_CPP			// Compile addresses references on this unit
#include <Windows.h>
#include <cstdio>
#include "Injector.h"

extern void ScriptCommands_Patch();
extern void GRGX_Patch();
extern void GRGX_Detach();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		GameVersion gv(SA);
		if(gv.Get() != gv.SA_10US)
		{
			char Buffer[128];
			sprintf(Buffer, "An incompatible exe version has been detected! (%s)\n"
								"Do you want to continue anyway? (Not recommended)", gv.GetString());
			if(MessageBoxA(0, Buffer, "GRGX", MB_YESNO|MB_ICONERROR|MB_DEFBUTTON2) == IDNO)
				return false;
		}

		GRGX_Patch();
		ScriptCommands_Patch();
	}
	else if(ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		GRGX_Detach();
	}
	return true;
}
