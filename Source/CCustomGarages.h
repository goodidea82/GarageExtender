/**
	Garage eXtender
		by LINK/2012 (http://www.link2012world.blogspot.com/)

	Custom garage system is over here, see more info on the .cpp
*/
#pragma once
#include "CGarage.h"
#include "CSaveSystem.h"
#include <vector>

#define GARAGE_TYPE_GRGX	(0xF6) //-10
#define DEFAULT_GRGX_SIZE	(5)
//Platinum Edit new ARRAY_ModelInfo pointer
static const DWORD		ARRAY_ModelInfo = (*(int*)(0x40122D));

enum eGrgxGarageType
{
	IS_NULL_GARAGE			= 0,		// This garage does nothing
	IS_SCRIPT_GARAGE		= 1,		// Script controlled garage, the script should open and close it.
	IS_HIDEOUT_GARAGE		= 3,		// Normal Save Garage
	IS_PARKING_GARAGE		= 4,		// Park Save Garage
	IS_IMPOUND_GARAGE		= 5,		// Impound Lot, not working very well
	IS_RESPRAY_GARAGE		= 6,		// Pay n' spray garage
	IS_BOMBSHOP1_GARAGE		= 7,		// Bomb shop with timed detonator				- don't move
	IS_BOMBSHOP2_GARAGE		= 8,		// Bomb shop with engine ignition detonation
	IS_BOMBSHOP3_GARAGE		= 9,		// Bomb shop with remote-control detonator
	IS_BURGALY_GARAGE		= 10,		// Burgaly. Must be opened by a script and is closed automatically.
	IS_WHEEL_ARCH_ANGEL		= 11,		// Only accepts vehicles with TUNE flag
	IS_LOCO_LOW_CO			= 12,		// Only accepts vehicles with LOWRIDER flag
	IS_TRANSFENDER			= 13,		// Accepts vehicles that aren't TUNE neither LOWRIDER
	IS_ANYCAR_TUNNING_SHOP	= 14,		// Accepts every car
	IS_OPENCLOSE_GARAGE		= 15,		// This garage does nothing but only opens and closes.
};

enum eGrgxDoorStyle
{
	DOOR_STYLE_OPENS_UP		= 0x1,
	DOOR_STYLE_ROTATES		= 0x2,
	DOOR_STYLE_GOES_IN		= 0x4,
	DOOR_STYLE_OPENS_SLOW	= 0x8,
	DOOR_STYLE_OPENS_TOSIDE = 0x10,
	GARAGE_DEPPENDS_ON_THREAD = 0x20,
	GARAGE_DONT_SAVE = 0x40,
};

enum eGrgeIPLFlags
{
	FLAG_IPL_DOOR_GOES_UP_AND_ROTATE = 0x1,
	FLAG_IPL_DOOR_GOES_IN = 0x2,
	FLAG_IPL_CAMERA_FOLLOWS_PLAYER = 0x4,
	//FLAG_IPL_IS_PARKING_GARAGE = 0x8,
};


//
//	CGarages
//
class CGarageBase;

class CGarages
{
	protected:
		static std::vector<CGarageBase*> garages;

	public:
		static void Load(const char* fname);
		static void Clear();

	public:
		static int __cdecl AddOne(float x1, float y1, float z1, float frontX, float frontY, float x2, float y2, float z2,
			char type, int a10, char *name, char flags)
			{ return CGarages__AddOne(x1, y1, z1, frontX, frontY, x2, y2, z2, type, a10, name, flags); }

		static short __cdecl FindIndexByName(const char *name)
			{ return CGarages__FindIndexByName(name); }

		static CGarage *__cdecl ChangeGarageType(short index, unsigned char type, int unused)
			{ return CGarages__ChangeGarageType(index, type, unused); }

		static void __cdecl PrintMessages()
			{ CGarages__PrintMessages(); }

		static char __cdecl IsVehicleAcceptedByResprayGarage(CVehicle *a1)
			{ return CGarages__IsVehicleAcceptedByResprayGarage(a1); }

		static void __cdecl BuildRotatedDoorMatrix(CObject *a1, float a2)
			{ CGarages__BuildRotatedDoorMatrix(a1, a2); }

		static void __cdecl TriggerMessage(char *gxt_entry, short number1, unsigned __int16 duration, short number2)
			{ CGarages__TriggerMessage(gxt_entry, number1, duration, number2); }

		static void __cdecl SetTargetCarForMissionGarage(short index, CVehicle *a2)
			{ return CGarages__SetTargetCarForMissionGarage(index, a2); }

		static char __cdecl HasCarBeenDroppedOffYet(short index)
			{ return CGarages__HasCarBeenDroppedOffYet(index);	}

		static CGarage *__cdecl DeActivateGarage(short index)
			{ return CGarages__DeActivateGarage(index); }

		static CGarage *__cdecl ActivateGarage(short index)
			{ return CGarages__ActivateGarage(index); }

		static char __cdecl IsGarageOpen(short index)
			{ return CGarages__IsGarageOpen(index); }

		static char __cdecl IsGarageClosed(short index)
			{ return CGarages__IsGarageClosed(index); }

		static char __cdecl HasResprayHappened(short index)
			{ return CGarages__HasResprayHappened(index); }

		static char __cdecl CameraShouldBeOutside()
			{ return CGarages__CameraShouldBeOutside(); }

		static void __cdecl GivePlayerDetonator()
			{ CGarages__GivePlayerDetonator(); }

		static void __cdecl UnknownRemoveEngineFire(CVehicle *a1)
			{ CGarages__UnknownRemoveEngineFire(a1); }

		static char __cdecl IsPointWithinHideOutGarage(RwV3D& a1)
			{ return CGarages__IsPointWithinHideOutGarage(&a1); }

		static char __cdecl IsPointWithinAnyGarage(RwV3D& a1)
			{ return CGarages__IsPointWithinAnyGarage(&a1);  }

		static int __cdecl GetStoreIndex(int garage_type)
			{ return CGarages__GetStoreIndex(garage_type); }

		static char __cdecl IsModelIndexADoor(int model)
			{ return CGarages__IsModelIndexADoor(model); }

		static void __cdecl OpenSprayGarages(char bOpen)
			{ CGarages__OpenSprayGarages(bOpen); }

		static void __cdecl RestoreGaragesForNewGame()
			{ CGarages__RestoreGaragesForNewGame(); }

		static char __cdecl IsThisCarWithinGarageArea(__int16 garage_index, CEntity *a2)
			{ return CGarages__IsThisCarWithinGarageArea(garage_index, a2); }

		static char __cdecl IsPointInAGarageCameraZone(RwV3D point)
			{ return CGarages__IsPointInAGarageCameraZone(point); }

		static long double __cdecl GetGarageDoorMoveMultiplier(int model)
			{ return CGarages__GetGarageDoorMoveMultiplier(model); }

		static void __cdecl PlayerArrestedOrDied()
			{ CGarages__PlayerArrestedOrDied(); }

		static void __cdecl CloseGaragesBeforeSave()
			{ CGarages__CloseGaragesBeforeSave(); }

		static void __cdecl CountCarsInGarage(unsigned char garage_type)
			{ CGarages__CountCarsInGarage(garage_type); }

		static int __cdecl FindGarageForThisDoor(CObject *a1)
			{ return CGarages__FindGarageForThisDoor(a1); }

		static void __cdecl StoreCarAtImpoundLot(CVehicle *a1)
			{ CGarages__StoreCarAtImpoundLot(a1); }

		static void __cdecl Update()
			{ CGarages__Update(); }


	public:

	public:
		static void Patch();

		static class CCustomGarage* AddOneX(float x1, float y1, float z1, float frontX, float frontY, float x2, float y2, float z2,
			char type, int a10, char *name, char flags, int car_count, eGrgxGarageType gtype, unsigned int doorstyle,
			bool created_by_script = false);

		static std::vector<CGarageBase*>& Garages() { return CGarages::garages; } 

		static void OnLoad(const CSaveSystem::SaveInfo& info);
		static void OnSave(const CSaveSystem::SaveInfo& info);

		static void TranslateGarageType_GRGE_to_GRGX(CGarageBase*, int type);
		static uint8_t TranslateGarageType_GRGX_to_GRGE(eGrgxGarageType type);

		static bool IsGRGX(CGarage* g);
		static bool IsParkingGarage(CGarage* g);
		static CGarageBase* FindGarageByName(const char* name);
		static CGarage* FindGarageByNameGlobal(const char* name);

		static bool IsGarageClosed(CGarage* g)
			{ return g->DoorState == GARAGE_DOOR_CLOSED; }
		static bool IsGarageOpen(CGarage* g)
			{ return g->DoorState == GARAGE_DOOR_OPEN || g->DoorState == GARAGE_DOOR_WAITING_PLAYER_TO_EXIT; }
	static bool HasCarBeenDroppedOffYet(CGarage* g);
		static void DeActivateGarage(CGarage* g)
			{ g->Flags |= FLAG_GARAGE_INACTIVE; }
		static void ActivateGarage(CGarage* g)
			{ g->Flags &= ~(FLAG_GARAGE_INACTIVE); }
		static bool HasResprayHappened(CGarage* g)
		{
			bool r = (g->Flags & FLAG_GARAGE_USED_RESPRAY)!=0;
			g->Flags &= ~FLAG_GARAGE_USED_RESPRAY;
			return r;
		}
		static void SetTargetCarForMissionGarage(CGarage* g, CVehicle* veh);
		static void ChangeGarageType(CGarage* g, int type);

		static void DestroyGarage(CGarageBase* g);
};



//
// CGarage with virtualization
//
class CGarageBase : public CGarage
{
	public:
		typedef unsigned int flag_t;

		// void* vtable; //
		bool CreatedByScript;
		size_t size;
		std::vector<CStoredCar> cars;
		eGrgxGarageType gType;
		flag_t gStyle;
		flag_t OriginalStyle;
		flag_t OriginalFlags;
		CObject* door[2];

		char thread_name[8];
		CRunningScript* thread;

	public:
		CGarageBase(size_t size = -1);
		~CGarageBase();

		virtual void RestoreThisGarage()
			{ CGarage__RestoreThisGarage(this); }
		
		virtual char IsVehicleAcceptByThisTunningGarage(CVehicle* veh)
			{ return CGarage__IsVehicleAcceptByThisTunningGarage(this, 0, veh); }
		
		void OpenThisGarage()
		{
			if(!(this->IsGRGX() && this->IsParkingGarage()))
				CGarage__OpenThisGarage(this);
		}
		
		void CloseThisGarage()
		{
			if(!(this->IsGRGX() && this->IsParkingGarage()))
				CGarage__CloseThisGarage(this);
		}
		
		long double GetDistanceFromPoint(float x, float y)
			{ return CGarage__GetDistanceFromPoint(this, 0, x, y); }

		void MakeCarCoorsForImpoundLot(CStoredCar *ar)
			{ CGarage__MakeCarCoorsForImpoundLot(this, 0, ar); }

		virtual char RestoreCarsForThisGarage(CStoredCar *ar)
			{ return CGarage__RestoreCarsForThisGarage(this, 0, ar); }

		virtual char RestoreCarsForThisImpoundLot(CStoredCar *ar)
			{ return CGarage__RestoreCarsForThisImpoundLot(this, 0, ar); }

		virtual void SetDoorsBackToOriginalHeight()
			{ CGarage__SetDoorsBackToOriginalHeight(this); }

		char IsPointWithinGarage(RwV3D vec)
			{ return CGarage__IsPointWithinGarage(this, 0, vec); }

		char IsPointWithinGarageWithDistance(RwV3D vec, float distance = 0.0)
			{ return CGarage__IsPointWithinGarageWithDistance(this, 0, vec, distance); }

		virtual int MaximumNumberOfCarsOnThisGarage()
			{ return CGarage__MaximumNumberOfCarsOnThisGarage(this); }

		char IsEntityEntirelyInside(CEntity *a2, float distance = 0.0)
			{ return CGarage__IsEntityEntirelyInside(this, 0, a2, distance);  }

		char IsEntityEntirelyOutside(CEntity *a2, float distance = 0.0)
			{ return CGarage__IsEntityEntirelyOutside(this, 0, a2, distance); }

		char IsPlayerEntirelyOutside(float distance = 0.0)
			{ return CGarage__IsPlayerEntirelyOutside(this, 0, distance); }

		char IsPlayerEntirelyInside()
			{ return CGarage__IsPlayerEntirelyInside(this); }

		char IsEntityTouching3D(CEntity *a2)
			{ return CGarage__IsEntityTouching3D(this, 0, a2); }

		char IsEntityTouchingOutside(CEntity *a2, float distance = 0.0)
			{ return CGarage__IsEntityTouchingOutside(this, 0, a2, distance); }

		char IsAnyOtherCarTouchingGarage(CVehicle *except)
			{ return CGarage__IsAnyOtherCarTouchingGarage(this, 0, except); }

		void PushOutVehicles(CVehicle *except)
			{ CGarage__PushOutVehicles(this, 0, except); }

		char IsAnyOtherPedTouchingGarage(CPed *except)
			{ return CGarage__IsAnyOtherPedTouchingGarage(this, 0, except); }

		char IsAnyCarBlockingDoor()
			{ return CGarage__IsAnyCarBlockingDoor(this); }

		int CountCarsWithCenterPointWithinGarage(CVehicle *except)
			{ return CGarage__CountCarsWithCenterPointWithinGarage(this, 0, except); }

		void RemoveCarsBlockingDoorNotInside()
		{
			if(!(this->IsGRGX() && this->IsParkingGarage()))
				CGarage__RemoveCarsBlockingDoorNotInside(this);
		}

		virtual void StoreAndRemoveCarsForThisGarage(CStoredCar *ar, signed int max_slot)
			{ CGarage__StoreAndRemoveCarsForThisGarage(this, 0, ar, max_slot); }

		virtual void StoreAndRemoveCarsForThisImpoundLot(CStoredCar *ar, signed int max_slot)
			{ CGarage__StoreAndRemoveCarsForThisImpoundLot(this, 0, ar, max_slot); }

		void TidyUpGarage()
			{ CGarage__TidyUpGarage(this); }

		void TidyUpGarageClose()
			{ CGarage__TidyUpGarageClose(this); }

		char Unknown(CEntity *a2)
			{ return CGarage__Unknown(this, 0, a2); }

		virtual void FindDoorsEntities(CObject **door1, CObject **door2)
			{ CGarage__FindDoorsEntities(this, 0, door1, door2); }

		virtual char UpdateOpeningDoor()
			{ return CGarage__UpdateOpeningDoor(this); }

		virtual char UpdateClosingDoor()
			{ return CGarage__UpdateClosingDoor(this); }

		char IsStaticPlayerCarEntirelyInside()
			{ return CGarage__IsStaticPlayerCarEntirelyInside(this); }

		char IsAnythingTouchingGarage()
			{ return CGarage__IsAnythingTouchingGarage(this); }

		virtual void Update(int my_index)
			{ return CGarage__Update(this, 0, my_index); }


		//
		bool IsGRGX()
			{ return this->Type == GARAGE_TYPE_GRGX; }

		bool IsParkingGarage()
			{ return this->gType == IS_PARKING_GARAGE; }

		bool CheckIfScriptStillAlive();

};

class CCustomGarage : public CGarageBase
{
	public:

	public:
		CCustomGarage(size_t size = -1) : CGarageBase(size)
		{ }
		virtual ~CCustomGarage() { }

		unsigned int GetBombflagForThisGarage()
		{
			unsigned int x = (unsigned int)(this->gType) - 5;
			return x >= 1 && x <= 3? x : 0;
		}

		static bool DoesThisGarageCanStoreVehicles(eGrgxGarageType t)
		{
			return (t == IS_HIDEOUT_GARAGE || t == IS_IMPOUND_GARAGE
				  ||t == IS_PARKING_GARAGE);
		}
		bool DoesThisGarageCanStoreVehicles()
		{
			return DoesThisGarageCanStoreVehicles(this->gType);
		}
		static bool IsThisGarageAHideout(eGrgxGarageType t)
		{
			return (t == IS_HIDEOUT_GARAGE  || t == IS_PARKING_GARAGE);
		}
		bool IsThisGarageAHideout()
		{
			return this->IsThisGarageAHideout(this->gType);
		}

		bool ResizeThisGarage(size_t size, bool basic_load = false);
		void ClearStoredCars();
		void AdjustGarageSlots(bool move = true, bool garbage = true);
		int CountNumCarsInThisGarage();

		void ProcessSprayGarage();
		void ProcessBombshop();
		void ProcessScriptGarage();
		void ProcessHideoutGarage();
		void ProcessImpoundGarage();
		void ProcessBurgalyGarage();
		void ProcessTunningGarage();
		void ProcessParkingGarage();
		void ProcessBasicGarage();
		void ProcessMissionGarage();

		char RestoreCarsForThisGarage(CStoredCar *ar = nullptr);
		void FindDoorsEntities(CObject** door1, CObject** door2);

		bool CheckIfScriptStillAlive();

		void RestoreThisGarage();
		char IsVehicleAcceptByThisTunningGarage(CVehicle* veh);
		void SetDoorsBackToOriginalHeight();
		int MaximumNumberOfCarsOnThisGarage();
		void StoreAndRemoveCarsForThisGarage(CStoredCar *ar = nullptr, signed int max_slot = -1);
		void StoreAndRemoveCarsForThisImpoundLot(CStoredCar *ar = nullptr, signed int max_slot = -1);
		char UpdateOpeningDoor();
		char UpdateClosingDoor();
		bool UpdateCustomGarage();
		void Update(int my_index = -1);
};





