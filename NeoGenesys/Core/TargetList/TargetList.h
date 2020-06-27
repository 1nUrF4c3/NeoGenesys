//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cTargetList
	{
	public:

		enum eBoneScan
		{
			BONESCAN_OFF,
			BONESCAN_ONTIMER,
			BONESCAN_IMMEDIATE,
			BONESCAN_MAX
		};

		enum eSortMethod
		{
			SORT_METHOD_DISTANCE,
			SORT_METHOD_DAMAGE,
			SORT_METHOD_FOV,
			SORT_METHOD_MAX
		};

		enum eRiotShield
		{
			RIOTSHIELD_OFF,
			RIOTSHIELD_IGNOREPLAYER,
			RIOTSHIELD_TARGETFEET,
			RIOTSHIELD_MAX
		};

		std::shared_ptr<sCvar> gAutoWall = std::make_shared<sCvar>("Autowall", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gTargetAgents = std::make_shared<sCvar>("Target Agents", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gTargetMissiles = std::make_shared<sCvar>("Target Missiles", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gBoneScan = std::make_shared<sCvar>("Bonescan", std::vector<std::string>({ "Off", "On Timer", "Immediate" }), BONESCAN_OFF, BONESCAN_OFF, BONESCAN_MAX);
		std::shared_ptr<sCvar> gSortMethod = std::make_shared<sCvar>("Sort Method", std::vector<std::string>({ "Distance", "Damage", "Field of View" }), SORT_METHOD_DISTANCE, SORT_METHOD_DISTANCE, SORT_METHOD_MAX);
		std::shared_ptr<sCvar> gRiotShielders = std::make_shared<sCvar>("Riotshielders", std::vector<std::string>({ "Off", "Ignore Player", "Target Feet" }), RIOTSHIELD_OFF, RIOTSHIELD_OFF, RIOTSHIELD_MAX);
		std::shared_ptr<sCvar> gAimBone = std::make_shared<sCvar>("Aimbone", std::vector<std::string>(), BONE_HELMET, BONE_HELMET, BONE_MAX - 1);
		std::shared_ptr<sCvar> gAimAngle = std::make_shared<sCvar>("Aimangle", std::vector<std::string>(), 180, 1, 180);

		struct sTargetInfo
		{
			bool bIsPriority;
			int iIndex;
			float flDistance = FLT_MAX, flDamage = FLT_MAX, flFOV = FLT_MAX;
		};

		struct sAntiAimTargetInfo
		{
			int iIndex;
			float flDistance = FLT_MAX, flDamage = FLT_MAX, flFOV = FLT_MAX;
		};

		struct sDamageInfo
		{
			float flDamage;
			eBone iBoneIndex;
		};

		struct sEntityList
		{
			bool bIsValid, bW2SSuccess, bAimFeet, bIsVisible;
			eBone iBoneIndex, iLastBone;
			std::string szWeapon;
			float flDamage;
			ImVec2 vBones2D[BONE_MAX], vCorners2D[8], vCenter2D, vPosition, vDimentions, vLower, vUpper;
			ImVec3 vBones3D[BONE_MAX], vCorners3D[8], vCenter3D, vHitLocation;
			ImVec4 cColor;
		} EntityList[MAX_ENTITIES];

		struct sPriorities
		{
			bool bIsPrioritized, bIsIgnored;
		} Priorities[MAX_CLIENTS];

		void GetInformation();
		bool EntityIsValid(int index);
		bool EntityIsEnemy(int index);
		bool IsVisibleInternal(sCEntity* entity, ImVec3 position, eHitLocation hitloc, bool autowall, float* damage);
		bool IsVisible(sCEntity* entity, ImVec3 bones3d[BONE_MAX], bool bonescan, bool autowall, eBone& index, float* damage);
	} extern _targetList;
}

//=====================================================================================
