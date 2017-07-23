/**
	by LINK/2012 (http://www.link2012world.blogspot.com/)
	--------------------
		This class has the objective to provide a save system interface with hooking only "once", so compatibility with other plugin is good
		Feel free to use :)

	How to use:
		Simple, just call CSaveSystem::AddCallback(FunctionToBeCalledWhen_Saving_Loading_Or_NewGame);
		To remove a callback use CSaveSystem::RemCallback(callback);

		The callback prototype must be "void CallbackPrototype(CSaveSystem::Action action, const CSaveSystem::SaveInfo& info)"
		action is what is going on now (e.g.: CSaveSystem::ACTION_NEWGAME, CSaveSystem::ACTION_LOADGAME, CSaveSystem::ACTION_SAVEGAME)
		info is useful informations about the saving process (e.g.: save slot, saving time (local time), playing time)
*/
#pragma once
#include <Windows.h>
#include <list>
#include "Injector.h"

// this class only contain static data\methods
class CSaveSystem
{
	// Internal Functions	- "Not" interesting for the user
	private:
		// the following static vars store the "original" call pointer
		static void* hook_53C6EF_ptr;
		static void* hook_5D16DB_ptr;
		static void* hook_5D18E1_ptr;
		static void* hook_53C784_ptr;
		static void* hook_5D14BD_ptr;

		static uint8_t slot;
		static SYSTEMTIME save_time;

		static void DoPatch();
		static void HOOK_GetLoadSlot();
		static void HOOK_Saving();
		static void HOOK_Loading();
		static void HOOK_GetTime();
		static void HOOK_GetTime_sv();
		static void HOOK_GetTime_ld();
		static SYSTEMTIME& GetTimeFromSaveBuffer(void* buffer, SYSTEMTIME* output = &CSaveSystem::save_time);
		static void ManifestOnLoad();
		static void ManifestOnSave();

	// Interesting for the user
	public:
		// Better to pass a struct to the callback,
		// because if I have to add more information I won't broken backward compatibility
		struct SaveInfo
		{
			uint8_t slot;			// >> 0 based << (e.g.: slot 1 in menu is stored as 0 in this var)
			SYSTEMTIME time;		// local time
			uint32_t playing_time;	// in miliseconds. you can see this on stats menu in hour:minutes format
		};
		enum Action { ACTION_NEWGAME, ACTION_LOADGAME, ACTION_SAVEGAME };
		typedef void (*CallbackPrototype)(Action action, const SaveInfo& info);


	private:
		static std::list<CallbackPrototype> callbacks;

	public:
		// Init() is called on AddCallback(), so you don't have to worry about it
		static void Init();

		static void AddCallback(CallbackPrototype callback);
		static void RemCallback(CallbackPrototype callback);
		static const SYSTEMTIME& GetLastSaveTime(){return CSaveSystem::save_time;}
};


