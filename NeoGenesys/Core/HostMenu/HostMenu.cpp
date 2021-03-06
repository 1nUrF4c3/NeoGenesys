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
					PlayerState[i].vVelocity *= 0.0f;
					PlayerState[i].iGravity = 0;
				}

				else
				{
					HostMenu.PlayerMod[i].vPosition = PlayerState[i].vOrigin;
				}
			}

			if (!CharacterInformation[i].iInfoValid)
			{
				HostMenu.PlayerMod[i].bGodMode = false;
				HostMenu.PlayerMod[i].bNoClip = false;
				HostMenu.PlayerMod[i].bInfiniteAmmo = false;
				HostMenu.PlayerMod[i].bInvisibility = false;
				HostMenu.PlayerMod[i].bSuperSpeed = false;
				HostMenu.PlayerMod[i].bFreezePosition = false;
			}

			if ((LocalClientIsInGame() ? !CharacterInformation[i].iInfoValid : !IsUserRegistered(GetCurrentSession(), i)))
			{
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
		if (gForgeMode->Current.iValue > FORGE_MODE_OFF)
		{
			if (LocalClientIsInGame() && IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum))
			{
				if (!(HostMenu.iGravityGunNum > -1 && _aimBot.AimState.bIsZooming && _targetList.EntityIsValid(HostMenu.iGravityGunNum) &&
					(CEntity[HostMenu.iGravityGunNum].NextEntityState.iEntityType == ET_PLAYER || CEntity[HostMenu.iGravityGunNum].NextEntityState.iEntityType == ET_AGENT)))
				{
					HostMenu.iGravityGunNum = -1;

					sTrace Trace;

					LocationalTrace(&Trace, GetViewOrigin(), _mathematics.AngleToForward(GetViewOrigin(), GetViewAngles(), 8192.0f), CG->PredictedPlayerState.iClientNum, MASK_NONKILLSTREAK);

					if (Trace.TraceHitType == TRACE_HITTYPE_ENTITY)
					{
						HostMenu.iGravityGunNum = GetTraceHitType(&Trace);
						HostMenu.flGravityGunDist = _mathematics.CalculateDistance3D(CEntity[HostMenu.iGravityGunNum].vOrigin, GetViewOrigin());
					}
				}

				if (HostMenu.iGravityGunNum > -1 && _aimBot.AimState.bIsZooming && _targetList.EntityIsValid(HostMenu.iGravityGunNum) &&
					(CEntity[HostMenu.iGravityGunNum].NextEntityState.iEntityType == ET_PLAYER || CEntity[HostMenu.iGravityGunNum].NextEntityState.iEntityType == ET_AGENT))
				{
					ImVec3 vCrosshair = _mathematics.AngleToForward(GetViewOrigin(), GetViewAngles(), HostMenu.flGravityGunDist);
					ImVec3 vOffset = _targetList.EntityList[HostMenu.iGravityGunNum].vBones3D[vBones[BONE_HEAD].first.first] - CEntity[HostMenu.iGravityGunNum].vOrigin;
					ImVec3 vTeleport = vCrosshair - vOffset;

					sPlayerState* pPlayerState = NULL;

					switch (CEntity[HostMenu.iGravityGunNum].NextEntityState.iEntityType)
					{
					case ET_PLAYER:
						pPlayerState = GEntity[HostMenu.iGravityGunNum].pClient;
						break;

					case ET_AGENT:
						pPlayerState = GEntity[HostMenu.iGravityGunNum].pAgent;
						break;
					}

					if (pPlayerState)
					{
						pPlayerState->vOrigin = vTeleport;
						pPlayerState->vVelocity *= 0.0f;
						pPlayerState->iGravity = 0;

						if (gForgeMode->Current.iValue == FORGE_MODE_LAUNCH)
						{
							sUserCmd* pUserCmd = ClientActive->GetUserCmd(ClientActive->iCurrentCmd - !WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)));

							if (pUserCmd->iButtons & BUTTON_FIRELEFT)
							{
								pPlayerState->vVelocity = _mathematics.AngleToForward(GetViewOrigin(), GetViewAngles(), 10000.0f) - GetViewOrigin();
								HostMenu.iGravityGunNum = -1;
								SetZoomState(false);
							}
						}
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
		ImVec3 vCrosshair = _mathematics.AngleToForward(GetViewOrigin(), GetViewAngles(), M_METERS);

		for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
		{
			if (_targetList.EntityIsValid(i))
			{
				ImVec3 vOffset = _targetList.EntityList[i].vBones3D[vBones[BONE_HEAD].first.first] - CEntity[i].vOrigin;
				ImVec3 vTeleport = vCrosshair - vOffset;

				PlayerState[i].vOrigin = vTeleport;
				PlayerState[i].vVelocity *= 0.0f;
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
			if (CharacterInformation[iTargetNum].iInfoValid && CharacterInformation[iTargetNum].iNextValid)
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
				if (CharacterInformation[iCurrentIndex].iInfoValid)
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
		ImGui::Begin("HOST MENU", &gHostMenu->Current.bValue, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

		for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
			if (CharacterInformation[i].iInfoValid)
				HostMenu.vPlayers.push_back(std::make_pair(i, ClientInformation[i].szName));

		ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
		if (ImGui::BeginCombo("", ClientInformation[HostMenu.iPlayer].szName))
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
			TeamChanged(ClientInformation[HostMenu.iPlayer].iClientNum);
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

			HostMenu.vWeaponCompleteNames.push_back(_strdup(acut::StripColorCodes(szComplete).c_str()));
			HostMenu.vWeaponDisplayNames.push_back(_strdup(acut::StripColorCodes(szDisplay).c_str()));
		}

		if (ImGui::Checkbox("", &HostMenu.bAkimbo))
		{
			HostMenu.bWriteLog = true;
		} ImGui::SameLine(0.0f, ImGui::GetStyle().FramePadding.x);

		ImGui::PushItemWidth(304.0f - ImGui::GetFrameHeight() - ImGui::GetStyle().FramePadding.x);
		if (ImGui::Combo("Weapon", &HostMenu.iWeaponID, HostMenu.vWeaponDisplayNames.data(), (int)vWeaponIDs.size()))
		{
			HostMenu.bWriteLog = true;
		} HostMenu.vWeaponCompleteNames.clear(); HostMenu.vWeaponDisplayNames.clear(); ImGui::PopItemWidth();

		if (ImGui::Button("Give Weapon", ImVec2(150.0f, 25.0f)))
		{
			std::string szWeaponID(HostMenu.vWeaponCompleteNames[HostMenu.iWeaponID]);

			if (HostMenu.bAkimbo)
				szWeaponID.append("+akimbo");

			int iWeaponID = GetWeaponForName(szWeaponID);

			TakePlayerWeapon(&PlayerState[HostMenu.iPlayer], GetViewmodelWeapon(&PlayerState[HostMenu.iPlayer]));
			GivePlayerWeapon(&PlayerState[HostMenu.iPlayer], iWeaponID, false, false, false);
			GameSendServerCommand(HostMenu.iPlayer, SV_CMD_RELIABLE, VariadicText("a %i", iWeaponID));
			AddAmmo(&PlayerState[HostMenu.iPlayer], iWeaponID, false, 200, true);

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
			PlayerState[CG->PredictedPlayerState.iClientNum].vVelocity *= 0.0f;
			PlayerState[CG->PredictedPlayerState.iClientNum].iGravity = 0;

			HostMenu.bWriteLog = true;
		} ImGui::SameLine(0.0f, 4.0f);

		if (ImGui::Button("Teleport From", ImVec2(98.0f, 25.0f)))
		{
			PlayerState[HostMenu.iPlayer].vOrigin = PlayerState[CG->PredictedPlayerState.iClientNum].vOrigin;
			PlayerState[HostMenu.iPlayer].vVelocity *= 0.0f;
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