//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cAimbot _aimBot;

	void cAimbot::StandardAim()
	{
		if (!gSilentAim->Current.bValue && AimState.bTargetAcquired)
		{
			AimState.vAimAngles *= gAimPower->Current.iValue / 100.0f;

			if (gAutoAimTime->Current.iValue)
			{
				AimState.vAimAngles *= (float)AimState.iCurrentAimTime / (float)gAutoAimTime->Current.iValue;
			}

			if (AimState.iCurrentAimDelay == gAutoAimDelay->Current.iValue)
			{
				if (AimState.bLockonTarget)
				{
					ViewMatrix->vViewAngles += AimState.vAimAngles;
				}
			}

			if (AimState.iCurrentZoomDelay == gAutoZoomDelay->Current.iValue)
				if (gAutoZoom->Current.bValue && gAimBotMode->Current.iValue == AIMBOT_MODE_AUTO)
					SetZoomState(true);
		}
	}
	/*
	//=====================================================================================
	*/
	void cAimbot::SilentAim(sUserCmd* usercmd)
	{
		if (gSilentAim->Current.bValue && AimState.bTargetAcquired)
		{
			if (AimState.bLockonTarget)
			{
				float flOldYaw = ShortToAngle(usercmd->iViewAngles[1]);

				usercmd->iViewAngles[0] += AngleToShort(AimState.vAimAngles.x);
				usercmd->iViewAngles[1] += AngleToShort(AimState.vAimAngles.y);

				_mathematics.MovementFix(usercmd, ShortToAngle(usercmd->iViewAngles[1]), flOldYaw, usercmd->szForwardMove, usercmd->szRightMove);
			}

			if (AimState.iCurrentZoomDelay == gAutoZoomDelay->Current.iValue)
				if (gAutoZoom->Current.bValue && gAimBotMode->Current.iValue == AIMBOT_MODE_AUTO)
					SetZoomState(true);
		}
	}
	/*
	//=====================================================================================
	*/
	void cAimbot::AutoFire(sUserCmd* usercmd)
	{
		if (gAutoFire->Current.bValue && AimState.bTargetAcquired)
		{
			if (AimState.iCurrentFireDelay == gAutoFireDelay->Current.iValue)
			{
				if (AimState.bLockonTarget)
				{
					if (WeaponIsDualWield(GetViewmodelWeapon(&CG->PredictedPlayerState)))
					{
						if (AimState.bAkimbo)
						{
							if (IsGamePadEnabled())
							{
								usercmd->iButtons |= BUTTON_FIRERIGHT;
							}

							else
							{
								usercmd->iButtons |= BUTTON_FIRELEFT;
							}
						}

						else
						{
							if (IsGamePadEnabled())
							{
								usercmd->iButtons |= BUTTON_FIRELEFT;
							}

							else
							{
								usercmd->iButtons |= BUTTON_FIRERIGHT;
							}
						}

						AimState.bAkimbo = !AimState.bAkimbo;
					}

					else
					{
						usercmd->iButtons |= BUTTON_FIRELEFT;
					}
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
		AimState.bIsZooming = (CEntity[CG->PredictedPlayerState.iClientNum].NextEntityState.LerpEntityState.iEntityFlags & EF_ZOOM);
		AimState.bLockonTarget = (gAimBotMode->Current.iValue == AIMBOT_MODE_AUTO || (gAimBotMode->Current.iValue == AIMBOT_MODE_MANUAL && AimState.bIsZooming));
		AimState.bIsAutoAiming = (AimState.bTargetAcquired && AimState.bLockonTarget);
		AimState.bIsAutoFiring = (gAutoFire->Current.bValue && AimState.bIsAutoAiming);

		if (AimState.bLockonTarget)
		{
			if (AimState.iCurrentAimDelay == gAutoAimDelay->Current.iValue)
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

		if (!AimState.bTargetAcquired || _packets.bIsAirStuck)
			AimState.iCurrentAimDelay = AimState.iCurrentZoomDelay = AimState.iCurrentFireDelay = 0;

		if (AimState.iCurrentAimTime > gAutoAimTime->Current.iValue)
			AimState.iCurrentAimTime = gAutoAimTime->Current.iValue;

		if (AimState.iCurrentAimDelay > gAutoAimDelay->Current.iValue)
			AimState.iCurrentAimDelay = gAutoAimDelay->Current.iValue;

		if (AimState.iCurrentZoomDelay > gAutoZoomDelay->Current.iValue)
			AimState.iCurrentZoomDelay = gAutoZoomDelay->Current.iValue;

		if (AimState.iCurrentFireDelay > gAutoFireDelay->Current.iValue)
			AimState.iCurrentFireDelay = gAutoFireDelay->Current.iValue;

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
	}
}

//=====================================================================================