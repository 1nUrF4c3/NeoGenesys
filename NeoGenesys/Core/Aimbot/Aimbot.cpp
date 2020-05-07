//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cAimbot _aimBot;

	void cAimbot::StandardAim()
	{
		if ((!_profiler.gSilentAim->Current.bValue || WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState))) && AimState.bTargetAcquired)
		{
			AimState.vAimAngles[0] *= _profiler.gAimPower->Current.iValue / 100.0f;
			AimState.vAimAngles[1] *= _profiler.gAimPower->Current.iValue / 100.0f;

			if (_profiler.gAutoAimTime->Current.iValue)
			{
				AimState.vAimAngles[0] *= (float)AimState.iCurrentAimTime / (float)_profiler.gAutoAimTime->Current.iValue;
				AimState.vAimAngles[1] *= (float)AimState.iCurrentAimTime / (float)_profiler.gAutoAimTime->Current.iValue;
			}

			if (AimState.iCurrentAimDelay == _profiler.gAutoAimDelay->Current.iValue)
			{
				if (AimState.bLockonTarget)
				{
					ViewMatrix->vViewAngles[0] += AimState.vAimAngles[0];
					ViewMatrix->vViewAngles[1] += AimState.vAimAngles[1];
				}
			}

			if (AimState.iCurrentZoomDelay == _profiler.gAutoZoomDelay->Current.iValue)
				if (_profiler.gAutoZoom->Current.bValue && _profiler.gAimBotMode->Current.iValue == cProfiler::AIMBOT_MODE_AUTO)
					SetZoomState(true);
		}
	}
	/*
	//=====================================================================================
	*/
	void cAimbot::SilentAim(sUserCmd* usercmd)
	{
		if (_profiler.gSilentAim->Current.bValue && !WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) && AimState.bTargetAcquired)
		{
			if (AimState.bLockonTarget)
			{
				float flOldYaw = ShortToAngle(usercmd->iViewAngles[1]);

				usercmd->iViewAngles[0] += AngleToShort(AimState.vAimAngles[0]);
				usercmd->iViewAngles[1] += AngleToShort(AimState.vAimAngles[1]);

				_mathematics.MovementFix(usercmd, ShortToAngle(usercmd->iViewAngles[1]), flOldYaw, usercmd->szMove[0], usercmd->szMove[1]);
			}

			if (AimState.iCurrentZoomDelay == _profiler.gAutoZoomDelay->Current.iValue)
				if (_profiler.gAutoZoom->Current.bValue && _profiler.gAimBotMode->Current.iValue == cProfiler::AIMBOT_MODE_AUTO)
					SetZoomState(true);
		}
	}
	/*
	//=====================================================================================
	*/
	void cAimbot::AutoFire(sUserCmd* usercmd)
	{
		if (_profiler.gAutoFire->Current.bValue && AimState.bTargetAcquired)
		{
			if (AimState.iCurrentFireDelay == _profiler.gAutoFireDelay->Current.iValue)
			{
				if (AimState.bLockonTarget)
				{
					if (WeaponIsAkimbo(GetViewmodelWeapon(&CG->PredictedPlayerState)))
					{
						if (AimState.bAkimbo)
							usercmd->iButtons |= (IsGamePadEnabled() ? BUTTON_FIRERIGHT : BUTTON_FIRELEFT);

						else
							usercmd->iButtons |= (IsGamePadEnabled() ? BUTTON_FIRELEFT : BUTTON_FIRERIGHT);
					}

					else
						usercmd->iButtons |= BUTTON_FIRELEFT;
				}
			}
		}
	}
	/*
	//=====================================================================================
	*/
	void cAimbot::SetAimState()
	{

		AimState.bTargetAcquired = (AimState.iTargetNum > -1);
		AimState.bLockonTarget = (_profiler.gAimBotMode->Current.iValue == cProfiler::AIMBOT_MODE_AUTO || (_profiler.gAimBotMode->Current.iValue == cProfiler::AIMBOT_MODE_MANUAL && CEntity[CG->PredictedPlayerState.iClientNum].NextEntityState.LerpEntityState.iEntityFlags & EF_ZOOM));
		AimState.bIsAutoAiming = (AimState.bTargetAcquired && AimState.bLockonTarget);
		AimState.bIsAutoFiring = (_profiler.gAutoFire->Current.bValue && AimState.bIsAutoAiming);

		if (AimState.bLockonTarget)
		{
			if (AimState.iCurrentAimDelay == _profiler.gAutoAimDelay->Current.iValue)
				AimState.iCurrentAimTime += clock() - AimState.iDeltaTMR;

			AimState.iCurrentAimDelay += clock() - AimState.iDeltaTMR;
			AimState.iCurrentZoomDelay += clock() - AimState.iDeltaTMR;
			AimState.iCurrentFireDelay += clock() - AimState.iDeltaTMR;
		}

		AimState.iDeltaTMR = clock();

		if (AimState.iLastTarget != AimState.iTargetNum)
		{
			AimState.iLastTarget = AimState.iTargetNum;
			AimState.iCurrentAimTime = 0;
		}

		if (_targetList.EntityList[AimState.iTargetNum].iLastBone != _targetList.EntityList[AimState.iTargetNum].iBoneIndex)
		{
			_targetList.EntityList[AimState.iTargetNum].iLastBone = _targetList.EntityList[AimState.iTargetNum].iBoneIndex;
			AimState.iCurrentAimTime = 0;
		}

		if (!AimState.bTargetAcquired)
			AimState.iCurrentAimDelay = AimState.iCurrentZoomDelay = AimState.iCurrentFireDelay = 0;

		if (AimState.iCurrentAimTime > _profiler.gAutoAimTime->Current.iValue)
			AimState.iCurrentAimTime = _profiler.gAutoAimTime->Current.iValue;

		if (AimState.iCurrentAimDelay > _profiler.gAutoAimDelay->Current.iValue)
			AimState.iCurrentAimDelay = _profiler.gAutoAimDelay->Current.iValue;

		if (AimState.iCurrentZoomDelay > _profiler.gAutoZoomDelay->Current.iValue)
			AimState.iCurrentZoomDelay = _profiler.gAutoZoomDelay->Current.iValue;

		if (AimState.iCurrentFireDelay > _profiler.gAutoFireDelay->Current.iValue)
			AimState.iCurrentFireDelay = _profiler.gAutoFireDelay->Current.iValue;

		if (AimState.bTargetAcquired)
		{
			Vector3 vViewOrigin;
			GetPlayerViewOrigin(&CG->PredictedPlayerState, vViewOrigin);

			VectorCopy(_targetList.EntityList[AimState.iTargetNum].vHitLocation, AimState.vAimPosition);

			_mathematics.CalculateAimAngles(AimState.vAimPosition, WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? RefDef->vViewOrigin : vViewOrigin, AimState.vAimAngles);
			_mathematics.CalculateAntiAimAngles(AimState.vAimPosition, WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? RefDef->vViewOrigin : vViewOrigin, AimState.vAntiAimAngles);
		}

		AimState.iFireTMR++;

		if (WeaponIsAkimbo(GetViewmodelWeapon(&CG->PredictedPlayerState)))
		{
			if (!(AimState.iFireTMR % ((BYTE)GetViewmodelWeapon(&CG->PredictedPlayerState) == WEAPON_44_MAGNUM ? 12 : 6)))
				AimState.bAkimbo = !AimState.bAkimbo;
		}

		else
			AimState.bAkimbo = false;
	}
}

//=====================================================================================