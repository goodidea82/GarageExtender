/**
	by LINK/2012 (http://www.link2012world.blogspot.com/)
*/
#define _CRT_SECURE_NO_WARNINGS
#include "CSaveSystem.h"

uint8_t CSaveSystem::slot;
void* CSaveSystem::hook_53C6EF_ptr = (void*)(0x53C6F5);
void* CSaveSystem::hook_5D16DB_ptr = (void*)(0x8213AE);
void* CSaveSystem::hook_5D18E1_ptr = (void*)(0x5D1EA0);
void* CSaveSystem::hook_53C784_ptr = (void*)(0x561B10);
void* CSaveSystem::hook_5D14BD_ptr = (void*)(0x5D1270);
std::list<CSaveSystem::CallbackPrototype> CSaveSystem::callbacks;
SYSTEMTIME CSaveSystem::save_time;


//===================================================================================

void CSaveSystem::Init()
{
	static bool first_time = true;
	if(first_time)
	{
		first_time = false;
		CSaveSystem::DoPatch();
	}
}

void CSaveSystem::AddCallback(CSaveSystem::CallbackPrototype callback)
{
	Init();
	CSaveSystem::callbacks.push_front(callback);
}

void CSaveSystem::RemCallback(CSaveSystem::CallbackPrototype callback)
{
	CSaveSystem::callbacks.remove(callback);
}


//===================================================================================


void CSaveSystem::DoPatch()
{
	void* p53C6EF = MakeJMP(0x53C6EF, &CSaveSystem::HOOK_GetLoadSlot);
	MakeNOP(0x53C6F4, 1);

	// Make hooks for notification, store the original call!
	void* p53C784 = MakeCALL(0x53C784, &CSaveSystem::HOOK_Loading);
	void* p5D16DB = MakeCALL(0x5D16DB, &CSaveSystem::HOOK_Saving);
	void* p5D18E1 = MakeCALL(0x5D18E1, &CSaveSystem::HOOK_GetTime_ld);
	void* p5D14BD = MakeCALL(0x5D14BD, &CSaveSystem::HOOK_GetTime_sv);

	// store the original calls
	if(p53C784) CSaveSystem::hook_53C784_ptr = p53C784;
	if(p5D16DB) CSaveSystem::hook_5D16DB_ptr = p5D16DB;
	if(p5D18E1) CSaveSystem::hook_5D18E1_ptr = p5D18E1;
	if(p5D14BD) CSaveSystem::hook_5D14BD_ptr = p5D14BD;
	if(p53C6EF) CSaveSystem::hook_53C6EF_ptr = p53C6EF;
}

void __declspec(naked) CSaveSystem::HOOK_GetLoadSlot()
{
	_asm
	{
		push ecx
		mov ecx, 0xBA6748
		mov al, byte ptr [ecx+0x60] // menu.field_60
		test al, al
		jnz _IsLoad

		//_IsNew:
			mov CSaveSystem::slot, -1
			jmp _Return

		_IsLoad:
			mov al, byte ptr [ecx+0x15F]
			mov CSaveSystem::slot, al

		_Return:
			mov al, byte ptr [ecx+0x60]
			pop ecx
			test al, al
			jmp dword ptr [CSaveSystem::hook_53C6EF_ptr]
	}
}

void __declspec(naked) CSaveSystem::HOOK_Loading()
{
	_asm
	{
		pushad
		call CSaveSystem::ManifestOnLoad
		popad
		jmp dword ptr [CSaveSystem::hook_53C784_ptr]
	}
}

void __declspec(naked)  CSaveSystem::HOOK_Saving()
{
	_asm
	{
		mov eax, dword ptr [esp+4]
		pushad
		push eax
		call CSaveSystem::ManifestOnSave
		add esp, 4
		popad
		jmp dword ptr [CSaveSystem::hook_5D16DB_ptr]
	}
}


void __declspec(naked) CSaveSystem::HOOK_GetTime_sv()
{
	_asm
	{
		push dword ptr [CSaveSystem::hook_5D14BD_ptr]
		push eax
		jmp CSaveSystem::HOOK_GetTime
	}
}

void __declspec(naked) CSaveSystem::HOOK_GetTime_ld()
{
	_asm
	{
		push dword ptr [CSaveSystem::hook_5D18E1_ptr]
		push ecx
		jmp CSaveSystem::HOOK_GetTime
	}
}

// Used to get the save_time when loading
void __declspec(naked)  CSaveSystem::HOOK_GetTime()	// nocall noreturn (void* savebuf, void* jmp_to)
{
	_asm
	{
		pop eax		// void* savebuf
		pushad
		push offset CSaveSystem::save_time
		push eax
		call CSaveSystem::GetTimeFromSaveBuffer
		add esp, 8
		popad
		retn		// jump to [void* jmp_to] and pop it
	}
}



SYSTEMTIME& CSaveSystem::GetTimeFromSaveBuffer(void* buffer, SYSTEMTIME* output)
{
	*output = *(SYSTEMTIME*)((uint8_t*)buffer + 0x011E);
	return *output;
}

void CSaveSystem::ManifestOnLoad()
{
	CSaveSystem::Action action;
	CSaveSystem::SaveInfo info;

	action = CSaveSystem::slot != -1? CSaveSystem::ACTION_LOADGAME : CSaveSystem::ACTION_NEWGAME;
	if(action == CSaveSystem::ACTION_NEWGAME)
	{
		info.playing_time = 0; info.slot = -1;
		memset(&info.time, 0, sizeof(info.time));
	}
	else
	{
		info.slot = CSaveSystem::slot;
		info.playing_time = *(uint32_t*)(0xB7CB84);
		info.time = CSaveSystem::GetLastSaveTime();
	}

	for(auto it = CSaveSystem::callbacks.begin(); it != CSaveSystem::callbacks.end(); ++it)
	{
		(**it)(action, info);
	}
}

void CSaveSystem::ManifestOnSave()
{
	CSaveSystem::SaveInfo info;
	info.playing_time = *(uint32_t*)(0xB7CB84);
	info.slot = *((uint8_t*)(0xBA6748) + 0x15F);;
	info.time = CSaveSystem::GetLastSaveTime();

	for(auto it = CSaveSystem::callbacks.begin(); it != CSaveSystem::callbacks.end(); ++it)
	{
		(**it)(CSaveSystem::ACTION_SAVEGAME, info);
	}
}



/*
-------------- callback used for testing

void callback(CSaveSystem::Action action, const CSaveSystem::SaveInfo& info)
{
	FILE* f = fopen("log.txt", "w");
	fprintf(f,	"Called with\n"
				"Action = %d\n"
				"Slot = %d\n"
				"Playing time = %u\n"
				"Time = %d:%d:%d:%d - %d\\%d\\%d - dw %d\n"
				"\n\n\n",	
		action,  info.slot, info.playing_time,
		info.time.wHour, info.time.wMinute, info.time.wSecond, info.time.wMilliseconds,
		info.time.wDay, info.time.wMonth, info.time.wYear, info.time.wDayOfWeek);
	
	fclose(f);
	return;
}
*/