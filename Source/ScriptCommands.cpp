/**
	Garage eXtender
		by LINK/2012 (http://www.link2012world.blogspot.com/)

	This file describe all the script commands (opcodes) for Garage eXtender,
	including the "reborn" from the III\VC opcodes into San Andreas
*/
#define _CRT_SECURE_NO_WARNINGS
#include "Injector.h"
#include "CCustomGarages.h"
#include "CPool.h"

enum GARAGE_COMMANDS
{
	GARAGE_COMMANDS_START							= -10000,
	DESTROY_THIS_GARAGE								= -10001,
	IS_GARAGE_CREATED_BY_SCRIPT						= -10002,
	DOES_THIS_GARAGE_CAN_STORE_VEHICLES				= -10003,
	IS_PLAYER_ENTIRELY_INSIDE_GARAGE				= -10004,
	IS_ANY_CAR_BLOCKING_GARAGE_DOOR					= -10005,
	REMOVE_CARS_BLOCKING_GARAGE_DOOR_NOT_INSIDE		= -10006,
	GARAGE_PUSH_OUT_VEHICLES						= -10007,
	GARAGE_PUSH_OUT_VEHICLES_BUT_NOT_TARGET			= -10008,
	IS_ANYTHING_TOUCHING_GARAGE						= -10009,
	IS_ANY_CAR_TOUCHING_GARAGE						= -10010,
	IS_ANY_PED_TOUCHING_GARAGE						= -10011,
	IS_ANY_CAR_TOUCHING_GARAGE_BUT_NOT_TARGET		= -10012,
	IS_ANY_PED_TOUCHING_GARAGE_BUT_NOT_PLAYER		= -10013,
	IS_STATIC_PLAYER_CAR_ENTIRELY_INSIDE_GARAGE		= -10014,
	IS_GARAGE_A_HIDEOUT								= -10015,
	CLEAR_STORED_CARS_IN_THIS_GARAGE				= -10016,
	TIDY_UP_GARAGE									= -10017,
	TIDY_UP_GARAGE_CLOSE							= -10018,
	GARAGE_STYLE_OPENS_UP							= -10019,
	GARAGE_STYLE_ROTATE								= -10020,
	GARAGE_STYLE_GOES_IN							= -10021,
	GARAGE_STYLE_OPENS_SLOW							= -10022,
	GARAGE_STYLE_OPENS_TOSIDE						= -10023,
	DOES_GARAGE_EXISTS								= -10024,
	SET_GARAGE_DEPPENDS_ON_THIS_THREAD				= -10025,
	SET_GARAGE_DONT_SAVE							= -10026,
};

void GARAGE_COMMAND(CRunningScript* script, CCustomGarage* g, int cmd)
{
	uint32_t flag = -1;	// if cmd is GARAGE_STYLE_? set this value to the style flag value
						// and in the end of the switch we're going to handle it.

	switch(cmd)
	{
		case DESTROY_THIS_GARAGE:
		{
			if(g->CreatedByScript) CGarages::DestroyGarage(g);
			break;
		}

		case IS_GARAGE_CREATED_BY_SCRIPT:
		{
			CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, g->CreatedByScript);
			break;
		}

		case DOES_THIS_GARAGE_CAN_STORE_VEHICLES:
		{
			CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, g->DoesThisGarageCanStoreVehicles()!=0);
			break;
		}

		case IS_PLAYER_ENTIRELY_INSIDE_GARAGE:
		{
			CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, g->IsPlayerEntirelyInside()!=0);
			break;
		}
		case IS_ANY_CAR_BLOCKING_GARAGE_DOOR:
		{
			CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, g->IsAnyCarBlockingDoor()!=0);
			break;
		}

		case REMOVE_CARS_BLOCKING_GARAGE_DOOR_NOT_INSIDE:
		{
			g->RemoveCarsBlockingDoorNotInside();
			break;
		}

		case GARAGE_PUSH_OUT_VEHICLES:
		{
			g->PushOutVehicles(nullptr);
			break;
		}

		case GARAGE_PUSH_OUT_VEHICLES_BUT_NOT_TARGET:
		{
			g->PushOutVehicles(g->pTargetCar);
			break;
		}

		case IS_ANYTHING_TOUCHING_GARAGE:
		{
			CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, g->IsAnythingTouchingGarage()!=0);
			break;
		}
		case IS_ANY_CAR_TOUCHING_GARAGE:
		{
			CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, g->IsAnyOtherCarTouchingGarage(nullptr)!=0);
			break;
		}

		case IS_ANY_PED_TOUCHING_GARAGE:
		{
			CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, g->IsAnyOtherPedTouchingGarage(nullptr)!=0);
			break;
		}

		case IS_ANY_CAR_TOUCHING_GARAGE_BUT_NOT_TARGET:
		{
			CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, g->IsAnyOtherCarTouchingGarage(g->pTargetCar)!=0);
			break;
		}

		case IS_ANY_PED_TOUCHING_GARAGE_BUT_NOT_PLAYER:
		{
			CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, g->IsAnyOtherPedTouchingGarage(FindPlayerPed(-1))!=0);
			break;
		}

		case IS_STATIC_PLAYER_CAR_ENTIRELY_INSIDE_GARAGE:
		{
			CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, g->IsStaticPlayerCarEntirelyInside()!=0);
			break;
		}

		case IS_GARAGE_A_HIDEOUT:
		{
			CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, g->IsThisGarageAHideout()!=0);
			break;
		}

		case CLEAR_STORED_CARS_IN_THIS_GARAGE:
		{
			g->ClearStoredCars();
			break;
		}

		case TIDY_UP_GARAGE:
		{
			g->TidyUpGarage();
			break;
		}

		case TIDY_UP_GARAGE_CLOSE:
		{
			g->TidyUpGarageClose();
			break;
		}

		case SET_GARAGE_DEPPENDS_ON_THIS_THREAD:
		{
			if(g->CreatedByScript)
			{
				flag = GARAGE_DEPPENDS_ON_THREAD;
				g->thread = script;
				strcpy(g->thread_name, script->threadName);
			}
			break;
		}

		case SET_GARAGE_DONT_SAVE:
		{
			if(g->CreatedByScript) flag = GARAGE_DONT_SAVE;
			break;
		}

		case GARAGE_STYLE_OPENS_UP:
		{
			flag = DOOR_STYLE_OPENS_UP;
			break;
		}

		case GARAGE_STYLE_ROTATE:
		{
			flag = DOOR_STYLE_ROTATES;
			break;
		}

		case GARAGE_STYLE_GOES_IN:
		{
			flag = DOOR_STYLE_GOES_IN;
			break;
		}

		case GARAGE_STYLE_OPENS_SLOW:
		{
			flag = DOOR_STYLE_OPENS_SLOW;
			break;
		}

		case GARAGE_STYLE_OPENS_TOSIDE:
		{
			flag = DOOR_STYLE_OPENS_TOSIDE;
			break;
		}
	}

	// Handle GARAGE_STYLE_? commands
	if(flag != -1)
	{
		if(script->notFlag)
			g->gStyle &= ~flag;
		else
			g->gStyle |= flag;
	}
}

void __declspec(naked) IS_GARAGE_OPEN()
{
	CRunningScript* script;
	char name[12];

	ASM_PROLOG()
	_asm mov script, esi
	{
		CTheScripts::CRunningScript__CollectStringParameter(script, 0, name, 8);
		name[7] = 0;

		CGarage* g = CGarages::FindGarageByNameGlobal(name);
		CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, g && CGarages::IsGarageOpen(g));
	}
	ASM_EPILOG()
	_asm push 0x485194
	_asm retn
}

void __declspec(naked) IS_GARAGE_CLOSED()
{
	CRunningScript* script;
	char name[12];

	ASM_PROLOG()
	_asm mov script, esi
	{
		CTheScripts::CRunningScript__CollectStringParameter(script, 0, name, 8);
		name[7] = 0;

		CGarage* g = CGarages::FindGarageByNameGlobal(name);
		CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, g && CGarages::IsGarageClosed(g));
	}
	ASM_EPILOG()
	_asm push 0x485194
	_asm retn
}



void __declspec(naked) SET_TARGET_CAR_FOR_MISSION_GARAGE()
{
	CRunningScript* script;
	CVehicle* veh;
	char name[12];

	ASM_PROLOG()
	_asm mov script, esi
	{
		CTheScripts::CRunningScript__CollectStringParameter(script, 0, name, 8);
		name[7] = 0;
		
		uint32_t param = CTheScripts::CRunningScript__CollectNextParameter(script).dwParam;
		CGarage* g = CGarages::FindGarageByNameGlobal(name);
		
		if(g)
		{
			if(param == -1 || (veh = CPools::VehiclePool->GetAt(param)) == 0)
				CGarages::SetTargetCarForMissionGarage(g, nullptr);
			else
				CGarages::SetTargetCarForMissionGarage(g, veh);
		}
	}
	ASM_EPILOG()
	_asm push 0x47E7FE
	_asm retn
}


void __declspec(naked) ACTIVATE_GARAGE()
{
	CRunningScript* script;
	char name[12];

	ASM_PROLOG()
	_asm mov script, esi
	{
		CTheScripts::CRunningScript__CollectStringParameter(script, 0, name, 8);
		name[7] = 0;	
		CGarage* g = CGarages::FindGarageByNameGlobal(name);
		if(g) CGarages::ActivateGarage(g);
	}
	ASM_EPILOG()
	_asm push 0x47F4E5
	_asm retn
}

void __declspec(naked) DEACTIVATE_GARAGE()
{
	CRunningScript* script;
	char name[12];

	ASM_PROLOG()
	_asm mov script, esi
	{
		CTheScripts::CRunningScript__CollectStringParameter(script, 0, name, 8);
		name[7] = 0;	
		CGarage* g = CGarages::FindGarageByNameGlobal(name);
		if(g) CGarages::DeActivateGarage(g);
	}
	ASM_EPILOG()
	_asm push 0x47F4E5
	_asm retn
}

void __declspec(naked) OPEN_GARAGE()
{
	CRunningScript* script;
	char name[12];

	ASM_PROLOG()
	_asm mov script, esi
	{
		CTheScripts::CRunningScript__CollectStringParameter(script, 0, name, 8);
		name[7] = 0;	
		CGarageBase* g = static_cast<CGarageBase*>(CGarages::FindGarageByNameGlobal(name));
		if(g) g->OpenThisGarage();
	}
	ASM_EPILOG()
	_asm push 0x4835B1
	_asm retn
}

void __declspec(naked) CLOSE_GARAGE()
{
	CRunningScript* script;
	char name[12];

	ASM_PROLOG()
	_asm mov script, esi
	{
		CTheScripts::CRunningScript__CollectStringParameter(script, 0, name, 8);
		name[7] = 0;	
		CGarageBase* g = static_cast<CGarageBase*>(CGarages::FindGarageByNameGlobal(name));
		if(g) g->CloseThisGarage();
	}
	ASM_EPILOG()
	_asm push 0x4835B1
	_asm retn
}

void __declspec(naked) CHANGE_GARAGE_TYPE()
{
	CRunningScript* script;
	char name[12];

	ASM_PROLOG()
	_asm mov script, esi
	{
		CTheScripts::CRunningScript__CollectStringParameter(script, 0, name, 8);
		name[7] = 0;	
		CGarage* g = CGarages::FindGarageByNameGlobal(name);
		int type = CTheScripts::CRunningScript__CollectNextParameter(script).nParam;

		if(g)
		{
			if(type <= GARAGE_COMMANDS_START && CGarages::IsGRGX(g))
			{
				if(type == DOES_GARAGE_EXISTS)
					CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, true);
				else
					GARAGE_COMMAND(script, static_cast<CCustomGarage*>(g), type);
			}
			else
				CGarages::ChangeGarageType(g, type);
		}
		else if(type == DOES_GARAGE_EXISTS)
			CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, false);
	}
	ASM_EPILOG()
	_asm push 0x480A9F
	_asm retn
}

void __declspec(naked) HAS_RESPRAY_HAPPENED()
{
	CRunningScript* script;
	char name[12];

	_asm
	{
		cmp eax, 8
		jz _Continue
		movzx eax, ds:0x4836B8[eax]
		push 0x481354
		retn

		_Continue:
	}

	ASM_PROLOG()
	_asm mov script, esi
	{
		CTheScripts::CRunningScript__CollectStringParameter(script, 0, name, 8);
		name[7] = 0;	
		CGarage* g = CGarages::FindGarageByNameGlobal(name);
		CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, g && CGarages::HasResprayHappened(g));
	}
	ASM_EPILOG()
	_asm push 0x4835B1
	_asm retn
}

void __declspec(naked) CREATE_GARAGE()
{
	CRunningScript* script;
	char name[12];

	_asm
	{
		cmp eax, 37
		jz _Continue
		movzx eax, ds:0x47F304[eax]
		push 0x47E0CA
		retn

		_Continue:
	}

	ASM_PROLOG()
	_asm mov script, esi
	{
		CTheScripts::CRunningScript__CollectParameters(script, 0, 9);
		
		RwV3D vec1 =
			{CTheScripts::CollectiveArray[0].fParam, CTheScripts::CollectiveArray[1].fParam, CTheScripts::CollectiveArray[2].fParam};
		RwV3D depth =
			{CTheScripts::CollectiveArray[3].fParam, CTheScripts::CollectiveArray[4].fParam, 0.0};
		RwV3D vec2 =
			{CTheScripts::CollectiveArray[5].fParam, CTheScripts::CollectiveArray[6].fParam, CTheScripts::CollectiveArray[7].fParam};

		CTheScripts::CRunningScript__CollectStringParameter(script, 0, name, 8);
		name[7] = 0;

		CCustomGarage* g = CGarages::AddOneX(vec1.x, vec1.y, vec1.z, depth.x, depth.y, vec2.x, vec2.y, vec2.z, -1, 0, name,
			0, -1, IS_NULL_GARAGE, DOOR_STYLE_OPENS_UP, true);

		CGarages::ChangeGarageType(g, CTheScripts::CollectiveArray[8].nParam);
		g->OriginalType = (uint8_t)(g->gType);
	}
	ASM_EPILOG()
	_asm push 0x47EA5E
	_asm retn
}

void __declspec(naked) SET_GARAGE_DOOR_TO_OPEN_UP_AND_ROTATE()
{
	CRunningScript* script;
	char name[12];

	ASM_PROLOG()
	_asm mov script, esi
	{
		CTheScripts::CRunningScript__CollectStringParameter(script, 0, name, 8);
		name[7] = 0;
		CCustomGarage* g = static_cast<CCustomGarage*>(CGarages::FindGarageByName(name));
		if(g) g->gStyle = DOOR_STYLE_OPENS_UP|DOOR_STYLE_ROTATES
							| (g->gStyle & (GARAGE_DEPPENDS_ON_THREAD|GARAGE_DONT_SAVE));
	}
	ASM_EPILOG()
	_asm xor al, al
	_asm push 0x48569B
	_asm retn
}

void __declspec(naked) SET_GARAGE_CAMERA_TO_FOLLOW_PLAYER()
{
	CRunningScript* script;
	char name[12];

	ASM_PROLOG()
	_asm mov script, esi
	{
		CTheScripts::CRunningScript__CollectStringParameter(script, 0, name, 8);
		name[7] = 0;
		CCustomGarage* g = static_cast<CCustomGarage*>(CGarages::FindGarageByName(name));
		if(g) g->Flags |= FLAG_GARAGE_CAMERA_FOLLOWS_PLAYER;
	}
	ASM_EPILOG()
	_asm xor al, al
	_asm push 0x48569B
	_asm retn
}

void __declspec(naked) OP2()
{
	_asm
	{
		cmp eax, 55
		jz _OpenUpRotate
		cmp eax, 86
		jz _FollowPlayer
		movzx eax, ds:0x4857C4[eax]
		push 0x483C0A
		retn

		_OpenUpRotate:
			jmp SET_GARAGE_DOOR_TO_OPEN_UP_AND_ROTATE

		_FollowPlayer:
			jmp SET_GARAGE_CAMERA_TO_FOLLOW_PLAYER
	}
}

void __declspec(naked) SET_GARAGE_MAX_CARS()
{
	CRunningScript* script;
	char name[12];

	_asm
	{
		cmp eax, -1
		jz _Continue
		push 0x490C0F
		retn

		_Continue:
	}

	ASM_PROLOG()
	_asm mov script, esi
	{
		CTheScripts::CRunningScript__CollectStringParameter(script, 0, name, 8);
		int i = CTheScripts::CRunningScript__CollectNextParameter(script).nParam;
		name[7] = 0;	
		CCustomGarage* g = static_cast<CCustomGarage*>(CGarages::FindGarageByName(name));
		if(g && g->DoesThisGarageCanStoreVehicles()) g->ResizeThisGarage(i);
	}
	ASM_EPILOG()
	_asm xor al, al
	_asm push 0x490C11
	_asm retn
}

void __declspec(naked) IS_CAR_IN_MISSION_GARAGE()
{
	CRunningScript* script;
	char name[12];

	ASM_PROLOG()
	_asm mov script, esi
	{
		if(*((WORD*)(script->_ip) - 1) == 0x021C)
		{
			CTheScripts::CRunningScript__CollectStringParameter(script, 0, name, 8);
			name[7] = 0;	
			CGarage* g = CGarages::FindGarageByNameGlobal(name);
			CTheScripts::CRunningScript__UpdateCompareFlag(script, 0, g && CGarages::HasCarBeenDroppedOffYet(g));
		}
	}
	ASM_EPILOG()
	_asm push 0x47F21F
	_asm retn
}


void ScriptCommands_Patch()
{
	MakeJMP(0x4847AD, &IS_GARAGE_OPEN);
	MakeNOP(0x4847B2, 1);
	MakeJMP(0x4847E4, &IS_GARAGE_CLOSED);
	MakeNOP(0x4847E9, 1);
	MakeJMP(0x47E78F, &SET_TARGET_CAR_FOR_MISSION_GARAGE);
	MakeNOP(0x47E794, 1);
	MakeJMP(0x47F4B6, &ACTIVATE_GARAGE);
	MakeNOP(0x47F4BB, 1);
	MakeJMP(0x47F976, &DEACTIVATE_GARAGE);
	MakeNOP(0x47F97B, 1);
	MakeJMP(0x482E1D, &OPEN_GARAGE);
	MakeNOP(0x482E1D+5, 4);
	MakeJMP(0x482E60, &CLOSE_GARAGE);
	MakeNOP(0x482E60+5, 4);
	MakeJMP(0x480A57, &CHANGE_GARAGE_TYPE);
	MakeNOP(0x482E60+5, 4);
	MakeJMP(0x48134D, &HAS_RESPRAY_HAPPENED);
	MakeNOP(0x48134D+5, 2);
	MakeJMP(0x47E0C3, &CREATE_GARAGE);
	MakeNOP(0x47E0C3+5, 2);
	MakeJMP(0x483C03, &OP2);
	MakeNOP(0x483C03+5, 2);
	MakeRelativeOffset(0x48EAD1+2, &SET_GARAGE_MAX_CARS);
	WriteMemory<void*>(0x47F24C + (0x14*4), (void*)(&IS_CAR_IN_MISSION_GARAGE), true);
}

