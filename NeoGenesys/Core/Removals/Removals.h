//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cRemovals
	{
	public:

		std::shared_ptr<sCvar> gRecoilFactor = std::make_shared<sCvar>("Recoil Factor", std::vector<std::string>(), 1.0f, 0.0f, 1.0f);
		std::shared_ptr<sCvar> gSpreadFactor = std::make_shared<sCvar>("Spread Factor", std::vector<std::string>(), 1.0f, 0.0f, 1.0f);

		void RecoilCompensation();
		void SpreadCompensationStandardAim(sUserCmd* usercmd, bool akimbo);
		void SpreadCompensationSilentAim(sUserCmd* usercmd, bool akimbo);
		void GetSpreadAngles(bool akimbo, int servertime, float spread, ImVec3& angles);
		void GetRandomFloats(int* seed, float* spreadx, float* spready);
		int TransformSeed(bool akimbo, int servertime);
		float GetWeaponSpread();
		void FirstBulletFix();
	} extern _removals;
}

//=====================================================================================