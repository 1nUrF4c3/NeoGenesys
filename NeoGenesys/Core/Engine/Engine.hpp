//=====================================================================================

#pragma once

#include <vector>
#include "Helpers.hpp"
#include "Thunk.hpp"

//=====================================================================================

#define MAX_CLIENTS 18
#define MAX_ENTITIES 2048
#define MASK_CONTENTS 0x803003
#define MASK_KILLSTREAK 0x2803001
#define MASK_NONKILLSTREAK 0x280E831
#define EF_CROUCH 0x4
#define EF_PRONE 0x8
#define EF_MANTLE 0x20000
#define EF_DEAD 0x40000
#define EF_ZOOM 0x80000
#define EF_FIRE 0x800000
#define EF_STREAK 0x10000000
#define TF_NONE 0x0
#define TF_SHADOW 0x3
#define WF_AKIMBO 0x20000
#define BUTTON_FIRELEFT 0x1
#define BUTTON_FIRERIGHT 0x80000
#define BUTTON_PRONE 0x100
#define BUTTON_CROUCH 0x200
#define BUTTON_JUMP 0x400
#define BUTTON_ZOOM 0x800
#define BIND_ATTACK1 0x1
#define BIND_ATTACK0 0x2
#define BIND_ATTACK_AKIMBO_ACCESSIBLE1 0x4B
#define BIND_ATTACK_AKIMBO_ACCESSIBLE0 0x4C
#define M_METERS 55.0f
#define M_FEET (M_METERS*3.28084f)
#define M_PI_DOUBLE ((float)M_PI*2.0f)

#define ByteToFloat(a) ((float)(a)/255.0f)
#define DwordFromBytes(a) (((BYTE)(a)[0])|((BYTE)(a)[1]<<8)|((BYTE)(a)[2]<<16)|((BYTE)(a)[3]<<24))
#define GetSign(a) ((a)?((*(int*)&(a)>>31)|1):0)
#define DegreesToRadians(a) ((a)*((float)M_PI/180.0f))
#define RadiansToDegrees(a) ((a)*(180.0f/(float)M_PI))
#define AngleToShort(a) ((int)((a)*(65536/360.0f))&65535)
#define ShortToAngle(a) ((float)((a)*(360.0f/65536)))
#define AngleNormalize(a) (ShortToAngle(AngleToShort((a))))
#define AngleCompare180(a) (((a)<90.0f&&(a)>-90.0f)||((a)>270.0f||(a)<-270.0f))
#define Dereference(a) (*(decltype(a)*)(a))

#define OFF_CHATHEIGHT_DVAR 0x1419A9788
#define OFF_CHATHEIGHT_EXCEPTION 0x14025C709
#define OFF_SMOOTHINGTIME_DVAR 0x141762418
#define OFF_SMOOTHINGTIME_EXCEPTION 0x140283257
#define OFF_HUDSAYPOSITION_DVAR 0x1419A94C8
#define OFF_HUDSAYPOSITION_EXCEPTION 0x14025C40B
#define OFF_PACKETDUPLICATION_DVAR 0x141E04AA8
#define OFF_PACKETDUPLICATION_EXCEPTION 0x1402C1FB2
#define OFF_SYSGETVALUE 0x1404237D0
#define OFF_SYSGETVALUEEXCEPTION 0x1404237F0
#define OFF_SYSMILLISECONDS 0x140501CA0
#define OFF_WINDOWHANDLE 0x147AD2640
#define OFF_SWAPCHAIN 0x1480B1D70
#define OFF_DEVICE 0x1480AA098
#define OFF_DEVICECONTEXT 0x1480AA0A0
#define OFF_REFRESH 0x14025CC50
#define OFF_WRITEPACKET 0x1402C1E70
#define OFF_PREDICTPLAYERSTATE 0x1402830B0
#define OFF_CREATENEWCOMMANDS 0x1402BEE80
#define OFF_BULLETFIREPENETRATE 0x1402ACBF0
#define OFF_BULLETHITEVENT 0x1402A68F0
#define OFF_CALCENTITYLERPPOSITIONS 0x140262F60
#define OFF_OBITUARY 0x14026A010
#define OFF_ADDCMDDRAWTEXT 0x140601070
#define OFF_CLIENTFRAME 0x140382890
#define OFF_ATOLCLHANDLERELAYPACKET 0x1407316B4
#define OFF_ATOLCLHANDLERELAYPACKETRET 0x1402C7BD4
#define OFF_ATOLCLHANDLERELAYPACKETCALL 0x1402C7BCF
#define OFF_LOCALCLIENTISINGAME 0x1402CFE50
#define OFF_ISMAINTHREAD 0x140423950
#define OFF_ISRENDERTHREAD 0x1404239A0
#define OFF_ISSERVERTHREAD 0x1404239E0
#define OFF_ISDATABASETHREAD 0x140424990
#define OFF_ENABLECONSOLE 0x140503130
#define OFF_PRINTTOCONSOLE 0x140502A80
#define OFF_COMERROR 0x140412740
#define OFF_CBUFADDCALL 0x1403F6AD0
#define OFF_CBUFADDTEXT 0x1403F6B50
#define OFF_GAMESENDSERVERCOMMAND 0x1404758C0
#define OFF_FINDVARIABLE 0x1404ECB60
#define OFF_GETVARIABLEINDEX 0x1403E9BC0
#define OFF_SETDVARFROMSTRINGBYNAMEFROMSOURCE 0x1404F00B0
#define OFF_UPDATEDVAR 0x1401D0800
#define OFF_SAY 0x140393010
#define OFF_SAYTO 0x1403931A0
#define OFF_GETCURRENTSESSION 0x1404FDA70
#define OFF_GETSESSIONHOST 0x140580260
#define OFF_ISSESSIONHOST 0x140580290
#define OFF_ISUSERREGISTERED 0x14048D320
#define OFF_GETPLAYERADDR 0x14048D040
#define OFF_PARTYDATA 0x141E16930
#define OFF_LOBBYDATA 0x141E0EEA0
#define OFF_MSGINIT 0x140417BE0
#define OFF_MSGWRITESTRING 0x140418A70
#define OFF_NETOUTOFBANDDATA 0x14041D4F0
#define OFF_CLSENDPEERDATA 0x1402D1050
#define OFF_CLSSTATE 0x1419E1AE0
#define OFF_MSGWRITEBYTE 0x140418950
#define OFF_CLCANWECONNECTTOCLIENT 0x1402D0440
#define OFF_PARTYFINDMEMBERBYXUID 0x14048CEA0
#define OFF_LIVEGETXUID 0x1404FDD00
#define OFF_GETCURRENTNAME 0x1404FDAA0
#define OFF_GETCURRENTXUID 0x1404FDAB0
#define OFF_GETSCREENMATRIX 0x1402F6D00
#define OFF_GETVIEWMODELWEAPON 0x140239200
#define OFF_GETWEAPONNAMECOMPLETE 0x140239370
#define OFF_GETWEAPONDISPLAYNAME 0x1402A9B80
#define OFF_GETWEAPONFORNAME 0x1403DA060
#define OFF_GETPERKNAME 0x14021BB00
#define OFF_GETPERKINDEX 0x14021BA90
#define OFF_ISPLAYERRELOADING 0x1402AA1A0
#define OFF_WEAPONBOTHCLIPEMPTY 0x1402386A0
#define OFF_WEAPONISDUALWIELD 0x140247B70
#define OFF_ISRIFLEBULLET 0x1402423F0
#define OFF_WORLDTOSCREEN 0x140262190
#define OFF_ADDMESSAGEICON 0x1402BAAE0
#define OFF_REGISTERFONT 0x1402C9AA0
#define OFF_REGISTERSHADER 0x1405F0E20
#define OFF_REGISTERTAG 0x1403C38F0
#define OFF_DRAWSTRETCHPIC 0x140600BE0
#define OFF_DRAWROTATEDPIC 0x140261270
#define OFF_DRAWENGINETEXT 0x140601070
#define OFF_STRINGHEIGHT 0x1405DFAE0
#define OFF_STRINGWIDTH 0x1405DFDB0
#define OFF_GETPLAYERVIEWORIGIN 0x1402A9830
#define OFF_ISTHIRDPERSONMODE 0x140219270
#define OFF_GETTHIRDPERSONCROSSHAIROFFSET 0x140219090
#define OFF_GETENTITYDOBJ 0x140416490
#define OFF_GETTAGPOSITION 0x140263F50
#define OFF_GETTAGORIENTATION 0x140269980
#define OFF_GETHITLOCATIONSTRING 0x140395A10
#define OFF_LOCATIONALTRACE 0x1402B6020
#define OFF_VECTORANGLES 0x1404E36A0
#define OFF_ANGLEVECTORS 0x1404E39E0
#define OFF_VECTORNORMALIZE 0x140147FE0
#define OFF_ANGLENORMALIZE180 0x1404DEA70
#define OFF_EXECUTEKEY 0x1402BF0E0
#define OFF_ISGAMEPADENABLED 0x1402C0DB0
#define OFF_SETZOOMSTATE 0x1402B9D70
#define OFF_CBULLETTRACE 0x1402A6EB0
#define OFF_GBULLETTRACE 0x140378370
#define OFF_GETTRACEHITTYPE 0x1403F3300
#define OFF_ADVANCETRACE 0x14023B010
#define OFF_GETSURFACEPENETRATIONDEPTH 0x140238FD0
#define OFF_GETWEAPONHITLOCATIONMULTIPLIER 0x140395AE0
#define OFF_GETWEAPONDAMAGE 0x140377420
#define OFF_GETBULLETDAMAGEFORPROJECTILE 0x14023DE60
#define OFF_PENETRATIONCHECK 0x1402AB6C0
#define OFF_GETHIPFIRESPREADFORWEAPON 0x1402409B0
#define OFF_GETZOOMSPREADFORWEAPON 0x14023A5A0
#define OFF_BULLETENDPOSITION 0x1402AC8B0
#define OFF_SEEDRANDOM 0x14021E420
#define OFF_RANDOM 0x14021E3F0
#define OFF_EVALUATETRAJECTORY 0x140218240
#define OFF_TEAMCHANGED 0x1403880D0
#define OFF_ADDSTRING 0x1404381D0
#define OFF_NOTIFY 0x1404397E0
#define OFF_ADDBOT 0x140470920
#define OFF_ADDTESTCLIENT 0x140470B70
#define OFF_SPAWNTESTCLIENT 0x1404740A0
#define OFF_ADDENTITY 0x1403D3480
#define OFF_TAKEPLAYERWEAPON 0x1402463F0
#define OFF_GIVEPLAYERWEAPON 0x1403DA5E0
#define OFF_ADDAMMO 0x140399E50
#define OFF_ISMIGRATING 0x14047BAB0
#define OFF_GAMEDROPCLIENT 0x140475890
#define OFF_KICKCLIENTNUM 0x14046F730
#define OFF_PLAYERDIE 0x140396920
#define OFF_PLAYERKILL 0x1403CE260
#define OFF_DAMAGE 0x140394DF0
#define OFF_SPREADMULTIPLIER 0x14187D43C
#define OFF_ZOOMMULTIPLIER 0x140B31D4C
#define OFF_FIREWEAPON 0x1402A8B00
#define OFF_WEAPONRECOIL 0x1402472A0
#define OFF_WEAPONSOUNDS 0x1402A8FA0
#define OFF_WEAPONRUMBLE 0x1402A90A0
#define OFF_FRAMEINTERPOLATION 0x1417E0488
#define OFF_FRAMETIME 0x1417E048C
#define OFF_NEWSERVERTIME 0x1417E0490
#define OFF_OLDSERVERTIME 0x1417E0494
#define OFF_PING 0x1419E5100
#define OFF_CG 0x14176EC00
#define OFF_REFDEF 0x1417E04D0
#define OFF_PLAYERNAME 0x141E0F0C4
#define OFF_CHARACTERINFORMATION 0x141869AB8
#define OFF_CENTITY 0x141887350
#define OFF_GENTITY 0x14427A0E0
#define OFF_PLAYERSTATE 0x14444CF10
#define OFF_CLIENTACTIVE 0x1419E5298
#define OFF_CLIENTINFORMATION 0x141879028
#define OFF_WEAPONCOMPLETEDEF 0x141734DE0
#define OFF_WEAPONDEF 0x1417352E0
#define OFF_WINDOW 0x14780C394
#define OFF_VIEWMATRIX 0x1419E51E4
#define OFF_PUNCH 0x141823E5C
#define OFF_CHALLENGES 0x1445A2B40
#define OFF_CHALLENGESIZE 0x2D0
#define OFF_SQUADPOINTS 0x1445A34A0
#define OFF_PRESTIGE 0x1445A3798
#define OFF_CURRENTNAME 0x147AC9DD4
#define OFF_SQUADMEMBERNAME 0x14459F835
#define OFF_SQUADMEMBEREXP 0x14459F857
#define OFF_SQUADMEMBERSIZE 0x564
#define OFF_EXTINCTIONTEETH 0x1445A6B62
#define OFF_EXTINCTIONPRESTIGE 0x1445A5F96
#define OFF_EXTINCTIONRANK 0x1445A5F90
#define OFF_EXTINCTIONEXP 0x1445A5EF4
#define OFF_STEAMAPI 0x147D4B048
#define OFF_STEAMNAME 0x1D2
#define OFF_ISPRIVATEMATCH 0x147AC9E50
#define OFF_ISCURRENTHOST 0x141734DD4
#define OFF_ISALIENSMODE 0x147AD0B42
#define OFF_ALTJUMPHEIGHT 0x140840088
#define OFF_SERVERID 0x141D9BA3C
#define OFF_SERVERSESSION 0x1474F0060
#define OFF_CONTROLLER 0x141E1F2EC
#define OFF_INFOARRAY 0x141640DA0
#define OFF_MENURESPONSE 0x14450D088
#define OFF_MOUSEINPUT 0x147AC9D85

//=====================================================================================

namespace NeoGenesys
{
	enum eErrorParam
	{
		ERR_FATAL,
		ERR_DROP,
		ERR_SERVERDISCONNECT,
		ERR_DISCONNECT,
		ERR_SCRIPT,
		ERR_SCRIPT_DROP,
		ERR_LOCALIZATION,
		ERR_MAPLOADERRORSUMMARY,
		ERR_MAX
	};
	/*
	//=====================================================================================
	*/
	enum eSvsCMDType
	{
		SV_CMD_CAN_IGNORE,
		SV_CMD_RELIABLE,
		SV_CMD_MAX
	};
	/*
	//=====================================================================================
	*/
	enum eDvarSetSource
	{
		DVAR_SOURCE_INTERNAL,
		DVAR_SOURCE_EXTERNAL,
		DVAR_SOURCE_SCRIPT,
		DVAR_SOURCE_UISCRIPT,
		DVAR_SOURCE_SERVERCMD,
		DVAR_SOURCE_MAX,
	};
	/*
	//=====================================================================================
	*/
	enum eWeaponIconRatio
	{
		WEAPON_ICON_RATIO_1TO1,
		WEAPON_ICON_RATIO_2TO1,
		WEAPON_ICON_RATIO_4TO1,
		WEAPON_ICON_RATIO_MAX
	};
	/*
	//=====================================================================================
	*/
	enum eTeam
	{
		TEAM_FREE,
		TEAM_AXIS,
		TEAM_ALLIES,
		TEAM_SPECTATOR,
		TEAM_MAX
	};
	/*
	//=====================================================================================
	*/
	enum eTestClientType
	{
		TC_NONE,
		TC_TEST_CLIENT,
		TC_BOT,
		TC_MAX
	};
	/*
	//=====================================================================================
	*/
	enum eTraceHitType
	{
		TRACE_HITTYPE_NONE,
		TRACE_HITTYPE_ENTITY,
		TRACE_HITTYPE_DYNENT_MODEL,
		TRACE_HITTYPE_DYNENT_BRUSH,
		TRACE_HITTYPE_GLASS,
		TRACE_HITTYPE_MAX
	};
	/*
	//=====================================================================================
	*/
	enum ePenetrateType
	{
		PENETRATE_TYPE_NONE,
		PENETRATE_TYPE_SMALL,
		PENETRATE_TYPE_MEDIUM,
		PENETRATE_TYPE_LARGE,
		PENETRATE_TYPE_MAX
	};
	/*
	//=====================================================================================
	*/
	enum eWeaponType
	{
		WEAPTYPE_NONE,
		WEAPTYPE_BULLET,
		WEAPTYPE_GRENADE,
		WEAPTYPE_PROJECTILE,
		WEAPTYPE_RIOTSHIELD,
		WEAPTYPE_MAX
	};
	/*
	//=====================================================================================
	*/
	enum eEntityType
	{
		ET_GENERAL,
		ET_PLAYER,
		ET_PLAYER_CORPSE,
		ET_ITEM,
		ET_MISSILE,
		ET_INVISIBLE,
		ET_SCRIPTMOVER,
		ET_SOUND_BLEND,
		ET_FX,
		ET_LOOP_FX,
		ET_PRIMARY_LIGHT,
		ET_TURRET,
		ET_HELICOPTER,
		ET_PLANE,
		ET_VEHICLE,
		ET_VEHICLE_COLLMAP,
		ET_VEHICLE_CORPSE,
		ET_VEHICLE_SPAWNER,
		ET_AGENT,
		ET_AGENT_CORPSE,
		ET_EVENTS,
		ET_MAX
	};
	/*
	//=====================================================================================
	*/
	enum ePerk
	{
		PERK_ACCURACY,
		PERK_FASTRELOAD,
		PERK_RATEOFFIRE,
		PERK_EXTRABREATH,
		PERK_LONGERSPRINT,
		PERK_DETECTEXPLOSIVE,
		PERK_EXPOSEENEMY,
		PERK_EXTRAAMMO,
		PERK_TWOPRIMARIES,
		PERK_ARMORVEST,
		PERK_FRAGGRENADE,
		PERK_SPECIALGRENADE,
		PERK_EXTENDEDMAGS,
		PERK_LIGHTWEIGHT,
		PERK_MARATHON,
		PERK_QUICKDRAW,
		PERK_IMPROVEDEXTRABREATH,
		PERK_FASTSPRINTRECOVERY,
		PERK_EXTENDEDMELEE,
		PERK_FASTOFFHAND,
		PERK_STALKER,
		PERK_HOLDBREATHWHILEADS,
		PERK_LONGERRANGE,
		PERK_FASTERMELEE,
		PERK_REDUCEDSWAY,
		PERK_JAMRADAR,
		PERK_EXTRABP,
		PERK_GRENADEDEATH,
		PERK_PISTOLDEATH,
		PERK_QUIETMOVE,
		PERK_PARABOLIC,
		PERK_BULLETDMG,
		PERK_SPRINTRELOAD,
		PERK_NOT_USED,
		PERK_EXPLOSIVEBULLETS,
		PERK_SCAVENGER,
		PERK_COLDBLOODED,
		PERK_BLINDEYE,
		PERK_NOPLAYERTARGET,
		PERK_HEARTBREAKER,
		PERK_SELECTIVEHEARING,
		PERK_FASTSNIPE,
		PERK_SPYGAME,
		PERK_AUTOMANTLE,
		PERK_QUICKSWAP,
		PERK_LOWPROFILE,
		PERK_HEARTBREAKER_PRO,
		PERK_THROWBACK,
		PERK_RADAR_ISREDBLIP,
		PERK_RADAR_ISREDARROW,
		PERK_RADAR_ISJUGGERNAUT,
		PERK_SILENTKILL,
		PERK_NOSCOPEOUTLINE,
		PREK_MAX
	};
	/*
	//=====================================================================================
	*/
	enum eBone
	{
		BONE_HELMET,
		BONE_HEAD,
		BONE_NECK,
		BONE_UPPER_SPINE,
		BONE_LOWER_SPINE,
		BONE_LEFT_SHOULDER,
		BONE_RIGHT_SHOULDER,
		BONE_LEFT_HIP,
		BONE_RIGHT_HIP,
		BONE_LEFT_ELBOW,
		BONE_RIGHT_ELBOW,
		BONE_LEFT_KNEE,
		BONE_RIGHT_KNEE,
		BONE_LEFT_WRIST,
		BONE_RIGHT_WRIST,
		BONE_LEFT_ANKLE,
		BONE_RIGHT_ANKLE,
		BONE_MAX
	};
	/*
	//=====================================================================================
	*/
	enum eHitLocation : SHORT
	{
		HITLOC_NONE,
		HITLOC_HELMET,
		HITLOC_HEAD,
		HITLOC_NECK,
		HITLOC_UPPER_SPINE,
		HITLOC_LOWER_SPINE,
		HITLOC_RIGHT_SHOULDER,
		HITLOC_LEFT_SHOULDER,
		HITLOC_RIGHT_ELBOW,
		HITLOC_LEFT_ELBOW,
		HITLOC_RIGHT_WRIST,
		HITLOC_LEFT_WRIST,
		HITLOC_RIGHT_HIP,
		HITLOC_LEFT_HIP,
		HITLOC_RIGHT_KNEE,
		HITLOC_LEFT_KNEE,
		HITLOC_RIGHT_ANKLE,
		HITLOC_LEFT_ANKLE,
		HITLOC_GUN,
		HITLOC_SHIELD,
		HITLOC_ARMOR,
		HITLOC_SOFT,
		HITLOC_MAX
	};
	/*
	//=====================================================================================
	*/
	enum eWeapon
	{
		WEAPON_C4 = 15,
		WEAPON_AK_12 = 38,
		WEAPON_AMELI,
		WEAPON_ARX_160,
		WEAPON_SA_805,
		WEAPON_CBJ_MS,
		WEAPON_FAD,
		WEAPON_FP6,
		WEAPON_MR_28 = 98349,
		WEAPON_LYNX = 49198,
		WEAPON_HONEY_BADGER = 48,
		WEAPON_IA_2 = 98353,
		WEAPON_K7 = 50,
		WEAPON_CHAIN_SAW,
		WEAPON_COMBAT_KNIFE,
		WEAPON_VECTOR_CRB = 54,
		WEAPON_L115 = 32823,
		WEAPON_LSAT = 56,
		WEAPON_M27_IAR,
		WEAPON_M9A1,
		WEAPON_44_MAGNUM = 62,
		WEAPON_WILD_WIDOW,
		WEAPON_BULLDOG = 65,
		WEAPON_MTAR_X,
		WEAPON_MINIGUN,
		WEAPON_MK14_EBR = 98373,
		WEAPON_MK32 = 70,
		WEAPON_MP_443_GRACH = 72,
		WEAPON_MSBS,
		WEAPON_MTS_255,
		WEAPON_P226,
		WEAPON_PANZERFAUST = 77,
		WEAPON_PDW,
		WEAPON_BIZON,
		WEAPON_REMINGTON_R5,
		WEAPON_KASTET,
		WEAPON_RIOT_SHIELD,
		WEAPON_SC_2010 = 84,
		WEAPON_SVU = 98389,
		WEAPON_USR = 49238,
		WEAPON_TAC_12 = 87,
		WEAPON_VEPR,
		WEAPON_VKS = 49241,
		WEAPON_HELO_PILOT = 95,
		WEAPON_LOKI = 102,
		WEAPON_ODIN,
		WEAPON_TRINITY_ROCKET,
		WEAPON_GRYPHON = 111,
		WEAPON_IED = 122,
		WEAPON_MAVERICK = 136,
		WEAPON_RIPPER = 98441,
		WEAPON_MAVERICK_A2 = 49290,
		WEAPON_GOLD_KNIFE = 139,
		WEAPON_GOLD_PDW
	};
	/*
	//=====================================================================================
	*/
	static std::vector<std::pair<eBone, eHitLocation>> iBones =
	{
		std::make_pair(BONE_HELMET, HITLOC_HELMET),
		std::make_pair(BONE_HEAD, HITLOC_HEAD),
		std::make_pair(BONE_NECK, HITLOC_NECK),
		std::make_pair(BONE_UPPER_SPINE, HITLOC_UPPER_SPINE),
		std::make_pair(BONE_LOWER_SPINE, HITLOC_LOWER_SPINE),
		std::make_pair(BONE_LEFT_SHOULDER, HITLOC_LEFT_SHOULDER),
		std::make_pair(BONE_RIGHT_SHOULDER, HITLOC_RIGHT_SHOULDER),
		std::make_pair(BONE_LEFT_HIP, HITLOC_LEFT_HIP),
		std::make_pair(BONE_RIGHT_HIP, HITLOC_RIGHT_HIP),
		std::make_pair(BONE_LEFT_ELBOW, HITLOC_LEFT_ELBOW),
		std::make_pair(BONE_RIGHT_ELBOW, HITLOC_RIGHT_ELBOW),
		std::make_pair(BONE_LEFT_KNEE, HITLOC_LEFT_KNEE),
		std::make_pair(BONE_RIGHT_KNEE, HITLOC_RIGHT_KNEE),
		std::make_pair(BONE_LEFT_WRIST, HITLOC_LEFT_WRIST),
		std::make_pair(BONE_RIGHT_WRIST, HITLOC_RIGHT_WRIST),
		std::make_pair(BONE_LEFT_ANKLE, HITLOC_LEFT_ANKLE),
		std::make_pair(BONE_RIGHT_ANKLE, HITLOC_RIGHT_ANKLE)
	};
	/*
	//=====================================================================================
	*/
	static std::vector<std::pair<std::string, std::string>> szBones =
	{
		std::make_pair("Helmet", "j_helmet"),
		std::make_pair("Head", "j_head"),
		std::make_pair("Neck", "j_neck"),
		std::make_pair("Upper Spine", "j_spineupper"),
		std::make_pair("Lower Spine", "j_spinelower"),
		std::make_pair("Left Shoulder", "j_shoulder_le"),
		std::make_pair("Right Shoulder", "j_shoulder_ri"),
		std::make_pair("Left Hip", "j_hip_le"),
		std::make_pair("Right Hip", "j_hip_ri"),
		std::make_pair("Left Elbow", "j_elbow_le"),
		std::make_pair("Right Elbow", "j_elbow_ri"),
		std::make_pair("Left Knee", "j_knee_le"),
		std::make_pair("Right Knee", "j_knee_ri"),
		std::make_pair("Left Wrist", "j_wrist_le"),
		std::make_pair("Right Wrist", "j_wrist_ri"),
		std::make_pair("Left Ankle", "j_ankle_le"),
		std::make_pair("Right Ankle", "j_ankle_ri")
	};
	/*
	//=====================================================================================
	*/
	static std::vector<std::pair<std::pair<eBone, eHitLocation>, std::pair<std::string, std::string>>> vBones =
	{
		std::make_pair(iBones[BONE_HELMET], szBones[BONE_HELMET]),
		std::make_pair(iBones[BONE_HEAD], szBones[BONE_HEAD]),
		std::make_pair(iBones[BONE_NECK], szBones[BONE_NECK]),
		std::make_pair(iBones[BONE_UPPER_SPINE], szBones[BONE_UPPER_SPINE]),
		std::make_pair(iBones[BONE_LOWER_SPINE], szBones[BONE_LOWER_SPINE]),
		std::make_pair(iBones[BONE_LEFT_SHOULDER], szBones[BONE_LEFT_SHOULDER]),
		std::make_pair(iBones[BONE_RIGHT_SHOULDER], szBones[BONE_RIGHT_SHOULDER]),
		std::make_pair(iBones[BONE_LEFT_HIP], szBones[BONE_LEFT_HIP]),
		std::make_pair(iBones[BONE_RIGHT_HIP], szBones[BONE_RIGHT_HIP]),
		std::make_pair(iBones[BONE_LEFT_ELBOW], szBones[BONE_LEFT_ELBOW]),
		std::make_pair(iBones[BONE_RIGHT_ELBOW], szBones[BONE_RIGHT_ELBOW]),
		std::make_pair(iBones[BONE_LEFT_KNEE], szBones[BONE_LEFT_KNEE]),
		std::make_pair(iBones[BONE_RIGHT_KNEE], szBones[BONE_RIGHT_KNEE]),
		std::make_pair(iBones[BONE_LEFT_WRIST], szBones[BONE_LEFT_WRIST]),
		std::make_pair(iBones[BONE_RIGHT_WRIST], szBones[BONE_RIGHT_WRIST]),
		std::make_pair(iBones[BONE_LEFT_ANKLE], szBones[BONE_LEFT_ANKLE]),
		std::make_pair(iBones[BONE_RIGHT_ANKLE], szBones[BONE_RIGHT_ANKLE]),
	};
	/*
	//=====================================================================================
	*/
	static std::vector<eWeapon> vWeaponIDs =
	{
		WEAPON_AK_12, WEAPON_AMELI, WEAPON_ARX_160, WEAPON_SA_805, WEAPON_CBJ_MS,
		WEAPON_FAD, WEAPON_FP6, WEAPON_MR_28, WEAPON_LYNX, WEAPON_HONEY_BADGER,
		WEAPON_IA_2, WEAPON_K7, WEAPON_CHAIN_SAW, WEAPON_COMBAT_KNIFE, WEAPON_VECTOR_CRB, 
		WEAPON_L115, WEAPON_LSAT, WEAPON_M27_IAR, WEAPON_M9A1, WEAPON_44_MAGNUM, 
		WEAPON_WILD_WIDOW, WEAPON_BULLDOG, WEAPON_MTAR_X, WEAPON_MINIGUN, WEAPON_MK14_EBR, 
		WEAPON_MK32, WEAPON_MP_443_GRACH, WEAPON_MSBS, WEAPON_MTS_255, WEAPON_P226, 
		WEAPON_PANZERFAUST, WEAPON_PDW, WEAPON_BIZON, WEAPON_REMINGTON_R5, WEAPON_KASTET, 
		WEAPON_RIOT_SHIELD, WEAPON_SC_2010, WEAPON_SVU, WEAPON_USR, WEAPON_TAC_12, 
		WEAPON_VEPR, WEAPON_VKS, WEAPON_MAVERICK, WEAPON_RIPPER, WEAPON_MAVERICK_A2, 
		WEAPON_GOLD_KNIFE, WEAPON_GOLD_PDW
	};
	/*
	//=====================================================================================
	*/
	static std::vector<LPCSTR> szPerkIDs =
	{
		"Accuracy", "Fast Reload", "Rate Of Fire", "Extra Breath", "Longer Sprint",
		"Detect Explosive", "Expose Enemy", "Extra Ammo", "Two Primaries", "Armor Vest",
		"Frag Grenade", "Special Grenade", "Extended Mags", "Lightweight", "Marathon",
		"Quick Draw", "Improved Extra Breath", "Fast Sprint Recovery", "Extended Melee", "Fast Off Hand",
		"Stalker", "Hold Breath While ADS", "Longer Range", "Faster Melee", "Reduced Sway",
		"Jam Radar", "Extra BP", "Grenade Death", "Pistol Death", "Quiet Move",
		"Parabolic", "Bullet DMG", "Sprint Reload", "Not Used", "Explosive Bullets",
		"Scavenger", "Cold Blooded", "Blind Eye", "No Player Target", "Heartbreaker",
		"Selective Hearing", "Fast Snipe", "Spy Game", "Auto Mantle", "Quick Swap",
		"Low Profile", "Heartbreaker Pro", "Throw Back", "Radar Is Red Blip", "Radar Is Red Arrow",
		"Radar Is Juggernaut", "Silent Kill", "No Scope Outline"
	};
	/*
	//=====================================================================================
	*/
	static std::vector<ePerk> vPerkIDs =
	{
		PERK_ACCURACY, PERK_FASTRELOAD, PERK_RATEOFFIRE, PERK_EXTRABREATH, PERK_LONGERSPRINT,
		PERK_DETECTEXPLOSIVE, PERK_EXPOSEENEMY, PERK_EXTRAAMMO, PERK_TWOPRIMARIES, PERK_ARMORVEST,
		PERK_FRAGGRENADE, PERK_SPECIALGRENADE, PERK_EXTENDEDMAGS, PERK_LIGHTWEIGHT, PERK_MARATHON,
		PERK_QUICKDRAW, PERK_IMPROVEDEXTRABREATH, PERK_FASTSPRINTRECOVERY, PERK_EXTENDEDMELEE, PERK_FASTOFFHAND,
		PERK_STALKER, PERK_HOLDBREATHWHILEADS, PERK_LONGERRANGE, PERK_FASTERMELEE, PERK_REDUCEDSWAY,
		PERK_JAMRADAR, PERK_EXTRABP, PERK_GRENADEDEATH, PERK_PISTOLDEATH, PERK_QUIETMOVE,
		PERK_PARABOLIC, PERK_BULLETDMG, PERK_SPRINTRELOAD, PERK_NOT_USED, PERK_EXPLOSIVEBULLETS,
		PERK_SCAVENGER, PERK_COLDBLOODED, PERK_BLINDEYE, PERK_NOPLAYERTARGET, PERK_HEARTBREAKER,
		PERK_SELECTIVEHEARING, PERK_FASTSNIPE, PERK_SPYGAME, PERK_AUTOMANTLE, PERK_QUICKSWAP,
		PERK_LOWPROFILE, PERK_HEARTBREAKER_PRO, PERK_THROWBACK, PERK_RADAR_ISREDBLIP, PERK_RADAR_ISREDARROW,
		PERK_RADAR_ISJUGGERNAUT, PERK_SILENTKILL, PERK_NOSCOPEOUTLINE
	};
	/*
	//=====================================================================================
	*/
	static std::vector<std::string> szAchievements =
	{
		"LEVEL_1",
		"LEVEL_1A",
		"LEVEL_2",
		"LEVEL_2A",
		"LEVEL_3",
		"LEVEL_3A",
		"LEVEL_4",
		"LEVEL_4A",
		"LEVEL_5",
		"LEVEL_5A",
		"LEVEL_6",
		"LEVEL_6A",
		"LEVEL_7",
		"LEVEL_7A",
		"LEVEL_7B",
		"LEVEL_8",
		"LEVEL_8A",
		"LEVEL_9",
		"LEVEL_9A",
		"LEVEL_10",
		"LEVEL_10A",
		"LEVEL_11",
		"LEVEL_11A",
		"LEVEL_11B",
		"LEVEL_12",
		"LEVEL_12A",
		"LEVEL_13",
		"LEVEL_13A",
		"LEVEL_14",
		"LEVEL_14A",
		"LEVEL_15",
		"LEVEL_15A",
		"LEVEL_16",
		"LEVEL_16A",
		"LEVEL_17",
		"LEVEL_17A",
		"LEVEL_18",
		"LEVEL_18A",
		"EXT_1",
		"ESCAPE_ALL_PLAYERS",
		"ESCAPE_IN_TIME",
		"ESCAPE_1ST_TIME",
		"ESCAPE_ALL_CHALLENGE",
		"REACH_CABIN",
		"REACH_CITY",
		"KILL_WITH_TRAP",
		"SCAVENGE_ITEM",
		"ESCAPE_WITH_NERF_ON",
		"EXTRA1",
		"EXTRA2",
		"REACH_COMPOUND",
		"REACH_FACILITY",
		"KILLBOSS_1ST_TIME",
		"KILLBOSS_IN_TIME",
		"KILL_WITH_SWEAPON",
		"COMPLETE_ALL_CHALLENGE",
		"KILLBOSS_WITH_RELIC",
		"KILL_PHANTOMS",
		"KILL_RHINO_PISTOL",
		"FOUND_ALL_INTELS",
		"GOT_THEEGGSTRA_XP",
		"REACH_CARGO",
		"REACH_DECK",
		"KILL_KRAKEN_1ST_TIME",
		"KILL_KRAKEN_WITH_RELIC",
		"KILL_KRAKEN_AND_ALL_CHALLENGES",
		"FOUND_ALL_INTELS_MAYDAY",
		"KILL_SEEDER_TURRETS",
		"HYPNO_TRAP_RHINO",
		"GOT_THEEGGSTRA_XP_DLC2",
		"CRAFT_ALL_ITEMS",
		"FINISH_GATE",
		"EXTEND_THE_BRIDGE",
		"AWAKENING_ESCAPE",
		"AWAKENING_RELIC_ESCAPE",
		"AWAKENING_ALL_CHALLENGES_ESCAPE",
		"AWAKENING_ALL_INTEL",
		"KILL_GARG_WITH_VANGUARD",
		"GOT_THEEGGSTRA_XP_DLC3",
		"LIKE_A_GLOVE",
		"AWAKENING_4_CLASS_ESCAPE",
		"LAST_COMPLETED",
		"LAST_RELIC_COMPLETED",
		"LAST_ALL_CHALLENGES_COMPLETED",
		"LAST_ALL_INTEL",
		"LAST_COMPLETE_AN_OUTPOST",
		"GOT_THEEGGSTRA_XP_DLC4",
		"TIMING_IS_EVERYTHING",
		"YOU_WISH",
		"ALWAYS_HARD",
		"HAT_TRICK"
	};
	/*
	//=====================================================================================
	*/
	struct sCvar
	{
		std::string szName;
		std::vector<std::string> szItems;

		union uCvarValue
		{
			bool bValue;
			std::int32_t iValue;
			std::uint32_t dwValue;
			std::uint64_t qwValue;
			std::float_t flValue;
			ImVec4 cValue;
			LPSTR szValue;

			uCvarValue(bool value) : bValue(value) {}
			uCvarValue(std::int32_t value) : iValue(value) {}
			uCvarValue(std::uint32_t value) : dwValue(value) {}
			uCvarValue(std::uint64_t value) : qwValue(value) {}
			uCvarValue(std::float_t value) : flValue(value) {}
			uCvarValue(ImVec4 value) : cValue(value) {}
			uCvarValue(LPSTR value) : szValue(value) {}
		} Current, Reset;

		union uCvarLimits
		{
			struct
			{
				std::int32_t iMin;
				std::int32_t iMax;
			};

			struct
			{
				std::uint32_t dwMin;
				std::uint32_t dwMax;
			};

			struct
			{
				std::uint64_t qwMin;
				std::uint64_t qwMax;
			};

			struct
			{
				std::float_t flMin;
				std::float_t flMax;
			};

			uCvarLimits(std::int32_t min, std::int32_t max) : iMin(min), iMax(max) {}
			uCvarLimits(std::uint32_t min, std::uint32_t max) : dwMin(min), dwMax(max) {}
			uCvarLimits(std::uint64_t min, std::uint64_t max) : qwMin(min), qwMax(max) {}
			uCvarLimits(std::float_t min, std::float_t max) : flMin(min), flMax(max) {}
		} Domain;

		sCvar(std::string name, std::vector<std::string> items, bool value) : szName(name), szItems(items), Current(value), Reset(value), Domain(NULL, NULL) {}
		sCvar(std::string name, std::vector<std::string> items, std::int32_t value, std::int32_t min, std::int32_t max) : szName(name), szItems(items), Current(value), Reset(value), Domain(min, max) {}
		sCvar(std::string name, std::vector<std::string> items, std::uint32_t value, std::uint32_t min, std::uint32_t max) : szName(name), szItems(items), Current(value), Reset(value), Domain(min, max) {}
		sCvar(std::string name, std::vector<std::string> items, std::uint64_t value, std::uint64_t min, std::uint64_t max) : szName(name), szItems(items), Current(value), Reset(value), Domain(min, max) {}
		sCvar(std::string name, std::vector<std::string> items, std::float_t value, std::float_t min, std::float_t max) : szName(name), szItems(items), Current(value), Reset(value), Domain(min, max) {}
		sCvar(std::string name, std::vector<std::string> items, ImVec4 value) : szName(name), szItems(items), Current(value), Reset(value), Domain(NULL, NULL) {}
		sCvar(std::string name, std::vector<std::string> items, LPSTR value) : szName(name), szItems(items), Current(value), Reset(value), Domain(NULL, NULL) {}
	};
	/*
	//=====================================================================================
	*/
	struct sDvar
	{
	public:
		LPSTR szName;
	private:
		char _0x8[0x8];
	public:
		union uDvarValue
		{
			bool bValue;
			std::int32_t iValue;
			std::uint32_t dwValue;
			std::uint64_t qwValue;
			std::float_t flValue;
			ImVec4 cValue;
			LPSTR szValue;
		} Current, Latched, Reset;
		union uDvarLimits
		{
			struct
			{
				std::int32_t iMin;
				std::int32_t iMax;
			};
			struct
			{
				std::uint32_t dwMin;
				std::uint32_t dwMax;
			};
			struct
			{
				std::uint64_t qwMin;
				std::uint64_t qwMax;
			};
			struct
			{
				std::float_t flMin;
				std::float_t flMax;
			};
		} Domain;
	private:
		char _0x50[0x10];
	};
	/*
	//=====================================================================================
	*/
	struct sSnapShot
	{
	private:
		char _0x0[0x3C78];
	public:
		int iPing;
		int iServerTime;
	};
	/*
	//=====================================================================================
	*/
	struct sClientState
	{
	public:
		int iClientIndex;
		eTeam iTeam;
	private:
		char _0x8[0x4];
	public:
		int iDualWielding;
	private:
		char _0x10[0x30];
	public:
		char szName[32];
		int iRank;
		int iPrestige;
	private:
		char _0x68[0x8];
	public:
		int iPerks[2];
	private:
		char _0x78[0x78];
	};
	/*
	//=====================================================================================
	*/
	struct sGlobalAmmo
	{
	private:
		char _0x0[0x8];
	public:
		int iAmmoCount;
	private:
		char _0xC[0x4];
	};
	/*
	//=====================================================================================
	*/
	struct sClipAmmo
	{
	private:
		char _0x0[0x8];
	public:
		int iAmmoCount[2];
	};
	/*
	//=====================================================================================
	*/
	struct sPlayerState
	{
	public:
		int iCommandTime;
		int iMoveType;
	private:
		char _0x8[0x8];
	public:
		int iOtherFlags;
		int iLinkFlags;
	private:
		char _0x18[0x4];
	public:
		ImVec3 vOrigin;
		ImVec3 vVelocity;
	private:
		char _0x34[0x28];
	public:
		int iGravity;
		int iSpeed;
		ImVec3 vDeltaAngles;
	private:
		char _0x70[0xB4];
	public:
		int iEntityFlags;
	private:
		char _0x128[0x54];
	public:
		int iEntityNum;
	private:
		char _0x180[0x4];
	public:
		ImVec3 vViewAngles;
	private:
		char _0x190[0x38];
	public:
		int iStats[4];
	private:
		char _0x1D8[0x84];
	public:
		float flSpeedMultiplier;
	private:
		char _0x260[0x1E0];
	public:
		sGlobalAmmo AmmoNotInClip[15];
		sClipAmmo AmmoInClip[15];
	private:
		char _0x620[0x2D40];
	public:
		int iClientNum;
	private:
		char _0x3364[0x24];
	public:
		sSnapShot* OldSnapShot;
		sSnapShot* NewSnapShot;
	private:
		char _0x3398[0x5C];
	public:
		sClientState ClientState;
	private:
		char _0x34E4[0x584];
	};
	/*
	//=====================================================================================
	*/
	struct sCG
	{
	public:
		sPlayerState PredictedPlayerState;
	private:
		char _0x3A68[0xA6028];
	public:
		ImVec3 vRefDefViewAngles;
		ImVec3 vWeaponAngles;
	private:
		char _0xA9AA8[0x24];
	public:
		ImVec3 vThirdPersonViewAngles;
	};
	/*
	//=====================================================================================
	*/
	struct sRefDef
	{
	public:
		int iWidth;
		int iHeight;
		float flFovX;
		float flFovY;
		ImVec3 vViewOrigin;
		ImVec3 vViewAxis[3];
	};
	/*
	//=====================================================================================
	*/
	struct sPlayerName
	{
	public:
		char szName[32];
	private:
		char _0x20[0x1A8];
	};
	/*
	//=====================================================================================
	*/
	struct sCharacterInformation
	{
	public:
		int iEntityNum;
		int iInfoValid;
		int iNextValid;
		eTeam iTeam;
		eTeam iOldTeam;
		int iPerks[2];
	private:
		char _0x1C[0x444];
	public:
		ImVec3 vViewAngles;
	private:
		char _0x46C[0x128];
	public:
		int iDualWielding;
		int iIsFemale;
	private:
		char _0x59C[0x3C];
	};
	/*
	//=====================================================================================
	*/
	struct sTrajectory
	{
	public:
		int iType;
		int iTime;
		int iDuration;
		ImVec3 vBase;
		ImVec3 vDelta;
	};
	/*
	//=====================================================================================
	*/
	struct sLerpEntityState
	{
	public:
		int iEntityFlags;
		sTrajectory PositionTrajectory;
		sTrajectory AngleTrajectory;
	private:
		char _0x4C[0x10];
	public:
		int iPrimaryWeapon;
		int iSecondaryWeapon;
	private:
		char _0x64[0x4];
	};
	/*
	//=====================================================================================
	*/
	struct sEntityEvent
	{
	public:
		int iEventType;
		int iEventParam;
	};
	/*
	//=====================================================================================
	*/
	struct sEntityState
	{
	public:
		int iEntityNum;
		int iEntityType;
		sLerpEntityState LerpEntityState;
	private:
		char _0x70[0x8];
	public:
		int iOtherEntityNum;
		int iAttackerEntityNum;
		int iGroundEntityNum;
	private:
		char _0x84[0xC];
	public:
		int iClientNum;
	private:
		char _0x94[0x4];
	public:
		int iSolid;
		int iEventParam;
		int iEventSequence;
		sEntityEvent EntityEvent[4];
		int iWeapon;
		int iInAltWeaponMode;
	private:
		char _0xCC[0x8];
	public:
		int iEventParam2;
	private:
		char _0xD8[0x30];
	};
	/*
	//=====================================================================================
	*/
	struct sCEntity
	{
	private:
		char _0x0[0x2];
	public:
		short wValid;
	private:
		char _0x4[0x14];
	public:
		ImVec3 vOrigin;
		ImVec3 vViewAngles;
	private:
		char _0x30[0x60];
	public:
		sLerpEntityState CurrentEntityState;
		sEntityState NextEntityState;
		int iIsAlive;
	private:
		char _0x204[0x34];
	};
	/*
	//=====================================================================================
	*/
	struct sGEntity
	{
	public:
		sEntityState EntityState;
	private:
		char _0x108[0x60];
	public:
		sPlayerState* pPlayerState;
	private:
		char _0x170[0x50];
	public:
		int iFlags;
	private:
		char _0x1C4[0x18];
	public:
		int iHealth;
		int iMaxHealth;
	private:
		char _0x1E4[0x10C];
	};
	/*
	//=====================================================================================
	*/
	struct sUserCmd
	{
	public:
		int iServerTime;
		int iButtons;
		int iViewAngles[3];
	private:
		char _0x14[0x8];
	public:
		char szForwardMove;
		char szRightMove;
	private:
		char _0x1E[0x1E];
	};
	/*
	//=====================================================================================
	*/
	struct sClientActive
	{
	public:
		sUserCmd UserCmd[128];
		int iCurrentCmd;

		sUserCmd* GetUserCmd(int number)
		{
			return &UserCmd[number & 0x7F];
		}
	};
	/*
	//=====================================================================================
	*/
	struct sClientInformation
	{
	public:
		int iClientNum;
		char szName[32];
		int iRankMP;
		int iPrestigeMP;
		int iRankEXT;
		int iPrestigeEXT;
		char szClan[8];
	private:
		char _0x3C[4];
	public:
		int iHealth;
	private:
		char _0x44[0x2C];
	};
	/*
	//=====================================================================================
	*/
	struct sTrace
	{
	public:
		float flFraction;
		ImVec3 vNormal;
	private:
		char _0x10[0x8];
	public:
		eTraceHitType TraceHitType;
		short wHitID;
	private:
		char _0x1E[0x6];
	public:
		eHitLocation wPartGroup;
		bool bAllSolid;
		bool bStartSolid;
	private:
		char _0x28[0x4];
	};
	/*
	//=====================================================================================
	*/
	struct sBulletTraceResults
	{
	public:
		sTrace Trace;
	private:
		char _0x2C[0x4];
	public:
		sCEntity* pHitEnt;
		ImVec3 vHitPos;
		int iIgnoreHitEnt;
		int iDepthSurfaceType;
		int iHitClientNum;
	};
	/*
	//=====================================================================================
	*/
	struct sBulletFireParams
	{
	public:
		int iMaxEntNum;
		int iEntityNum;
		float flPower;
		int iBulletType;
	private:
		char _0x10[0x4];
	public:
		ImVec3 vViewOrigin;
		ImVec3 vStart;
		ImVec3 vEnd;
		ImVec3 vDir;
	};
	/*
	//=====================================================================================
	*/
	struct sOrientation
	{
	public:
		ImVec3 vOrigin;
		ImVec3 vAxis[3];
	};
	/*
	//=====================================================================================
	*/
	struct sNetAddr
	{
	public:
		int iType;
		char szIPAddress[4];
		short wPort;
	private:
		char _0xA[0x2];
	public:
		int iLocalNetID;
		int iServerID;
	};
	/*
	//=====================================================================================
	*/
	struct sEntRef
	{
	public:
		short wEntityNum;
		short wClassNum;

		sEntRef(short entitynum, short classnum) : 
			wEntityNum(entitynum),
			wClassNum(classnum) 
		{

		}
	};
	/*
	//=====================================================================================
	*/
	struct sWeaponDef
	{
	private:
		char _0x0[0x94];
	public:
		eWeaponType iWeaponType;
	private:
		char _0x98[0x4];
	public:
		ePenetrateType iPenetrateType;
	private:
		char _0x100[0x2D0];
	public:
		LPVOID lpHUDIcon;
		eWeaponIconRatio iHUDIconRatio;
	private:
		char _0x37C[0x4];
	public:
		LPVOID lpPickupIcon;
		eWeaponIconRatio iPickupIconRatio;
	private:
		char _0x38C[0x4];
	public:
		LPVOID lpAmmoCounterIcon;
		eWeaponIconRatio iAmmoCounterIconRatio;
	private:
		char _0x39C[0x320];
	public:
		eWeaponIconRatio iKillIconRatio;
	private:
		char _0x6C0[0x391];
	public:
		bool bRifleBullet;
	private:
		char _0xA52[0x13];
	public:
		bool bNoDualWield;
		bool bFlipKillIcon;
	private:
		char _0xA66[0x32];
	};
	/*
	//=====================================================================================
	*/
	struct sWeapons
	{
	public:
		sWeaponDef* WeaponDef[160];
	};
	/*
	//=====================================================================================
	*/
	struct sWeaponCompleteDef
	{
	public:
		LPSTR szInternalName;
		sWeaponDef* WeaponDef;
		LPSTR szDisplayName;
	private:
		char _0x18[0x94];
	public:
		eWeaponIconRatio iDPadIconRatio;
	private:
		char _0xB0[0x28];
	public:
		LPVOID lpKillIcon;
		LPVOID lpDPadIcon;
	private:
		char _0xE8[0x40];
	};
	/*
	//=====================================================================================
	*/
	struct sCompleteWeapons
	{
	public:
		sWeaponCompleteDef* WeaponCompleteDef[160];
	};
	/*
	//=====================================================================================
	*/
	struct sWindow
	{
	public:
		int iWidth;
		int iHeight;
	};
	/*
	//=====================================================================================
	*/
	struct sViewMatrix
	{
	public:
		ImVec3 vRecoilAngles;
		ImVec3 vOrigin;
	private:
		char _0xC[0x84];
	public:
		ImVec3 vViewAngles;
	};
	/*
	//=====================================================================================
	*/
	struct sPunch
	{
	public:
		ImVec2 vPunchAngles;
		float flVelocity;
		ImVec3 vWeaponPunchAngles[3];
	};
	/*
	//=====================================================================================
	*/
	static MODULEINFO hIw6mp64_ship = GetModuleInfo(NULL);
	static MODULEINFO hGameOverlayRenderer64;
	/*
	//=====================================================================================
	*/
#define CG ((sCG*)OFF_CG)
#define RefDef ((sRefDef*)OFF_REFDEF)
#define PlayerName ((sPlayerName*)OFF_PLAYERNAME)
#define CharacterInformation ((sCharacterInformation*)OFF_CHARACTERINFORMATION)
#define CEntity ((sCEntity*)OFF_CENTITY)
#define GEntity ((sGEntity*)OFF_GENTITY)
#define PlayerState ((sPlayerState*)OFF_PLAYERSTATE)
#define ClientActive ((sClientActive*)OFF_CLIENTACTIVE)
#define ClientInformation ((sClientInformation*)OFF_CLIENTINFORMATION)
#define Weapons ((sWeapons*)OFF_WEAPONDEF)
#define CompleteWeapons ((sCompleteWeapons*)OFF_WEAPONCOMPLETEDEF)
#define Window ((sWindow*)OFF_WINDOW)
#define ViewMatrix ((sViewMatrix*)OFF_VIEWMATRIX)
#define Punch ((sPunch*)OFF_PUNCH)
	/*
	//=====================================================================================
	*/
	template <typename... Parameters>
	FORCEINLINE void Com_Error(eErrorParam code, LPCSTR format, Parameters... params)
	{
		return VariadicCall<void>(OFF_COMERROR, code, format, params...);
	}
	/*
	//=====================================================================================
	*/
	typedef void(__fastcall* tFunction)(int localnum);
	FORCEINLINE void Cbuf_AddCall(tFunction function)
	{
		return VariadicCall<void>(OFF_CBUFADDCALL, 0, function);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void Cbuf_AddText(std::string command)
	{
		return VariadicCall<void>(OFF_CBUFADDTEXT, 0, command.c_str());
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE std::uint64_t Sys_GetValue(int value)
	{
		return VariadicCall<std::uint64_t>(OFF_SYSGETVALUE, value);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int Sys_Milliseconds()
	{
		return VariadicCall<int>(OFF_SYSMILLISECONDS);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void GameSendServerCommand(int clientnum, eSvsCMDType type, std::string command)
	{
		return VariadicCall<void>(OFF_GAMESENDSERVERCOMMAND, clientnum, type, command.c_str());
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool LocalClientIsInGame()
	{
		return VariadicCall<bool>(OFF_LOCALCLIENTISINGAME, 0);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool IsMainThread()
	{
		return VariadicCall<bool>(OFF_ISMAINTHREAD);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool IsRenderThread()
	{
		return VariadicCall<bool>(OFF_ISRENDERTHREAD);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool IsServerThread()
	{
		return VariadicCall<bool>(OFF_ISSERVERTHREAD);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool IsDatabaseThread()
	{
		return VariadicCall<bool>(OFF_ISDATABASETHREAD);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE LRESULT EnableConsole()
	{
		return VariadicCall<LRESULT>(OFF_ENABLECONSOLE);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void PrintToConsole(std::string message)
	{
		return VariadicCall<void>(OFF_PRINTTOCONSOLE, message.c_str());
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE sDvar* FindVariable(std::string name)
	{
		return VariadicCall<sDvar*>(OFF_FINDVARIABLE, name.c_str());
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool GetVariableIndex(sDvar* dvar, DWORD* index)
	{
		return VariadicCall<bool>(OFF_GETVARIABLEINDEX, dvar, index);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE sDvar* SetDvarFromStringByNameFromSource(std::string name, std::string value, eDvarSetSource source)
	{
		return VariadicCall<sDvar*>(OFF_SETDVARFROMSTRINGBYNAMEFROMSOURCE, name.c_str(), value.c_str(), source);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE sDvar* UpdateDvar(DWORD controller, sDvar* dvar, std::uint64_t infoarray)
	{
		return VariadicCall<sDvar*>(OFF_UPDATEDVAR, controller, dvar, infoarray);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void Say(sGEntity* from, sGEntity* to, int mode, std::string message)
	{
		return VariadicCall<void>(OFF_SAY, from, to, mode, message.c_str());
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void SayTo(sGEntity* from, sGEntity* to, int mode, int color, std::string team, std::string name, std::string message)
	{
		return VariadicCall<void>(OFF_SAYTO, from, to, mode, color, team.c_str(), name.c_str(), message.c_str());
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE LPVOID GetCurrentSession()
	{
		return VariadicCall<LPVOID>(OFF_GETCURRENTSESSION);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int GetSessionHost(LPVOID session)
	{
		return VariadicCall<int>(OFF_GETSESSIONHOST, session);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool IsSessionHost(LPVOID session, int clientnum)
	{
		return VariadicCall<bool>(OFF_ISSESSIONHOST, session, clientnum);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool IsUserRegistered(LPVOID session, int clientnum)
	{
		return VariadicCall<bool>(OFF_ISUSERREGISTERED, session, clientnum);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE sNetAddr* GetPlayerAddr(sNetAddr* netaddr, LPVOID session, int clientnum)
	{
		return VariadicCall<sNetAddr*>(OFF_GETPLAYERADDR, netaddr, session, clientnum);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE LPCSTR GetCurrentName()
	{
		return VariadicCall<LPCSTR>(OFF_GETCURRENTNAME);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE LPCSTR GetCurrentXUID()
	{
		return VariadicCall<LPCSTR>(OFF_GETCURRENTXUID, 0);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE LPVOID GetScreenMatrix()
	{
		return VariadicCall<LPVOID>(OFF_GETSCREENMATRIX);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int GetViewmodelWeapon(sPlayerState* playerstate)
	{
		return VariadicCall<int>(OFF_GETVIEWMODELWEAPON, playerstate);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE LPCSTR GetWeaponNameComplete(int weapon, bool alternate, LPSTR name, int length)
	{
		return VariadicCall<LPCSTR>(OFF_GETWEAPONNAMECOMPLETE, weapon, alternate, name, length);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE LPCSTR GetWeaponDisplayName(int weapon, bool alternate, LPSTR name, int length)
	{
		return VariadicCall<LPCSTR>(OFF_GETWEAPONDISPLAYNAME, weapon, alternate, name, length);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int GetWeaponForName(std::string name)
	{
		return VariadicCall<int>(OFF_GETWEAPONFORNAME, name.c_str());
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool GetPerkName(ePerk perk, LPSTR* name)
	{
		return VariadicCall<bool>(OFF_GETPERKNAME, perk, name);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool GetPerkIndex(LPSTR name, ePerk* perk)
	{
		return VariadicCall<bool>(OFF_GETPERKINDEX, name, perk);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool IsPlayerReloading()
	{
		return VariadicCall<bool>(OFF_ISPLAYERRELOADING, 0);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool WeaponBothClipEmpty(sPlayerState* playerstate)
	{
		return VariadicCall<bool>(OFF_WEAPONBOTHCLIPEMPTY, playerstate);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool WeaponIsDualWield(int weapon)
	{
		return VariadicCall<bool>(OFF_WEAPONISDUALWIELD, weapon);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool IsRifleBullet(int weapon, bool alternate)
	{
		return VariadicCall<bool>(OFF_ISRIFLEBULLET, weapon, alternate);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool WorldToScreen(LPVOID screenmatrix, const ImVec3 world, ImVec2* screen)
	{
		return VariadicCall<bool>(OFF_WORLDTOSCREEN, 0, screenmatrix, world, screen);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int AddMessageIcon(LPSTR icon, int length, LPVOID shader, float width, float height, bool flip)
	{
		return VariadicCall<int>(OFF_ADDMESSAGEICON, icon, length, 1024, shader, width, height, flip);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE LPVOID RegisterFont(std::string name)
	{
		return VariadicCall<LPVOID>(OFF_REGISTERFONT, name.c_str());
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE LPVOID RegisterShader(std::string name)
	{
		return VariadicCall<LPVOID>(OFF_REGISTERSHADER, name.c_str());
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int RegisterTag(std::string name)
	{
		return VariadicCall<int>(OFF_REGISTERTAG, name.c_str());
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void DrawStretchPic(float x, float y, float w, float h, const ImVec4 color, LPVOID shader)
	{
		return VariadicCall<void>(OFF_DRAWSTRETCHPIC, x, y, w, h, 0.0f, 0.0f, 1.0f, 1.0f, color, shader);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void DrawRotatedPic(LPVOID screenmatrix, float x, float y, float w, float h, float angle, const ImVec4 color, LPVOID shader)
	{
		return VariadicCall<void>(OFF_DRAWROTATEDPIC, screenmatrix, x, y, w, h, angle, color, shader);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void DrawEngineText(std::string text, LPVOID font, float x, float y, float w, float h, const ImVec4 color, int flags)
	{
		return VariadicCall<void>(OFF_DRAWENGINETEXT, text.c_str(), text.length(), font, x, y, w, h, 0.0f, color, flags);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int StringHeight(LPVOID font)
	{
		return VariadicCall<int>(OFF_STRINGHEIGHT, font);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int StringWidth(std::string text, LPVOID font)
	{
		return VariadicCall<int>(OFF_STRINGWIDTH, text.c_str(), text.length(), font);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void GetPlayerViewOrigin(sPlayerState* playerstate, ImVec3* position)
	{
		return VariadicCall<void>(OFF_GETPLAYERVIEWORIGIN, 0, playerstate, position);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool IsThirdPersonMode(sPlayerState* playerstate)
	{
		return VariadicCall<bool>(OFF_ISTHIRDPERSONMODE, playerstate);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE float GetThirdPersonCrosshairOffset(sPlayerState* playerstate)
	{
		return VariadicCall<float>(OFF_GETTHIRDPERSONCROSSHAIROFFSET, playerstate);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE LPVOID GetEntityDObj(int entitynum)
	{
		return VariadicCall<LPVOID>(OFF_GETENTITYDOBJ, entitynum);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int GetTagPosition(sCEntity* entity, LPVOID entitydobj, int tag, ImVec3* position)
	{
		return VariadicCall<int>(OFF_GETTAGPOSITION, entity, entitydobj, tag, position);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool GetTagOrientation(int entitynum, int tag, sOrientation* orientation)
	{
		return VariadicCall<bool>(OFF_GETTAGORIENTATION, 0, entitynum, tag, orientation);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int GetHitLocationString(eHitLocation hitloc)
	{
		return VariadicCall<int>(OFF_GETHITLOCATIONSTRING, hitloc);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int LocationalTrace(sTrace* trace, const ImVec3 from, const ImVec3 to, int skip, DWORD mask)
	{
		return VariadicCall<int>(OFF_LOCATIONALTRACE, trace, from, to, skip, mask);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void VectorAngles(const ImVec3 direction, ImVec3* angles)
	{
		return VariadicCall<void>(OFF_VECTORANGLES, direction, angles);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void AngleVectors(const ImVec3 angles, ImVec3* forward, ImVec3* right, ImVec3* up)
	{
		return VariadicCall<void>(OFF_ANGLEVECTORS, angles, forward, right, up);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void VectorNormalize(ImVec3* angles)
	{
		return VariadicCall<void>(OFF_VECTORNORMALIZE, angles);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE float AngleNormalize180(float angle1, float angle2)
	{
		return VariadicCall<float>(OFF_ANGLENORMALIZE180, angle1, angle2);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void ExecuteKey(DWORD keyindex)
	{
		return VariadicCall<void>(OFF_EXECUTEKEY, 0, keyindex, 1, 0);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool IsGamePadEnabled()
	{
		return VariadicCall<bool>(OFF_ISGAMEPADENABLED);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void SetZoomState(bool enable)
	{
		return VariadicCall<void>(OFF_SETZOOMSTATE, 0, enable);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool C_BulletTrace(sBulletFireParams* fireparams, sCEntity* entity, sBulletTraceResults* traceresults, int surfacetype)
	{
		return VariadicCall<bool>(OFF_CBULLETTRACE, 0, fireparams, 0, entity, traceresults, surfacetype);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool G_BulletTrace(sBulletFireParams* fireparams, int weapon, bool alternate, sGEntity* entity, sBulletTraceResults* traceresults, int surfacetype)
	{
		return VariadicCall<bool>(OFF_GBULLETTRACE, fireparams, weapon, alternate, entity, traceresults, surfacetype);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE WORD GetTraceHitType(sTrace* trace)
	{
		return VariadicCall<WORD>(OFF_GETTRACEHITTYPE, trace);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool AdvanceTrace(sBulletFireParams* fireparams, sBulletTraceResults* traceresults, float distance)
	{
		return VariadicCall<bool>(OFF_ADVANCETRACE, fireparams, traceresults, distance);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE float GetSurfacePenetrationDepth(int weapon, bool alternate, int surfacetype)
	{
		return VariadicCall<float>(OFF_GETSURFACEPENETRATIONDEPTH, weapon, alternate, surfacetype);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE float GetWeaponHitLocationMultiplier(eHitLocation hitloc, int weapon, bool alternate)
	{
		return VariadicCall<float>(OFF_GETWEAPONHITLOCATIONMULTIPLIER, hitloc, weapon, alternate);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int GetWeaponDamage(sBulletFireParams* fireparams, sBulletTraceResults* traceresults, int weapon, bool alternate, sGEntity* attacker)
	{
		return VariadicCall<int>(OFF_GETWEAPONDAMAGE, fireparams, traceresults, weapon, alternate, attacker);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int GetBulletDamageForProjectile(sPlayerState* playerstate, int weapon, bool alternate, const ImVec3 startpos, const ImVec3 hitpos)
	{
		return VariadicCall<int>(OFF_GETBULLETDAMAGEFORPROJECTILE, playerstate, weapon, alternate, startpos, hitpos);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool PenetrationCheck(sBulletFireParams* fireparams)
	{
		return VariadicCall<bool>(OFF_PENETRATIONCHECK, 0, fireparams);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE float GetHipfireSpreadForWeapon(sPlayerState* playerstate, int weapon, float* minimum, float* maximum)
	{
		return VariadicCall<float>(OFF_GETHIPFIRESPREADFORWEAPON, playerstate, weapon, minimum, maximum);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE float GetZoomSpreadForWeapon(sPlayerState* playerstate, int weapon, bool alternate)
	{
		return VariadicCall<float>(OFF_GETZOOMSPREADFORWEAPON, playerstate, weapon, alternate);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int BulletEndPosition(int* seed, float spread, const ImVec3 vieworg, ImVec3* end, ImVec3* direction, const ImVec3 forward, const ImVec3 right, const ImVec3 up)
	{
		return VariadicCall<int>(OFF_BULLETENDPOSITION, seed, 0.0f, spread, vieworg, end, direction, 0.0f, 360.0f, forward, right, up, 8192.0f);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int SeedRandom(int* servertime)
	{
		return VariadicCall<int>(OFF_SEEDRANDOM, servertime);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE float RandomFloat(int* seed)
	{
		return VariadicCall<float>(OFF_RANDOM, seed);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void EvaluateTrajectory(sTrajectory* trajectory, int time, ImVec3* result)
	{
		return VariadicCall<void>(OFF_EVALUATETRAJECTORY, trajectory, time, result);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void TeamChanged(int clientnum)
	{
		return VariadicCall<void>(OFF_TEAMCHANGED, clientnum);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void AddString(std::string value)
	{
		return VariadicCall<void>(OFF_ADDSTRING, value.c_str());
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void Notify(int entitynum, DWORD stringvalue, int paramnum)
	{
		return VariadicCall<void>(OFF_NOTIFY, entitynum, 0, stringvalue, paramnum);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE sGEntity* AddBot(std::string name, int head, int body, int helmet)
	{
		return VariadicCall<sGEntity*>(OFF_ADDBOT, name.c_str(), head, body, helmet);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE sGEntity* AddTestClient(eTestClientType type, eTeam team, int index, sEntRef entref)
	{
		return VariadicCall<sGEntity*>(OFF_ADDTESTCLIENT, type, team, index, entref);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE sEntRef AddEntity(sGEntity* entity)
	{
		return VariadicCall<sEntRef>(OFF_ADDENTITY, entity);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool SpawnTestClient(sGEntity* entity)
	{
		return VariadicCall<bool>(OFF_SPAWNTESTCLIENT, entity);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int TakePlayerWeapon(sPlayerState* playerstate, int weapon)
	{
		return VariadicCall<int>(OFF_TAKEPLAYERWEAPON, playerstate, weapon);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int GivePlayerWeapon(sPlayerState* playerstate, int weapon, bool akimbo, bool alternate, bool previous)
	{
		return VariadicCall<int>(OFF_GIVEPLAYERWEAPON, playerstate, weapon, akimbo, alternate, previous);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE int AddAmmo(sPlayerState* playerstate, int weapon, bool alternate, int count, bool fill)
	{
		return VariadicCall<int>(OFF_ADDAMMO, playerstate, weapon, alternate, count, fill);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool IsMigrating()
	{
		return VariadicCall<bool>(OFF_ISMIGRATING);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void GameDropClient(int clientnum, std::string reason)
	{
		return VariadicCall<void>(OFF_GAMEDROPCLIENT, clientnum, reason.c_str());
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void KickClientNum(int clientnum, std::string reason)
	{
		return VariadicCall<void>(OFF_KICKCLIENTNUM, clientnum, reason.c_str());
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void PlayerDie(sGEntity* target, sGEntity* inflictor, sGEntity* attacker, int method, int weapon, bool alternate, const ImVec3 direction)
	{
		return VariadicCall<void>(OFF_PLAYERDIE, target, inflictor, attacker, 100000, method, weapon, alternate, direction, 0, 0);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void PlayerKill(sGEntity* target, sGEntity* inflictor, sGEntity* attacker, int method, int weapon, bool alternate, const ImVec3 direction)
	{
		return VariadicCall<void>(OFF_PLAYERKILL, target, inflictor, attacker, 100000, method, weapon, alternate, direction, 0, 0, 0);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void Damage(sGEntity* target, sGEntity* inflictor, sGEntity* attacker, const ImVec3 direction, const ImVec3 position, int method, int weapon, bool alternate)
	{
		return VariadicCall<void>(OFF_DAMAGE, target, inflictor, attacker, direction, position, 100000, 0, method, weapon, alternate, 0, 0, 0, 0);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool EntityHasRiotShield(sCEntity* entity)
	{
		return ((BYTE)entity->NextEntityState.iWeapon == WEAPON_RIOT_SHIELD || (BYTE)entity->NextEntityState.LerpEntityState.iSecondaryWeapon == WEAPON_RIOT_SHIELD);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool WeaponIsAkimbo(int weapon)
	{
		return (((BYTE)weapon == WEAPON_M9A1 || (BYTE)weapon == WEAPON_44_MAGNUM || (BYTE)weapon == WEAPON_MP_443_GRACH || (BYTE)weapon == WEAPON_P226) && weapon & WF_AKIMBO);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool WeaponIsVehicle(int weapon)
	{
		return ((BYTE)weapon == WEAPON_HELO_PILOT || (BYTE)weapon == WEAPON_LOKI || (BYTE)weapon == WEAPON_ODIN || (BYTE)weapon == WEAPON_TRINITY_ROCKET || (BYTE)weapon == WEAPON_GRYPHON);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE bool HasPerk(int clientnum, ePerk perk)
	{
		return (*(std::uintptr_t*)&CharacterInformation[clientnum].iPerks >> perk) & 0x1;
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void EnablePerk(int clientnum, ePerk perk)
	{
		*(int*)((std::uintptr_t)&PlayerState[clientnum] + 0x4 * (perk >> 0x5) + 0xE14) |= 0x1 << (perk & 0x1F);
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE void DisablePerk(int clientnum, ePerk perk)
	{
		*(int*)((std::uintptr_t)&PlayerState[clientnum] + 0x4 * (perk >> 0x5) + 0xE14) &= ~(0x1 << (perk & 0x1F));
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE ImVec3 GetViewOrigin()
	{
		ImVec3 vViewOrigin;
		GetPlayerViewOrigin(&CG->PredictedPlayerState, &vViewOrigin);
		return WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? RefDef->vViewOrigin : vViewOrigin;
	}
	/*
	//=====================================================================================
	*/
	FORCEINLINE ImVec3 GetViewAngles()
	{
		return WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? CG->vRefDefViewAngles : IsThirdPersonMode(&CG->PredictedPlayerState) ? CG->vThirdPersonViewAngles : CG->vWeaponAngles;
	}
	/*
	//=====================================================================================
	*/
	struct sTimer
	{
	private:

		int iTick = 0, iWait = 0;

	public:

		bool Ready()
		{
			return ((Sys_Milliseconds() - iTick) >= iWait);
		};

		void Wait(int timeout)
		{
			iTick = Sys_Milliseconds();
			iWait = timeout;
		};
	};
}

//=====================================================================================