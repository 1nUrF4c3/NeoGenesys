//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cTargetList _targetList;

	void cTargetList::GetInformation()
	{
		sTargetInfo TargetInfo;
		std::vector<sTargetInfo> vTargetInfo;

		_aimBot.AimState.iTargetNum = -1;

		bool bW2SSuccess = false;
		static int iCounter = 0;
		int iBonescanNum = iCounter % FindVariable("sv_maxclients")->Current.iValue;

		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			EntityList[i].bIsValid = false;
			EntityList[i].bAimFeet = false;

			if (!EntityIsValid(i))
				continue;

			if (CEntity[i].NextEntityState.iEntityType == ET_PLAYER || CEntity[i].NextEntityState.iEntityType == ET_AGENT)
			{
				LPVOID pDObj = GetEntityDObj(CEntity[i].NextEntityState.iEntityNum);

				if (!pDObj)
					continue;

				Vector3 vMinTemp = { FLT_MAX, FLT_MAX, FLT_MAX }, vMaxTemp = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

				for (int j = BONE_HEAD; j < BONE_MAX; j++)
				{
					GetTagPosition(&CEntity[i], pDObj, RegisterTag(vBones[j].second), EntityList[i].vBones3D[j]);

					if (EntityList[i].vBones3D[j][0] < vMinTemp[0])
						vMinTemp[0] = EntityList[i].vBones3D[j][0];

					if (EntityList[i].vBones3D[j][0] > vMaxTemp[0])
						vMaxTemp[0] = EntityList[i].vBones3D[j][0];

					if (EntityList[i].vBones3D[j][1] < vMinTemp[1])
						vMinTemp[1] = EntityList[i].vBones3D[j][1];

					if (EntityList[i].vBones3D[j][1] > vMaxTemp[1])
						vMaxTemp[1] = EntityList[i].vBones3D[j][1];

					if (EntityList[i].vBones3D[j][2] < vMinTemp[2])
						vMinTemp[2] = EntityList[i].vBones3D[j][2];

					if (EntityList[i].vBones3D[j][2] > vMaxTemp[2])
						vMaxTemp[2] = EntityList[i].vBones3D[j][2];
				}

				VectorAverage(vMinTemp, vMaxTemp, EntityList[i].vCenter3D);
			}

			char szWeapon[1024] = { NULL };

			GetWeaponDisplayName((BYTE)CEntity[i].NextEntityState.iWeapon, CEntity[i].NextEntityState.iInAltWeaponMode, szWeapon, sizeof(szWeapon));
			EntityList[i].szWeapon = acut::StripColorCodes(szWeapon);

			EntityList[i].bIsValid = true;

			if (CEntity[i].NextEntityState.iEntityType == ET_PLAYER)
			{
				Vector3 vViewOrigin;
				VectorCopy(CEntity[i].vOrigin, vViewOrigin);
				vViewOrigin[2] += M_METERS;

				EntityList[i].bW2SSuccess = _drawing.Calculate2D(EntityList[i].vBones3D, EntityList[i].vBones2D, EntityList[i].vPosition, EntityList[i].vDimentions) &&
					_drawing.Calculate3D(&CEntity[i], EntityList[i].vCenter3D, EntityList[i].vCorners3D, EntityList[i].vCorners2D) &&
					WorldToScreen(GetScreenMatrix(), EntityList[i].vCenter3D, EntityList[i].vCenter2D) &&
					WorldToScreen(GetScreenMatrix(), CEntity[i].vOrigin, EntityList[i].vLower) &&
					WorldToScreen(GetScreenMatrix(), vViewOrigin, EntityList[i].vUpper);

				_mathematics.WorldToCompass(CEntity[i].vOrigin, _drawing.Compass.vCompassPosition, _drawing.Compass.flCompassSize, _drawing.Compass.vArrowPosition[i]);
				_mathematics.WorldToRadar(CEntity[i].vOrigin, _drawing.Radar.vRadarPosition, _drawing.Radar.flScale, _drawing.Radar.flRadarSize, _drawing.Radar.flBlipSize, _drawing.Radar.vBlipPosition[i]);

				if (!EntityIsEnemy(i))
				{
					EntityList[i].cColor = _profiler.gColorAllies->Custom.cValue;

					continue;
				}

				EntityList[i].cColor = _profiler.gColorAxis->Custom.cValue;
			}

			else if (CEntity[i].NextEntityState.iEntityType == ET_ITEM)
			{
				EntityList[i].bW2SSuccess = WorldToScreen(GetScreenMatrix(), CEntity[i].vOrigin, EntityList[i].vCenter2D);

				continue;
			}

			else if (CEntity[i].NextEntityState.iEntityType == ET_MISSILE)
			{
				EntityList[i].bW2SSuccess = WorldToScreen(GetScreenMatrix(), CEntity[i].vOrigin, EntityList[i].vCenter2D);

				if (!EntityIsEnemy(i))
					continue;
			}

			else if (CEntity[i].NextEntityState.iEntityType == ET_AGENT)
			{
				EntityList[i].bW2SSuccess = WorldToScreen(GetScreenMatrix(), EntityList[i].vBones3D[BONE_HEAD], EntityList[i].vCenter2D);

				if (!EntityIsEnemy(i))
					continue;
			}

			if (!(CEntity[i].NextEntityState.iEntityType == ET_PLAYER ||
				(_profiler.gTargetMissiles->Custom.bValue && CEntity[i].NextEntityState.iEntityType == ET_MISSILE &&
				(CEntity[i].NextEntityState.iWeapon == WEAPON_C4 || CEntity[i].NextEntityState.iWeapon == WEAPON_IED)) ||
					(_profiler.gTargetAgents->Custom.bValue && CEntity[i].NextEntityState.iEntityType == ET_AGENT)))
				continue;

			if (WeaponIsVehicle(CEntity[i].NextEntityState.iWeapon))
				continue;

			Vector3 vDirection, vAngles, vDelta;

			VectorSubtract(CEntity[i].vOrigin, CG->PlayerState.vOrigin, vDirection);

			VectorNormalize(vDirection);
			VectorAngles(vDirection, vAngles);
			_mathematics.NormalizeAngles(vAngles);

			VectorSubtract(vAngles, CEntity[i].vAngles, vDelta);

			if (((BYTE)CEntity[i].NextEntityState.iWeapon == WEAPON_RIOT_SHIELD && !AngleCompare180(vDelta[1])) ||
				((BYTE)CEntity[i].NextEntityState.LerpEntityState.iSecondaryWeapon == WEAPON_RIOT_SHIELD && AngleCompare180(vDelta[1])))
			{
				if (_profiler.gRiotShield->Custom.iValue == cProfiler::RIOTSHIELD_IGNOREPLAYER)
					continue;

				else if (_profiler.gRiotShield->Custom.iValue == cProfiler::RIOTSHIELD_TARGETFEET)
					EntityList[i].bAimFeet = true;
			}

			if (EntityList[i].bAimFeet)
			{
				bool bIsLeftAnkleVisible = IsVisible(&CEntity[i], EntityList[i].vBones3D[BONE_LEFT_ANKLE], _profiler.gAutoWall->Custom.bValue, NULL),
					bIsRightAnkleVisible = IsVisible(&CEntity[i], EntityList[i].vBones3D[BONE_RIGHT_ANKLE], _profiler.gAutoWall->Custom.bValue, NULL);

				if (bIsLeftAnkleVisible && bIsRightAnkleVisible)
				{
					EntityList[i].iBoneIndex = EntityList[i].vBones3D[BONE_LEFT_ANKLE][2] < EntityList[i].vBones3D[BONE_RIGHT_ANKLE][2] ? BONE_LEFT_ANKLE : BONE_RIGHT_ANKLE;
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vHitLocation);
					EntityList[i].bIsVisible = true;
				}

				else if (bIsLeftAnkleVisible)
				{
					EntityList[i].iBoneIndex = BONE_LEFT_ANKLE;
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vHitLocation);
					EntityList[i].bIsVisible = true;
				}

				else if (bIsRightAnkleVisible)
				{
					EntityList[i].iBoneIndex = BONE_RIGHT_ANKLE;
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vHitLocation);
					EntityList[i].bIsVisible = true;
				}

				else
					EntityList[i].bIsVisible = false;
			}

			else if (CEntity[i].NextEntityState.iEntityType == ET_PLAYER)
			{
				if (_profiler.gBoneScan->Custom.iValue == cProfiler::BONESCAN_ONTIMER)
				{
					if (iBonescanNum == i)
					{
						EntityList[i].bIsVisible = Bonescan(&CEntity[i], EntityList[i].vBones3D, _profiler.gAutoWall->Custom.bValue, &EntityList[i].iBoneIndex);
						VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vHitLocation);
					}

					else
					{
						EntityList[i].bIsVisible = IsVisible(&CEntity[i], EntityList[i].vBones3D[EntityList[i].iBoneIndex], _profiler.gAutoWall->Custom.bValue, NULL);
						VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vHitLocation);
					}
				}

				else if (_profiler.gBoneScan->Custom.iValue == cProfiler::BONESCAN_IMMEDIATE)
				{
					EntityList[i].bIsVisible = Bonescan(&CEntity[i], EntityList[i].vBones3D, _profiler.gAutoWall->Custom.bValue, &EntityList[i].iBoneIndex);
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vHitLocation);
				}

				else
				{
					EntityList[i].iBoneIndex = (eBone)_profiler.gAimBone->Custom.iValue;
					EntityList[i].bIsVisible = IsVisible(&CEntity[i], EntityList[i].vBones3D[EntityList[i].iBoneIndex], _profiler.gAutoWall->Custom.bValue, NULL);
					VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vHitLocation);
				}
			}

			else if (CEntity[i].NextEntityState.iEntityType == ET_AGENT)
			{
				EntityList[i].iBoneIndex = BONE_HEAD;
				EntityList[i].bIsVisible = IsVisible(&CEntity[i], EntityList[i].vBones3D[EntityList[i].iBoneIndex], _profiler.gAutoWall->Custom.bValue, NULL);
				VectorCopy(EntityList[i].vBones3D[EntityList[i].iBoneIndex], EntityList[i].vHitLocation);
			}

			else
			{
				EntityList[i].bIsVisible = IsVisible(&CEntity[i], CEntity[i].vOrigin, _profiler.gAutoWall->Custom.bValue, NULL);
				VectorCopy(CEntity[i].vOrigin, EntityList[i].vHitLocation);
			}

			if (_mathematics.CalculateFOV(EntityList[i].vHitLocation) > _profiler.gAimAngle->Custom.flValue)
				EntityList[i].bIsVisible = false;

			if (i < FindVariable("sv_maxclients")->Current.iValue && *(int*)OFF_ISCURRENTHOST)
				if (GEntity[i].iHealth < 1)
					continue;

			if (std::find(vIsTarget.begin(), vIsTarget.end(), TRUE) != vIsTarget.end())
			{
				if (i < FindVariable("sv_maxclients")->Current.iValue)
				{
					if (!vIsTarget[CEntity[i].NextEntityState.iEntityNum])
						continue;
				}

				else if (!vIsTarget[CEntity[i].NextEntityState.iOtherEntityNum])
					continue;
			}

			if (EntityList[i].bIsVisible)
			{
				ImVec2 vTarget, vCenter = ImGui::GetIO().DisplaySize / 2.0f;

				if (WorldToScreen(GetScreenMatrix(), EntityList[i].vHitLocation, vTarget))
				{
					bW2SSuccess = true;

					TargetInfo.iIndex = i;
					TargetInfo.flDistance2D = GetDistance2D(vTarget, vCenter);

					vTargetInfo.push_back(TargetInfo);
				}

				if (!bW2SSuccess)
				{
					TargetInfo.iIndex = i;
					TargetInfo.flDistance3D = GetDistance3D(CEntity[i].vOrigin, CG->PlayerState.vOrigin);

					vTargetInfo.push_back(TargetInfo);
				}
			}
		}

		if (!vTargetInfo.empty())
		{
			if (bW2SSuccess)
			{
				std::sort(vTargetInfo.begin(), vTargetInfo.end(), [&](const sTargetInfo& a, const sTargetInfo& b) { return a.flDistance2D < b.flDistance2D; });
				_aimBot.AimState.iTargetNum = vTargetInfo.front().iIndex;
			}

			else
			{
				std::sort(vTargetInfo.begin(), vTargetInfo.end(), [&](const sTargetInfo& a, const sTargetInfo& b) { return a.flDistance3D < b.flDistance3D; });
				_aimBot.AimState.iTargetNum = vTargetInfo.front().iIndex;
			}

			vTargetInfo.clear();
		}

		_aimBot.AimState.bTargetAcquired = (_aimBot.AimState.iTargetNum > -1);
		_aimBot.AimState.bLockonTarget = (_profiler.gAimBotMode->Custom.iValue == cProfiler::AIMBOT_MODE_AUTO || (_profiler.gAimBotMode->Custom.iValue == cProfiler::AIMBOT_MODE_MANUAL && CEntity[CG->PlayerState.iClientNum].NextEntityState.LerpEntityState.iEntityFlags & EF_ZOOM));
		_aimBot.AimState.bIsAutoAiming = (_aimBot.AimState.bTargetAcquired && _aimBot.AimState.bLockonTarget);
		_aimBot.AimState.bIsAutoFiring = (_profiler.gAutoFire->Custom.bValue && _aimBot.AimState.bIsAutoAiming);

		if (_aimBot.AimState.bLockonTarget)
		{
			if (_aimBot.AimState.iCurrentAimDelay == _profiler.gAutoAimDelay->Custom.iValue)
				_aimBot.AimState.iCurrentAimTime += clock() - _aimBot.AimState.iDeltaTMR;

			_aimBot.AimState.iCurrentAimDelay += clock() - _aimBot.AimState.iDeltaTMR;
			_aimBot.AimState.iCurrentZoomDelay += clock() - _aimBot.AimState.iDeltaTMR;
			_aimBot.AimState.iCurrentFireDelay += clock() - _aimBot.AimState.iDeltaTMR;
		}

		_aimBot.AimState.iDeltaTMR = clock();

		if (_aimBot.AimState.iLastTarget != _aimBot.AimState.iTargetNum)
		{
			_aimBot.AimState.iLastTarget = _aimBot.AimState.iTargetNum;
			_aimBot.AimState.iCurrentAimTime = 0;
		}

		if (EntityList[_aimBot.AimState.iTargetNum].iLastBone != EntityList[_aimBot.AimState.iTargetNum].iBoneIndex)
		{
			EntityList[_aimBot.AimState.iTargetNum].iLastBone = EntityList[_aimBot.AimState.iTargetNum].iBoneIndex;
			_aimBot.AimState.iCurrentAimTime = 0;
		}

		if (!_aimBot.AimState.bTargetAcquired)
			_aimBot.AimState.iCurrentAimDelay = _aimBot.AimState.iCurrentZoomDelay = _aimBot.AimState.iCurrentFireDelay = 0;

		if (_aimBot.AimState.iCurrentAimTime > _profiler.gAutoAimTime->Custom.iValue)
			_aimBot.AimState.iCurrentAimTime = _profiler.gAutoAimTime->Custom.iValue;

		if (_aimBot.AimState.iCurrentAimDelay > _profiler.gAutoAimDelay->Custom.iValue)
			_aimBot.AimState.iCurrentAimDelay = _profiler.gAutoAimDelay->Custom.iValue;

		if (_aimBot.AimState.iCurrentZoomDelay > _profiler.gAutoZoomDelay->Custom.iValue)
			_aimBot.AimState.iCurrentZoomDelay = _profiler.gAutoZoomDelay->Custom.iValue;

		if (_aimBot.AimState.iCurrentFireDelay > _profiler.gAutoFireDelay->Custom.iValue)
			_aimBot.AimState.iCurrentFireDelay = _profiler.gAutoFireDelay->Custom.iValue;

		if (_aimBot.AimState.bTargetAcquired)
		{
			VectorCopy(EntityList[_aimBot.AimState.iTargetNum].vHitLocation, _aimBot.AimState.vAimbotPosition);
			_mathematics.CalculateAngles(_aimBot.AimState.vAimbotPosition, _aimBot.AimState.vAimbotAngles);
		}

		iCounter++;
		_aimBot.AimState.iFireTMR++;

		if (WeaponIsAkimbo(GetViewmodelWeapon(&CG->PlayerState)))
		{
			if (!(_aimBot.AimState.iFireTMR % ((BYTE)GetViewmodelWeapon(&CG->PlayerState) == WEAPON_44_MAGNUM ? 12 : 6)))
				_aimBot.AimState.bAkimbo = !_aimBot.AimState.bAkimbo;
		}

		else
			_aimBot.AimState.bAkimbo = false;
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::EntityIsValid(int index)
	{
		return (index != CG->PlayerState.iClientNum && CEntity[index].wValid && CEntity[index].iIsAlive & 1);
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::EntityIsEnemy(int index)
	{
		if (CEntity[index].NextEntityState.iEntityType == ET_PLAYER)
		{
			if (CharacterInfo[index].iTeam == TEAM_FREE ||
				CharacterInfo[index].iTeam != CharacterInfo[CG->PlayerState.iClientNum].iTeam)
				return true;
		}

		else
		{
			if (CharacterInfo[CEntity[index].NextEntityState.iOtherEntityNum].iTeam == TEAM_FREE ||
				CharacterInfo[CEntity[index].NextEntityState.iOtherEntityNum].iTeam != CharacterInfo[CG->PlayerState.iClientNum].iTeam)
				return true;
		}

		return false;
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::IsVisible(sCEntity* entity, Vector3 position, bool autowall, float* damage)
	{
		ApplyPrediction(entity, position);

		if (WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)))
		{
			bool bTraceHit = _autoWall.TraceLine(RefDef->vViewOrg, position, entity->NextEntityState.iEntityNum);

			if (bTraceHit)
				return true;
		}

		else if (autowall)
		{
			float flDamage = _autoWall.C_Autowall(RefDef->vViewOrg, position);

			if (damage)
				*damage = flDamage;

			if (flDamage > 0.0f)
				return true;
		}

		else
		{
			bool bTraceHit = _autoWall.C_TraceBullet(RefDef->vViewOrg, position, entity->NextEntityState.iEntityNum);

			if (bTraceHit)
				return true;
		}

		return false;
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::Bonescan(sCEntity* entity, Vector3 bones3d[BONE_MAX], bool autowall, eBone* index)
	{
		bool bReturn = false;

		sDamageInfo DamageInfo;
		std::vector<sDamageInfo> vDamageInfo;

		for (int i = BONE_HEAD; i < BONE_MAX; i++)
		{
			if (autowall)
			{
				if (IsVisible(entity, bones3d[i], true, &DamageInfo.flDamage))
				{
					DamageInfo.iBoneIndex = (eBone)i;
					vDamageInfo.push_back(DamageInfo);

					bReturn = true;
				}
			}

			else
			{
				if (IsVisible(entity, bones3d[i], false, NULL))
				{
					*index = (eBone)i;
					return true;
				}
			}
		}

		if (!vDamageInfo.empty())
		{
			std::sort(vDamageInfo.begin(), vDamageInfo.end(), [&](const sDamageInfo& a, const sDamageInfo& b) { return a.flDamage > b.flDamage; });
			*index = vDamageInfo.front().iBoneIndex;
			vDamageInfo.clear();
		}

		return bReturn;
	}
	/*
	//=====================================================================================
	*/
	void cTargetList::ApplyPrediction(sCEntity* entity, Vector3 position)
	{
		Vector3 vOldPosition, vNewPosition, vVelocity;

		EvaluateTrajectory(&entity->CurrentEntityState.PositionTrajectory, CG->PlayerState.OldSnapShot->iServerTime, vOldPosition);
		EvaluateTrajectory(&entity->NextEntityState.LerpEntityState.PositionTrajectory, CG->PlayerState.NewSnapShot->iServerTime, vNewPosition);

		VectorSubtract(vNewPosition, vOldPosition, vVelocity);

		VectorMA(position, *(int*)OFF_FRAMETIME / 1000.0f, vVelocity, position);
		VectorMA(position, *(int*)OFF_PING / 1000.0f, vVelocity, position);
	}
}

//=====================================================================================