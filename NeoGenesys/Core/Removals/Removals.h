//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cRemovals
	{
	public:

		void RecoilCompensation();
		void SpreadCompensationStandardAim(sUserCMD* usercmd, bool akimbo);
		void SpreadCompensationSilentAim(sUserCMD* usercmd, bool akimbo);
		void GetSpreadAngles(bool akimbo, int servertime, float spread, Vector3 angles);
		void GetRandomFloats(int* seed, float* spreadx, float* spready);
		int TransformSeed(bool akimbo, int servertime);
		float GetWeaponSpread();
		void FirstBulletFix();
	} extern _removals;
}

//=====================================================================================