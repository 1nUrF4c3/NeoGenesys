//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cAntiAim _antiAim;

	void cAntiAim::AntiAim(sUserCmd* usercmd)
	{
		if (gAntiAim->Custom.iValue == ANTIAIM_SPINBOT)
		{
			static float flAngle = 0.0f;

			if (flAngle > 360.0f)
				flAngle -= 360.0f;

			vAntiAimAngles.x = 85.0f - CG->PredictedPlayerState.vDeltaAngles.x;
			vAntiAimAngles.y = flAngle - CG->PredictedPlayerState.vDeltaAngles.y;
			vAntiAimAngles.z = ShortToAngle(usercmd->iViewAngles[2]);

			flAngle += 45.0f;
		}

		else if (gAntiAim->Custom.iValue == ANTIAIM_JITTERBOT)
		{
			static int iMode = 1;

			switch (iMode)
			{
			case 1:
				vAntiAimAngles.x = -85.0f - CG->PredictedPlayerState.vDeltaAngles.x;
				vAntiAimAngles.y = 0.0f - CG->PredictedPlayerState.vDeltaAngles.y;
				vAntiAimAngles.z = ShortToAngle(usercmd->iViewAngles[2]);
				iMode = 2;
				break;

			case 2:
				vAntiAimAngles.x = 85.0f - CG->PredictedPlayerState.vDeltaAngles.x;
				vAntiAimAngles.y = 90.0f - CG->PredictedPlayerState.vDeltaAngles.y;
				vAntiAimAngles.z = ShortToAngle(usercmd->iViewAngles[2]);
				iMode = 3;
				break;

			case 3:
				vAntiAimAngles.x = -85.0f - CG->PredictedPlayerState.vDeltaAngles.x;
				vAntiAimAngles.y = 180.0f - CG->PredictedPlayerState.vDeltaAngles.y;
				vAntiAimAngles.z = ShortToAngle(usercmd->iViewAngles[2]);
				iMode = 4;
				break;

			case 4:
				vAntiAimAngles.x = 85.0f - CG->PredictedPlayerState.vDeltaAngles.x;
				vAntiAimAngles.y = 270.0f - CG->PredictedPlayerState.vDeltaAngles.y;
				vAntiAimAngles.z = ShortToAngle(usercmd->iViewAngles[2]);
				iMode = 1;
				break;
			}
		}

		else if (gAntiAim->Custom.iValue == ANTIAIM_RANDOMIZED)
		{
			std::random_device Device;
			std::uniform_real_distribution<float> RandomPitch(-85.0f, 85.0f), RandomYaw(0.0f, 360.0f);

			vAntiAimAngles.x = RandomPitch(Device) - CG->PredictedPlayerState.vDeltaAngles.x;
			vAntiAimAngles.y = RandomYaw(Device) - CG->PredictedPlayerState.vDeltaAngles.y;
			vAntiAimAngles.z = ShortToAngle(usercmd->iViewAngles[2]);
		}

		else if (gAntiAim->Custom.iValue == ANTIAIM_REVERSED)
		{

			if (_aimBot.AimState.bAntiAimTargetAcquired || _aimBot.AimState.bIsAutoAiming)
			{
				vAntiAimAngles.x = _aimBot.AimState.vAntiAimAngles.x;
				vAntiAimAngles.y = _aimBot.AimState.vAntiAimAngles.y;
				vAntiAimAngles.z = ShortToAngle(usercmd->iViewAngles[2]);
			}

			else
			{
				vAntiAimAngles.x = -40.0f - CG->vRefDefViewAngles.x;

				if (vAntiAimAngles.x > 85.0f) vAntiAimAngles.x = 85.0f;
				if (vAntiAimAngles.x < -85.0f) vAntiAimAngles.x = -85.0f;

				vAntiAimAngles.x -= CG->PredictedPlayerState.vDeltaAngles.x;
				vAntiAimAngles.y = CG->vRefDefViewAngles.y - 180.0f;

				while (vAntiAimAngles.y > 180.0f) vAntiAimAngles.y -= 360.0f;
				while (vAntiAimAngles.y < -180.0f) vAntiAimAngles.y += 360.0f;

				vAntiAimAngles.y -= CG->PredictedPlayerState.vDeltaAngles.y;
				vAntiAimAngles.z = ShortToAngle(usercmd->iViewAngles[2]);
			}
		}

		else if (gAntiAim->Custom.iValue == ANTIAIM_INVERTED)
		{
			vAntiAimAngles.x = ShortToAngle(usercmd->iViewAngles[0]);
			vAntiAimAngles.y = ShortToAngle(usercmd->iViewAngles[1]);
			vAntiAimAngles.z = -180 - CG->PredictedPlayerState.vDeltaAngles.z;
		}

		if (IsAntiAiming())
		{
			usercmd->iViewAngles[0] = AngleToShort(vAntiAimAngles.x);
			usercmd->iViewAngles[1] = AngleToShort(vAntiAimAngles.y);
			usercmd->iViewAngles[2] = AngleToShort(vAntiAimAngles.z);

			ImVec3 vViewOrigin, vDirection, vAngles, vOneVector = { 1.0f, 1.0f, 1.0f };

			GetPlayerViewOrigin(&CG->PredictedPlayerState, &vViewOrigin);
			vDirection = vOneVector - vViewOrigin;

			_mathematics.VectorNormalize(vDirection);
			_mathematics.VectorAngles(vDirection, vAngles);
			_mathematics.ClampAngles(vAngles);

			ViewMatrix->vViewAngles.z = vAngles.z - CG->PredictedPlayerState.vDeltaAngles.z;
		}
	}
	/*
	//=====================================================================================
	*/
	bool cAntiAim::IsAntiAiming()
	{
		return (GetViewmodelWeapon(&CG->PredictedPlayerState) && !WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) &&
			!(CEntity[CG->PredictedPlayerState.iClientNum].NextEntityState.LerpEntityState.iEntityFlags & EF_PRONE) &&
			!(CEntity[CG->PredictedPlayerState.iClientNum].NextEntityState.LerpEntityState.iEntityFlags & EF_MANTLE) &&
			(gAntiAim->Custom.iValue > ANTIAIM_OFF));
	}
}

//=====================================================================================