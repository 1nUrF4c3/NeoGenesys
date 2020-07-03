//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cHostMenu _hostMenu;

	void cHostMenu::PlayerMods()
	{
		for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
		{
			if (LocalClientIsInGame() && IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum))
			{
				if (HostMenu.PlayerMod[i].bGodMode)
				{
					if (GEntity[i].iHealth != -10000000)
						GEntity[i].iHealth = -10000000;
				}

				else
				{
					if (GEntity[i].iHealth == -10000000)
						GEntity[i].iHealth = 100;
				}

				if (HostMenu.PlayerMod[i].bNoClip)
					PlayerState[i].iMoveType = 2;

				else
					PlayerState[i].iMoveType = 0;

				if (HostMenu.PlayerMod[i].bInfiniteAmmo)
				{
					for (int j = 0; j < 15; j++)
					{
						if (PlayerState[i].AmmoNotInClip[j].iAmmoCount != 600)
							PlayerState[i].AmmoNotInClip[j].iAmmoCount = 600;

						if (PlayerState[i].AmmoInClip[j].iAmmoCount[0] != 200)
							PlayerState[i].AmmoInClip[j].iAmmoCount[0] = 200;

						if (PlayerState[i].AmmoInClip[j].iAmmoCount[1] != 200)
							PlayerState[i].AmmoInClip[j].iAmmoCount[1] = 200;
					}
				}

				if (HostMenu.PlayerMod[i].bInvisibility)
					PlayerState[i].iEntityFlags |= 0x20;

				else
					PlayerState[i].iEntityFlags &= ~0x20;

				if (HostMenu.PlayerMod[i].bSuperSpeed)
					PlayerState[i].flSpeedMultiplier = 3.0f;

				if (HostMenu.PlayerMod[i].bFreezePosition)
				{
					PlayerState[i].vOrigin = HostMenu.PlayerMod[i].vPosition;
					PlayerState[i].vVelocity = ImVec3(0.0f, 0.0f, 0.0f);
					PlayerState[i].iGravity = 0;
				}

				else
				{
					HostMenu.PlayerMod[i].vPosition = PlayerState[i].vOrigin;
				}
			}

			if (!CharacterInfo[i].iInfoValid)
			{
				HostMenu.PlayerMod[i].bGodMode = false;
				HostMenu.PlayerMod[i].bNoClip = false;
				HostMenu.PlayerMod[i].bInfiniteAmmo = false;
				HostMenu.PlayerMod[i].bInvisibility = false;
				HostMenu.PlayerMod[i].bSuperSpeed = false;
				HostMenu.PlayerMod[i].bFreezePosition = false;

				_targetList.Priorities[i].bIsPrioritized = false;
				_targetList.Priorities[i].bIsIgnored = false;
			}
		}
	}
	/*
	//=====================================================================================
	*/
	void cHostMenu::GravityGun()
	{
		if (gGravityGun->Current.bValue)
		{
			if (LocalClientIsInGame() && IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum))
			{
				if (!(HostMenu.iGravityGunNum > -1 && HostMenu.iGravityGunNum < FindVariable("sv_maxclients")->Current.iValue && _aimBot.AimState.bIsZooming && _targetList.EntityIsValid(HostMenu.iGravityGunNum)))
				{
					HostMenu.iGravityGunNum = -1;

					sTrace Trace;

					LocationalTrace(&Trace, RefDef->vViewOrigin, _mathematics.AngleToForward(RefDef->vViewOrigin, WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? CG->vRefDefViewAngles : IsThirdPersonMode(&CG->PredictedPlayerState) ? CG->vThirdPersonViewAngles : CG->vWeaponAngles, 8192.0f), CG->PredictedPlayerState.iClientNum, MASK_NONKILLSTREAK);

					if (Trace.TraceHitType == TRACE_HITTYPE_ENTITY)
					{
						HostMenu.iGravityGunNum = GetTraceHitType(&Trace);
						HostMenu.flGravityGunDist = _mathematics.CalculateDistance(CEntity[HostMenu.iGravityGunNum].vOrigin, RefDef->vViewOrigin);
					}
				}

				if (HostMenu.iGravityGunNum > -1 && HostMenu.iGravityGunNum < FindVariable("sv_maxclients")->Current.iValue && _aimBot.AimState.bIsZooming && _targetList.EntityIsValid(HostMenu.iGravityGunNum))
				{
					ImVec3 vCrosshair = _mathematics.AngleToForward(RefDef->vViewOrigin, WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? CG->vRefDefViewAngles : IsThirdPersonMode(&CG->PredictedPlayerState) ? CG->vThirdPersonViewAngles : CG->vWeaponAngles, HostMenu.flGravityGunDist);
					ImVec3 vOffset = _targetList.EntityList[HostMenu.iGravityGunNum].vCenter3D - CEntity[HostMenu.iGravityGunNum].vOrigin;
					ImVec3 vTeleport = vCrosshair - vOffset;

					PlayerState[HostMenu.iGravityGunNum].vOrigin = vTeleport;
					PlayerState[HostMenu.iGravityGunNum].vVelocity = ImVec3(0.0f, 0.0f, 0.0f);
					PlayerState[HostMenu.iGravityGunNum].iGravity = 0;

					sUserCmd* pUserCmd = ClientActive->GetUserCmd(ClientActive->iCurrentCmd - !WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)));

					if (pUserCmd->iButtons & BUTTON_FIRELEFT)
					{
						PlayerState[HostMenu.iGravityGunNum].vVelocity = _mathematics.AngleToForward(RefDef->vViewOrigin, WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? CG->vRefDefViewAngles : IsThirdPersonMode(&CG->PredictedPlayerState) ? CG->vThirdPersonViewAngles : CG->vWeaponAngles, 10000.0f) - RefDef->vViewOrigin;
						HostMenu.iGravityGunNum = -1;
						SetZoomState(false);
					}
				}
			}
		}
	}
	/*
	//=====================================================================================
	*/
	void cHostMenu::TeleportAll()
	{
		ImVec3 vCrosshair = _mathematics.AngleToForward(RefDef->vViewOrigin, WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? CG->vRefDefViewAngles : IsThirdPersonMode(&CG->PredictedPlayerState) ? CG->vThirdPersonViewAngles : CG->vWeaponAngles, M_METERS);

		for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
		{
			if (_targetList.EntityIsValid(i))
			{
				ImVec3 vOffset = _targetList.EntityList[i].vCenter3D - CEntity[i].vOrigin;
				ImVec3 vTeleport = vCrosshair - vOffset;

				PlayerState[i].vOrigin = vTeleport;
				PlayerState[i].vVelocity = ImVec3(0.0f, 0.0f, 0.0f);
				PlayerState[i].iGravity = 0;
			}
		}
	}
	/*
	//=====================================================================================
	*/
	void cHostMenu::StartMatch()
	{
		if (!_mutex.try_lock())
			return;

		bool bTeamBased = FindVariable("party_teambased")->Current.bValue;
		LPBYTE lpResult = VariadicCall<LPBYTE>(0x1402D6880);

		if (lpResult[78])
		{
			LPVOID* v1 = VariadicCall<LPVOID*>(0x1402D6870);
			VariadicCall<LPBYTE>(0x1402E9A60, v1);
		}

		if (!lpResult[78] || !bTeamBased)
		{
			_mutex.unlock();
			return;
		}

		for (int i = Sys_Milliseconds(); !LocalClientIsInGame(); Sleep(100))
		{
			if (Sys_Milliseconds() - i > 60000)
			{
				_mutex.unlock();
				return;
			}
		}

		if (PlayerState[CG->PredictedPlayerState.iClientNum].ClientState.iTeam == TEAM_FREE)
		{
			PlayerState[CG->PredictedPlayerState.iClientNum].ClientState.iTeam = TEAM_ALLIES;
			TeamChanged(CG->PredictedPlayerState.iClientNum);
		}

		_mutex.unlock();
	}
	/*
	//=====================================================================================
	*/
	void cHostMenu::MassKill()
	{
		static int iCounter = 0;
		int iTargetNum = iCounter % FindVariable("sv_maxclients")->Current.iValue;

		if (iTargetNum != CG->PredictedPlayerState.iClientNum && CEntity[iTargetNum].NextEntityState.iWeapon)
		{
			if (CharacterInfo[iTargetNum].iInfoValid && CharacterInfo[iTargetNum].iNextValid)
			{
				if ((gMassKill->Current.iValue == MASSKILL_AXIS && _targetList.EntityIsEnemy(iTargetNum)) ||
					(gMassKill->Current.iValue == MASSKILL_ALLIES && !_targetList.EntityIsEnemy(iTargetNum)) ||
					gMassKill->Current.iValue == MASSKILL_ALL)
				{
					PlayerKill(&GEntity[iTargetNum],
						_targetList.EntityIsEnemy(iTargetNum) ? &GEntity[CG->PredictedPlayerState.iClientNum] : &GEntity[iTargetNum],
						_targetList.EntityIsEnemy(iTargetNum) ? &GEntity[CG->PredictedPlayerState.iClientNum] : &GEntity[iTargetNum],
						_targetList.EntityIsEnemy(iTargetNum) ? 9 : 14,
						_targetList.EntityIsEnemy(iTargetNum) ? CEntity[CG->PredictedPlayerState.iClientNum].NextEntityState.iWeapon : CEntity[iTargetNum].NextEntityState.iWeapon,
						_targetList.EntityIsEnemy(iTargetNum) ? CEntity[CG->PredictedPlayerState.iClientNum].NextEntityState.iInAltWeaponMode : CEntity[iTargetNum].NextEntityState.iInAltWeaponMode,
						CEntity[iTargetNum].vOrigin - CEntity[CG->PredictedPlayerState.iClientNum].vOrigin);
				}
			}
		}

		iCounter++;
	}
	/*
	//=====================================================================================
	*/
	void cHostMenu::SpawnBots(int count)
	{
		int iCurrentIndex = 0;

		auto AddEntities = [&]()
		{
			for (auto& SpawnedBot : vSpawnedBots)
			{
				AddEntity(SpawnedBot);
			}
		};

		auto SpawnTestClients = [&]()
		{
			for (auto& SpawnedBot : vSpawnedBots)
			{
				SpawnTestClient(SpawnedBot);
			}
		};

		auto AddTestClients = [&]()
		{
			vSpawnedBots.clear();

			for (int i = 0; i < count; iCurrentIndex++)
			{
				if (CharacterInfo[iCurrentIndex].iInfoValid)
					continue;

				vSpawnedBots.push_back(AddTestClient(TC_NONE, TEAM_FREE, iCurrentIndex, sEntRef(iCurrentIndex, 0)));

				i++;
			}
		};

		AddTestClients();
		AddEntities();
		SpawnTestClients();
	}
	/*
	//=====================================================================================
	*/
	void cHostMenu::DrawHostMenu()
	{
		if (HostMenu.bWriteLog)
		{
			ImGui::LogToFile();
			HostMenu.bWriteLog = false;
		}

		ImGui::SetNextWindowSize(ImVec2(484.0f, 650.0f));
		ImGui::Begin("HOST MENU", &HostMenu.bShowWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

		for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
			if (CharacterInfo[i].iInfoValid)
				HostMenu.vPlayers.push_back(std::make_pair(ClientInfo[i].iClientNum, ClientInfo[i].szName));

		ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
		if (ImGui::BeginCombo("", ClientInfo[HostMenu.iPlayer].szName))
		{
			HostMenu.GetPlayerSelection();
			ImGui::EndCombo();

			HostMenu.bWriteLog = true;
		} ImGui::PopItemWidth();

		ImGui::BeginChild("ContentRegion", ImVec2(0.0f, 0.0f), true);
		ImGui::Separator();
		ImGui::NewLine();

		if (ImGui::Checkbox("God Mode", &HostMenu.PlayerMod[HostMenu.iPlayer].bGodMode))
		{
			HostMenu.bWriteLog = true;
		} ImGui::SameLine(292.0f);

		if (ImGui::Checkbox("No Clip", &HostMenu.PlayerMod[HostMenu.iPlayer].bNoClip))
		{
			HostMenu.bWriteLog = true;
		} ImGui::NewLine();

		if (ImGui::Checkbox("Infinite Ammo", &HostMenu.PlayerMod[HostMenu.iPlayer].bInfiniteAmmo))
		{
			HostMenu.bWriteLog = true;
		} ImGui::SameLine(292.0f);

		if (ImGui::Checkbox("Invisibility", &HostMenu.PlayerMod[HostMenu.iPlayer].bInvisibility))
		{
			HostMenu.bWriteLog = true;
		} ImGui::NewLine();

		if (ImGui::Checkbox("Super Speed", &HostMenu.PlayerMod[HostMenu.iPlayer].bSuperSpeed))
		{
			HostMenu.bWriteLog = true;
		} ImGui::SameLine(292.0f);

		if (ImGui::Checkbox("Freeze Position", &HostMenu.PlayerMod[HostMenu.iPlayer].bFreezePosition))
		{
			HostMenu.bWriteLog = true;
		} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

		if (ImGui::InputText("Name", PlayerState[HostMenu.iPlayer].ClientState.szName, 16))
		{
			HostMenu.bWriteLog = true;
		}

		if (ImGui::Combo("Team", (int*)&PlayerState[HostMenu.iPlayer].ClientState.iTeam, HostMenu.szTeam.data(), TEAM_MAX))
		{
			TeamChanged(ClientInfo[HostMenu.iPlayer].iClientNum);
			HostMenu.bWriteLog = true;
		}

		if (ImGui::InputInt("Health", &GEntity[HostMenu.iPlayer].iHealth))
		{
			HostMenu.bWriteLog = true;
		} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

		for (auto& WeaponID : vWeaponIDs)
		{
			char szComplete[1024] = { NULL }, szDisplay[1024] = { NULL };

			GetWeaponNameComplete(WeaponID, false, szComplete, 1024);
			GetWeaponDisplayName(WeaponID, false, szDisplay, 1024);

			HostMenu.vWeaponCompleteNames.push_back(VariadicText(acut::StripColorCodes(szComplete)));
			HostMenu.vWeaponDisplayNames.push_back(VariadicText(acut::StripColorCodes(szDisplay)));
		}

		if (ImGui::Combo("Weapon", &HostMenu.iWeaponID, HostMenu.vWeaponDisplayNames.data(), (int)vWeaponIDs.size()))
		{
			HostMenu.bWriteLog = true;
		} HostMenu.vWeaponCompleteNames.clear(); HostMenu.vWeaponDisplayNames.clear();

		if (ImGui::Button("Give Weapon", ImVec2(150.0f, 25.0f)))
		{
			TakePlayerWeapon(&PlayerState[HostMenu.iPlayer], GetViewmodelWeapon(&PlayerState[HostMenu.iPlayer]));
			GivePlayerWeapon(&PlayerState[HostMenu.iPlayer], vWeaponIDs[HostMenu.iWeaponID], false, false, true);
			GameSendServerCommand(HostMenu.iPlayer, SV_CMD_RELIABLE, VariadicText("a %i", vWeaponIDs[HostMenu.iWeaponID]));
			AddAmmo(&PlayerState[HostMenu.iPlayer], vWeaponIDs[HostMenu.iWeaponID], false, 255, true);

			HostMenu.bWriteLog = true;
		} ImGui::SameLine(0.0f, 4.0f);

		if (ImGui::Button("Take Weapon", ImVec2(150.0f, 25.0f)))
		{
			TakePlayerWeapon(&PlayerState[HostMenu.iPlayer], GetViewmodelWeapon(&PlayerState[HostMenu.iPlayer]));
			GameSendServerCommand(HostMenu.iPlayer, SV_CMD_RELIABLE, VariadicText("a %i", 0));

			HostMenu.bWriteLog = true;
		} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

		if (ImGui::Combo("Perk", &HostMenu.iPerkID, szPerkIDs.data(), (int)vPerkIDs.size()))
		{
			HostMenu.bWriteLog = true;
		}

		if (ImGui::Button("Enable Perk", ImVec2(150.0f, 25.0f)))
		{
			EnablePerk(HostMenu.iPlayer, vPerkIDs[HostMenu.iPerkID]);

			HostMenu.bWriteLog = true;
		} ImGui::SameLine(0.0f, 4.0f);

		if (ImGui::Button("Disable Perk", ImVec2(150.0f, 25.0f)))
		{
			DisablePerk(HostMenu.iPlayer, vPerkIDs[HostMenu.iPerkID]);

			HostMenu.bWriteLog = true;
		} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

		if (ImGui::SliderFloat3("Position", PlayerState[HostMenu.iPlayer].vOrigin, -8192.0f, 8192.0f, "%.0f"))
		{
			HostMenu.bWriteLog = true;
		}

		if (ImGui::Button("Teleport To", ImVec2(98.0f, 25.0f)))
		{
			PlayerState[CG->PredictedPlayerState.iClientNum].vOrigin = PlayerState[HostMenu.iPlayer].vOrigin;
			PlayerState[CG->PredictedPlayerState.iClientNum].vVelocity = ImVec3(0.0f, 0.0f, 0.0f);
			PlayerState[CG->PredictedPlayerState.iClientNum].iGravity = 0;

			HostMenu.bWriteLog = true;
		} ImGui::SameLine(0.0f, 4.0f);

		if (ImGui::Button("Teleport From", ImVec2(98.0f, 25.0f)))
		{
			PlayerState[HostMenu.iPlayer].vOrigin = PlayerState[CG->PredictedPlayerState.iClientNum].vOrigin;
			PlayerState[HostMenu.iPlayer].vVelocity = ImVec3(0.0f, 0.0f, 0.0f);
			PlayerState[HostMenu.iPlayer].iGravity = 0;

			HostMenu.bWriteLog = true;
		} ImGui::SameLine(0.0f, 4.0f);
		
		if (ImGui::Button("Launch", ImVec2(100.0f, 25.0f)))
		{
			PlayerState[HostMenu.iPlayer].vVelocity.z = 10000.0f;
			HostMenu.bWriteLog = true;
		} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

		if (ImGui::Button("Kick", ImVec2(304.0f, 25.0f)))
		{
			GameSendServerCommand(HostMenu.iPlayer, SV_CMD_RELIABLE, VariadicText("r %s", HostMenu.szKickReason));
			HostMenu.bWriteLog = true;
		}

		if (ImGui::InputText("Reason", HostMenu.szKickReason, 64))
		{
			HostMenu.bWriteLog = true;
		} ImGui::NewLine(); ImGui::Separator(); ImGui::EndChild();

		HostMenu.vPlayers.clear();

		ImGui::End();
	}
}

//=====================================================================================