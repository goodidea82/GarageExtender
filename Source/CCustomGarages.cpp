/**
	Garage eXtender
		by LINK/2012 (http://www.link2012world.blogspot.com/)

	Here is our custom garage system, please note that this mod doesn't just replace the pointer to the garages array,
	this would screw the save game and other stuff, I preefer to create my own system that have more possibilities than the
	original game garages.

	The code is not good commented, sorry for that.
*/
#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)
#include "CCustomGarages.h"
#include <cmath>
#include "CAutomobile.h"
#include "CMatrix_Padded.h"	// Same as RwMatrix
std::vector<CGarageBase*> CGarages::garages;
void GRGX_Parse(const char*);
namespace GRGX_HOOKS{char GRGX_ProcessGarageDoor(CGarage* _garage, CObject* door);}
using GRGX_HOOKS::GRGX_ProcessGarageDoor;


//======================================================================
//		CGarages
//======================================================================
CCustomGarage* CGarages::AddOneX(float x1, float y1, float z1, float frontX, float frontY, float x2, float y2, float z2, char type, int a10, char *name,
	char flags, int car_count, eGrgxGarageType gtype, unsigned int doorstyle, bool created_by_script)
{
	float top, back, front, width, left, right, depth;
	float kx = frontX + x2 - x1;
	float ky = frontY + y2 - y1;
	top = z2;

	// find left
	{
		left = x1 >= frontX? frontX : x1;
		if(left >= x2) left = x2;
		if(left >= kx) left = kx; 
	}

	// find right
	{
		right = x1 <= frontX? frontX : x1;
		if(right <= x2) right = x2;
		if(right <= kx) right = kx; 
	}

	// find front
	{
		front = y1 >= frontY? frontY : y1;
		if(front >= y2) front = y2;
		if(front >= ky) front = ky;
	}

	// find back
	{
		back = y1 <= frontY? frontY : y1;
		if(back <= y2) back = y2;
		if(back <= ky) back = ky;
	}


	{
		if(gtype == IS_SCRIPT_GARAGE || created_by_script)
			car_count = -1;
		else if(car_count && !CCustomGarage::DoesThisGarageCanStoreVehicles(gtype))
			car_count = 0;

		CCustomGarage* g = new CCustomGarage(car_count);
		CGarages::Garages().push_back(g);

		g->CreatedByScript = created_by_script;
		strncpy(g->Name, name, 8);
		g->Name[7] = 0;
		g->gType = gtype;
		g->OriginalType = (int8_t)(gtype);

		g->Position.x = x1;
		g->Position.y = y1;
		g->Position.z = z1;

		g->DirectionA.x = frontX - x1;
		g->DirectionA.y = frontY - y1;
		g->DirectionB.x = x2 - x1;
		g->DirectionB.y = y2 - y1;

		g->Left = left;
		g->Right = right;
		g->Back = back;
		g->Front = front;
		g->TopZ = top;

		width = sqrt( (g->DirectionA.y*g->DirectionA.y) + (g->DirectionA.x*g->DirectionA.x) );
		depth = sqrt( (g->DirectionB.y*g->DirectionB.y) + (g->DirectionB.x*g->DirectionB.x) );
		const float height = top - z1;

		g->Width = width;
		g->Depth = depth;

		g->DirectionA.x /= width;
		g->DirectionA.y /= width;
		g->DirectionB.x /= depth;
		g->DirectionB.y /= depth;

		float dotProduct = g->DirectionA.x * g->DirectionB.x + g->DirectionA.y * g->DirectionB.y;
		g->Flags = 0;
		//if(flags & FLAG_IPL_DOOR_GOES_UP_AND_ROTATE)	g->Flags |= FLAG_GARAGE_DOOR_OPENS_UP_AND_ROTATE;
		//if(flags & FLAG_IPL_DOOR_GOES_IN)				g->Flags |= FLAG_GARAGE_DOOR_GOES_IN;
		if(flags & FLAG_IPL_CAMERA_FOLLOWS_PLAYER)		g->Flags |= FLAG_GARAGE_CAMERA_FOLLOWS_PLAYER;
		g->OriginalFlags = g->Flags;

		g->gStyle = doorstyle;
		g->OriginalStyle = doorstyle;

		CDebugLog::Trace(
			"\tAdded GRGX.\n"
			"\t\tName: %s\n"
			"\t\tCan store vehicle: %s\n"
			"\t\tIs Parking Garage: %s\n"
			"\t\tIs GARAGE_DONT_SAVE: %s\n"
			"\t\tWidth=%4.1f, Depth=%4.1f, Height=%4.1f,  dotProduct(dirA, dirB)=%4.2f",
				g->Name,
				g->DoesThisGarageCanStoreVehicles() ? "true" : "false",
				g->IsParkingGarage() ? "true" : "false",
				(g->gStyle & GARAGE_DONT_SAVE)? "true" : "false",
				g->Width, g->Depth, height, dotProduct);

		return g;
	}

	
}

void CGarages::Clear()
{
	for(auto it = garages.begin(); it != garages.end(); ++it)
		delete *it;
	garages.clear();
}

CGarageBase* CGarages::FindGarageByName(const char* name)
{
	for(auto it = CGarages::Garages().begin(); it != CGarages::Garages().end(); ++it)
	{
		if(!_stricmp(name, (**it).Name))
			return &(**it);
	}
	return nullptr;
}

void CGarages::Load(const char* fname)
{
	// BAD CODE OVER HERE!! WRITTEN FOR TESTING PURPOSES

	char Buffer[512];
	FILE* f = fopen(fname, "r");
	bool in_grge = false;
	bool in_section = false;

	while(fgets(Buffer, sizeof(Buffer), f))
	{
		if(Buffer[0] == '#') continue;

		if(in_section || in_grge)
		{
			if(Buffer[0] == 'e' && Buffer[1] == 'n' && Buffer[2] == 'd')
			{
				in_section = in_grge = false;
				continue;
			}
		}

		if(in_section)
			continue;
		else if(in_grge)
		{
			for(char* p = Buffer; *p; ++p)
			{
				if(*p == '\n' || *p == '#') {*p = 0; break;}
				if(*p == ',') *p = ' ';
			}
			GRGX_Parse(Buffer);
		}
		else if(Buffer[0] == 'i' && Buffer[1] == 'n' && Buffer[2] == 's' && Buffer[3] == 't')
			in_section = true;
		else if(Buffer[0] == 'g' && Buffer[1] == 'r' && Buffer[2] == 'g' && Buffer[3] == 'e')
			in_grge = true;
	}

	fclose(f);
}

#pragma pack(push, 1)
struct SaveFileStruct
{
	static const uint32_t magic = 0x58475247;
	static const uint32_t version = 0;

	struct Header
	{
		uint32_t magic;		// 0x58475247 - "GRGX"
		uint32_t version;
		SYSTEMTIME save_time;
		uint32_t num_garages;
	};

	struct GarageSection	// repeated num_garages times
	{
		char name[8];
		char script_name[8];
		uint32_t garage_data_offset;
		uint32_t garage_cars_offset;
		uint32_t num_cars;
		uint32_t gsize;
		uint8_t gtype;
		uint8_t gstyle;
		uint8_t flag_created_by_script;
		uint8_t original_gstyle;
		uint8_t original_flags;
		uint8_t original_gtype;	// unused, just to pad
		uint8_t pad[2];
	};

	typedef CGarageData GarageData;
	typedef CStoredCar CarData;
};
#pragma pack(pop)

void CreateSaveDirectory()
{
	if(CreateDirectoryA("grgx", nullptr))
	{
		if(FILE* f = fopen("grgx\\what_is_this.txt", "w"))
		{
			fputs("This directory stores garages for the mod GRGX (Garage eXtender)\n"
				  "Delete only if you've unistalled this mod\n",
				  f);
			fclose(f);
		}
	}
}

void CGarages::OnLoad(const CSaveSystem::SaveInfo& info)
{
	CDebugLog::Trace("CGarages::OnLoad() called with slot %d", info.slot + 1);
	if(info.slot == -1) return;
	CFileMgr::ChangeToUserDir();
	CreateSaveDirectory();
	
	char Buffer[128];
	sprintf(Buffer, "grgx\\%d.sav", info.slot + 1);
	if(FILE* f = fopen(Buffer, "rb"))
	{
		// No exception handling system!!!! (neither C++ streams)
		// a do...while can handle this.

		bool Done = false;
		do
		{
			SaveFileStruct::Header header;

			if (fread(&header, sizeof(header), 1, f) != 1) {
				CDebugLog::Trace("\tERROR fread(&header, sizeof(header), 1, f) != 1");
				break;
			}
			if (header.magic != SaveFileStruct::magic) {
				CDebugLog::Trace("\tERROR header.magic != SaveFileStruct::magic");
				break;
			}
			CDebugLog::Trace("\tnum_garages = %d", header.num_garages);

			if(memcmp(&header.save_time, &info.time, sizeof(SYSTEMTIME)) || header.num_garages == 0)
			{
				Done = true;
				break;
			}

			{
				SaveFileStruct::GarageSection* pGarages = new SaveFileStruct::GarageSection[header.num_garages];
				if(fread(pGarages, sizeof(*pGarages), header.num_garages, f) != header.num_garages)
				{
					delete[] pGarages;
					break;
				}

				bool Failure = false;
				for(size_t i = 0; i < header.num_garages; ++i)
				{
					CGarageData garage_data;
					CCustomGarage* garage;
					auto& grg = pGarages[i];

					if(fseek(f, grg.garage_data_offset, SEEK_SET) != 0
					|| fread(&garage_data, sizeof(garage_data), 1, f) != 1
					|| fseek(f, grg.garage_cars_offset, SEEK_SET) != 0)
					{
						Failure = true;
						break;
					}

					if(grg.flag_created_by_script)
					{
						garage = new CCustomGarage;
						CGarages::Garages().push_back(garage);

						memcpy(static_cast<CGarage*>(garage), &garage_data, sizeof(garage_data));
						strncpy(garage->thread_name, grg.script_name, 8);
						garage->thread = nullptr;
						garage->size = grg.gsize;
						garage->gType = static_cast<eGrgxGarageType>(grg.gtype);
						garage->gStyle = grg.gstyle;
						garage->OriginalFlags = grg.original_flags;
						garage->OriginalStyle = grg.original_gstyle;
						garage->OriginalType = garage_data.OriginalType;
						garage->Flags = garage_data.Flags;
					}
					else if(CGarageBase* gbase = CGarages::FindGarageByName(grg.name))
					{
						garage = static_cast<CCustomGarage*>(gbase);
						
						if (garage->Position.x != garage_data.Position.x
							|| garage->Position.y != garage_data.Position.y
							|| garage->Position.z != garage_data.Position.z) 
						{
							CDebugLog::Trace("\tWARNING Garage definition not found that corresponds to the save file!");
							continue;
						}
							

						garage->DoorPosition = garage_data.DoorPosition;
						garage->TimeToOpen = garage_data.TimeToOpen;
						garage->DoorState = garage_data.DoorState;
						garage->Flags = garage_data.Flags;
					}
					else continue;

					garage->CreatedByScript = grg.flag_created_by_script != 0;
					garage->pTargetCar = nullptr;
					garage->Type = GARAGE_TYPE_GRGX;

					if(!garage->DoesThisGarageCanStoreVehicles())
						continue;
					
					garage->ClearStoredCars();
					garage->ResizeThisGarage(grg.num_cars, true);

					size_t num_cars = garage->cars.size();
					if(num_cars > grg.num_cars) num_cars = grg.num_cars;
					if(num_cars)
					{
						if(fread(&garage->cars[0], sizeof(CStoredCar), num_cars, f) != num_cars)
						{
							CDebugLog::Trace("\tERROR Failed to restore cars!");
							Failure = true;
							break;
						}
						CDebugLog::Trace("\tRestored %d cars:", num_cars);
					}
				}
				delete[] pGarages;
				if(Failure) break;
			}

			Done = true;
			break;
		}
		while(false);

		if(!Done) CDebugLog::Trace("Corrupt save file: %s\n", Buffer);
		fclose(f);
	}

	CFileMgr::ChangeDir("");
}

void CGarages::OnSave(const CSaveSystem::SaveInfo& info)
{
	CDebugLog::Trace("CGarages::OnSave() called with slot %d", info.slot + 1);
	CFileMgr::ChangeToUserDir();
	CreateSaveDirectory();

	char Buffer[128];
	sprintf(Buffer, "grgx\\%d.sav", info.slot + 1);
	if(FILE* f = fopen(Buffer, "wb"))
	{
		size_t num_garages = 0;
		size_t next_data_block;
		int dontSaveCount = 0;

		for(auto it = CGarages::Garages().begin(); it != CGarages::Garages().end(); ++it)
		{
			if (!((**it).gStyle & GARAGE_DONT_SAVE)) {
				++num_garages;
			}
			else {
				dontSaveCount++;
			}

		}
		CDebugLog::Trace("\tnum_garages = %d, GARAGE_DONT_SAVE = %d", num_garages, dontSaveCount);

		SaveFileStruct::Header header =
			{ SaveFileStruct::magic, SaveFileStruct::version, info.time, num_garages };
		fwrite(&header, sizeof(header), 1, f);
		next_data_block = sizeof(header) + (sizeof(SaveFileStruct::GarageSection) * header.num_garages);

		if(header.num_garages)
		{
			SaveFileStruct::GarageSection* pGarages = new SaveFileStruct::GarageSection[header.num_garages];
			for(size_t k = 0, i = 0; k < CGarages::Garages().size(); ++k)
			{
				CCustomGarage* garage = static_cast<CCustomGarage*>(CGarages::Garages()[k]);
				if(garage->gStyle & GARAGE_DONT_SAVE) continue;

				garage->AdjustGarageSlots();

				SaveFileStruct::GarageSection& grg = pGarages[i];

				strncpy(grg.name, garage->Name, 7);
				grg.name[7] = 0;
				strncpy(grg.script_name, garage->thread_name, 8);

				grg.garage_data_offset = 0;
				grg.garage_cars_offset = 0;
				grg.gsize = garage->size;
				grg.num_cars = garage->CountNumCarsInThisGarage();
				grg.gtype = garage->gType;
				grg.gstyle = garage->gStyle;
				grg.original_flags = garage->OriginalFlags;
				grg.original_gstyle = garage->OriginalStyle;
				grg.original_gtype = garage->OriginalType;
				grg.pad[0] = 0xDE; grg.pad[1] = 0xAD;
				grg.flag_created_by_script = garage->CreatedByScript;

				grg.garage_data_offset = next_data_block; next_data_block += sizeof(SaveFileStruct::GarageData);
				if(grg.num_cars)
				{
					grg.garage_cars_offset = next_data_block;
					next_data_block += sizeof(SaveFileStruct::CarData) * grg.num_cars;
				}
				++i;
			}
			fwrite(pGarages, sizeof(*pGarages), header.num_garages, f);
			delete[] pGarages;

			for(size_t i = 0; i < CGarages::Garages().size(); ++i)
			{
				auto* garage = static_cast<CCustomGarage*>(CGarages::Garages()[i]);
				if (garage->gStyle & GARAGE_DONT_SAVE) {
					CDebugLog::Trace("\tGarage id=%d. GARAGE_DONT_SAVE", i);
					continue;
				}

				fwrite(static_cast<SaveFileStruct::GarageData*>(garage),
					sizeof(SaveFileStruct::GarageData), 1, f);
				const int count = garage->CountNumCarsInThisGarage();
				const bool canStore = garage->DoesThisGarageCanStoreVehicles();
				CDebugLog::Trace("\tGarage id=%d. CanStoreVehicle = %s, CountNumCarsInThisGarage() = %d", i, (canStore?"true":"false"), count);
				if(count)
				{
					fwrite(&garage->cars[0], sizeof(SaveFileStruct::CarData), count, f);
				}
			}
		}

		fclose(f);
	}

	CFileMgr::ChangeDir("");
}

bool CGarages::IsGRGX(CGarage* g)
{
	return g->Type == GARAGE_TYPE_GRGX;
}

bool CGarages::IsParkingGarage(CGarage* g)
{
	return (CGarages::IsGRGX(g) && static_cast<CGarageBase*>(g)->IsParkingGarage());
}

CGarage* CGarages::FindGarageByNameGlobal(const char* name)
{
	int index = CGarages::FindIndexByName(name);
	if(index >= 0) return &CGarages__Garages[index];
	else return CGarages::FindGarageByName(name);
}

void CGarages::SetTargetCarForMissionGarage(CGarage* g, CVehicle* veh)
{
	g->pTargetCar = veh;
	if(veh)
	{
		CEntity__RegisterReference(&veh->__parent.__parent,
			0, reinterpret_cast<CEntity**>(&g->pTargetCar));
		if(g->DoorState == 5) g->DoorState = 0;
	}
}

void CGarages::ChangeGarageType(CGarage* g, int type)
{
	if(CGarages::IsParkingGarage(g) && !static_cast<CCustomGarage*>(g)->CreatedByScript)
		return;
	
	bool CloseGarage = false;
	bool OpenGarage = false;

	if(!CGarages::IsGRGX(g))
	{
		g->Type = type >= 0? type : TranslateGarageType_GRGX_to_GRGE(static_cast<eGrgxGarageType>(-type));
		if(g->Type != 43)
		{
			OpenGarage = g->Type >= 2 && g->Type <= 5;
			CloseGarage = !OpenGarage;
		}
	}
	else
	{
		CCustomGarage* grgx = static_cast<CCustomGarage*>(g);
		TranslateGarageType_GRGE_to_GRGX(static_cast<CGarageBase*>(g), type);
		
		if(grgx->gType != IS_BURGALY_GARAGE)
		{
			OpenGarage = grgx->gType == IS_BOMBSHOP1_GARAGE || grgx->gType == IS_BOMBSHOP2_GARAGE
						 || grgx->gType == IS_BOMBSHOP3_GARAGE || grgx->gType == IS_RESPRAY_GARAGE;
			CloseGarage = !OpenGarage;
		}

		if(grgx->DoesThisGarageCanStoreVehicles())
			grgx->ResizeThisGarage(DEFAULT_GRGX_SIZE);
		else
			grgx->ResizeThisGarage(0);
	}

	if(CloseGarage)
	{
		g->DoorState = 0;
		g->DoorPosition = 0.0;
	}
	else if(OpenGarage)
	{
		g->DoorState = 1;
		g->DoorPosition = 1.0;
	}
}

void CGarages::TranslateGarageType_GRGE_to_GRGX(CGarageBase* g, int type)
{
	if(type < 0)
	{
		g->gType = static_cast<eGrgxGarageType>(-type);
		return;
	}

	g->gStyle = (DOOR_STYLE_OPENS_UP|DOOR_STYLE_ROTATES) | (g->gStyle & (GARAGE_DEPPENDS_ON_THREAD|GARAGE_DONT_SAVE));
	g->Flags &= ~(FLAG_GARAGE_CAMERA_FOLLOWS_PLAYER|FLAG_GARAGE_DOOR_GOES_IN|FLAG_GARAGE_DOOR_OPENS_UP_AND_ROTATE);
	switch(type)
	{
		default: g->Type = IS_NULL_GARAGE; break;
		case 2: g->gType = IS_BOMBSHOP1_GARAGE; break;
		case 3: g->gType = IS_BOMBSHOP2_GARAGE; break;
		case 4: g->gType = IS_BOMBSHOP3_GARAGE; break;
		case 5: g->gType = IS_RESPRAY_GARAGE; break;
		case 19: g->gType = IS_SCRIPT_GARAGE; break;
		case 36: g->gType = IS_LOCO_LOW_CO; break;
		case 37: g->gType = IS_WHEEL_ARCH_ANGEL; break;
		case 38: g->gType = IS_TRANSFENDER; break;
		case 43: g->gType = IS_BURGALY_GARAGE; break;
		
		case 44:
			g->gType = IS_OPENCLOSE_GARAGE;
			g->Flags |= FLAG_GARAGE_CAMERA_FOLLOWS_PLAYER;
			g->gStyle = DOOR_STYLE_OPENS_SLOW | (g->gStyle & (GARAGE_DEPPENDS_ON_THREAD|GARAGE_DONT_SAVE));
			break;
		
		case 33: case 34: case 35:
			g->gType = IS_IMPOUND_GARAGE;
			break;

		case 16: case 17: case 18: case 24: case 25: case 26: case 27: case 28:
		case 29: case 30: case 31: case 32: case 39: case 40: case 41: case 42:
			g->gType = IS_HIDEOUT_GARAGE;
			break;

		case 45:
			 g->gType = IS_HIDEOUT_GARAGE;
			 g->Flags |= FLAG_GARAGE_CAMERA_FOLLOWS_PLAYER;
			 g->gStyle = DOOR_STYLE_OPENS_TOSIDE|DOOR_STYLE_OPENS_SLOW|
				 (g->gStyle & (GARAGE_DEPPENDS_ON_THREAD|GARAGE_DONT_SAVE));
			 break;
	}
}

uint8_t CGarages::TranslateGarageType_GRGX_to_GRGE(eGrgxGarageType type)
{
	switch(type)
	{
		case IS_BOMBSHOP1_GARAGE:	return 2;
		case IS_BOMBSHOP2_GARAGE:	return 3;
		case IS_BOMBSHOP3_GARAGE:	return 4;
		case IS_RESPRAY_GARAGE:		return 5;
		case IS_SCRIPT_GARAGE:		return 19;
		case IS_LOCO_LOW_CO:		return 36;
		case IS_WHEEL_ARCH_ANGEL:	return 37;
		case IS_TRANSFENDER:		return 38;
		case IS_BURGALY_GARAGE:		return 43;
		case IS_OPENCLOSE_GARAGE:	return 44;
		case IS_HIDEOUT_GARAGE:		return 17;
		case IS_IMPOUND_GARAGE: 	return 33;
		default:					return 0;
	}
}

bool CGarages::HasCarBeenDroppedOffYet(CGarage* g)
{
	if(CGarages::IsGRGX(g))
	{
		CGarageBase* grgx = static_cast<CGarageBase*>(g);
		if(grgx->pTargetCar && grgx->IsEntityEntirelyInside(&grgx->pTargetCar->__parent.__parent))
			return 1;
	}
	return g->DoorState == GARAGE_DOOR_CLOSED_DROPPED_CAR;
}

void CGarages::DestroyGarage(CGarageBase* g)
{
	bool found = false;
	
	for(size_t idx = 0; idx < CGarages::Garages().size(); ++idx)
	{
		if(!found && CGarages::Garages()[idx] == g)
			found = true;

		if(found)
		{
			if(idx + 1 >= CGarages::Garages().size())
				break;
			CGarages::Garages()[idx] = CGarages::Garages()[idx+1];
		}
	}

	if(found)
	{
		g->DoorPosition = 0.0;
		g->DoorState = 0;
		if(g->door[0]) GRGX_ProcessGarageDoor(static_cast<CGarage*>(g), g->door[0]);
		if(g->door[1]) GRGX_ProcessGarageDoor(static_cast<CGarage*>(g), g->door[1]);

		delete g;
		CGarages::Garages().resize(CGarages::Garages().size() - 1);
	}
}


//======================================================================
//		CGarageBase
//======================================================================
CGarageBase::CGarageBase(size_t size)
{
	CGarage__constructor(this);
	this->size = size;
	this->Type = char(GARAGE_TYPE_GRGX);
	this->door[0] = this->door[1] = nullptr;
	this->CreatedByScript = false;
	this->pTargetCar = nullptr;
	this->thread = nullptr;
	this->thread_name[0] = 0;

	if(size == -1)
		this->cars.resize(DEFAULT_GRGX_SIZE);
	else if(size != 0)
		this->cars.resize(size);

	CDebugLog::Trace("\tCGarageBase constructed with size: %d = %d", size, this->cars.size());
	if(this->cars.size())
	{
		for(size_t x = 0; x < this->cars.size(); ++x)
			this->cars[x].model = 0;
	}
}

CGarageBase::~CGarageBase()
{
	void (__fastcall *sub_4DC6B0)(void* self) = (decltype(sub_4DC6B0))(0x4DC6B0);
	CDebugLog::Trace("\tCGarageBase %s destructed", this->Name);

	if(this->pTargetCar)
	{
		CEntity__UnRegisterReference(reinterpret_cast<CEntity*>(this->pTargetCar),
			0, reinterpret_cast<CEntity**>(&this->pTargetCar));
		this->pTargetCar = nullptr;
	}

	if(camera.field_820 == DWORD( static_cast<CGarage*>(this) ) )
		camera.field_820 = 0;
	if(camera.pInteractingGarage == DWORD( static_cast<CGarage*>(this) ) )
		camera.pInteractingGarage = 0;

	this->DoorPosition = 0.0;
	this->DoorState = 0;

	sub_4DC6B0(&this->_unknown_audio_object[0]);
	CGarage__destructor(this);
}


//======================================================================
//		CCustomGarage
//======================================================================
void CCustomGarage::FindDoorsEntities(CObject **door1, CObject **door2)
{
	*door1 = this->door[0];
	*door2 = this->door[1];
}

bool CCustomGarage::CheckIfScriptStillAlive()
{
	if(!(this->gStyle & GARAGE_DEPPENDS_ON_THREAD) || !this->CreatedByScript)
		return true;

	CRunningScript* script = CTheScripts::pActiveScripts;
	
	while(true)
	{
		while(script)
		{
			if(this->thread)
			{
				if(this->thread == script)
					return true;
			}
			else if(this->thread_name[0])
			{
				if(!strcmp(this->thread_name, script->threadName))
				{
					this->thread = script;
					return true;
				}
			}
			script = script->next;
		}

		if(this->thread == nullptr)
			break;
		else
			this->thread = nullptr;
	}

	return false;
}

bool CCustomGarage::ResizeThisGarage(size_t s, bool basic_load)
{
	if(!this->CreatedByScript)
	{
		if(this->size != -1) return 0;
		else if(s == this->cars.size()) return 1;
	}
	else
	{
		if(basic_load == false)
		{
			this->size = s;
			if(s == -1) s = DEFAULT_GRGX_SIZE;
			if(this->cars.size() != s)
				this->cars.resize(s);
			return 1;
		}
		else if(this->size != -1)
		{
			if(this->cars.size() != this->size)
				this->cars.resize(this->size);
			return 1;
		}
	}

	if(s <= DEFAULT_GRGX_SIZE && this->cars.size() != DEFAULT_GRGX_SIZE)
		this->cars.resize(DEFAULT_GRGX_SIZE);
	else if(s > DEFAULT_GRGX_SIZE && this->cars.size() != s)
		this->cars.resize(s);
	return 1;
}

void CCustomGarage::ClearStoredCars()
{
	for(auto it = this->cars.begin(); it != this->cars.end(); ++it)
		it->model = 0;
}

int CCustomGarage::MaximumNumberOfCarsOnThisGarage()
{
	return this->size == -1? 999 : this->size;
}

int CCustomGarage::CountNumCarsInThisGarage()
{
	if(this->size == 0 || !this->DoesThisGarageCanStoreVehicles())
		return 0;

	int result = 0;
	for(auto it = this->cars.begin(); it != this->cars.end(); ++it)
		if(it->model != 0) ++result;
	return result;
}

void CCustomGarage::AdjustGarageSlots(bool move, bool garbage)
{
	if(this->size == 0) return;

	if(move)
	{
		CStoredCar* last_free = nullptr;
		for(auto it = this->cars.begin(); it != this->cars.end(); ++it)
		{
			if(it->model == 0)
			{
				if(last_free == nullptr)
					last_free = &(*it);
			}
			else if(last_free)
			{
				memcpy(last_free, &(*it), sizeof(CStoredCar));
				it->model = 0;
				++last_free;
			}
		}
	}

	if(garbage)
	{
		// Collect garbage on this->cars()
		if(this->size == -1 && this->cars.size() > DEFAULT_GRGX_SIZE)
		{
			size_t nStored = this->CountNumCarsInThisGarage();
			if(nStored <= DEFAULT_GRGX_SIZE) this->cars.resize(DEFAULT_GRGX_SIZE);
			else if(this->cars.size() > nStored) this->cars.resize(nStored);
		}
	}
}

char CCustomGarage::RestoreCarsForThisGarage(CStoredCar*)
{
	if(this->size == 0) return 1;

	size_t restored_counter = 0;
	for(auto it = this->cars.begin(); it != this->cars.end(); ++it)
	{
		if(it->model)
		{
			if(CVehicle* veh = CStoredCar__RestoreCar(&(*it)))
			{
				veh->field_42D &= 0xDF;
				CWorld::Add(&veh->__parent.__parent);
				it->model = 0;
				++restored_counter;
			}
		}
		else ++restored_counter;
	}

	return (this->cars.size() - restored_counter) == 0;
}

void CCustomGarage::StoreAndRemoveCarsForThisGarage(CStoredCar*, signed int)
{
	CDebugLog::Trace("CCustomGarage::StoreAndRemoveCarsForThisGarage(...)");

	if(this->size == 0) return;
	this->ClearStoredCars();

	size_t nStored = 0;
	CPools::VehiclePool->for_each([this, &nStored](CVehicle* pVehicle)
	{
		const RwV3D vehicleCoords = GetCoords(&pVehicle->__parent.__parent);
		bool pointInGarage = this->IsPointWithinGarage(vehicleCoords);
		CDebugLog::Trace("\tGarage=%s. Car (%4.2f, %4.2f, %4.2f) Left=%4.2f, Right=%4.2f, Front=%4.2f, Back=%4.2f, Top=%4.2f, pointInGarage=%d", 
			this->Name, vehicleCoords.x, vehicleCoords.y, vehicleCoords.z, this->Left, this->Right, this->Front, this->Back, this->TopZ, pointInGarage);
		if (this->Left <= vehicleCoords.x && vehicleCoords.x <= this->Right &&
			this->Front <= vehicleCoords.y && vehicleCoords.y <= this->Back &&
			vehicleCoords.z <= this->TopZ) {
			CDebugLog::Trace("\tIS INSIDE");
		}
		if( pointInGarage )
		{
			// not a script vehicle and not garage full
			if(pVehicle->VehicleCreatedBy != 2)
			{
				bool CanStore = true;
				if(nStored >= this->cars.size() && this->size == -1)
				{
					this->cars.push_back(CStoredCar());
					this->cars.back().model = 0;
				}

				if(this->IsParkingGarage())
				{
					// has driver or (vehicle created by parked generator and !(player owned))
					bool PlayerOwned = (pVehicle->field_42A & 2)!=0;
					if(pVehicle->pDriver) return;
					
					if((pVehicle->VehicleCreatedBy == 3 && !PlayerOwned)
					|| ((pVehicle->__parent.__parent.type & 0xF8)==4 && !PlayerOwned) )
					{
						CanStore = false;
					}
				}

				if(CanStore && nStored < this->cars.size()
				&& (this->IsParkingGarage() || !this->IsEntityTouchingOutside(&pVehicle->__parent.__parent, 1.0)) )
				{
					CStoredCar__Store(&this->cars[nStored++], 0, pVehicle);
				}
				CWorld::Remove(&pVehicle->__parent.__parent);
				delete_me(pVehicle, 1);
			}
		}
	});

	this->AdjustGarageSlots(false, true);
}

void CCustomGarage::StoreAndRemoveCarsForThisImpoundLot(CStoredCar *ar, signed int max_slot)
{
	// Same as above
	this->StoreAndRemoveCarsForThisGarage(ar, max_slot);
}

void CCustomGarage::RestoreThisGarage()
{
	this->Flags = (this->Flags & 0x39) | 0x40;
	this->Type = char(GARAGE_TYPE_GRGX);
	this->gType = static_cast<eGrgxGarageType>(this->OriginalType);
	this->TimeToOpen = 0;

	switch(this->gType)
	{
		case IS_SCRIPT_GARAGE:
		case IS_HIDEOUT_GARAGE:		case IS_IMPOUND_GARAGE: case IS_PARKING_GARAGE:
		case IS_RESPRAY_GARAGE:		case IS_BURGALY_GARAGE:
		case IS_LOCO_LOW_CO:		case IS_WHEEL_ARCH_ANGEL:
		case IS_TRANSFENDER:		default:
			this->DoorState = GARAGE_DOOR_CLOSED;
			this->DoorPosition = 0.0;
			break;

		case IS_BOMBSHOP1_GARAGE:
		case IS_BOMBSHOP2_GARAGE:
		case IS_BOMBSHOP3_GARAGE:
			this->DoorState = GARAGE_DOOR_OPEN;
			this->DoorPosition = 1.0;
			break;
	}
}

char CCustomGarage::IsVehicleAcceptByThisTunningGarage(CVehicle* veh)
{
	if(veh == nullptr) return 0;
	//unsigned long handling_flags = *(DWORD*)(0xD0 + ((0xC2B9C8 + 0x14) + ((*(WORD*)(*((DWORD*)0xA9B0C8 + veh->__parent.__parent.m_wModelIndex) + 0x4A)) * 0xE0)));
	//Platinum Edit:
	//unsigned long handling_flags = *(DWORD*)( 0xD0 + ((0xC2B9C8 + 0x14) + ((*(WORD*)( *((DWORD*)ARRAY_ModelInfo + veh->__parent.__parent.m_wModelIndex) + 0x4A )) * 0xE0)) );
	//Goodidea82:
	DWORD modelIdx = veh->__parent.__parent.m_wModelIndex;
	unsigned long handling_flags =	*(DWORD*)(0xD0 + ((0xC2B9C8 + 0x14) + ((*(WORD*)(*((DWORD*)ARRAY_ModelInfo + (modelIdx*4)) + 0x4A)) * 0xE0)));

	switch(this->gType)
	{
		case IS_LOCO_LOW_CO:		return (handling_flags & 0x2000000)!=0;
		case IS_WHEEL_ARCH_ANGEL:	return (handling_flags & 0x4000000)!=0;
		case IS_TRANSFENDER:		return (!(handling_flags & 0x6000000));
		case IS_ANYCAR_TUNNING_SHOP:return 1;
		default:					return 0;
	}
}

void CCustomGarage::SetDoorsBackToOriginalHeight()
{
	if(this->IsParkingGarage()) return;
	switch(this->gType)
	{
		case IS_SCRIPT_GARAGE:
		case IS_HIDEOUT_GARAGE:		case IS_IMPOUND_GARAGE: case IS_PARKING_GARAGE:
		case IS_RESPRAY_GARAGE:		case IS_BURGALY_GARAGE:
		case IS_LOCO_LOW_CO:		case IS_WHEEL_ARCH_ANGEL:
		case IS_TRANSFENDER:		default:
			if(this->DoorState == GARAGE_DOOR_OPEN || this->DoorState == GARAGE_DOOR_OPENING)
				this->DoorState = GARAGE_DOOR_CLOSING;
			break;

		case IS_BOMBSHOP1_GARAGE:
		case IS_BOMBSHOP2_GARAGE:
		case IS_BOMBSHOP3_GARAGE:
			if(this->DoorState == GARAGE_DOOR_CLOSED || this->DoorState == GARAGE_DOOR_CLOSING)
				this->DoorState = GARAGE_DOOR_OPENING;
			break;
	}

}

char CCustomGarage::UpdateOpeningDoor()
{
	if(this->IsParkingGarage()) return 1;

	void (__fastcall *sub_4DC860)(void* self, int dummy, int, RwV3D*, int, float) = (decltype(sub_4DC860))(0x4DC860);
	CObject* door1 = nullptr, *door2 = nullptr;
	this->FindDoorsEntities(&door1, &door2);
	this->DoorPosition += CTimer::TimeStep *
		(float)(this->gStyle & DOOR_STYLE_OPENS_SLOW? 0.001099999994039536 : 0.01099999994039536);

	if(door1)
	{
		RwV3D* pos = GetCoords(&door1->__parent.__parent, 0);
		if(this->DoorPosition < 1.0)
			sub_4DC860(&this->_unknown_audio_object[0], 0, 159, pos, 0, 1.0);
		else
			sub_4DC860(&this->_unknown_audio_object[0], 0, 160, pos, 0, 1.0);
	}

	if(this->DoorPosition > 1.0) this->DoorPosition = 1.0;
	return this->DoorPosition == 1.0;
}

char CCustomGarage::UpdateClosingDoor()
{
	if(this->IsParkingGarage()) return 1;

	void (__fastcall *sub_4DC860)(void* self, int dummy, int, RwV3D*, int, float) = (decltype(sub_4DC860))(0x4DC860);
	CObject* door1 = nullptr, *door2 = nullptr;
	this->FindDoorsEntities(&door1, &door2);
	this->DoorPosition -= CTimer::TimeStep *
		(float)(this->gStyle & DOOR_STYLE_OPENS_SLOW? 0.001300000003539026 : 0.0130000002682209);

	if(door1)
	{
		RwV3D* pos = GetCoords(&door1->__parent.__parent, 0);
		if(this->DoorPosition > 0.0)
			sub_4DC860(&this->_unknown_audio_object[0], 0, 161, pos, 0, 1.0);
		else
			sub_4DC860(&this->_unknown_audio_object[0], 0, 162, pos, 0, 1.0);
	}

	if(this->DoorPosition < 0.0) this->DoorPosition = 0.0;
	return this->DoorPosition == 0.0;
}

void CCustomGarage::Update(int)
{
	this->UpdateCustomGarage();
}

bool CCustomGarage::UpdateCustomGarage()
{
	// Check if garage door still exists
	{
		if(this->IsParkingGarage())
		{
			this->door[0] = this->door[1] = nullptr;
		}
		else for(size_t i = 0; i < 2; ++i)
		{
			if(this->door[i])
			{
				if(!CPools::ObjectPool->Exists(this->door[i])
				|| !CGarages::IsModelIndexADoor(this->door[i]->__parent.__parent.m_wModelIndex))
				{
					this->door[i] = nullptr;
				}
			}
		}
	}


	// Do tidy up if there's a blowed vehicle blocking the garage
	if( !(CTimer::FrameCounter & 0x1F) )
	{
		if(this->CreatedByScript && this->gStyle & GARAGE_DEPPENDS_ON_THREAD)
		{
			if(!this->CheckIfScriptStillAlive())
			{
				CDebugLog::Trace("Destroying garage %s because script %s isn't alive!", this->Name, this->thread_name);
				CGarages::DestroyGarage(this);
				return false;
			}
		}

		if(this->IsParkingGarage())
		{
			//this->TidyUpGarage();
		}
		else
		{
			RwV3D dist;
			RwV3D* CameraPos = GetCoords(&camera.__parent, 0);
			dist.x = fabs(CameraPos->x - this->Left);
			dist.y = fabs(CameraPos->y - this->Front);
			if(dist.x > 40.0 || dist.y > 40.0)
				this->TidyUpGarageClose();
			else
				this->TidyUpGarage();
		}
	}

	if(!this->IsParkingGarage())
	{
		if(/*this->gStyle & DOOR_STYLE_OPENS_UP &&*/ this->gStyle & DOOR_STYLE_ROTATES)
			this->Flags |= FLAG_GARAGE_DOOR_OPENS_UP_AND_ROTATE;
		else
			this->Flags &= ~FLAG_GARAGE_DOOR_OPENS_UP_AND_ROTATE;

		if(this->gStyle & DOOR_STYLE_GOES_IN)
			this->Flags |= FLAG_GARAGE_DOOR_GOES_IN;
		else
			this->Flags &= ~FLAG_GARAGE_DOOR_GOES_IN;
	}

	if( !this->IsParkingGarage() && !(this->Flags & FLAG_GARAGE_CAMERA_FOLLOWS_PLAYER) )
	{
		if(CPed* xplayer = FindPlayerPed(-1))
		{
			CEntity* player = &xplayer->__parent.__parent;
			CVehicle* player_vehicle = FindPlayerVehicle(-1, 0);
			if(player_vehicle && player_vehicle->__parent.__parent.m_wModelIndex == 571)	// KART
				player = &player_vehicle->__parent.__parent;

			if(this->IsEntityEntirelyInside(player, 0.25))
			{
				CGarages__bCamShouldBeOutside = 1;
				camera.pInteractingGarage = DWORD(static_cast<CGarage*>(this));
			}

			if(player_vehicle)
			{
				RwV3D* vpos = GetCoords(&player_vehicle->__parent.__parent, 0);

				if(!this->IsEntityEntirelyOutside(&player_vehicle->__parent.__parent, 0.0))
					camera.field_820 = DWORD(static_cast<CGarage*>(this));
				
				if(player_vehicle->__parent.__parent.m_wModelIndex == 423 // MRWHOOP
					&& ((this->Left - 0.5) < vpos->x && (this->Right + 0.5) > vpos->x)
					&& ((this->Front - 0.5) < vpos->y && (this->Back + 0.5) > vpos->y))
				{
					CGarages__bCamShouldBeOutside = 1;
					camera.pInteractingGarage = DWORD(static_cast<CGarage*>(this));
				}
			}
		}
	}


	// Process garage
	if( !(this->Flags & FLAG_GARAGE_INACTIVE) || this->DoorState != GARAGE_DOOR_CLOSED )
	{
		if(this->IsParkingGarage())
		{
			this->Flags &= ~FLAG_GARAGE_DOOR_COLLIDABLE;
			this->gStyle = (this->gStyle & (GARAGE_DEPPENDS_ON_THREAD|GARAGE_DONT_SAVE));
		}
		else if( this->Flags & FLAG_GARAGE_DOOR_OPENS_UP_AND_ROTATE )
		{
			if( (this->DoorState != GARAGE_DOOR_OPENING || this->DoorPosition <= 0.4)
				&& this->DoorState != GARAGE_DOOR_OPEN )

				this->Flags |= FLAG_GARAGE_DOOR_COLLIDABLE;
			else
				this->Flags &= ~FLAG_GARAGE_DOOR_COLLIDABLE;
		}
		else this->Flags |= FLAG_GARAGE_DOOR_COLLIDABLE;


		switch(this->gType)
		{
			case IS_SCRIPT_GARAGE:
				this->ProcessScriptGarage();
				break;

			case IS_HIDEOUT_GARAGE:
				this->ProcessHideoutGarage();
				break;

			case IS_PARKING_GARAGE:
				this->ProcessParkingGarage();
				break;
			
			case IS_OPENCLOSE_GARAGE:
				this->ProcessBasicGarage();
				break;

			case IS_IMPOUND_GARAGE:
				this->ProcessImpoundGarage();
				break;

			case IS_RESPRAY_GARAGE:
				this->ProcessSprayGarage();
				break;

			case IS_BOMBSHOP1_GARAGE: case IS_BOMBSHOP2_GARAGE: case IS_BOMBSHOP3_GARAGE:
				this->ProcessBombshop();
				break;

			case IS_BURGALY_GARAGE:
				this->ProcessBurgalyGarage();
				break;

			case IS_WHEEL_ARCH_ANGEL: case IS_LOCO_LOW_CO: case IS_TRANSFENDER: case IS_ANYCAR_TUNNING_SHOP:
				this->ProcessTunningGarage();
				break;
		}
	}

	return true;
}

void CCustomGarage::ProcessSprayGarage()
{
	void (__fastcall *CParticleInfo__Set)(void*, int dummy, float, float, float, float, float, float, float)
	= (decltype(CParticleInfo__Set))(0x4AB290);
	void (__fastcall *CVehicleModelInfo__ChooseVehicleColour)(DWORD modelinf, int dummy, BYTE*, BYTE*, BYTE*, BYTE*, int)
		= (decltype(CVehicleModelInfo__ChooseVehicleColour))(0x4C8500);
	int (__fastcall *CVehicle__GetRemapIndex)(CVehicle*) = (decltype(CVehicle__GetRemapIndex))(0x6D0B70);
	void (*sub_566A60)(float a, float b, float c, float d) = (decltype(sub_566A60))(0x566A60);
	void (*IncreasePlayerStat)(short stat, float val) = (decltype(IncreasePlayerStat))(0x55C180);
	float (*GetPlayerStat)(short stat) = (decltype(GetPlayerStat))(0x558E40);
	void (*SetPlayerStatValue)(short stat, float value) = (decltype(SetPlayerStatValue))(0x55A070);
	CPad* pad = (CPad*)(0x53FB70);
	void* gxt = (void*)(0xC1B340);
	void* audio = (void*)(0xB6BC90);

	CVehicle* veh = FindPlayerVehicle(-1, 0);

	static CCustomGarage* UsingPnS = nullptr;

	switch(this->DoorState)
	{
		case GARAGE_DOOR_OPEN:
		{
			if(CGarage__ResprayGaragesDisabled) return;
			if(veh && this->IsStaticPlayerCarEntirelyInside())
			{
				if(CGarages::IsVehicleAcceptedByResprayGarage(veh))
				{
					if(CGarages__RespraysAreFree || this->Flags & FLAG_GARAGE_RESPRAY_ALWAYS_FREE || PlayerMoney() >= 100)
					{
						this->DoorState = GARAGE_DOOR_CLOSING;
						MakePlayerControllable(false);
						veh->field_4B0 = 0;	// dirt level
					}
					else
					{
						CGarages::TriggerMessage("GA_3", -1, 4000, -1);		// No more freebies~n~$100 to respray!
						CAudio__ReportFrontendAudioEvent(audio, 0, 14, 0, 1.0);
					}
				}
				else
				{
					CAudio__ReportFrontendAudioEvent(audio, 0, 15, 0, 1.0);
					if(veh->SubClass == 10) // CBMX
						CGarages::TriggerMessage("GA_1B", -1, 4000, -1);	// ~r~I dont do toys!
					else
						CGarages::TriggerMessage("GA_1", -1, 4000, -1);		// ~r~Whoa! I dont touch nothing THAT hot!
				}

				UsingPnS = this;
				MakePlayerPoliceFree(true);
			}
			else
			{
				if(this->IsPlayerEntirelyOutside(0.0))
				{
					if(UsingPnS == this)
					{
						MakePlayerPoliceFree(true);
						UsingPnS = nullptr;
					}
				}
				else
				{
					UsingPnS = this;
					MakePlayerPoliceFree(false);
				}
			}

			if(veh)
			{
				RwV3D* pos = GetCoords(&veh->__parent.__parent, 0);
				if(this->GetDistanceFromPoint(pos->x, pos->y) < 60.0)
					sub_566A60(this->Left - 10.0, this->Front - 10.0, this->Right + 10.0, this->Back + 10.0);
			}
			break;
		}

		case GARAGE_DOOR_CLOSING:
		{
			if(veh) this->PushOutVehicles(veh);

			if(this->UpdateClosingDoor())
			{
				this->DoorState = GARAGE_DOOR_CLOSED;
				this->TimeToOpen = CTimer::TimeInMiliseconds + 2000;
				CAudio__ReportFrontendAudioEvent(audio, 0, 16, 0.0, 1.0);
				IncreasePlayerStat(177, GetPlayerStat(176));
				SetPlayerStatValue(176, 0);
			}

			if(veh)
			{
				if(!veh->Class) // Is CAutomobile
					reinterpret_cast<CAutomobile*>(veh)->m_fBurningTime = 0;
				else if(veh->Class == 9)
					*(float*)((uint8_t*)veh + 0x7BC) = 0;
				veh->field_42E |= 0x80;	// Disable particles
			}
			sub_566A60(this->Left - 10.0, this->Front - 10.0, this->Right + 10.0, this->Back + 10.0);
			break;
		}

		case GARAGE_DOOR_CLOSED:
		{
			if(CTimer::TimeInMiliseconds < this->TimeToOpen)
			{
				sub_566A60(this->Left - 10.0, this->Front - 10.0, this->Right + 10.0, this->Back + 10.0);
				return;
			}

			bool WasWanted = false;
			bool ChangedColour = false;
			bool ShouldPay = false;
			this->DoorState = GARAGE_DOOR_OPENING;

			if(WasWanted = PlayerWantedLevel() != 0)
			{
				ShouldPay = true;
				MakePlayerNotable();
			}

			if(veh && (veh->Class == 0 || veh->Class == 9))
			{
				typedef void (__fastcall *CVehicle__Fix_)(CVehicle*);

				if(*(float*)(&veh->fHealth) < 970.0)
					ShouldPay = true;
				*(float*)(&veh->fHealth) = max(1000.0, *(float*)(&veh->fHealth));
				if(!veh->Class) // Is CAutomobile
					reinterpret_cast<CAutomobile*>(veh)->m_fBurningTime = 0;
				else if(veh->Class == 9)
					*(float*)((uint8_t*)veh + 0x7BC) = 0;
				(reinterpret_cast<CVehicle__Fix_>(*(DWORD*)((*(DWORD*)(veh) + 0xC8))))(veh);	// fix
				IncreasePlayerStat(138, 1.0);

				CMatrix_Padded* matrix = (CMatrix_Padded*)(veh->__parent.__parent.__parent.m_pCoords);
				if(matrix->vUp.z < 0.0)
				{
					matrix->vUp.x = -matrix->vUp.x;
					matrix->vUp.y = -matrix->vUp.y;
					matrix->vUp.z = -matrix->vUp.z;
					matrix->vRight.x = -matrix->vRight.x;
					matrix->vRight.y = -matrix->vRight.y;
					matrix->vRight.z = -matrix->vRight.z;
				}

				if( ((*(uint8_t*)((uint8_t*)veh + 0x868)) & 0x2)==0 // Unknown check
				&&	CVehicle__GetRemapIndex(veh) < 0 )	
				{
					uint8_t c1, c2, c3, c4;
					DWORD modelIdx = veh->__parent.__parent.m_wModelIndex; //Goodidea82
					CVehicleModelInfo__ChooseVehicleColour(
						//*(DWORD*)(0xA9B0C8 + veh->__parent.__parent.m_wModelIndex*4), 0, &c1, &c2, &c3, &c4, 1);
						//Platinum Edit:
						//*(DWORD*)(ARRAY_ModelInfo + veh->__parent.__parent.m_wModelIndex * 4), 0, &c1, &c2, &c3, &c4, 1);
						//Goodidea82:
						*(DWORD*)(ARRAY_ModelInfo + modelIdx * 4), 0, &c1, &c2, &c3, &c4, 1);

					if(veh->primaryColor != c1 || veh->secondaryColor != c2
					|| veh->tertiaryColor != c3 || veh->quaternaryColor != c4)
						ChangedColour = true;

					veh->primaryColor = c1, veh->secondaryColor = c2,
					veh->tertiaryColor = c3, veh->quaternaryColor = c4;
					// change remap?

					// create particles of the spray
					if(ChangedColour)
					{
						char Buffer[512];	// CParticleInfo
						uint8_t* Colour = (uint8_t*)(0xB4E480 + veh->primaryColor*4);

						CParticleInfo__Set(Buffer, 0, Colour[0] / 255.0, Colour[1] / 255.0, Colour[2] / 255.0,
							0.6, 0.699, 1.0, 0.4);

						for(size_t i = 0; i < 10; ++i)
						{
							RwV3D velocity;
							RwV3D pos = GetCoords(&veh->__parent.__parent.__parent);
							pos.x += randomf(-3.0, 3.0);
							pos.y += randomf(-3.0, 3.0);
							velocity.x = 0.0;
							velocity.y = 0.0;
							velocity.z = randomf(0.0, 0.05);
							CParticleFx__Add( *(DWORD**)(0xA9AE20), 0, &pos, &velocity, 0.0, Buffer, -1.0, 1.2, 0.6, 0 );
						}
					}
				}

				veh->field_4B0 = 0;	// dirt level
				veh->field_42E &= 0x7F;	// Reactive particles
			}

			char* msg = nullptr;
			{
				if(this->Flags & FLAG_GARAGE_RESPRAY_ALWAYS_FREE)
				{
					msg = "GA_22"; // Your girlfriend has fixed your vehicle.
				}
				else if(!ShouldPay || CGarages__RespraysAreFree)
				{
					if(ChangedColour)
						msg = rand() & 1? "GA_15" : "GA_16"; // Hope you like the new color -- Respray is complementary
				}
				else
				{
					PlayerMoney() -= 100;
					IncreasePlayerStat(16, 100.0);
					if(WasWanted)
						msg = "GA_2";		// New engine and paint job: $100~n~Cops wont recognize you!
					else
						msg = "GA_XX";		//New engine and paint job: $100
				}
			}
			if(msg) CGarages::TriggerMessage(msg, -1, 4000, -1);
			if(veh) veh->field_42F |= 1;
			this->Flags |= FLAG_GARAGE_USED_RESPRAY;

			sub_566A60(this->Left - 10.0, this->Front - 10.0, this->Right + 10.0, this->Back + 10.0);
			break;
		}

		case GARAGE_DOOR_OPENING:
		{
			if(this->UpdateOpeningDoor())
				this->DoorState = GARAGE_DOOR_WAITING_PLAYER_TO_EXIT;
			if(this->DoorPosition > 0.5)
				MakePlayerControllable(true);
			break;
		}

		case GARAGE_DOOR_WAITING_PLAYER_TO_EXIT:
		{
			if(this->IsPlayerEntirelyOutside(0.0))
				this->DoorState = GARAGE_DOOR_OPEN;
			break;
		}
	}
}

void CCustomGarage::ProcessBombshop()
{
	void (*IncreasePlayerStat)(short stat, float val) = (decltype(IncreasePlayerStat))(0x55C180);
	CPad* pad = (CPad*)(0x53FB70);
	void* gxt = (void*)(0xC1B340);
	void* audio = (void*)(0xB6BC90);
	CVehicle* veh = FindPlayerVehicle(-1, 0);

	switch(this->DoorState)
	{
		case GARAGE_DOOR_OPEN:
		{
			if(veh && this->IsStaticPlayerCarEntirelyInside())
			{
				if(veh->SubClass != 9 && veh->SubClass != 10)	// Isn't bike
				{
					if(veh->flags2 & 7)
					{
						CGarages::TriggerMessage("GA_5", -1, 4000, -1);		// ~r~Your car is already fitted with a bomb!
						CAudio__ReportFrontendAudioEvent(audio, 0, 17, 0.0, 1.0);
						this->DoorState = GARAGE_DOOR_WAITING_PLAYER_TO_EXIT;
						return;
					}

					if(CGarages__BombsAreFree || PlayerMoney() >= 500)
					{
						this->DoorState = GARAGE_DOOR_CLOSING;
						MakePlayerControllable(false);
					}
					else
					{
						CGarages::TriggerMessage("GA_4", -1, 4000, -1);		// Car bombs are $500 each.
						CAudio__ReportFrontendAudioEvent(audio, 0, 14, 0.0, 1.0);
						this->DoorState = GARAGE_DOOR_WAITING_PLAYER_TO_EXIT;
						return;
					}
				}
			}
			break;
		}

		case GARAGE_DOOR_CLOSING:
		{
			if(veh) this->PushOutVehicles(veh);
			if(this->UpdateClosingDoor())
			{
				this->DoorState = GARAGE_DOOR_CLOSED;
				this->TimeToOpen = CTimer::TimeInMiliseconds + 2000;
			}
			if(this->gType == IS_BOMBSHOP3_GARAGE)
				CStreaming::RequestModel(364, 2);
			break;
		}

		case GARAGE_DOOR_CLOSED:
		{
			const char* msg;

			if(CTimer::TimeInMiliseconds < this->TimeToOpen)
				return;
			else if((this->gType == IS_BOMBSHOP3_GARAGE && CStreaming::aInfoForModel[364].State != 1))
			{
				CStreaming::RequestModel(364, 2);
				return;
			}
			else if(this->gType == IS_BOMBSHOP1_GARAGE)
			{
				CAudio__ReportFrontendAudioEvent(audio, 0, 18, 0.0, 1.0);
				msg = "GA_6";	// Park it, prime it by pressing ~k~~PED_FIREWEAPON~ and LEG IT!
			}
			else if(this->gType == IS_BOMBSHOP2_GARAGE)
			{
				CAudio__ReportFrontendAudioEvent(audio, 0, 19, 0.0, 1.0);
				msg = "GA_7";	// Arm with ~k~~PED_FIREWEAPON~. Bomb will go off when engine is started.
			}
			else if(this->gType == IS_BOMBSHOP3_GARAGE)
			{
				CAudio__ReportFrontendAudioEvent(audio, 0, 20, 0.0, 1.0);
				msg = "GA_8";	// Use the detonator to activate the bomb.
			}

			PrintHelp(CText__Get(gxt, 0, msg), 0, 0, 1);
			this->DoorState = GARAGE_DOOR_OPENING;
			if(!CGarages__BombsAreFree) PlayerMoney() -= 500;

			if(veh && veh->Class == 0 || veh->Class == 9)
			{
				IncreasePlayerStat(127, 10.0);	// Kgs of explosives used
				veh->flags2 |= this->GetBombflagForThisGarage();
				if(this->gType == IS_BOMBSHOP3_GARAGE)
					CGarages::GivePlayerDetonator();
			}
			break;
		}

		case GARAGE_DOOR_OPENING:
		{
			if(this->UpdateOpeningDoor())
				this->DoorState = GARAGE_DOOR_WAITING_PLAYER_TO_EXIT;
			if(this->DoorPosition > 0.5)
				MakePlayerControllable(true);
			break;
		}

		case GARAGE_DOOR_WAITING_PLAYER_TO_EXIT:
		{
			if(this->IsPlayerEntirelyOutside(0.0))
				this->DoorState = GARAGE_DOOR_OPEN;
			break;
		}
	}
}

void CCustomGarage::ProcessScriptGarage()
{
	switch(this->DoorState)
	{
		case GARAGE_DOOR_OPENING:
		{
			if(this->UpdateOpeningDoor())
				this->DoorState = GARAGE_DOOR_OPEN;
			break;
		}

		case GARAGE_DOOR_CLOSING:
		{
			if(this->UpdateClosingDoor())
				this->DoorState = GARAGE_DOOR_CLOSED;
			break;
		}
	}
}

void CCustomGarage::ProcessHideoutGarage()
{
	CVehicle* veh = FindPlayerVehicle(-1, 0);
	RwV3D pos;
	if(CPed* ped = FindPlayerPed(-1)) pos = GetCoords(&ped->__parent.__parent);
	else pos.x = 0, pos.y = 0;

	switch(this->DoorState)
	{
		case GARAGE_DOOR_CLOSED:
		{
			CObject* door1 = nullptr, *door2 = nullptr;
			float dist = this->GetDistanceFromPoint(pos.x, pos.y);
			if(dist < 12.25 || (veh && veh->SubClass != 10 && dist < 100.0))
			{
				// if has no car open it
				if(veh && this->CountNumCarsInThisGarage() >= this->MaximumNumberOfCarsOnThisGarage())
				{
					RwV3D* veh_pos = GetCoords(&veh->__parent.__parent, 0);
					this->FindDoorsEntities(&door1, &door2);
					if((door1 && GetDistanceBetweenPoints2D(*veh_pos, *GetCoords(&door1->__parent.__parent, 0)) < 25.0)
					|| (door2 && GetDistanceBetweenPoints2D(*veh_pos, *GetCoords(&door2->__parent.__parent, 0)) < 25.0))
					{
						 int (__fastcall *CVehicle__GetVehicleType)(CVehicle* self)
							 = (decltype(CVehicle__GetVehicleType))(0x6D1080);

						if(CTimer::TimeInMiliseconds - CGarages__LastTimeHelpMessage > 18000)
						{
							int type = CVehicle__GetVehicleType(veh);
							if(type != 3 && type != 5)	// not CHeli neither CPlane
							{
								// ~w~You cannot store any more vehicles in this garage!!
								PrintHelp(CText__Get((void*)(0xC1B340), 0, "GA_21"), 0, 0, 1);
								CGarages__LastTimeHelpMessage = CTimer::TimeInMiliseconds;
							}
						}
					}
				}
				else if(this->RestoreCarsForThisGarage())
				{
					this->DoorState = GARAGE_DOOR_OPENING;
				}
			}
			break;
		}
		
		case GARAGE_DOOR_OPENING:
		{
			if(this->UpdateOpeningDoor())
				this->DoorState = GARAGE_DOOR_OPEN;
			break;
		}

		case GARAGE_DOOR_OPEN:
		{
			float dist = this->GetDistanceFromPoint(pos.x, pos.y);
			if(dist > 4900.0)
			{
				this->RemoveCarsBlockingDoorNotInside();
				this->DoorState = GARAGE_DOOR_CLOSING;
			}
			else if( (dist > 255.0 || (dist > 16.0 && (!veh || veh->SubClass == 10))) 
			||  (veh && this->CountCarsWithCenterPointWithinGarage(veh) >= this->MaximumNumberOfCarsOnThisGarage()
					&& this->IsPlayerEntirelyOutside(0.25))
			)
			{
				if(!this->IsAnyCarBlockingDoor())
					this->DoorState = GARAGE_DOOR_CLOSING;
			}
			break;
		}

		case GARAGE_DOOR_CLOSING:
		{
			if(!this->IsPlayerEntirelyOutside(0.0))
			{
				this->DoorState = GARAGE_DOOR_OPENING;
			}
			else if(this->UpdateClosingDoor())
			{
				this->DoorState = GARAGE_DOOR_CLOSED;
				this->StoreAndRemoveCarsForThisGarage();
			}
			break;
		}

	}
}

// IMPOUND LOT IS FUCKED UP, NO IDEA HOW R* PLANNED TO DO THIS SHITTTTTTTTT
void CCustomGarage::ProcessImpoundGarage()
{
	CVehicle* veh = FindPlayerVehicle(-1, 0);
	bool on_lot = false;
	RwV3D pos;
	if(CPed* ped = FindPlayerPed(-1)) pos = GetCoords(&ped->__parent.__parent);
	else pos.x = 0, pos.y = 0;

	switch(this->DoorState)
	{
		case GARAGE_DOOR_CLOSING:
		case GARAGE_DOOR_OPEN:
		{
			if(this->TopZ - 2.0 > pos.z && pos.z > this->Position.z)
				on_lot = true;

			if(this->GetDistanceFromPoint(pos.x, pos.y) > 4225.0 || !on_lot || this->DoorState == GARAGE_DOOR_CLOSING)
			{
				this->DoorState = GARAGE_DOOR_CLOSED;
				this->StoreAndRemoveCarsForThisImpoundLot();
			}
			break;
		}


		case GARAGE_DOOR_CLOSED:
		{
			if(this->TopZ - 2.0 > pos.z && pos.z > this->Position.z)
				on_lot = true;

			if(this->GetDistanceFromPoint(pos.x, pos.y) < 3600.0 && on_lot)
			{
				this->MakeCarCoorsForImpoundLot(&this->cars[0]);
				if(this->RestoreCarsForThisImpoundLot(&this->cars[0]))
					this->DoorState = GARAGE_DOOR_OPEN;
			}
			break;
		}
	}
}

void CCustomGarage::ProcessBurgalyGarage()
{
	CVehicle* veh = FindPlayerVehicle(-1, 0);

	switch(this->DoorState)
	{
		case GARAGE_DOOR_OPEN:
		{
			RwV3D pos;
			if(CPed* ped = FindPlayerPed(-1)) pos = GetCoords(&ped->__parent.__parent);
			else pos.x = 0, pos.y = 0;

			if(this->GetDistanceFromPoint(pos.x, pos.y) > 900.0)
			{
				if(!this->IsAnyOtherCarTouchingGarage(nullptr))
					this->DoorState = GARAGE_DOOR_CLOSING;
			}
			break;
		}

		case GARAGE_DOOR_OPENING:
		{
			if(this->UpdateOpeningDoor())
				this->DoorState = GARAGE_DOOR_OPEN;
			break;
		}

		case GARAGE_DOOR_CLOSING:
		{
			if(veh) this->PushOutVehicles(veh);
			if(this->UpdateClosingDoor())
				this->DoorState = GARAGE_DOOR_CLOSED;
			break;
		}
	}
}

void CCustomGarage::ProcessTunningGarage()
{
	CVehicle* veh = FindPlayerVehicle(-1, 0);

	switch(this->DoorState)
	{
		case GARAGE_DOOR_CLOSED:
		{
			if(veh && this->IsVehicleAcceptByThisTunningGarage(veh))
			{
				RwV3D* pos = GetCoords(&veh->__parent.__parent, 0);
				if(this->GetDistanceFromPoint(pos->x, pos->y) < 64.0)
					this->DoorState = GARAGE_DOOR_OPENING;
			}
			break;
		}
		
		case GARAGE_DOOR_OPENING:
		{
			if(this->UpdateOpeningDoor())
				this->DoorState = GARAGE_DOOR_OPEN;
			break;
		}

		case GARAGE_DOOR_OPEN:
		{
			RwV3D pos;
			if(CPed* ped = FindPlayerPed(-1))
				pos = GetCoords(&ped->__parent.__parent);
			else 
				pos.x = 0, pos.y = 0;

			if(!(CTimer::FrameCounter & 0x1F) && this->GetDistanceFromPoint(pos.x, pos.y) > 900.0)
			{
				if(!veh || !this->IsVehicleAcceptByThisTunningGarage(veh)
				|| !this->IsEntityTouching3D(&veh->__parent.__parent))
				{
					if(!this->IsAnyOtherCarTouchingGarage(nullptr))
					{
						this->Flags |= FLAG_GARAGE_NOT_UNDER_SCRIPT_CONTROL;
						this->DoorState = GARAGE_DOOR_CLOSING;
					}
				}
			}
			break;
		}

		case GARAGE_DOOR_CLOSING:
		{
			if(veh) this->PushOutVehicles(veh);
			if(this->UpdateClosingDoor())
				this->DoorState = GARAGE_DOOR_CLOSED;
			break;
		}

	}
}

void CCustomGarage::ProcessParkingGarage()
{
	uint32_t& ActiveInterior = *(uint32_t*)(0xB72914);
	static const float DistanceToRestoreCarsOnFoot = 36000.0;
	static const float DistanceToRestoreCarsOnCar = 40000.0;
	static const float DistanceToRemoveCars = DistanceToRestoreCarsOnCar + 10.0;

	
	CVehicle* veh = FindPlayerVehicle(-1, 0);
	RwV3D pos;
	if(CPed* ped = FindPlayerPed(-1)) pos = GetCoords(&ped->__parent.__parent);
	else pos.x = 0, pos.y = 0;

	switch(this->DoorState)
	{
		case GARAGE_DOOR_CLOSED:
		{
			if(ActiveInterior == 0)
			{
				float dist = this->GetDistanceFromPoint(pos.x, pos.y);
				if(veh? dist < DistanceToRestoreCarsOnCar : dist < DistanceToRestoreCarsOnFoot)
					this->DoorState = GARAGE_DOOR_OPENING;
			}
			break;
		}
		
		case GARAGE_DOOR_OPENING:
		{
			if(this->RestoreCarsForThisGarage())
				this->DoorState = GARAGE_DOOR_OPEN;
			break;
		}

		case GARAGE_DOOR_OPEN:
		{
			float dist = this->GetDistanceFromPoint(pos.x, pos.y);
			if(ActiveInterior != 0 || dist >= DistanceToRemoveCars)
			{
				this->DoorState = GARAGE_DOOR_CLOSING;
				this->StoreAndRemoveCarsForThisGarage();
				this->DoorState = GARAGE_DOOR_CLOSED;
			}
			break;
		}

		case GARAGE_DOOR_CLOSING:
		{
			this->DoorState = GARAGE_DOOR_CLOSED;
			this->StoreAndRemoveCarsForThisGarage();
			break;
		}

	}
}

void CCustomGarage::ProcessBasicGarage()
{
	CVehicle* veh = FindPlayerVehicle(-1, 0);
	RwV3D pos;
	if(CPed* ped = FindPlayerPed(-1)) pos = GetCoords(&ped->__parent.__parent);
	else pos.x = 0, pos.y = 0;

	switch(this->DoorState)
	{
		case GARAGE_DOOR_CLOSED:
		{
			if(!this->pTargetCar)
			{
				float dist = this->GetDistanceFromPoint(pos.x, pos.y);
				if(dist < 12.25 || (veh && veh->SubClass != 10 && dist < 100.0))
					this->DoorState = GARAGE_DOOR_OPENING;
				break;
			}
			else
			{
				RwV3D* car_pos = GetCoords(&this->pTargetCar->__parent.__parent.__parent, 0);
				if(!this->IsEntityEntirelyInside(&this->pTargetCar->__parent.__parent)
				&&  this->GetDistanceFromPoint(car_pos->x, car_pos->y) < 255.0)
					this->DoorState = GARAGE_DOOR_OPENING;
				break;
			}
			break;
		}
		
		case GARAGE_DOOR_OPENING:
		{
			if(this->pTargetCar)
			{
				RwV3D* car_pos = GetCoords(&this->pTargetCar->__parent.__parent.__parent, 0);
				if(this->GetDistanceFromPoint(car_pos->x, car_pos->y) > 300.0)
				{
					this->Flags |= 1;
					this->DoorState = GARAGE_DOOR_CLOSING;
					break;
				}
				//else this->PushOutVehicles(this->pTargetCar);
			}
			if(this->UpdateOpeningDoor())
				this->DoorState = GARAGE_DOOR_OPEN;
			break;
		}

		case GARAGE_DOOR_OPEN:
		{
			float dist = this->GetDistanceFromPoint(pos.x, pos.y);

			if(this->pTargetCar)
			{
				RwV3D* car_pos = GetCoords(&this->pTargetCar->__parent.__parent.__parent, 0);
				if(this->GetDistanceFromPoint(car_pos->x, car_pos->y) > 300.0 && !this->IsAnyCarBlockingDoor())
				{
					this->Flags |= 1;
					this->DoorState = GARAGE_DOOR_CLOSING;
					break;
				}
				else if( true )
				{
					if(this->IsEntityEntirelyInside(&this->pTargetCar->__parent.__parent)
					&& this->IsPlayerEntirelyOutside(0.0) && !this->IsAnyCarBlockingDoor())
					{
						this->Flags &= 0xFE;
						this->DoorState = GARAGE_DOOR_CLOSING;
					}
				}
			}
			else if(dist > 4900.0)
			{
				this->RemoveCarsBlockingDoorNotInside();
				this->DoorState = GARAGE_DOOR_CLOSING;
			}
			else if( (dist > 255.0 || (dist > 16.0 && (!veh || veh->SubClass == 10))) )
			{
				if(!this->IsAnyCarBlockingDoor())
					this->DoorState = GARAGE_DOOR_CLOSING;
			}
			break;
		}

		case GARAGE_DOOR_CLOSING:
		{
			if(!this->IsPlayerEntirelyOutside(0.0))
				this->DoorState = GARAGE_DOOR_OPENING;
			else if(this->UpdateClosingDoor())
				this->DoorState = GARAGE_DOOR_CLOSED;
			break;
		}
	}

}


//======================================================================
//		Hooks
//			Low-level part of the mod is here!
//======================================================================
namespace GRGX_HOOKS
{
	char HOODLUM;

	void HOOK_IsPointWithinHideOutGarage();
	void HOOK_IsPointWithinAnyGarage();
	void __cdecl HOOK_OpenSprayGarages(char bOpen);
	void __cdecl HOOK_RestoreGaragesForNewGame();
	void HOOK_IsPointInAGarageCameraZone();
	void __cdecl HOOK_PlayerArrestedOrDied();
	void HOOK_CloseGaragesBeforeSave();
	void __cdecl HOOK_StoreCarAtImpoundLot(CVehicle *veh);
	char __fastcall HOOK_RestoreCarsForThisImpoundLot(CGarage* g, int, CStoredCar* cars);
	void HOOK_Obj_ProcessGarageDoor();
	void HOOK_UpdateGarages();
	void HOOK_RestoreCarsForParkingGarage();
//	void HOOK_CheckForAttachedGarage();

	void HOOKVIRTUAL_RestoreThisGarage();
	void HOOKVIRTUAL_IsVehicleAcceptByThisTunningGarage();
	void HOOKVIRTUAL_SetDoorsBackToOriginalHeight();
	int __fastcall HOOKVIRTUAL_MaximumNumberOfCarsOnThisGarage(CGarage* self);
	void HOOKVIRTUAL_StoreAndRemoveCarsForThisGarage();
	void HOOKVIRTUAL_StoreAndRemoveCarsForThisImpoundLot();
	void HOOKVIRTUAL_RestoreCarsForThisGarage();
	void HOOKVIRTUAL_UpdateOpeningDoor();
	void HOOKVIRTUAL_UpdateClosingDoor();
	void HOOKVIRTUAL_Update();
	void HOOKVIRTUAL_FindDoorsEntities();
};

void SaveSystemHandler(CSaveSystem::Action action, const CSaveSystem::SaveInfo& info)
{
	switch(action)
	{
		case CSaveSystem::ACTION_NEWGAME: case CSaveSystem::ACTION_LOADGAME:
			CGarages::OnLoad(info);
			break;

		case CSaveSystem::ACTION_SAVEGAME:
			CGarages::OnSave(info);
			break;
	}
}

void CGarages::Patch()
{
	using namespace GRGX_HOOKS;
	uint32_t zero = 0;
	HOODLUM = ReadMemoryNow<uint8_t>(0x401000, true) == 0xE9;
	CDebugLog::Trace("CGarages::Patch() with is HOODLUM: %s", HOODLUM? "true" : "false");

	//
	CSaveSystem::AddCallback(SaveSystemHandler);
	MakeJMP(0x5B4530, &GRGX_Parse);

	// Fix some impound lot stuff
	MakeJMP(0x44A3C0, &HOOK_StoreCarAtImpoundLot);
	WriteMemoryRaw(0x44896C + (33-16), &zero, 3, true);

	// ...
	MakeJMP(0x44895A, &HOOK_IsPointWithinHideOutGarage);
	MakeJMP(HOODLUM? 0x0156EFD7 : 0x4489D7, &HOOK_IsPointWithinAnyGarage);
	MakeJMP(HOODLUM? 0x01563A3A : 0x448B5A, &HOOK_OpenSprayGarages);
	MakeJMP(0x448BD8, &HOOK_RestoreGaragesForNewGame);
	MakeJMP(0x449BF3, &HOOK_IsPointInAGarageCameraZone);
	MakeJMP(HOODLUM? 0x0156917B : 0x449E8B, &HOOK_PlayerArrestedOrDied);
	MakeJMP(0x44A20B, &HOOK_CloseGaragesBeforeSave);
	MakeCALL(0x44A4E1, &HOOK_Obj_ProcessGarageDoor);
	MakeJMP(0x44C924, &HOOK_UpdateGarages);
//	MakeJMP(0x44A0D6, &HOOK_CheckForAttachedGarage);
//	MakeNOP(0x44A0DB, 3);
	MakeJMP(0x4482E9, &HOOK_RestoreCarsForParkingGarage);

	// HOOKVIRTUAL
	MakeJMP(0x447600, &HOOKVIRTUAL_RestoreThisGarage);
	MakeJMP(0x447720, &HOOKVIRTUAL_IsVehicleAcceptByThisTunningGarage);
	MakeNOP(0x447725, 1);
	MakeJMP(0x4486C0, &HOOKVIRTUAL_SetDoorsBackToOriginalHeight);
	MakeJMP(0x448880, &HOOKVIRTUAL_MaximumNumberOfCarsOnThisGarage);
	MakeJMP(0x449900, &HOOKVIRTUAL_StoreAndRemoveCarsForThisGarage);
	MakeNOP(0x449905, 2);
	MakeJMP(0x449A50, &HOOKVIRTUAL_StoreAndRemoveCarsForThisImpoundLot);
	MakeNOP(0x449A55, 2);
	MakeJMP(0x44A660, &HOOKVIRTUAL_UpdateOpeningDoor);
	MakeNOP(0x44A665, 1);
	MakeJMP(0x44A750, &HOOKVIRTUAL_UpdateClosingDoor);
	MakeNOP(0x44A755, 1);
	MakeJMP(0x44AA50, &HOOKVIRTUAL_Update);
	MakeNOP(0x44AA55, 1);
	MakeJMP(0x448550, &HOOKVIRTUAL_RestoreCarsForThisGarage);
	MakeJMP(0x449FF0, &HOOKVIRTUAL_FindDoorsEntities);
	MakeNOP(0x449FF5, 2);
}

void GRGX_Patch()
{
	CGarages::Patch();
}

void GRGX_Detach()
{
	for(auto it = CGarages::Garages().begin(); it != CGarages::Garages().end(); ++it)
		delete *it;
	CGarages::Garages().clear();
}


void GRGX_Parse(const char* line)
{
	RwV3D pos1;
	RwV3D pos2;
	RwV3D depth;
	unsigned int flags;
	unsigned int type;
	char name[8];
	int gtype;
	int doorstyle;
	int car_count;

	int c = sscanf(line, "%f %f %f %f %f %f %f %f %d %d %s   %d %d %d",
			&pos1.x, &pos1.y, &pos1.z,
			&depth.x, &depth.y,
			&pos2.x, &pos2.y, &pos2.z,
			&flags,
			&type,
			name,
			&car_count, &gtype, &doorstyle
		);
	
	bool IsGRGX = c == (11+3);

	CDebugLog::Trace("Parsing grge line: %s", line);
	CDebugLog::Trace("\tIs GRGX: %s", IsGRGX? "true" : "false");
		
	if(c == 11)
		CGarages::AddOne(pos1.x, pos1.y, pos1.z, depth.x, depth.y, pos2.x, pos2.y, pos2.z, type, 0, name, flags);
	else if(IsGRGX)
		CGarages::AddOneX(pos1.x, pos1.y, pos1.z, depth.x, depth.y, pos2.x, pos2.y, pos2.z, -1, 0, name, flags,
		car_count, static_cast<eGrgxGarageType>(gtype), doorstyle);
}

namespace GRGX_HOOKS
{

char GRGX_IsPointWithinHideOutGarage(RwV3D* pPoint)
{
	for(auto it = CGarages::Garages().begin(); it != CGarages::Garages().end(); ++it)
	{
		if(((**it).gType == IS_IMPOUND_GARAGE || CCustomGarage::IsThisGarageAHideout((**it).gType))
			&& (**it).IsPointWithinGarage(*pPoint)
			&& (!(**it).IsParkingGarage() || !((**it).Flags & FLAG_GARAGE_INACTIVE)) )
			return 1;
	}
	return 0;
}

char GRGX_IsPointWithinAnyGarage(RwV3D* pPoint)
{
	for(auto it = CGarages::Garages().begin(); it != CGarages::Garages().end(); ++it)
	{
		if((**it).IsPointWithinGarage(*pPoint))
			return 1;
	}
	return 0;
}

char GRGX_IsPointInAGarageCameraZone(RwV3D* pPoint)
{
	for(auto it = CGarages::Garages().begin(); it != CGarages::Garages().end(); ++it)
	{
		if(!(**it).IsParkingGarage() && (**it).IsPointWithinGarageWithDistance(*pPoint, 0.5))
			return 1;
	}
	return 0;
}

void GRGX_CloseGaragesBeforeSave()
{
	for(auto it = CGarages::Garages().begin(); it != CGarages::Garages().end(); ++it)
	{
		if(CCustomGarage::IsThisGarageAHideout((**it).gType))
		{
			if( (**it).DoorState != GARAGE_DOOR_CLOSED )
			{
				if( true )
				{
					(**it).DoorState = GARAGE_DOOR_CLOSED;
					(**it).DoorPosition = 0.0;
					(**it).RemoveCarsBlockingDoorNotInside();
					(**it).StoreAndRemoveCarsForThisGarage(nullptr, 0);
				}
			}
		}
	}
}

void GRGX_UpdateGarages()
{
	if(DebugCheat("GRG"))
	{
		CGarages::Clear();
		CFileMgr::ChangeDir("");
		CGarages::Load("DATA\\TEST.IPL");
		PrintNow("DONE", 1000, false);
	}

	for(size_t i = 0; i < CGarages::Garages().size(); ++i)
	{
		if(static_cast<CCustomGarage*>(CGarages::Garages()[i])->UpdateCustomGarage() == false)
			--i;
	}	
}

CGarage* GRGX_FindGarageForThisDoor(CObject* door)
{
	if(door->GarageThisDoorIsAttachedTo == GARAGE_TYPE_GRGX)
	{
		for(auto it = CGarages::Garages().begin(); it != CGarages::Garages().end(); ++it)
		{
			auto* p = *it;
			if(p->door[0] == door || p->door[1] == door)
				return p;
		}
		door->GarageThisDoorIsAttachedTo = -1;
		// return nullptr;
	}


	float fDist;
	float fNearestGarage = (float)(9999999999.0);
	RwV3D doorPos = GetCoords(&door->__parent.__parent);
	CGarageBase* pNearest = nullptr;

	for(auto _it = CGarages::Garages().begin(); _it != CGarages::Garages().end(); ++_it)
	{
		auto* it = *_it;
		if(!it->IsParkingGarage() && it->IsPointWithinGarageWithDistance(doorPos, 7.0))
		{
			RwV3D garagePos;
			{
				float hw = float(it->Width * 0.5);
				float hd = float(it->Depth * 0.5);
				garagePos.x = (it->DirectionB.x * hd + (it->DirectionA.x * hw + it->Position.x));
				garagePos.y = (it->DirectionB.y * hd + (it->DirectionA.y * hw + it->Position.y));
				garagePos.z = (it->Position.z);
			}

			fDist = GetDistanceBetweenPoints3D(doorPos, garagePos);
			if(fDist < fNearestGarage)
			{
				fNearestGarage = fDist;
				pNearest = &(*it);
			}
		}
	}

	return pNearest;
}

char GRGX_ProcessGarageDoor(CGarage* _garage, CObject* door)
{
	void (__fastcall *CEntity__UpdateRwFrame)(CEntity* self) = memory_pointer_a(0x532B00);
	void (__fastcall *CMatrix__CopyTo)(CMatrix* self, int, RwMatrix* m) = memory_pointer_a(0x59AD70);
	void (__fastcall *Placement__ToRwMatrix)(Placement* self, int, RwMatrix* m) = memory_pointer_a(0x054EF40);

	CGarageBase* garage = static_cast<CGarageBase*>(_garage);

	if(!CPools::ObjectPool->Exists(door))
	{
		if(garage->door[0] == door) garage->door[0] = nullptr;
		if(garage->door[1] == door) garage->door[1] = nullptr;
		return 0;
	}

	if((door->GarageThisDoorIsAttachedTo & 0x80) && door->GarageThisDoorIsAttachedTo != GARAGE_TYPE_GRGX)
	{
		if(garage->door[0] == nullptr) garage->door[0] = door;
		else if(garage->door[1] == nullptr) garage->door[1] = door;
		else if(garage->door[0] != door && garage->door[1] != door)
		{
			garage->door[0] = garage->door[1] = nullptr;
			door->GarageThisDoorIsAttachedTo = -1;
			return 0;
		}
		door->GarageThisDoorIsAttachedTo = GARAGE_TYPE_GRGX;
	}


	float mult = (float)(CGarages::GetGarageDoorMoveMultiplier(door->__parent.__parent.m_wModelIndex));
	CMatrix_Padded* matrix = (CMatrix_Padded*)(door->__parent.__parent.__parent.m_pCoords);
	RwV3D* f170Pos = GetCoords((CEntity*)(door->field_170), 0);

	if(garage->gStyle & DOOR_STYLE_ROTATES)
	{
		float door_pos = garage->DoorPosition;
		if(garage->gStyle & DOOR_STYLE_OPENS_UP)
			matrix->vPos.z = f170Pos->z + mult * garage->DoorPosition * (float)(0.4799999892711639);
		else
			matrix->vPos.z = f170Pos->z - mult * garage->DoorPosition * (float)(0.4799999892711639);
		if(garage->Flags & FLAG_GARAGE_DOOR_GOES_IN)
			door_pos = -door_pos;
		CGarages::BuildRotatedDoorMatrix(door, door_pos);
	}
	else
	{
		if(garage->gStyle & DOOR_STYLE_OPENS_TOSIDE)	// Vendant Meadows hangar
		{
			matrix->vPos.x = float(f170Pos->x - garage->DoorPosition * matrix->vRight.x * 14.0);
		}
		else if(garage->gStyle & DOOR_STYLE_OPENS_UP)	// Created by me
		{
			matrix->vPos.z = float(f170Pos->z + mult * garage->DoorPosition * 0.909090);
		}
		else	// AT-400 Hangar
		{
			matrix->vPos.z = float(f170Pos->z - mult * garage->DoorPosition * 0.909090);
		}
	}

	door->__parent.__parent.field_1C ^= ((uint8_t)door->__parent.__parent.field_1C ^ ((uint32_t)garage->Flags >> 6)) & 1;
	if(DWORD pClump = door->__parent.__parent.m_pRWObject)	// RpClump*
	{
		RwMatrix* m = (RwMatrix*)(*(DWORD*)(pClump+4)+16);
		if(door->__parent.__parent.__parent.m_pCoords)
			CMatrix__CopyTo((CMatrix*)(door->__parent.__parent.__parent.m_pCoords), 0, m);
		else
			Placement__ToRwMatrix(&door->__parent.__parent.__parent.placement, 0, m);
	}

	CEntity__UpdateRwFrame(&door->__parent.__parent);
	return 1;
}

void __declspec(naked) HOOK_RestoreCarsForParkingGarage()
{
	_asm
	{
		push edi
		call CGarages::IsParkingGarage
		add esp, 4
		test eax, eax
		jz _NormalStuff

		//_LoadAllModelsNow:
			push 0
			call CTimer::StartUserPause
			call CStreaming::LoadAllRequestedModels
			call CTimer::EndUserPause
			add esp, 4

			push 0x447E91
			retn

		_NormalStuff:
			mov ecx, [esp+0x20+0xC]
			pop edi
			push 0x4482EE
			retn

	}
}

void __declspec(naked) HOOK_IsPointWithinHideOutGarage()
{
	_asm
	{
		push esi
		call GRGX_IsPointWithinHideOutGarage
		add esp, 4
		pop edi
		pop esi
		retn
	}
}

void __declspec(naked) HOOK_IsPointWithinAnyGarage()
{
	_asm
	{
		push esi
		call GRGX_IsPointWithinAnyGarage
		add esp, 4
		pop edi
		pop esi
		retn
	}
}

// no need for naked here because the hook hooks the retn
void __cdecl HOOK_OpenSprayGarages(char bOpen)	
{
	for(auto it = CGarages::Garages().begin(); it != CGarages::Garages().end(); ++it)
	{
		if((**it).gType == IS_RESPRAY_GARAGE)
			(**it).DoorState = bOpen;
	}
}

void __cdecl HOOK_RestoreGaragesForNewGame()
{
	void (__fastcall *sub_4DC6B0)(void* self) = (decltype(sub_4DC6B0))(0x4DC6B0);

	for(size_t idx = 0; idx < CGarages::Garages().size(); ++idx)
	{
		auto* it = CGarages::Garages()[idx];
		if(it->CreatedByScript == true)
		{
			CGarages::DestroyGarage(it);
			--idx;
			continue;
		}

		it->Type = GARAGE_TYPE_GRGX;
		it->gType = static_cast<eGrgxGarageType>(it->OriginalType);
		it->gStyle = it->OriginalStyle;
		it->Flags = it->OriginalFlags;
		it->RestoreThisGarage();
		it->door[0] = it->door[1] = nullptr;
		sub_4DC6B0(&it->_unknown_audio_object[0]);

		if(it->size == -1 && it->cars.size() != DEFAULT_GRGX_SIZE)
			it->cars.resize(DEFAULT_GRGX_SIZE);
		if(it->size == 0 && it->cars.size() != 0)
			it->cars.clear();

		for(size_t x = 0; x < it->cars.size(); ++x)
			it->cars[x].model = 0;
	}
}

void __declspec(naked) HOOK_IsPointInAGarageCameraZone()
{
	_asm
	{
		lea eax, [esp+12]
		push eax
		call GRGX_IsPointInAGarageCameraZone
		add esp, 4
		pop edi
		pop esi
		retn
	}
}

void __cdecl HOOK_PlayerArrestedOrDied()
{
	for(auto it = CGarages::Garages().begin(); it != CGarages::Garages().end(); ++it)
		(**it).SetDoorsBackToOriginalHeight();
}

void __declspec(naked) HOOK_CloseGaragesBeforeSave()
{
	_asm
	{
		call GRGX_CloseGaragesBeforeSave
		pop edi
		pop esi
		pop ebx
		retn
	}
}

void __cdecl HOOK_StoreCarAtImpoundLot(CVehicle *veh)
{
	bool IsGRGX = false;
	float fDist;
	float fNearestGarage = (float)(9999999999.0);
	CGarage* garage = nullptr;
	RwV3D VehCoords = GetCoords(&veh->__parent.__parent);

	// Try to find nearest impound on GRGX garages
	for(auto it = CGarages::Garages().begin(); it != CGarages::Garages().end(); ++it)
	{
		if((**it).gType == IS_IMPOUND_GARAGE)
		{
			fDist = GetDistanceBetweenPoints2D(VehCoords, (**it).Position);
			if(fDist < fNearestGarage)
			{
				fNearestGarage = fDist;
				garage = &(**it);
				IsGRGX = true;
			}
		}
	}

	// Try to find nearest impound on original garages
	if(CGarages__NumGarages)
	{
		for(int i = 0, max = CGarages__NumGarages; i < max; ++i)
		{
			auto* p = &CGarages__Garages[i];
			if(p->Type >= 33 && p->Type <= 35)
			{
				fDist = GetDistanceBetweenPoints2D(VehCoords, p->Position);
				if(fDist < fNearestGarage)
				{
					fNearestGarage = fDist;
					garage = p;
					IsGRGX = false;
				}
			}
		}
	}

	// Put car on nearest impound
	if(garage)
	{
		CStoredCar* cars;
		size_t max, idx;

		if(IsGRGX)
		{
			CGarageBase* x = static_cast<CGarageBase*>(garage);
			max = x->cars.size();
			cars = &x->cars[0];
		}
		else
		{
			max = 3;
			cars = &CGarages__aStoredCars[CGarages::GetStoreIndex(garage->Type) * 4];
		}

		if(max == 0) return;
		for(size_t i = 0; i <= max; ++i)
		{
			if(i == max)
			{
				if(max != 1)
				{
					memcpy( cars, cars+1, sizeof(CStoredCar) * (max-1) );
					idx = max - 1;
				}
				else
				{
					idx = 0;
				}
				break;
			}
			else if(cars[i].model == 0)
			{
				idx = i;
				break;
			}
		}

		DebugMessage("Storing car on impound lot");
		CStoredCar__Store(&cars[idx], 0, veh);
	}

}

void __declspec(naked) HOOK_Obj_ProcessGarageDoor()
{
	_asm
	{
		//_DoTest:
			push [esp+4]
			call GRGX_FindGarageForThisDoor
			add esp, 4
			test eax, eax
			jz _TryOriginalGarages
		
			push [esp+4]	// CObject*
			push eax		// CGarage*
			call GRGX_ProcessGarageDoor
			add esp, 8

			test al, al
			jz _MakeGrgNothingAndRet
			mov eax, GARAGE_TYPE_GRGX
			retn
		_MakeGrgNothingAndRet:
			mov eax, -1
			retn

		_TryOriginalGarages:
			jmp CGarages__FindGarageForThisDoor
	}
}

void __declspec(naked) HOOK_UpdateGarages()
{
	_asm
	{
		call GRGX_UpdateGarages
		mov ecx, dword ptr [0xB7CB4C]
		push 0x44C92A	// return to
		retn
	}
}

void __declspec(naked) HOOKVIRTUAL_RestoreThisGarage()
{
	_asm
	{
		push ecx
		call CGarages::IsGRGX
		pop ecx
		test al, al
		jnz _virtual

		// original stuff
		mov al, [ecx+0x4E]
		and al, 0x39
		push 0x447605
		retn

		_virtual:
	};

	CGarage* self;
	ASM_PROLOG()
	_asm mov self, ecx
	static_cast<CGarageBase*>(self)->RestoreThisGarage();
	ASM_EPILOG();
	_asm retn
}

void __declspec(naked) HOOKVIRTUAL_IsVehicleAcceptByThisTunningGarage()
{
	_asm
	{
		push ecx
		call CGarages::IsGRGX
		pop ecx
		test al, al
		jnz _virtual

		// check if is hoodlum version
		mov al, HOODLUM
		test al, al
		jnz _hoodlum

		// original stuff
		mov eax, [esp+4]
		test eax, eax
		push 0x447726
		retn

		_hoodlum:
			push 0x1565260
			retn

		_virtual:
	};

	CGarage* self;
	CVehicle* veh;
	_asm mov eax, [esp+4]
	ASM_PROLOG()
	_asm mov self, ecx
	_asm mov veh, eax
	static_cast<CGarageBase*>(self)->IsVehicleAcceptByThisTunningGarage(veh);
	ASM_EPILOG();
	_asm retn 4
}

void __declspec(naked) HOOKVIRTUAL_SetDoorsBackToOriginalHeight()
{
	_asm
	{
		push ecx
		call CGarages::IsGRGX
		pop ecx
		test al, al
		jnz _virtual

		// original stuff
		movzx eax, byte ptr [ecx+0x4c]
		dec eax
		push 0x4486C5
		retn

		_virtual:
	};

	CGarage* self;
	ASM_PROLOG()
	_asm mov self, ecx
	static_cast<CGarageBase*>(self)->SetDoorsBackToOriginalHeight();
	ASM_EPILOG();
	_asm retn
}

int __fastcall HOOKVIRTUAL_MaximumNumberOfCarsOnThisGarage(CGarage* self)
{
	return CGarages::IsGRGX(self)? static_cast<CGarageBase*>(self)->MaximumNumberOfCarsOnThisGarage()
		: (2 * (self->Type != 16)) + 2;
}

void __declspec(naked) HOOKVIRTUAL_StoreAndRemoveCarsForThisGarage()
{
	_asm
	{
		push ecx
		call CGarages::IsGRGX
		pop ecx
		test al, al
		jnz _virtual

		// original stuff
		sub esp, 0xC
		mov edx, [esp+0x10]
		push 0x449907
		retn

		_virtual:
	};

	CGarage* self;
	ASM_PROLOG()
	_asm mov self, ecx
	static_cast<CGarageBase*>(self)->StoreAndRemoveCarsForThisGarage(nullptr, -1);
	ASM_EPILOG();
	_asm retn 8
}

void __declspec(naked) HOOKVIRTUAL_StoreAndRemoveCarsForThisImpoundLot()
{
	_asm
	{
		push ecx
		call CGarages::IsGRGX
		pop ecx
		test al, al
		jnz _virtual

		// original stuff
		sub esp, 0xC
		mov edx, [esp+0x10]
		push 0x449A57
		retn

		_virtual:
	};

	CGarage* self;
	ASM_PROLOG()
	_asm mov self, ecx
	static_cast<CGarageBase*>(self)->StoreAndRemoveCarsForThisImpoundLot(nullptr, -1);
	ASM_EPILOG();
	_asm retn 8
}

void __declspec(naked) HOOKVIRTUAL_UpdateOpeningDoor()
{
	_asm
	{
		push ecx
		call CGarages::IsGRGX
		pop ecx
		test al, al
		jnz _virtual

		// original stuff
		fld dword ptr ds:[0xB7CB5C]
		push 0x44A666
		retn

		_virtual:
	};

	CGarage* self;
	ASM_PROLOG()
	_asm mov self, ecx
	static_cast<CGarageBase*>(self)->UpdateOpeningDoor();
	ASM_EPILOG();
	_asm retn
}

void __declspec(naked) HOOKVIRTUAL_UpdateClosingDoor()
{
	_asm
	{
		push ecx
		call CGarages::IsGRGX
		pop ecx
		test al, al
		jnz _virtual

		// original stuff
		fld dword ptr ds:[0xB7CB5C]
		push 0x44A756
		retn

		_virtual:
	};

	CGarage* self;
	ASM_PROLOG()
	_asm mov self, ecx
	static_cast<CGarageBase*>(self)->UpdateClosingDoor();
	ASM_EPILOG();
	_asm retn
}

void __declspec(naked) HOOKVIRTUAL_Update()
{
	_asm
	{
		push ecx
		call CGarages::IsGRGX
		pop ecx
		test al, al
		jnz _virtual

		// original stuff
		sub esp, 0x1E8
		push 0x44AA56
		retn

		_virtual:
	};

	CGarage* self;
	ASM_PROLOG()
	_asm mov self, ecx
	static_cast<CGarageBase*>(self)->Update(-1);
	ASM_EPILOG();
	_asm retn 4
}

void __declspec(naked) HOOKVIRTUAL_RestoreCarsForThisGarage()
{
	_asm
	{
		push ecx
		call CGarages::IsGRGX
		pop ecx
		test al, al
		jnz _virtual

		// check if is hoodlum version
		mov al, HOODLUM
		test al, al
		jnz _hoodlum

		// original stuff
		push ebp
		mov ebp, [esp+8]
		push 0x448555
		retn

		_hoodlum:
			push 0x156D8D0
			retn

		_virtual:
	};

	CGarage* self;
	CStoredCar* s;
	_asm mov eax, [esp+4]
	ASM_PROLOG()
	_asm mov self, ecx
	_asm mov s, eax
	static_cast<CGarageBase*>(self)->RestoreCarsForThisGarage(s);
	ASM_EPILOG();
	_asm retn 4
}

void __declspec(naked) HOOKVIRTUAL_FindDoorsEntities()
{
	_asm
	{
		push ecx
		call CGarages::IsGRGX
		pop ecx
		test al, al
		jnz _virtual

		// original stuff
		sub esp, 0x20
		mov eax, [esp+0x28]
		push 0x449FF7
		retn

		_virtual:
	};

	CGarage* self;
	CObject** p1, **p2;
	_asm mov eax, [esp+4]
	_asm mov edx, [esp+8]
	ASM_PROLOG()
	_asm mov self, ecx
	_asm mov p1, eax
	_asm mov p2, edx
	static_cast<CGarageBase*>(self)->FindDoorsEntities(p1, p2);
	ASM_EPILOG();
	_asm retn 8
}


};
