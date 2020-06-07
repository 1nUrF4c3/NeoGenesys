//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cTargetList
	{
	public:

		typedef enum
		{
			BONESCAN_OFF,
			BONESCAN_ONTIMER,
			BONESCAN_IMMEDIATE,
			BONESCAN_MAX
		} eBoneScan;

		typedef enum
		{
			SORT_METHOD_FOV,
			SORT_METHOD_DISTANCE,
			SORT_METHOD_MAX
		} eSortMethod;

		typedef enum
		{
			RIOTSHIELD_OFF,
			RIOTSHIELD_IGNOREPLAYER,
			RIOTSHIELD_TARGETFEET,
			RIOTSHIELD_MAX
		} eRiotShield;

		std::shared_ptr<sCvar> gAutoWall = std::make_shared<sCvar>("Autowall", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gTargetAgents = std::make_shared<sCvar>("Target Agents", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gTargetMissiles = std::make_shared<sCvar>("Target Missiles", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gBoneScan = std::make_shared<sCvar>("Bonescan", std::vector<std::string>({ "Off", "On Timer", "Immediate" }), BONESCAN_OFF, BONESCAN_OFF, BONESCAN_MAX);
		std::shared_ptr<sCvar> gSortMethod = std::make_shared<sCvar>("Sort Method", std::vector<std::string>({ "Field of View", "Distance" }), SORT_METHOD_FOV, SORT_METHOD_FOV, SORT_METHOD_MAX);
		std::shared_ptr<sCvar> gRiotShielders = std::make_shared<sCvar>("Riotshielders", std::vector<std::string>({ "Off", "Ignore Player", "Target Feet" }), RIOTSHIELD_OFF, RIOTSHIELD_OFF, RIOTSHIELD_MAX);
		std::shared_ptr<sCvar> gAimBone = std::make_shared<sCvar>("Aimbone", std::vector<std::string>(), BONE_HELMET, BONE_HELMET, BONE_MAX - 1);
		std::shared_ptr<sCvar> gAimAngle = std::make_shared<sCvar>("Aimangle", std::vector<std::string>(), 180, 1, 180);

		typedef struct
		{
			bool bIsPriority;
			int iIndex;
			float flFOV = FLT_MAX, flDistance = FLT_MAX;
		} sTargetInfo;

		typedef struct
		{
			int iIndex;
			float flFOV = FLT_MAX, flDistance = FLT_MAX;
		} sAntiAimTargetInfo;

		typedef struct
		{
			float flDamage;
			eBone iBoneIndex;
		} sDamageInfo;

		struct sEntityList
		{
			bool bIsValid, bW2SSuccess, bAimFeet, bIsVisible;
			eBone iBoneIndex, iLastBone;
			std::string szWeapon;
			ImVec2 vBones2D[BONE_MAX], vCorners2D[8], vCenter2D, vPosition, vDimentions, vLower, vUpper;
			ImVec3 vBones3D[BONE_MAX], vCorners3D[8], vCenter3D, vHitLocation;
			ImVec4 cColor;
		} EntityList[MAX_ENTITIES];

		bool bIsPriority[MAX_CLIENTS] = { false };

		void GetInformation();
		bool EntityIsValid(int index);
		bool EntityIsEnemy(int index);
		bool IsVisibleInternal(sCEntity* entity, ImVec3 position, eHitLocation hitloc, bool autowall, float* damage);
		bool IsVisible(sCEntity* entity, ImVec3 bones3d[BONE_MAX], bool bonescan, bool autowall, eBone& index);
		void ApplyPrediction(sCEntity* entity, ImVec3& position);
	} extern _targetList;
}

//=====================================================================================
