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

		sAntiAimTargetInfo AntiAimTargetInfo;
		std::vector<sAntiAimTargetInfo> vAntiAimTargetInfo;

		_aimBot.AimState.iTargetNum = -1;
		_aimBot.AimState.iAntiAimTargetNum = -1;

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
				LPVOID lpDObj = GetEntityDObj(i);

				if (!lpDObj)
					continue;

				if (_targetList.Priorities[i].bIsPrioritized && !_targetList.Priorities[i].bIsIgnored)
				{
					AntiAimTargetInfo.iIndex = i;

					AntiAimTargetInfo.flDistance = _mathematics.CalculateDistance(CEntity[i].vOrigin, CG->PredictedPlayerState.vOrigin);
					AntiAimTargetInfo.flDamage = EntityList[i].flDamage;
					AntiAimTargetInfo.flFOV = _mathematics.CalculateFOV(EntityList[i].vHitLocation);

					vAntiAimTargetInfo.push_back(AntiAimTargetInfo);
				}

				ImVec3 vMinTemp = { FLT_MAX, FLT_MAX, FLT_MAX }, vMaxTemp = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

				for (auto& Bone : vBones)
				{
					GetTagPosition(&CEntity[i], lpDObj, RegisterTag(Bone.second.second), &EntityList[i].vBones3D[Bone.first.first]);

					if (EntityList[i].vBones3D[Bone.first.first].x < vMinTemp.x)
						vMinTemp.x = EntityList[i].vBones3D[Bone.first.first].x;

					if (EntityList[i].vBones3D[Bone.first.first].x > vMaxTemp.x)
						vMaxTemp.x = EntityList[i].vBones3D[Bone.first.first].x;

					if (EntityList[i].vBones3D[Bone.first.first].y < vMinTemp.y)
						vMinTemp.y = EntityList[i].vBones3D[Bone.first.first].y;

					if (EntityList[i].vBones3D[Bone.first.first].y > vMaxTemp.y)
						vMaxTemp.y = EntityList[i].vBones3D[Bone.first.first].y;

					if (EntityList[i].vBones3D[Bone.first.first].z < vMinTemp.z)
						vMinTemp.z = EntityList[i].vBones3D[Bone.first.first].z;

					if (EntityList[i].vBones3D[Bone.first.first].z > vMaxTemp.z)
						vMaxTemp.z = EntityList[i].vBones3D[Bone.first.first].z;
				}

				EntityList[i].vCenter3D = (vMinTemp + vMaxTemp) / 2.0f;
			}

			char szWeapon[1024] = { NULL };

			GetWeaponDisplayName((BYTE)CEntity[i].NextEntityState.iWeapon, CEntity[i].NextEntityState.iInAltWeaponMode, szWeapon, sizeof(szWeapon));
			EntityList[i].szWeapon = szWeapon;

			EntityList[i].bIsValid = true;

			if (CEntity[i].NextEntityState.iEntityType == ET_PLAYER)
			{
				ImVec3 vViewOrigin;
				vViewOrigin = CEntity[i].vOrigin;
				vViewOrigin.z += M_METERS;

				EntityList[i].bW2SSuccess = _drawing.Calculate2D(EntityList[i].vBones3D, EntityList[i].vBones2D, EntityList[i].vPosition, EntityList[i].vDimentions) &&
					_drawing.Calculate3D(&CEntity[i], EntityList[i].vCenter3D, EntityList[i].vCorners3D, EntityList[i].vCorners2D) &&
					WorldToScreen(GetScreenMatrix(), EntityList[i].vCenter3D, &EntityList[i].vCenter2D) &&
					WorldToScreen(GetScreenMatrix(), CEntity[i].vOrigin, &EntityList[i].vLower) &&
					WorldToScreen(GetScreenMatrix(), vViewOrigin, &EntityList[i].vUpper);

				_mathematics.WorldToCompass(CEntity[i].vOrigin, _drawing.Compass.vCompassPosition, _drawing.Compass.flCompassSize, _drawing.Compass.vArrowPosition[i]);
				_mathematics.WorldToRadar(CEntity[i].vOrigin, _drawing.Radar.vRadarPosition, _drawing.Radar.flScale, _drawing.Radar.flRadarSize, _drawing.Radar.flBlipSize, _drawing.Radar.vBlipPosition[i]);

				if (!EntityIsEnemy(i))
				{
					EntityList[i].cColor = _drawing.gColorAllies->Current.cValue;
					continue;
				}

				EntityList[i].cColor = _drawing.gColorAxis->Current.cValue;
			}

			else if (CEntity[i].NextEntityState.iEntityType == ET_ITEM)
			{
				EntityList[i].bW2SSuccess = WorldToScreen(GetScreenMatrix(), CEntity[i].vOrigin, &EntityList[i].vCenter2D);
				continue;
			}

			else if (CEntity[i].NextEntityState.iEntityType == ET_MISSILE)
			{
				EntityList[i].bW2SSuccess = WorldToScreen(GetScreenMatrix(), CEntity[i].vOrigin, &EntityList[i].vCenter2D);

				if (!EntityIsEnemy(i))
					continue;
			}

			else if (CEntity[i].NextEntityState.iEntityType == ET_AGENT)
			{
				EntityList[i].bW2SSuccess = WorldToScreen(GetScreenMatrix(), EntityList[i].vBones3D[vBones[BONE_HEAD].first.first], &EntityList[i].vCenter2D);

				if (!EntityIsEnemy(i))
					continue;
			}

			if (!(CEntity[i].NextEntityState.iEntityType == ET_PLAYER ||
				(gTargetMissiles->Current.bValue && CEntity[i].NextEntityState.iEntityType == ET_MISSILE &&
				(CEntity[i].NextEntityState.iWeapon == WEAPON_C4 || CEntity[i].NextEntityState.iWeapon == WEAPON_IED)) ||
					(gTargetAgents->Current.bValue && CEntity[i].NextEntityState.iEntityType == ET_AGENT)))
				continue;

			ImVec3 vDirection, vAngles, vDelta;

			vDirection = CEntity[i].vOrigin - CG->PredictedPlayerState.vOrigin;

			VectorNormalize(&vDirection);
			VectorAngles(vDirection, &vAngles);
			_mathematics.ClampAngles(vAngles);

			vDelta = vAngles - CEntity[i].vViewAngles;

			if (((BYTE)CEntity[i].NextEntityState.iWeapon == WEAPON_RIOT_SHIELD && !AngleCompare180(vDelta.y)) ||
				((BYTE)CEntity[i].NextEntityState.LerpEntityState.iSecondaryWeapon == WEAPON_RIOT_SHIELD && AngleCompare180(vDelta.y)))
			{
				if (gRiotShielders->Current.iValue == RIOTSHIELD_IGNOREPLAYER)
					continue;

				else if (gRiotShielders->Current.iValue == RIOTSHIELD_TARGETFEET)
					EntityList[i].bAimFeet = true;
			}

			if (EntityList[i].bAimFeet)
			{
				bool bIsLeftAnkleVisible = IsVisible(&CEntity[i], EntityList[i].vBones3D, false, gAutoWall->Current.bValue, vBones[BONE_LEFT_ANKLE].first.first, NULL),
					bIsRightAnkleVisible = IsVisible(&CEntity[i], EntityList[i].vBones3D, false, gAutoWall->Current.bValue, vBones[BONE_RIGHT_ANKLE].first.first, NULL);

				if (bIsLeftAnkleVisible && bIsRightAnkleVisible)
				{
					EntityList[i].iBoneIndex = EntityList[i].vBones3D[vBones[BONE_LEFT_ANKLE].first.first].z < EntityList[i].vBones3D[vBones[BONE_RIGHT_ANKLE].first.first].z ? vBones[BONE_LEFT_ANKLE].first.first : vBones[BONE_RIGHT_ANKLE].first.first;
					EntityList[i].vHitLocation = EntityList[i].vBones3D[EntityList[i].iBoneIndex];
					EntityList[i].bIsVisible = true;
				}

				else if (bIsLeftAnkleVisible)
				{
					EntityList[i].iBoneIndex = vBones[BONE_LEFT_ANKLE].first.first;
					EntityList[i].vHitLocation = EntityList[i].vBones3D[EntityList[i].iBoneIndex];
					EntityList[i].bIsVisible = true;
				}

				else if (bIsRightAnkleVisible)
				{
					EntityList[i].iBoneIndex = vBones[BONE_RIGHT_ANKLE].first.first;
					EntityList[i].vHitLocation = EntityList[i].vBones3D[EntityList[i].iBoneIndex];
					EntityList[i].bIsVisible = true;
				}

				else
					EntityList[i].bIsVisible = false;
			}

			else if (CEntity[i].NextEntityState.iEntityType == ET_PLAYER)
			{
				if (gBoneScan->Current.iValue == BONESCAN_ONTIMER)
				{
					EntityList[i].bIsVisible = IsVisible(&CEntity[i], EntityList[i].vBones3D, iBonescanNum == i, gAutoWall->Current.bValue, EntityList[i].iBoneIndex, &EntityList[i].flDamage);
					EntityList[i].vHitLocation = EntityList[i].vBones3D[EntityList[i].iBoneIndex];
				}

				else if (gBoneScan->Current.iValue == BONESCAN_IMMEDIATE)
				{
					EntityList[i].bIsVisible = IsVisible(&CEntity[i], EntityList[i].vBones3D, true, gAutoWall->Current.bValue, EntityList[i].iBoneIndex, &EntityList[i].flDamage);
					EntityList[i].vHitLocation = EntityList[i].vBones3D[EntityList[i].iBoneIndex];
				}

				else
				{
					EntityList[i].iBoneIndex = (eBone)gAimBone->Current.iValue;
					EntityList[i].bIsVisible = IsVisible(&CEntity[i], EntityList[i].vBones3D, false, gAutoWall->Current.bValue, EntityList[i].iBoneIndex, &EntityList[i].flDamage);
					EntityList[i].vHitLocation = EntityList[i].vBones3D[EntityList[i].iBoneIndex];
				}
			}

			else if (CEntity[i].NextEntityState.iEntityType == ET_AGENT)
			{
				EntityList[i].iBoneIndex = vBones[BONE_HEAD].first.first;
				EntityList[i].bIsVisible = IsVisible(&CEntity[i], EntityList[i].vBones3D, false, gAutoWall->Current.bValue, EntityList[i].iBoneIndex, nullptr);
				EntityList[i].vHitLocation = EntityList[i].vBones3D[EntityList[i].iBoneIndex];
			}

			else
			{
				EntityList[i].bIsVisible = std::async(&cTargetList::IsVisibleInternal, this, &CEntity[i], CEntity[i].vOrigin, HITLOC_NONE, gAutoWall->Current.bValue, nullptr).get();
				EntityList[i].vHitLocation = CEntity[i].vOrigin;
			}

			if (i < FindVariable("sv_maxclients")->Current.iValue && IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum))
				if (GEntity[i].iHealth < 1)
					continue;

			if (i < FindVariable("sv_maxclients")->Current.iValue)
			{
				if (Priorities[i].bIsIgnored)
					continue;
			}

			else
			{
				if (Priorities[CEntity[i].NextEntityState.iOtherEntityNum].bIsIgnored)
					continue;
			}

			if (EntityList[i].bIsVisible && _mathematics.CalculateFOV(EntityList[i].vHitLocation) <= gAimAngle->Current.iValue)
			{
				TargetInfo.bIsPriority = _targetList.Priorities[i].bIsPrioritized;
				TargetInfo.iIndex = i;

				TargetInfo.flDistance = _mathematics.CalculateDistance(CEntity[i].vOrigin, CG->PredictedPlayerState.vOrigin);
				TargetInfo.flDamage = EntityList[i].flDamage;
				TargetInfo.flFOV = _mathematics.CalculateFOV(EntityList[i].vHitLocation);

				vTargetInfo.push_back(TargetInfo);
			}
		}

		if (!vTargetInfo.empty())
		{
			if (gSortMethod->Current.iValue == SORT_METHOD_DISTANCE)
			{
				std::sort(vTargetInfo.begin(), vTargetInfo.end(), [&](const sTargetInfo& a, const sTargetInfo& b) { return a.flDistance < b.flDistance; });

				auto itTargetInfo = std::find_if(vTargetInfo.begin(), vTargetInfo.end(), [&](const sTargetInfo& targetinfo) { return targetinfo.bIsPriority; });

				if (itTargetInfo != vTargetInfo.end())
					_aimBot.AimState.iTargetNum = itTargetInfo->iIndex;

				else
					_aimBot.AimState.iTargetNum = vTargetInfo.front().iIndex;
			}

			else if (gSortMethod->Current.iValue == SORT_METHOD_DAMAGE)
			{
				std::sort(vTargetInfo.begin(), vTargetInfo.end(), [&](const sTargetInfo& a, const sTargetInfo& b) { return a.flDamage > b.flDamage; });

				auto itTargetInfo = std::find_if(vTargetInfo.begin(), vTargetInfo.end(), [&](const sTargetInfo& targetinfo) { return targetinfo.bIsPriority; });

				if (itTargetInfo != vTargetInfo.end())
					_aimBot.AimState.iTargetNum = itTargetInfo->iIndex;

				else
					_aimBot.AimState.iTargetNum = vTargetInfo.front().iIndex;
			}

			else if (gSortMethod->Current.iValue == SORT_METHOD_FOV)
			{
				std::sort(vTargetInfo.begin(), vTargetInfo.end(), [&](const sTargetInfo& a, const sTargetInfo& b) { return a.flFOV < b.flFOV; });

				auto itTargetInfo = std::find_if(vTargetInfo.begin(), vTargetInfo.end(), [&](const sTargetInfo& targetinfo) { return targetinfo.bIsPriority; });

				if (itTargetInfo != vTargetInfo.end())
					_aimBot.AimState.iTargetNum = itTargetInfo->iIndex;

				else
					_aimBot.AimState.iTargetNum = vTargetInfo.front().iIndex;
			}

			vTargetInfo.clear();
		}

		if (!vAntiAimTargetInfo.empty())
		{
			if (gSortMethod->Current.iValue == SORT_METHOD_DISTANCE)
			{
				std::sort(vAntiAimTargetInfo.begin(), vAntiAimTargetInfo.end(), [&](const sAntiAimTargetInfo& a, const sAntiAimTargetInfo& b) { return a.flDistance < b.flDistance; });

				_aimBot.AimState.iAntiAimTargetNum = vAntiAimTargetInfo.front().iIndex;
			}

			else if (gSortMethod->Current.iValue == SORT_METHOD_DAMAGE)
			{
				std::sort(vAntiAimTargetInfo.begin(), vAntiAimTargetInfo.end(), [&](const sAntiAimTargetInfo& a, const sAntiAimTargetInfo& b) { return a.flDamage > b.flDamage; });

				_aimBot.AimState.iAntiAimTargetNum = vAntiAimTargetInfo.front().iIndex;
			}

			else if (gSortMethod->Current.iValue == SORT_METHOD_FOV)
			{
				std::sort(vAntiAimTargetInfo.begin(), vAntiAimTargetInfo.end(), [&](const sAntiAimTargetInfo& a, const sAntiAimTargetInfo& b) { return a.flFOV < b.flFOV; });

				_aimBot.AimState.iAntiAimTargetNum = vAntiAimTargetInfo.front().iIndex;
			}

			vAntiAimTargetInfo.clear();
		}

		iCounter++;
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::EntityIsValid(int index)
	{
		return ((index != CG->PredictedPlayerState.iClientNum) && (CEntity[index].iIsAlive & 1) && !(CEntity[index].NextEntityState.LerpEntityState.iEntityFlags & EF_DEAD));
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::EntityIsEnemy(int index)
	{
		if (CEntity[index].NextEntityState.iEntityType == ET_PLAYER)
		{
			if (CharacterInfo[index].iTeam > TEAM_FREE)
			{
				if (CharacterInfo[index].iTeam != CharacterInfo[CG->PredictedPlayerState.iClientNum].iTeam)
					return true;
			}

			else
			{
				if (index != CG->PredictedPlayerState.iClientNum)
					return true;
			}
		}

		else
		{
			if (CharacterInfo[CEntity[index].NextEntityState.iOtherEntityNum].iTeam > TEAM_FREE)
			{
				if (CharacterInfo[CEntity[index].NextEntityState.iOtherEntityNum].iTeam != CharacterInfo[CG->PredictedPlayerState.iClientNum].iTeam)
					return true;
			}

			else
			{
				if (CEntity[index].NextEntityState.iOtherEntityNum != CG->PredictedPlayerState.iClientNum)
					return true;
			}
		}

		return false;
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::IsVisibleInternal(sCEntity* entity, ImVec3 position, eHitLocation hitloc, bool autowall, float* damage)
	{
		ImVec3 vViewOrigin;

		GetPlayerViewOrigin(&CG->PredictedPlayerState, &vViewOrigin);

		if (WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)))
		{
			bool bTraceHit = _autoWall.TraceLine(entity, RefDef->vViewOrigin, position);

			if (bTraceHit)
				return true;
		}

		else if (autowall)
		{
			float flDamage = _autoWall.C_Autowall(entity, vViewOrigin, position, hitloc);

			if (damage)
				*damage = flDamage;

			if (flDamage >= 1.0f)
				return true;
		}

		else
		{
			float flDamage = _autoWall.C_TraceBullet(entity, vViewOrigin, position, hitloc);

			if (damage)
				*damage = flDamage;

			if (flDamage >= 1.0f)
				return true;
		}

		return false;
	}
	/*
	//=====================================================================================
	*/
	bool cTargetList::IsVisible(sCEntity* entity, ImVec3 bones3d[BONE_MAX], bool bonescan, bool autowall, eBone& index, float* damage)
	{
		bool bReturn = false;

		sDamageInfo DamageInfo;
		std::vector<sDamageInfo> vDamageInfo;
		std::vector<std::future<bool>> vIsVisible(BONE_MAX);

		if (bonescan)
		{
			for (auto& Bone : vBones)
			{
				vIsVisible[Bone.first.first] = std::async(&cTargetList::IsVisibleInternal, this, entity, bones3d[Bone.first.first], Bone.first.second, autowall, &DamageInfo.flDamage);
			}

			for (auto& Bone : vBones)
			{
				if (vIsVisible[Bone.first.first].get())
				{
					DamageInfo.iBoneIndex = Bone.first.first;
					vDamageInfo.push_back(DamageInfo);

					bReturn = true;
				}
			}
		}

		else
		{
			return std::async(&cTargetList::IsVisibleInternal, this, entity, bones3d[index], vBones[index].first.second, autowall, damage).get();
		}

		if (!vDamageInfo.empty())
		{
			std::stable_sort(vDamageInfo.begin(), vDamageInfo.end(), [&](const sDamageInfo& a, const sDamageInfo& b) { return a.flDamage > b.flDamage; });

			index = vDamageInfo.front().iBoneIndex;

			if (damage)
				*damage = vDamageInfo.front().flDamage;

			vDamageInfo.clear();
		}

		return bReturn;
	}
}

//=====================================================================================