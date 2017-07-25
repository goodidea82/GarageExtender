/**
	by LINK/2012 (http://www.link2012world.blogspot.com/)
*/
#pragma once
#include <Windows.h>
#include <type_traits>
#include <cstdint>

#ifdef _DEBUG
#include <cstdio>
#include <cstdarg>
#include <math.h>
#endif


//
//	ASM EPILOG and ASM PROLOG macros
//
#define ASM_PROLOG() _asm					\
	{										\
		_asm push	ebp						\
		_asm mov	ebp, esp				\
		_asm sub	esp, __LOCAL_SIZE		\
	}

#define ASM_EPILOG() _asm					\
	{										\
		_asm mov	esp, ebp				\
		_asm pop	ebp						\
	}


//
//
//
union memory_pointer
{
	void*	 p;
	uintptr_t a;

	memory_pointer()						{ p = nullptr; }
	memory_pointer(void* x) : p(x)			{}
	memory_pointer(uint32_t x) : a(x)		{}

	operator void*()		{ return p; }
	operator uintptr_t()	{ return a; }

	memory_pointer& operator=(void* x)		{ return p = x, *this; }
	memory_pointer& operator=(uintptr_t x)	{ return a = x, *this; }
};

union memory_pointer_a	// used to hack the compiler, don't use for general purposes
{
	void*	 p;
	uintptr_t a;

	memory_pointer_a()						{ p = nullptr; }
	memory_pointer_a(void* x) : p(x)		{}
	memory_pointer_a(uint32_t x) : a(x)		{}

	template<class T>
	operator T*() { return reinterpret_cast<T*>(p); }
};



//
//
//
inline bool ProtectMemory(memory_pointer Address, size_t size, DWORD Protect)
{
	return VirtualProtect(Address, size, Protect, &Protect) != 0;
}

inline bool UnprotectMemory(memory_pointer Address, size_t size, DWORD& Out_OldProtect)
{
	return VirtualProtect(Address, size, PAGE_EXECUTE_READWRITE, &Out_OldProtect) != 0;
}

inline void WriteMemoryRaw(memory_pointer Address, void* value, size_t size, bool vp)
{
	DWORD oldProtect;
	if(vp) UnprotectMemory(Address, size, oldProtect);
 
	memcpy(Address, value, size);
 
	if(vp) ProtectMemory(Address, size, oldProtect);
}

inline void ReadMemoryRaw(memory_pointer Address, void* ret, size_t size, bool vp)
{
	DWORD oldProtect;
	if(vp) UnprotectMemory(Address, size, oldProtect);
 
	if(size == 1) *(uint8_t*)ret = *(uint8_t*)Address.p;
	else if(size == 2) *(uint16_t*)ret = *(uint16_t*)Address.p;
	else if(size == 4) *(uint32_t*)ret = *(uint32_t*)Address.p;
	else memcpy(ret, Address, size);
	
	if(vp) ProtectMemory(Address, size, oldProtect);
}



template<class T> inline
	void WriteMemory(memory_pointer Address, const T& value, bool vp = false)
	{
		DWORD oldProtect;
		if(vp) UnprotectMemory(Address, sizeof(T), oldProtect);
		*(T*)Address.p = value;
		if(vp) VirtualProtect(Address, sizeof(T), oldProtect, &oldProtect);
	}

template<class T> inline
	T& ReadMemory(memory_pointer Address, T& value, bool vp = false)
	{
		DWORD oldProtect;
		if(vp) UnprotectMemory(Address, sizeof(T), oldProtect);
		value = *(T*)Address.p;
		if(vp) VirtualProtect(Address, sizeof(T), oldProtect, &oldProtect);
		return value;
	}

template<class T> inline
	void WriteMemoryNow(memory_pointer Address, T value, bool vp = false)
	{
		WriteMemory(Address, value, vp);
	}

template<class T> inline
	T ReadMemoryNow(memory_pointer Address, bool vp = false)
	{
		T value;
		return ReadMemory(Address, value, vp), value;
	}




//
//
//
enum game_t { III, VC, SA };
class GameVersion
{
	public:
		enum eVersion
		{
			UNKNOWN = -1,
			SA_10US, SA_10EU, SA_101EU
		};

	private:
		game_t m_Game;
		eVersion m_Version;
	public:
		GameVersion(game_t game = SA) : m_Game(game), m_Version(GameVersion::UNKNOWN)
		{}

		GameVersion(const GameVersion& v)
		{ *this = v; }

		bool IsUnknown()	{ return m_Version == this->UNKNOWN; }
		game_t Game()		{ return m_Game; }
		eVersion Version()	{ return m_Version; }

		operator eVersion() { return Version(); }

		GameVersion& operator=(const GameVersion& v)
		{
			this->m_Game = v.m_Game;
			this->m_Version = v.m_Version;
			return *this;
		}

		GameVersion& operator=(eVersion v) { return m_Version = v, *this; }
		bool operator==(eVersion v) { return m_Version == v; }
		bool operator!=(eVersion v) { return !operator==(v); }

		GameVersion& Get()
		{
			switch(ReadMemoryNow<uint8_t>(0x400088, true))
			{
				case 0x8A:
					return m_Version = this->SA_10EU, *this;
				case 0xCA:
					return m_Version = this->SA_10US, *this;
				case 0xD0:
					return m_Version = this->SA_101EU, *this;
				default:
					return m_Version = this->UNKNOWN, *this;
			}
		}

		const char* GetString()
		{
			switch(this->m_Version)
			{
				case this->SA_10US:
					return "GTA_SA 1.0 US";
				case this->SA_10EU:
					return "GTA_SA 1.0 EU";
				case this->SA_101EU:
					return "GTA_SA 1.01 EU";
				default:
					return "UNKNOWN";
			}
		}
};

class CDebugLog
{
	private:
#ifdef _DEBUG
		// Use C stream
		FILE* stream;
#else
		int dummy;
#endif

		static CDebugLog x_static_log;

	public:
		static void Trace(const char* format, ...)
		{
#ifdef _DEBUG
			va_list va; va_start(va, format);
			x_static_log.Print(format, va);
			va_end(va);
#endif
		}

		void Print(const char* format, va_list va)
		{
#ifdef _DEBUG
			if(this->stream)
			{
				vfprintf(this->stream, format, va);
				fputs("\n", this->stream);
				fflush(this->stream);
			}
#endif
		}
		
		void Print(const char* format, ...)
		{
#ifdef _DEBUG
			if(this->stream)
			{
				va_list va; va_start(va, format);
				this->Print(format, va);
				va_end(va);
			}
#endif
		}

		CDebugLog(const char* name = nullptr)
		{
#ifdef _DEBUG
			if(name == nullptr) name = "log.txt";
			this->stream = fopen(name, "w");
			this->Print("Log started");
#endif
		}

		~CDebugLog()
		{
#ifdef _DEBUG
			this->Print("Log finished");
			fclose(stream);
#endif
		}
};


#ifdef IS_ADDRESSES_CPP
CDebugLog CDebugLog::x_static_log;
#endif







//
//
//
inline memory_pointer GetAbsoluteOffset(int rel_value, memory_pointer end_of_instruction)
{
	return end_of_instruction + rel_value;
}

inline long GetRelativeOffset(int abs_value, memory_pointer end_of_instruction)
{
	return abs_value - end_of_instruction;
}

inline memory_pointer ReadRelativeOffset(memory_pointer at, size_t sizeof_addr = 4)
{
	switch(sizeof_addr)
	{
		case 1: return (GetAbsoluteOffset(ReadMemoryNow<int8_t>(at, true), at+sizeof_addr));
		case 2: return (GetAbsoluteOffset(ReadMemoryNow<int16_t>(at, true), at+sizeof_addr));
		case 4: return (GetAbsoluteOffset(ReadMemoryNow<int32_t>(at, true), at+sizeof_addr));
	}
	return nullptr;
}

inline void MakeRelativeOffset(memory_pointer at, memory_pointer dest, size_t sizeof_addr = 4)
{
	switch(sizeof_addr)
	{
		case 1: WriteMemoryNow<int8_t>(at, static_cast<int8_t>(GetRelativeOffset(dest, at+sizeof_addr)), true);
		case 2: WriteMemoryNow<int16_t>(at, static_cast<int16_t>(GetRelativeOffset(dest, at+sizeof_addr)), true);
		case 4: WriteMemoryNow<int32_t>(at, static_cast<int32_t>(GetRelativeOffset(dest, at+sizeof_addr)), true);
	}
}

inline memory_pointer GetAbsoluteOffsetInOpcode(memory_pointer at)
{
	switch(ReadMemoryNow<uint8_t>(at, true))
	{
		// TODO: Handle other instructions
		// TODO: Handle prefix
		case 0xE8:	// call rel
		case 0xE9:	// jmp rel
			return ReadRelativeOffset(at+1, 4);
	}
	return nullptr;
}



inline memory_pointer MakeJMP(memory_pointer at, memory_pointer dest)
{
	memory_pointer p = GetAbsoluteOffsetInOpcode(at);
	WriteMemoryNow<uint8_t>(at, 0xE9, true);
	MakeRelativeOffset(at+1, dest);
	return p;
}

inline memory_pointer MakeCALL(memory_pointer at, memory_pointer dest)
{
	memory_pointer p = GetAbsoluteOffsetInOpcode(at);
	WriteMemoryNow<uint8_t>(at, 0xE8, true);
	MakeRelativeOffset(at+1, dest);
	return p;
}

inline void MakeJA(memory_pointer at, memory_pointer dest)
{
	WriteMemoryNow<uint16_t>(at, 0x87F0, true);
	MakeRelativeOffset(at+2, dest);
}

inline void MakeNOP(memory_pointer at, size_t size)
{
	DWORD oldProtect;
	UnprotectMemory(at, size, oldProtect);
	memset(at.p, 0x90, size);
	VirtualProtect(at, size, oldProtect, &oldProtect);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------	Functions\Variables and etc ------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Trick macro!
#ifdef IS_ADDRESSES_CPP
#define DeclareFunc(address, func)	func = memory_pointer_a(address)
#define DeclareArray(address, xtype, name) std::decay<xtype>::type name = memory_pointer_a(address);
#define DeclareVar(address, xtype, name) xtype& name = *reinterpret_cast<xtype*>(address)
#else
#define DeclareFunc(address, func)	extern func
#define DeclareArray(address, xtype, name) extern std::decay<xtype>::type name
#define DeclareVar(address, xtype, name) extern xtype& name
#endif
#define DeclareMethod(address, func) func { _asm mov eax, address _asm jmp eax }


// Basic headers


// RenderWare structures
#include "RwV3D.h"
#include "RwTexture.h"
#include "RwEngineInstance.h"
#include "RxObjSpace3dVertex.h"

// Game structures
#include "CRunningScript.h"	
#include "CColLine.h"
#include "ColSphere.h"
#include "CCamera.h"
#include "CMatrix.h"
#include "CParticle.h"
#include "CPad.h"

#include "CGarage.h"
#include "CPed.h"
#include "CObject.h"
#include "CVehicle.h"

#include "CPool.h"

#include "SImgGTAItemInfo.h"


//==========================================================
// CFileMgr
//==========================================================
namespace CFileMgr
{
	DeclareArray(0xB71AE0, char[128], RootDirName);
	DeclareArray(0xB71A60, char[128], DirName);
	DeclareFunc(0x5387D0, void (*ChangeDir)(const char* dir));
	DeclareFunc(0x538860, void (*ChangeToUserDir)());
	DeclareFunc(0x536F80, char* (*ReadConfigLine)(FILE* file));
};


//==========================================================
// CTheScripts
//==========================================================
namespace CTheScripts
{
	DeclareFunc(0x463D50, void (__fastcall *CRunningScript__CollectStringParameter)(CRunningScript* thread, BYTE _unused, char* ptr, size_t len));
	DeclareFunc(0x464080, void (__fastcall *CRunningScript__CollectParameters)(CRunningScript* thread, BYTE _unused, short count));
	DeclareFunc(0x463CA0, ScriptVar* (__fastcall *CRunningScript__GetLocalVariable)(CRunningScript* thread, BYTE _unused, int idx));
	DeclareFunc(0x4859D0, void (__fastcall *CRunningScript__UpdateCompareFlag)(CRunningScript* thread, BYTE _unused, bool a2));
	DeclareArray(0xA43C78, ScriptVar[32], CollectiveArray);
	DeclareVar(0xA8B42C, CRunningScript*, pActiveScripts);

	inline ScriptVar CRunningScript__CollectNextParameter(CRunningScript* thread)
	{
		CRunningScript__CollectParameters(thread, 0, 1);
		return CollectiveArray[0];
	}
};


//==========================================================
// CTxdStore
//==========================================================
namespace CTxdStore
{
	#pragma pack(push,1)
	struct TxdDef
	{
		void* dictionary;	// RwTextureDictionary*
		int16_t ref;
		uint16_t field_6;
		uint32_t hash;
	};
	static_assert(sizeof(TxdDef) == 0xC, "Incorrect struct size: CTxdStore::TxdDef");
	#pragma pack(pop)

	DeclareVar(0xC8800C, CPool<TxdDef>, pTxdPool);

	DeclareFunc(0x7319C0, void (*SetCurrentTxd)(int index));
	DeclareFunc(0x731990, void (*Create)(int index));
	DeclareFunc(0x731C80, int (*AddTxdSlot)(const char* name));
	DeclareFunc(0x731CD0, void (*RemoveTxdSlot)(int index));
	DeclareFunc(0x731850, int (*FindTxdSlot)(const char* name));
	DeclareFunc(0x7318E0, int (*FindTxdSlotByHash)(unsigned int hash));
	DeclareFunc(0x7320B0, int (*LoadTxd)(int index, const char* filename));
	DeclareFunc(0x731930, char (*LoadTxdFromStream)(int index, void* rw_stream));
	DeclareFunc(0x731E90, void (*RemoveTxd)(int index));
	DeclareFunc(0x731A00, void (*AddRef)(int index));
	DeclareFunc(0x731A30, void (*RemoveRef)(int index));
	DeclareFunc(0x731A70, void (*RemoveRefWithoutDelete)(int index));
	DeclareFunc(0x731AA0, int (*GetNumRefs)(int index));
	DeclareFunc(0x7316A0, void (*PushCurrentTxd)());
	DeclareFunc(0x7316B0, void (*PopCurrentTxd)());
};


//==========================================================
// CPools
//==========================================================
namespace CPools
{
	DeclareVar(0xB74494, CVehiclePool*, VehiclePool);
	DeclareVar(0xB74490, CPedPool*, PedPool);
	DeclareVar(0xB7449C, CObjectPool*, ObjectPool);
};


//==========================================================
// RenderWare
//==========================================================
	DeclareVar(0xC97B24, RwEngineInstance*, pRwEngineInstance);
	DeclareFunc(0x7F3AC0, RwTexture* (*RwReadTexture)(const char* name, const char* mask));
	DeclareFunc(0x7F3820, void (*RwTextureDestroy)(RwTexture* texture));
	DeclareFunc(0x7EF450, void*(*RwIm3DTransform)(RxObjSpace3dVertex* pVerts, unsigned int numVerts, RwMatrix* ltm, unsigned int flags));
	DeclareFunc(0x7EF520, void (*RwIm3DEnd)(void));
	DeclareFunc(0x7EF6B0, char (*RwIm3DRenderPrimitive)(int primType));
	DeclareFunc(0x7EF550, char (*RwIm3DRenderIndexedPrimitive)(int primtype, void *indices, int numIndices));


//==========================================================
// CVector
//==========================================================
typedef RwV3D CVector;
DeclareFunc(0x59C910, void (__fastcall *CVector__Normalise)(CVector* self));


//==========================================================
// CMatrix
//==========================================================
DeclareFunc(0x59C890, void (*TransformPoint)(CVector* OutPoint, CMatrix* m, RwV3D* point));
DeclareFunc(0x70CE30, char (*ConvertGlobalXYToScreenXY)(RwV3D* inPoint, RwV3D* out, float* a3, float* a4, bool a5, bool a6));
DeclareFunc(0x59AF40, void (__fastcall *CMatrix__InitIdentityMatrixAtPoint)(CMatrix* self, int dummy, RwV3D point));
DeclareFunc(0x59ACD0, void (__fastcall *CMatrix__destructor)(CMatrix* self));


//==========================================================
// CColLine
//==========================================================
DeclareFunc(0x40EF50, void (__fastcall *CColLine__Constructor_t)(CColLine* self, int dummy, RwV3D* p1, RwV3D* p2));
DeclareFunc(0x417470, char (__cdecl *CColLine__HasCollisionWithSphere_t)(CColLine* a1, ColSphere* a2));


//==========================================================
// CParticle
//==========================================================
DeclareFunc(0x4AA440, int (__fastcall *CParticleFx__Add)(void* self, int dummy, CVector* pVecPosition, CVector* pVecVelocity, float fBlur, void* pParticleInfo, float a6, float fBrightness, float a8, int a9));
DeclareFunc(0x4AA3F0, void (__fastcall *CParticle__Destroy)(CParticle* self));
DeclareFunc(0x4A9BE0, CParticle* (__fastcall *CParticleData__CreateParticle)(/*CParticleData*/void* self, int dummy, char* name, RwV3D* point, RwMatrix* m, int));
DeclareFunc(0x4AA2F0, void (__fastcall *CParticle__MakeVisible)(CParticle* self));
DeclareFunc(0x4AA390, void (__fastcall *CParticle__MakeInvisible)(CParticle* self));
DeclareFunc(0x4AA900, char (__fastcall *CParticle__IsInactive)(CParticle* self));


//==========================================================
// CMessages - TODO
//==========================================================
DeclareFunc(0x69F1E0, void (*PrintNow)(const char* text, int time, bool addToBrief));


//==========================================================
// CPad
//==========================================================
DeclareFunc(0x53FF90, char (__fastcall *CPad__GetCarGunFired)(CPad* self));
DeclareFunc(0x53FE70, char (__fastcall *CPad__GetLookBehindForCar)(CPad* self));
DeclareFunc(0x53FDD0, char (__fastcall *CPad__GetLookLeft)(CPad* self));
DeclareFunc(0x53FE10, char (__fastcall *CPad__GetLookRight)(CPad* self));



//==========================================================
// CGarages
//==========================================================
// !!!! IMPLEMENTED ON CGarage.h !!!!!!


//==========================================================
// CWorld
//==========================================================
namespace CWorld
{
	DeclareFunc(0x563220, void (*Add)(CEntity *object));
	DeclareFunc(0x563280, void (*Remove)(CEntity *object));

	DeclareFunc(0x56A490, char (*IsLineOfSightClear)(RwV3D *a1, RwV3D *a2, bool checkBuildings, bool checkVehicles,
		bool checkPeds, bool checkObjects, bool checkDummies, bool seeThroughStuff, bool ignoreSomeObjectsForCamera));
	DeclareFunc(0x5696C0, float (*FindGroundZFor3DCoord)(float x, float y, float z, bool* success, void* unk));
	DeclareFunc(0x569660, float (*FindGroundZForCoord)(float x, float y));
};

//==========================================================
// CWaterLevel
//==========================================================
namespace CWaterLevel
{
	DeclareFunc(0x6EB690, char (*GetWaterLevel)(float x, float y, float z, float *out1, char bCheckWaves, RwV3D *pvecUnknown));
	DeclareFunc(0x6E8580, char (*GetWaterLevelNoWaves)(float x, float y, float z, float* out1, float* unk1, float* unk2));
};



//==========================================================
// CCamera
//==========================================================
DeclareVar(0xB6F028, CCamera, camera);


//==========================================================
// CTimer
//==========================================================
namespace CTimer
{
	DeclareFunc(0x5619D0, void (*StartUserPause)());
	DeclareFunc(0x561A00, void (*EndUserPause)());
	DeclareVar(0xB7CB5C, float, TimeStep);
	DeclareVar(0xB7CB4C, long, FrameCounter);
	DeclareVar(0xB7CB84, unsigned long, TimeInMiliseconds);
};


//==========================================================
// CClock
//==========================================================
namespace CClock
{
	typedef uint8_t month_t;	typedef uint8_t day_t;		typedef uint8_t weekday_t;
	typedef uint8_t hour_t;		typedef uint8_t minute_t;	typedef uint16_t second_t;

	DeclareFunc(0x52CDE0, void (*Validate)());
	DeclareFunc(0x52CEE0, char (*GetIsTimeInRange)(hour_t a, hour_t b));
	DeclareFunc(0x52CEB0, int (*GetGameClockMinutesUntil)(hour_t hours, minute_t minutes));
	DeclareFunc(0x52D0B0, void (*SetNextDay)(int bGoForward));
	DeclareFunc(0x52D150, void (*SetGameClock)(hour_t hour, minute_t minute, weekday_t weekday));
		
	DeclareVar(0xB70155, uint8_t, GameMonth);
	DeclareVar(0xB70154, uint8_t, GameDay);
	DeclareVar(0xB7014E, uint8_t, GameWeekday);
	DeclareVar(0xB70153, uint8_t, GameClockHours);
    DeclareVar(0xB70152, uint8_t, GameClockMinutes);
    DeclareVar(0xB70150, uint16_t, GameClockSeconds);
};


//==========================================================
// CWeather - TODO
//==========================================================
namespace CWeather
{

};

//==========================================================
// CStreaming
//==========================================================
namespace CStreaming
{
	//Platinum Edit:
	//const int MODELINFO_MAX = (int *)alloca(sizeof(int) * (*(int*)(0x5B8AFA + 2) - *(int*)(0x408ADA + 3)) / 0x14);
	typedef SImgGTAItemInfo ModelInfo;

	DeclareFunc(0x40EA10, char (*LoadAllRequestedModels)(bool OnlyPriorityModels));
	DeclareFunc(0x4087E0, char (*RequestModel)(int model, int));
	//DeclareArray(0x8E4CC0, ModelInfo[25000], aInfoForModel);
	//PlatinumEdit:
	//DeclareArray(((0x408ADA + 3)), ModelInfo[50000], aInfoForModel);
	//Goodidea82 Edit: Because: (0x408ADA + 3) - holds a POINTER TO CStreaming::ms_aInfoForModel array (0x8E4CC0 by default)
	DeclareArray((*(ModelInfo**)(0x408ADA + 3)), ModelInfo[50000], aInfoForModel);
};


//==========================================================
// Other stuff
//==========================================================
inline void delete_me(CEntity* entity, int dealloc = 1)
{
	void (__fastcall *sdtor)(CEntity* self, int dummy, int dealloc);
	sdtor = *(decltype(sdtor)*)(entity->__parent.__vmt);
	sdtor(entity, 0, dealloc);
}

inline void delete_me(CVehicle* veh, int dealloc = 1)
{
	delete_me(&veh->__parent.__parent, dealloc);
}


enum TownID
{
	Countryside,
	LS,
	SF,
    LV,
	Desert,
	_Others
};

DeclareFunc(0x571B70, char (__fastcall *CEntity__RegisterReference)(CEntity *self, int dummy, CEntity **a2));
DeclareFunc(0x571A00, int (__fastcall *CEntity__UnRegisterReference)(CEntity* self, int dummy, CEntity **a2));

DeclareFunc(0x506EA0, void (__fastcall *CAudio__ReportFrontendAudioEvent)(void* self, int dummy, int a1, float a2, float a3));
DeclareFunc(0x56E0D0, CVehicle* (*FindPlayerVehicle)(int number, char));
DeclareFunc(0x56E210, CPed* (*FindPlayerPed)(int number)); 

DeclareFunc(0x6A0050, const char* (__fastcall *CText__Get)(void* self, int dummy, const char *key));
DeclareFunc(0x588BE0, void (*PrintHelp)(const char* a1, char a2, char a3, char addToBrief));

DeclareVar(0xC81314, uint16_t, CurrLevel);
DeclareVar(0xB72914, uint32_t, CurrInterior);

DeclareVar(0xC4B954, uint32_t, NumIndices);
DeclareVar(0x4B950, uint32_t, NumVertices);
DeclareArray(0xC4B958, uint16_t[4096], Indices);
DeclareArray(0xC4D958, RxObjSpace3dVertex[2048], Vertices);


#define RAND_MAX_DIV_BY_1	0.00003051850947599719
inline float randomf(float a, float b)
{
	return float((rand() * RAND_MAX_DIV_BY_1) * (b-a) + a);
}
inline int random(int a, int b)
{
	return(a + ((b-a) * rand() / (RAND_MAX + 1)));
}
#undef RAND_MAX_DIV_BY_1


inline int& PlayerMoney()
{
	return *(int*)((0xB7CD98 + (*(uint8_t*)(0xB7CD74) * 0x190)) + 0xB8);
}

inline int& PlayerWantedLevel()
{
	typedef void* (*GetPlayerCWanted)(int number);
	void* w = (reinterpret_cast<GetPlayerCWanted>(0x56E230))(-1);
	return *(int*)((uint8_t*)w + 0x2C);
}

inline void MakePlayerNotable()
{
	typedef void* (*GetPlayerCWanted)(int number);
	void (__fastcall *sub_5625A0)(void*) = (decltype(sub_5625A0))(0x5625A0);
	sub_5625A0((reinterpret_cast<GetPlayerCWanted>(0x56E230))(-1));
}


inline void MakePlayerPoliceFree(bool make)
{
	typedef void* (*GetPlayerCWanted)(int number);
	void* w = (reinterpret_cast<GetPlayerCWanted>(0x56E230))(-1);
	if(make) *(uint8_t*)((uint8_t*)w + 0x1E) &= 0xFD;
	else *(uint8_t*)((uint8_t*)w + 0x1E) |= 2;
}

inline void MakePlayerControllable(bool make)
{
	MakePlayerPoliceFree(make);
	CPad* pad = (CPad*)(0xB73458);
	if(make)
		*(uint8_t*)((uint8_t*)pad + 0x10E) &= 0xFB;
	else
		*(uint8_t*)((uint8_t*)pad + 0x10E) |= 4;
}


inline RwV3D GetCoords(CPlaceable* x)
{
	if(DWORD p = x->m_pCoords)
		return (*(RwV3D*)(p+0x30));
	else
		return x->placement.pos;
}

inline RwV3D* GetCoords(CPlaceable* x, int dummy_want_pointer)
{
	(void)(dummy_want_pointer);
	if(DWORD p = x->m_pCoords)
		return (RwV3D*)(p+0x30);
	else
		return &x->placement.pos;
}

inline RwV3D GetCoords(CEntity* x)
{
	return GetCoords(&x->__parent);
}

inline RwV3D* GetCoords(CEntity* x, int dummy_want_pointer)
{
	return GetCoords(&x->__parent, 0);
}

inline float GetDistanceBetweenPoints3D(const RwV3D& p1, const RwV3D& p2)
{
	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;
	float dz = p1.z - p2.z;
	return sqrt(dx*dx + dy*dy + dz*dz);
}

inline float GetDistanceBetweenPoints2D(const RwV3D& p1, const RwV3D& p2)
{
	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;
	return sqrt(dx*dx + dy*dy);
}

inline void DebugMessage(const char* format, ...)
{
#ifdef _DEBUG
	static char PrintBuffer[512] = {0};
	va_list v; va_start(v, format);
	vsprintf(PrintBuffer, format, v);
	va_end(v);
	PrintNow(PrintBuffer, 2000, true);
#endif
}

inline bool DebugCheat(const char* cheat)
{
#ifdef _DEBUG
	char* cheat_string = (char*)(0x969110);
	size_t l = strlen(cheat);
	if(l-- == 0) return false;

	while(*cheat_string)
	{
		if(cheat[l] != *cheat_string) return false;
		if(l-- == 0) return true;
		++cheat_string;
	}
#endif
	return false;
}



