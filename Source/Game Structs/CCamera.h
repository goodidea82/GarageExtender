// Generated by IDAStruct_To_CHeader.py
#pragma once
#include <windows.h>
#include "CPlaceable.h"
#include "CCamera_obj2.h"
#include "CCam.h"
#include "RwV3D.h"
#include "CMatrix.h"


#pragma pack(push, 1)
struct CCamera	// sizeof = 0xD78
{
	CPlaceable __parent;
	BYTE bAboveGroundTrainNodesLoaded;
	BYTE bBelowGroundTrainNodesLoaded;
	BYTE bCamDirectlyBehind;
	BYTE bCamDirectlyInFront;
	BYTE field_1C;
	BYTE field_1D;
	BYTE field_1E;
	BYTE field_1F;
	BYTE field_20;
	BYTE field_21;
	BYTE field_22;
	BYTE field_23;
	BYTE field_24;
	BYTE field_25;
	BYTE field_26;
	BYTE field_27;
	BYTE field_28;
	BYTE field_29;
	BYTE field_2A;
	BYTE field_2B;
	BYTE field_2C;
	BYTE field_2D;
	BYTE field_2E;
	BYTE field_2F;
	BYTE field_30;
	BYTE field_31;
	BYTE field_32;
	BYTE field_33;
	BYTE field_34;
	BYTE field_35;
	BYTE field_36;
	BYTE field_37;
	BYTE field_38;
	BYTE field_39;
	BYTE field_3A;
	BYTE field_3B;
	BYTE field_3C;
	BYTE field_3D;
	BYTE field_3E;
	BYTE field_3F;
	BYTE field_40;
	BYTE field_41;
	BYTE field_42;
	BYTE twoPlayerMode;
	BYTE field_44;
	BYTE field_45;
	WORD field_46;
	WORD field_48;
	WORD field_4A;
	WORD field_4C;
	BYTE field_4E;
	BYTE field_4F;
	BYTE field_50;
	BYTE isFading;
	BYTE field_52;
	BYTE field_53;
	BYTE field_54;
	BYTE field_55;
	BYTE field_56;
	BYTE field_57;
	BYTE field_58;
	BYTE activeCam;
	BYTE field_5A;
	BYTE field_5B;
	DWORD field_5C;
	DWORD field_60;
	DWORD field_64;
	DWORD field_68;
	DWORD field_6C;
	DWORD field_70;
	DWORD field_74;
	DWORD field_78;
	DWORD field_7C;
	DWORD field_80;
	DWORD field_84;
	DWORD field_88;
	DWORD field_8C;
	DWORD field_90;
	DWORD field_94;
	DWORD field_98;
	DWORD field_9C;
	DWORD field_A0;
	DWORD field_A4;
	DWORD field_A8;
	DWORD field_AC;
	DWORD field_B0;
	DWORD field_B4;
	DWORD field_B8;
	DWORD field_BC;
	DWORD field_C0;
	DWORD field_C4;
	DWORD field_C8;
	DWORD field_CC;
	DWORD field_D0;
	DWORD field_D4;
	DWORD field_D8;
	DWORD field_DC;
	DWORD field_E0;
	DWORD DistanceToWater;
	DWORD HeightOfNearestWater;
	DWORD field_EC;
	DWORD field_F0;
	DWORD field_F4;
	DWORD field_F8;
	DWORD field_FC;
	DWORD field_100;
	DWORD field_104;
	DWORD field_108;
	DWORD field_10C;
	DWORD field_110;
	DWORD field_114;
	DWORD field_118;
	DWORD field_11C;
	DWORD field_120;
	DWORD field_124;
	DWORD field_128;
	DWORD field_12C;
	DWORD field_130;
	DWORD field_134;
	DWORD nearClipPlane;
	DWORD field_13C;
	DWORD field_140;
	DWORD field_144;
	DWORD field_148;
	DWORD field_14C;
	DWORD field_150;
	DWORD field_154;
	DWORD field_158;
	DWORD field_15C;
	DWORD field_160;
	DWORD field_164;
	WORD field_168;
	WORD field_16A;
	DWORD field_16C;
	DWORD field_170;
	CCam cams[3];
	DWORD pInteractingGarage;
	DWORD field_820;
	DWORD field_824;
	DWORD field_828;
	DWORD field_82C;
	WORD field_830;
	WORD field_832;
	DWORD field_834;
	WORD field_838;
	WORD field_83A;
	DWORD field_83C;
	DWORD field_840;
	DWORD field_844;
	RwV3D field_848;
	RwV3D field_854;
	RwV3D field_860;
	RwV3D cameraPosition;
	RwV3D cameraRotation;
	DWORD field_884;
	DWORD field_888;
	DWORD field_88C;
	DWORD field_890;
	DWORD field_894;
	DWORD field_898;
	DWORD field_89C;
	DWORD field_8A0;
	DWORD field_8A4;
	DWORD field_8A8;
	DWORD field_8AC;
	DWORD field_8B0;
	DWORD field_8B4;
	DWORD field_8B8;
	DWORD field_8BC;
	DWORD field_8C0;
	DWORD field_8C4;
	DWORD field_8C8;
	DWORD field_8CC;
	DWORD field_8D0;
	DWORD field_8D4;
	DWORD field_8D8;
	DWORD field_8DC;
	DWORD field_8E0;
	DWORD field_8E4;
	DWORD field_8E8;
	DWORD field_8EC;
	DWORD field_8F0;
	DWORD field_8F4;
	DWORD field_8F8;
	DWORD field_8FC;
	DWORD field_900;
	DWORD field_904;
	RwV3D field_908;
	DWORD field_914;
	DWORD field_918;
	DWORD field_91C;
	RwV3D field_920;
	RwV3D field_92C;
	RwV3D field_938;
	RwV3D field_944;
	DWORD field_950;
	DWORD rwCamera;
	DWORD targetEntity;
	DWORD field_95C;
	DWORD field_960[4];
	BYTE field_970;
	BYTE field_971;
	BYTE field_972;
	BYTE field_973;
	CMatrix field_974;
	CMatrix field_9BC;
	CMatrix cameraViewMatrix;
	CMatrix field_A4C;
	CMatrix field_A94;
	CMatrix TvCamMatrix;
	RwV3D field_B24;
	RwV3D field_B30;
	RwV3D field_B3C;
	RwV3D field_B48;
	DWORD field_B54;
	DWORD field_B58;
	DWORD field_B5C;
	DWORD field_B60;
	DWORD field_B64;
	DWORD field_B68;
	DWORD field_B6C;
	DWORD field_B70;
	DWORD field_B74;
	DWORD field_B78;
	DWORD field_B7C;
	DWORD field_B80;
	DWORD field_B84;
	DWORD field_B88;
	DWORD field_B8C;
	DWORD field_B90;
	DWORD field_B94;
	DWORD field_B98;
	DWORD field_B9C;
	DWORD field_BA0;
	DWORD field_BA4;
	DWORD field_BA8;
	DWORD field_BAC;
	DWORD field_BB0;
	DWORD field_BB4;
	DWORD field_BB8;
	DWORD field_BBC;
	DWORD field_BC0;
	DWORD field_BC4;
	DWORD field_BC8;
	DWORD field_BCC;
	DWORD field_BD0;
	DWORD field_BD4;
	DWORD field_BD8;
	DWORD field_BDC;
	DWORD field_BE0;
	DWORD field_BE4;
	DWORD field_BE8;
	DWORD field_BEC;
	DWORD field_BF0;
	DWORD field_BF4;
	DWORD field_BF8;
	DWORD _fadeAlpha;
	DWORD field_C00;
	DWORD _fFadeDuration;
	DWORD field_C08;
	DWORD field_C0C;
	DWORD field_C10;
	DWORD field_C14;
	DWORD field_C18;
	DWORD field_C1C;
	DWORD field_C20;
	DWORD field_C24;
	DWORD field_C28;
	DWORD field_C2C;
	WORD _fadeInOutFlag;
	WORD field_C32;
	DWORD field_C34;
	WORD field_C38;
	WORD field_C3A;
	WORD field_C3C;
	WORD field_C3E;
	DWORD _fadeStartTime;
	DWORD field_C44;
	DWORD field_C48;
	DWORD field_C4C;
	DWORD field_C50;
	DWORD field_C54;
	DWORD field_C58;
	DWORD transverseStartTime;
	DWORD transverseEndTime;
	RwV3D transverseEndPoint;
	RwV3D transverseStartPoint;
	BYTE transverseMode;
	BYTE field_C7D;
	BYTE field_C7E;
	BYTE field_C7F;
	RwV3D field_C80;
	BYTE field_C8C;
	BYTE field_C8D;
	BYTE field_C8E;
	BYTE field_C8F;
	DWORD field_C90;
	DWORD _startJiggleTime;
	DWORD _endJiggleTime;
	DWORD field_C9C;
	DWORD field_CA0;
	DWORD _startZoomTime;
	DWORD _endZoomTime;
	DWORD _zoomInFactor;
	DWORD _zoomOutFactor;
	BYTE _zoomMode;
	BYTE field_CB5;
	BYTE field_CB6;
	BYTE field_CB7;
	DWORD field_CB8;
	DWORD field_CBC;
	DWORD field_CC0;
	RwV3D field_CC4;
	RwV3D field_CD0;
	BYTE field_CDC;
	BYTE field_CDD;
	BYTE field_CDE;
	BYTE field_CDF;
	RwV3D field_CE0;
	BYTE field_CEC;
	BYTE field_CED;
	BYTE field_CEE;
	BYTE field_CEF;
	BYTE field_CF0;
	BYTE field_CF1;
	BYTE field_CF2;
	BYTE field_CF3;
	CCamera_obj2 field_CF4[5];
	DWORD field_D44;
	DWORD field_D48;
	DWORD field_D4C;
	DWORD field_D50;
	DWORD field_D54;
	DWORD field_D58;
	DWORD field_D5C;
	DWORD field_D60;
	DWORD field_D64;
	DWORD field_D68;
	DWORD field_D6C;
	DWORD field_D70;
	DWORD field_D74;
};
#pragma pack(pop)

static_assert(sizeof(CCamera) == 0xD78, "Incorrect struct size: CCamera");