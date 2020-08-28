//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cAntiAim _antiAim;

	void cAntiAim::AntiAimPitch(sUserCmd* usercmd)
	{
		if (gAntiAim->Current.iValue == ANTIAIM_SPINBOT)
		{
			vAntiAimAngles.x = 85.0f - CG->PredictedPlayerState.vDeltaAngles.x;
		}

		else if (gAntiAim->Current.iValue == ANTIAIM_JITTERBOT)
		{
			static bool bMode = false;

			switch (bMode)
			{
			case false:
				vAntiAimAngles.x = -85.0f - CG->PredictedPlayerState.vDeltaAngles.x;
				bMode = true;
				break;

			case true:
				vAntiAimAngles.x = 85.0f - CG->PredictedPlayerState.vDeltaAngles.x;
				bMode = false;
				break;
			}
		}

		else if (gAntiAim->Current.iValue == ANTIAIM_RANDOMIZED)
		{
			std::random_device Device;
			std::uniform_real_distribution<float> RandomPitch(-85.0f, 85.0f);

			vAntiAimAngles.x = RandomPitch(Device) - CG->PredictedPlayerState.vDeltaAngles.x;
		}

		else if (gAntiAim->Current.iValue == ANTIAIM_REVERSED)
		{

			if (_aimBot.AimState.bTargetAcquired || _aimBot.AimState.bAntiAimTargetAcquired)
			{
				vAntiAimAngles.x = _aimBot.AimState.vAntiAimAngles.x;
			}

			else
			{
				vAntiAimAngles.x = -40.0f - GetViewAngles().x;

				if (vAntiAimAngles.x > 85.0f) vAntiAimAngles.x = 85.0f;
				if (vAntiAimAngles.x < -85.0f) vAntiAimAngles.x = -85.0f;

				vAntiAimAngles.x -= CG->PredictedPlayerState.vDeltaAngles.x;
			}
		}

		if (IsAntiAiming() && gAntiAim->Current.iValue > ANTIAIM_OFF && gAntiAim->Current.iValue < ANTIAIM_INVERTED)
		{
			usercmd->iViewAngles[0] = AngleToShort(vAntiAimAngles.x);
		}
	}
	/*
	//=====================================================================================
	*/
	void cAntiAim::AntiAimYaw(sUserCmd* usercmd)
	{
		if (gAntiAim->Current.iValue == ANTIAIM_SPINBOT)
		{
			static float flAngle = 0.0f;

			if (flAngle > 360.0f)
				flAngle -= 360.0f;

			vAntiAimAngles.y = flAngle - CG->PredictedPlayerState.vDeltaAngles.y;

			flAngle += 45.0f;
		}

		else if (gAntiAim->Current.iValue == ANTIAIM_JITTERBOT)
		{
			static int iMode = 1;

			switch (iMode)
			{
			case 1:
				vAntiAimAngles.y = 0.0f - CG->PredictedPlayerState.vDeltaAngles.y;
				iMode = 2;
				break;

			case 2:
				vAntiAimAngles.y = 90.0f - CG->PredictedPlayerState.vDeltaAngles.y;
				iMode = 3;
				break;

			case 3:
				vAntiAimAngles.y = 180.0f - CG->PredictedPlayerState.vDeltaAngles.y;
				iMode = 4;
				break;

			case 4:
				vAntiAimAngles.y = 270.0f - CG->PredictedPlayerState.vDeltaAngles.y;
				iMode = 1;
				break;
			}
		}

		else if (gAntiAim->Current.iValue == ANTIAIM_RANDOMIZED)
		{
			std::random_device Device;
			std::uniform_real_distribution<float> RandomYaw(0.0f, 360.0f);

			vAntiAimAngles.y = RandomYaw(Device) - CG->PredictedPlayerState.vDeltaAngles.y;
		}

		else if (gAntiAim->Current.iValue == ANTIAIM_REVERSED)
		{

			if (_aimBot.AimState.bTargetAcquired || _aimBot.AimState.bAntiAimTargetAcquired)
			{
				vAntiAimAngles.y = _aimBot.AimState.vAntiAimAngles.y;
			}

			else
			{
				vAntiAimAngles.y = GetViewAngles().y - 180.0f;

				while (vAntiAimAngles.y > 180.0f) vAntiAimAngles.y -= 360.0f;
				while (vAntiAimAngles.y < -180.0f) vAntiAimAngles.y += 360.0f;

				vAntiAimAngles.y -= CG->PredictedPlayerState.vDeltaAngles.y;
			}
		}

		if (IsAntiAiming() && gAntiAim->Current.iValue > ANTIAIM_OFF && gAntiAim->Current.iValue < ANTIAIM_INVERTED)
		{
			usercmd->iViewAngles[1] = AngleToShort(vAntiAimAngles.y);
		}
	}
	/*
	//=====================================================================================
	*/
	void cAntiAim::AntiAimRoll(sUserCmd* usercmd)
	{
		if (gAntiAim->Current.iValue == ANTIAIM_INVERTED)
		{
			vAntiAimAngles.z = -180 - CG->PredictedPlayerState.vDeltaAngles.z;
		}

		if (IsAntiAiming() && gAntiAim->Current.iValue == ANTIAIM_INVERTED)
		{
			usercmd->iViewAngles[2] = AngleToShort(vAntiAimAngles.z);

			ImVec3 vDirection, vAngles, vOneVector = { 1.0f, 1.0f, 1.0f };

			vDirection = vOneVector - GetViewOrigin();

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
			!(CEntity[CG->PredictedPlayerState.iClientNum].NextEntityState.LerpEntityState.iEntityFlags & EF_MANTLE));
	}
}

//=====================================================================================