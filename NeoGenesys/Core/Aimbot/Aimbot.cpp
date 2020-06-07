//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cAimbot _aimBot;

	void cAimbot::StandardAim()
	{
		if (!gSilentAim->Custom.bValue && AimState.bTargetAcquired)
		{
			AimState.vAimAngles *= gAimPower->Custom.iValue / 100.0f;

			if (gAutoAimTime->Custom.iValue)
			{
				AimState.vAimAngles *= (float)AimState.iCurrentAimTime / (float)gAutoAimTime->Custom.iValue;
			}

			if (AimState.iCurrentAimDelay == gAutoAimDelay->Custom.iValue)
			{
				if (AimState.bLockonTarget)
				{
					ViewMatrix->vViewAngles += AimState.vAimAngles;
				}
			}

			if (AimState.iCurrentZoomDelay == gAutoZoomDelay->Custom.iValue)
				if (gAutoZoom->Custom.bValue && gAimBotMode->Custom.iValue == AIMBOT_MODE_AUTO)
					SetZoomState(true);
		}
	}
	/*
	//=====================================================================================
	*/
	void cAimbot::SilentAim(sUserCmd* usercmd)
	{
		if (gSilentAim->Custom.bValue && AimState.bTargetAcquired)
		{
			if (AimState.bLockonTarget)
			{
				float flOldYaw = ShortToAngle(usercmd->iViewAngles[1]);

				usercmd->iViewAngles[0] += AngleToShort(AimState.vAimAngles.x);
				usercmd->iViewAngles[1] += AngleToShort(AimState.vAimAngles.y);

				_mathematics.MovementFix(usercmd, ShortToAngle(usercmd->iViewAngles[1]), flOldYaw, usercmd->szMove[0], usercmd->szMove[1]);
			}

			if (AimState.iCurrentZoomDelay == gAutoZoomDelay->Custom.iValue)
				if (gAutoZoom->Custom.bValue && gAimBotMode->Custom.iValue == AIMBOT_MODE_AUTO)
					SetZoomState(true);
		}
	}
	/*
	//=====================================================================================
	*/
	void cAimbot::AutoFire(sUserCmd* usercmd)
	{
		if (gAutoFire->Custom.bValue && AimState.bTargetAcquired)
		{
			if (AimState.iCurrentFireDelay == gAutoFireDelay->Custom.iValue)
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
		AimState.bAntiAimTargetAcquired = (AimState.iAntiAimTargetNum > -1);
		AimState.bLockonTarget = (gAimBotMode->Custom.iValue == AIMBOT_MODE_AUTO || (gAimBotMode->Custom.iValue == AIMBOT_MODE_MANUAL && CEntity[CG->PredictedPlayerState.iClientNum].NextEntityState.LerpEntityState.iEntityFlags & EF_ZOOM));
		AimState.bIsAutoAiming = (AimState.bTargetAcquired && AimState.bLockonTarget);
		AimState.bIsAutoFiring = (gAutoFire->Custom.bValue && AimState.bIsAutoAiming);

		if (AimState.bLockonTarget)
		{
			if (AimState.iCurrentAimDelay == gAutoAimDelay->Custom.iValue)
				AimState.iCurrentAimTime += Sys_Milliseconds() - AimState.iDeltaTMR;

			AimState.iCurrentAimDelay += Sys_Milliseconds() - AimState.iDeltaTMR;
			AimState.iCurrentZoomDelay += Sys_Milliseconds() - AimState.iDeltaTMR;
			AimState.iCurrentFireDelay += Sys_Milliseconds() - AimState.iDeltaTMR;
		}

		AimState.iDeltaTMR = Sys_Milliseconds();

		if (AimState.iLastTargetNum != AimState.iTargetNum)
		{
			AimState.iLastTargetNum = AimState.iTargetNum;
			AimState.iCurrentAimTime = 0;
		}

		if (_targetList.EntityList[AimState.iTargetNum].iLastBone != _targetList.EntityList[AimState.iTargetNum].iBoneIndex)
		{
			_targetList.EntityList[AimState.iTargetNum].iLastBone = _targetList.EntityList[AimState.iTargetNum].iBoneIndex;
			AimState.iCurrentAimTime = 0;
		}

		if (!AimState.bTargetAcquired)
			AimState.iCurrentAimDelay = AimState.iCurrentZoomDelay = AimState.iCurrentFireDelay = 0;

		if (AimState.iCurrentAimTime > gAutoAimTime->Custom.iValue)
			AimState.iCurrentAimTime = gAutoAimTime->Custom.iValue;

		if (AimState.iCurrentAimDelay > gAutoAimDelay->Custom.iValue)
			AimState.iCurrentAimDelay = gAutoAimDelay->Custom.iValue;

		if (AimState.iCurrentZoomDelay > gAutoZoomDelay->Custom.iValue)
			AimState.iCurrentZoomDelay = gAutoZoomDelay->Custom.iValue;

		if (AimState.iCurrentFireDelay > gAutoFireDelay->Custom.iValue)
			AimState.iCurrentFireDelay = gAutoFireDelay->Custom.iValue;

		if (AimState.bTargetAcquired)
		{
			ImVec3 vViewOrigin;
			GetPlayerViewOrigin(&CG->PredictedPlayerState, &vViewOrigin);

			AimState.vAimPosition = _targetList.EntityList[AimState.iTargetNum].vHitLocation;

			_mathematics.CalculateAimAngles(AimState.vAimPosition, WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? RefDef->vViewOrigin : vViewOrigin, AimState.vAimAngles);
			_mathematics.CalculateAntiAimAngles(CEntity[AimState.iTargetNum].vOrigin, CG->PredictedPlayerState.vOrigin, AimState.vAntiAimAngles);
		}

		if (AimState.bAntiAimTargetAcquired)
		{
			_mathematics.CalculateAntiAimAngles(CEntity[AimState.iAntiAimTargetNum].vOrigin, CG->PredictedPlayerState.vOrigin, AimState.vAntiAimAngles);
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