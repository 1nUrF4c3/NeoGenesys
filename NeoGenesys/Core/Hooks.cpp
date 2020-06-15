//=====================================================================================

#include "../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cHooks _hooks;

	void cHooks::Refresh(int localnum)
	{
		dwSysValue = Sys_GetValue(3);

		if (LocalClientIsInGame() && CG->PredictedPlayerState.iOtherFlags & 0x4000)
		{
			_targetList.GetInformation();
			_drawing.CalculateTracers();
			_aimBot.SetAimState();

			if (WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) || (!IsPlayerReloading() && !WeaponBothClipEmpty(&CG->PredictedPlayerState)))
				_aimBot.StandardAim();

			_removals.RecoilCompensation();
		}

		else
			_drawing.vTracers.clear();

		_hostMenu.PlayerMods();
	}
	/*
	//=====================================================================================
	*/
	void cHooks::WritePacket(int localnum)
	{
		if (LocalClientIsInGame() && CG->PredictedPlayerState.iOtherFlags & 0x4000)
		{
			sUserCmd* pCurrentCmd = ClientActive->GetUserCmd(ClientActive->iCurrentCmd);

			_packets.WritePacket(pCurrentCmd);
		}
	}
	/*
	//=====================================================================================
	*/
	void cHooks::PredictPlayerState(int localnum)
	{
		if (LocalClientIsInGame() && CG->PredictedPlayerState.iOtherFlags & 0x4000)
		{
			static int iBackupAngles[3];

			sUserCmd* pOldCmd = ClientActive->GetUserCmd(ClientActive->iCurrentCmd - 1);
			sUserCmd* pCurrentCmd = ClientActive->GetUserCmd(ClientActive->iCurrentCmd);
			sUserCmd* pNewCmd = ClientActive->GetUserCmd(ClientActive->iCurrentCmd + 1);

			if (_mainGui.GetKeyPress(VK_DELETE, true))
				CopyMemory(pCurrentCmd, pOldCmd, sizeof(sUserCmd));

			CopyMemory(pNewCmd, pCurrentCmd, sizeof(sUserCmd));
			++ClientActive->iCurrentCmd;
			
			pOldCmd->iViewAngles[0] = iBackupAngles[0];
			pOldCmd->iViewAngles[1] = iBackupAngles[1];
			pOldCmd->iViewAngles[2] = iBackupAngles[2];

			iBackupAngles[0] = pCurrentCmd->iViewAngles[0];
			iBackupAngles[1] = pCurrentCmd->iViewAngles[1];
			iBackupAngles[2] = pCurrentCmd->iViewAngles[2];

			++pOldCmd->iServerTime;
			--pCurrentCmd->iServerTime;

			_packets.PredictPlayerState(pOldCmd);
		}
	}
	/*
	//=====================================================================================
	*/
	void cHooks::BulletFirePenetrate(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sCEntity* attacker, int servertime)
	{
		if (LocalClientIsInGame())
		{
			if (bp->iEntityNum == CG->PredictedPlayerState.iClientNum)
			{
				sUserCmd* pUserCmd = ClientActive->GetUserCmd(ClientActive->iCurrentCmd - !WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)));
				int iSeed = _removals.TransformSeed(WeaponIsAkimbo(GetViewmodelWeapon(&CG->PredictedPlayerState)) && pUserCmd->iButtons & (IsGamePadEnabled() ? BUTTON_FIRERIGHT : BUTTON_FIRELEFT), pUserCmd->iServerTime);

				ImVec3 vAngles, vForward, vRight, vUp;
				vAngles = _aimBot.AimState.vAimAngles;

				vAngles += WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? CG->vRefDefViewAngles : IsThirdPersonMode(&CG->PredictedPlayerState) ? CG->vThirdPersonViewAngles : CG->vWeaponAngles;

				AngleVectors(_aimBot.gSilentAim->Current.bValue && _aimBot.AimState.bIsAutoAiming ? vAngles : WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? CG->vRefDefViewAngles : IsThirdPersonMode(&CG->PredictedPlayerState) ? CG->vThirdPersonViewAngles : CG->vWeaponAngles, &vForward, &vRight, &vUp);
				BulletEndPosition(&iSeed, _removals.GetWeaponSpread() * _removals.gSpreadFactor->Current.flValue, bp->vStart, &bp->vEnd, &bp->vDir, vForward, vRight, vUp);
			}
		}
	}
	/*
	//=====================================================================================
	*/
	void cHooks::BulletHitEvent(int localnum, int sourcenum, int targetnum, int weapon, bool alternate, ImVec3 start, ImVec3 position, ImVec3 normal, int surface, int _event, char param, int contents)
	{
		if (LocalClientIsInGame() && CG->PredictedPlayerState.iOtherFlags & 0x4000)
		{
			if (gPlayerBulletTracers->Current.bValue)
			{
				if (sourcenum == CG->PredictedPlayerState.iClientNum &&
					_targetList.EntityIsEnemy(targetnum) &&
					_aimBot.AimState.bIsAutoFiring &&
					(CEntity[targetnum].NextEntityState.iEntityType == ET_PLAYER ||
					(_targetList.gTargetMissiles->Current.bValue && CEntity[targetnum].NextEntityState.iEntityType == ET_MISSILE &&
					(CEntity[targetnum].NextEntityState.iWeapon == WEAPON_C4 || CEntity[targetnum].NextEntityState.iWeapon == WEAPON_IED)) ||
					(_targetList.gTargetAgents->Current.bValue && CEntity[targetnum].NextEntityState.iEntityType == ET_AGENT)))
				{
					ImVec3 vTracerStart;
					GetPlayerViewOrigin(&CG->PredictedPlayerState, &vTracerStart);

					sOrientation Orientation;
					sUserCmd* pUserCmd = ClientActive->GetUserCmd(ClientActive->iCurrentCmd - !WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)));

					if (GetTagOrientation((WeaponIsAkimbo(GetViewmodelWeapon(&CG->PredictedPlayerState)) && pUserCmd->iButtons & (IsGamePadEnabled() ? BUTTON_FIRERIGHT : BUTTON_FIRELEFT)) + 2048, RegisterTag("tag_flash"), &Orientation))
					{
						cDrawing::sTracer Tracer;

						if (IsThirdPersonMode(&CG->PredictedPlayerState))
							vTracerStart += (Orientation.vAxis[0] * 30.0f);
						else
							vTracerStart = Orientation.vOrigin;

						Tracer.iStartTime = Sys_Milliseconds();
						Tracer.vStartPos3D = vTracerStart;
						Tracer.vHitPos3D = position;

						Tracer.cColorShadow = _drawing.gColorShadow->Current.cValue;
						Tracer.cColorHitMarker = _drawing.gColorText->Current.cValue;
						Tracer.cColorTracer = _drawing.gColorAccents->Current.cValue;

						Tracer.flAlphaShadow = _drawing.gColorShadow->Current.cValue.w;
						Tracer.flAlphaHitMarker = _drawing.gColorText->Current.cValue.w;
						Tracer.flAlphaTracer = _drawing.gColorAccents->Current.cValue.w;

						_drawing.vTracers.push_back(Tracer);
					}
				}
			}
		}
	}
	/*
	//=====================================================================================
	*/
	void cHooks::CalcEntityLerpPositions(int localnum, sCEntity* entity)
	{
		if (LocalClientIsInGame())
		{
			if ((FindVariable("camera_thirdPerson")->Current.bValue = gThirdPerson->Current.bValue) && _antiAim.IsAntiAiming() && !_mainGui.GetKeyPress(VK_DELETE, true))
			{
				if (entity->NextEntityState.iEntityNum == CG->PredictedPlayerState.iClientNum)
				{
					CharacterInfo[entity->NextEntityState.iEntityNum].vViewAngles.x = _antiAim.vAntiAimAngles.x + CG->PredictedPlayerState.vDeltaAngles.x;
					entity->vViewAngles.y = _antiAim.vAntiAimAngles.y + CG->PredictedPlayerState.vDeltaAngles.y;
				}
			}

			_mathematics.ApplyPositionPrediction(entity);
			_mathematics.ApplyAnglePrediction(entity);
		}
	}
	/*
	//=====================================================================================
	*/
	void cHooks::Obituary(int localnum, sEntityState* entitystate, int weapon)
	{
		if (LocalClientIsInGame())
		{
			if (entitystate->iAttackerEntityNum == CG->PredictedPlayerState.iClientNum)
			{
				if (gTeaBag->Current.bValue && IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum))
				{
					_packets.iTeaBagTime = Sys_Milliseconds();
					_packets.vTeaBagPos = PlayerState[entitystate->iOtherEntityNum].vOrigin;

					std::string szTeaBag = gTeaBagMessage->Current.szValue;

					if (!szTeaBag.empty())
					{
						szTeaBag = acut::FindAndReplaceString(szTeaBag, "%attacker", ClientInfo[entitystate->iAttackerEntityNum].szName);
						szTeaBag = acut::FindAndReplaceString(szTeaBag, "%victim", ClientInfo[entitystate->iOtherEntityNum].szName);

						Say(&GEntity[entitystate->iOtherEntityNum], NULL, 0, szTeaBag);
					}
				}

				if (gKillSpam->Current.bValue)
				{
					std::string szKillSpam = gKillSpamMessage->Current.szValue;

					if (!szKillSpam.empty())
					{
						szKillSpam = acut::FindAndReplaceString(szKillSpam, "%attacker", ClientInfo[entitystate->iAttackerEntityNum].szName);
						szKillSpam = acut::FindAndReplaceString(szKillSpam, "%victim", ClientInfo[entitystate->iOtherEntityNum].szName);

						Cbuf_AddText(VariadicText("say \"%s\"\n", szKillSpam.c_str()));
					}

					else
					{
						int iWeapon = 0;

						if ((BYTE)weapon >= 160)
							iWeapon = (BYTE)weapon - 160;

						char szIcon[64] = { NULL };
						int iLength = AddMessageIcon(szIcon, 0, iWeapon == 9 ? RegisterShader("killiconheadshot") : RegisterShader("killicondied"), 1.4f, 1.4f, false);

						szIcon[iLength] = NULL;

						Cbuf_AddText(VariadicText("say \"^5%s ^7- Get ^6%s ^7courtesy of ^6Neo^0Genesys\"\n",
							ClientInfo[entitystate->iOtherEntityNum].szName, szIcon));
					}
				}

				if (gNameStealer->Current.bValue)
				{
					strncpy_s((LPSTR)FindDmaAddy(OFF_STEAMAPI, std::vector<DWORD_PTR>({ OFF_STEAMNAME })),
						strlen(ClientInfo[entitystate->iOtherEntityNum].szName) + 1, 
						ClientInfo[entitystate->iOtherEntityNum].szName, 32);
					
					Cbuf_AddText(VariadicText("name \"%s\"\n", ClientInfo[entitystate->iOtherEntityNum].szName));
				}
			}
		}
	}
	/*
	//=====================================================================================
	*/
	void cHooks::AddCmdDrawText(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, ImVec4 color, int flags)
	{
		LPSTR szInvalidText;

		if (szInvalidText = strstr(text, "\x5E\x01\x3D\x3D\xFF"))
			strcpy_s(szInvalidText, strlen("crash") + 1, "crash");
	}
	/*
	//=====================================================================================
	*/
	void cHooks::ClientFrame(sGEntity* entity)
	{
		if (LocalClientIsInGame() && IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum))
		{
			_hostMenu.GravityGun();
			_hostMenu.MassKill();

			for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
				if (gAntiLeave->Current.bValue && i != CG->PredictedPlayerState.iClientNum)
					GameSendServerCommand(i, SV_CMD_RELIABLE, "o 11 1");
		}
	}
	/*
	//=====================================================================================
	*/
	LONG cHooks::VectoredExceptionHandler(_In_ LPEXCEPTION_POINTERS ExceptionInfo)
	{
		if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
		{
			if (ExceptionInfo->ContextRecord->Rip == OFF_SYSGETVALUEEXCEPTION)
			{
				ExceptionInfo->ContextRecord->Rax = dwSysValue;
				ExceptionInfo->ContextRecord->Rip += 0x4;

				return EXCEPTION_CONTINUE_EXECUTION;
			}

			else
			{
				_console.AddLog("%s STATUS_ACCESS_VIOLATION @ 0x%llX", PREFIX_WARNING, ExceptionInfo->ExceptionRecord->ExceptionAddress);
				Com_Error(ERR_DROP, "STATUS_ACCESS_VIOLATION @ 0x%llX", ExceptionInfo->ExceptionRecord->ExceptionAddress);

				return EXCEPTION_CONTINUE_EXECUTION;
			}
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}
}

//=====================================================================================