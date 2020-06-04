//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cAntiAim _antiAim;

	void cAntiAim::AntiAim(sUserCmd* usercmd)
	{
		if (_profiler.gAntiAim->Current.iValue == cProfiler::ANTIAIM_SPINBOT)
		{
			static float flAngle = 0.0f;

			if (flAngle > 360.0f)
				flAngle -= 360.0f;

			vAntiAimAngles[0] = 85.0f - CG->PredictedPlayerState.vDeltaAngles[0];
			vAntiAimAngles[1] = flAngle - CG->PredictedPlayerState.vDeltaAngles[1];
			vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);

			flAngle += 45.0f;
		}

		else if (_profiler.gAntiAim->Current.iValue == cProfiler::ANTIAIM_JITTERBOT)
		{
			static int iMode = 1;

			switch (iMode)
			{
			case 1:
				vAntiAimAngles[0] = -85.0f - CG->PredictedPlayerState.vDeltaAngles[0];
				vAntiAimAngles[1] = 0.0f - CG->PredictedPlayerState.vDeltaAngles[1];
				vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);
				iMode = 2;
				break;

			case 2:
				vAntiAimAngles[0] = 85.0f - CG->PredictedPlayerState.vDeltaAngles[0];
				vAntiAimAngles[1] = 90.0f - CG->PredictedPlayerState.vDeltaAngles[1];
				vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);
				iMode = 3;
				break;

			case 3:
				vAntiAimAngles[0] = -85.0f - CG->PredictedPlayerState.vDeltaAngles[0];
				vAntiAimAngles[1] = 180.0f - CG->PredictedPlayerState.vDeltaAngles[1];
				vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);
				iMode = 4;
				break;

			case 4:
				vAntiAimAngles[0] = 85.0f - CG->PredictedPlayerState.vDeltaAngles[0];
				vAntiAimAngles[1] = 270.0f - CG->PredictedPlayerState.vDeltaAngles[1];
				vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);
				iMode = 1;
				break;
			}
		}

		else if (_profiler.gAntiAim->Current.iValue == cProfiler::ANTIAIM_RANDOMIZED)
		{
			std::random_device Device;
			std::uniform_real_distribution<float> RandomPitch(-85.0f, 85.0f), RandomYaw(0.0f, 360.0f);

			vAntiAimAngles[0] = RandomPitch(Device) - CG->PredictedPlayerState.vDeltaAngles[0];
			vAntiAimAngles[1] = RandomYaw(Device) - CG->PredictedPlayerState.vDeltaAngles[1];
			vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);
		}

		else if (_profiler.gAntiAim->Current.iValue == cProfiler::ANTIAIM_REVERSED)
		{
			if (_targetList.vIsTarget[_targetList.iRiotShieldTarget] && _targetList.iRiotShieldTarget != CG->PredictedPlayerState.iClientNum)
			{
				_mathematics.CalculateAntiAimAngles(CEntity[_targetList.iRiotShieldTarget].vOrigin, CG->PredictedPlayerState.vOrigin, _targetList.vRiotShieldAimAngles);

				vAntiAimAngles[0] = _targetList.vRiotShieldAimAngles[0];
				vAntiAimAngles[1] = _targetList.vRiotShieldAimAngles[1];
				vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);
			}

			else
			{
				if (_aimBot.AimState.bIsAutoAiming)
				{
					vAntiAimAngles[0] = _aimBot.AimState.vAntiAimAngles[0];
					vAntiAimAngles[1] = _aimBot.AimState.vAntiAimAngles[1];
					vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);
				}

				else
				{
					vAntiAimAngles[0] = -40.0f - CG->vRefDefViewAngles[0];

					if (vAntiAimAngles[0] > 85.0f) vAntiAimAngles[0] = 85.0f;
					if (vAntiAimAngles[0] < -85.0f) vAntiAimAngles[0] = -85.0f;

					vAntiAimAngles[0] -= CG->PredictedPlayerState.vDeltaAngles[0];

					vAntiAimAngles[1] = CG->vRefDefViewAngles[1] - 170.0f;

					while (vAntiAimAngles[1] > 180.0f) vAntiAimAngles[1] -= 360.0f;
					while (vAntiAimAngles[1] < -180.0f) vAntiAimAngles[1] += 360.0f;

					vAntiAimAngles[1] -= CG->PredictedPlayerState.vDeltaAngles[1];

					vAntiAimAngles[2] = ShortToAngle(usercmd->iViewAngles[2]);
				}
			}
		}

		else if (_profiler.gAntiAim->Current.iValue == cProfiler::ANTIAIM_INVERTED)
		{
			vAntiAimAngles[0] = ShortToAngle(usercmd->iViewAngles[0]);
			vAntiAimAngles[1] = ShortToAngle(usercmd->iViewAngles[1]);
			vAntiAimAngles[2] = -180 - CG->PredictedPlayerState.vDeltaAngles[2];
		}

		if (IsAntiAiming())
		{
			usercmd->iViewAngles[0] = AngleToShort(vAntiAimAngles[0]);
			usercmd->iViewAngles[1] = AngleToShort(vAntiAimAngles[1]);
			usercmd->iViewAngles[2] = AngleToShort(vAntiAimAngles[2]);

			ImVec3 vViewOrigin, vDirection, vAngles, vOneVector = { 1.0f, 1.0f, 1.0f };

			GetPlayerViewOrigin(&CG->PredictedPlayerState, &vViewOrigin);
			VectorSubtract(vOneVector, vViewOrigin, vDirection);

			_mathematics.VectorNormalize(vDirection);
			_mathematics.VectorAngles(vDirection, vAngles);
			_mathematics.ClampAngles(vAngles);

			ViewMatrix->vViewAngles[2] = vAngles[2] - CG->PredictedPlayerState.vDeltaAngles[2];
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
			(_profiler.gAntiAim->Current.iValue > cProfiler::ANTIAIM_OFF));
	}
}

//=====================================================================================