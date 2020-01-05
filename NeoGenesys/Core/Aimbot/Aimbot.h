//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cAimbot
	{
	public:

		struct sAimState
		{
			bool bAkimbo, bTargetAcquired, bLockonTarget, bIsAutoAiming, bIsAutoFiring;
			int iFireTMR, iCurrentAimTime, iCurrentAimDelay, iCurrentZoomDelay, iCurrentFireDelay, iDeltaTMR, iTargetNum, iLastTarget;
			Vector3 vAimbotPosition, vAimbotAngles;
		} AimState;

		void StandardAim();
		void SilentAim(sUserCMD* usercmd);
		void AutoFire(sUserCMD* usercmd);
	} extern _aimBot;
}

//=====================================================================================
