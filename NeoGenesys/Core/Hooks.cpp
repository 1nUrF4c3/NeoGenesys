//=====================================================================================

#include "../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cHooks _hooks;

	void cHooks::Refresh(int localnum)
	{
		dwThreadLocalStoragePointer = Sys_GetValue(3);

		if (LocalClientIsInGame() && CG->PlayerState.iOtherFlags & 0x4000)
		{
			_targetList.GetInformation();

			if (!IsPlayerReloading() && !WeaponBothClipEmpty(&CG->PlayerState))
				_aimBot.StandardAim();

			_removals.RecoilCompensation();
		}

		_host.PlayerMods();
	}
	/*
	//=====================================================================================
	*/
	void cHooks::WritePacket(int localnum)
	{
		if (LocalClientIsInGame() && CG->PlayerState.iOtherFlags & 0x4000)
		{
			sUserCmd* pCurrentCmd = ClientActive->GetUserCmd(ClientActive->iCurrentCmd);

			_packets.WritePacket(pCurrentCmd);
		}
	}
	/*
	//=====================================================================================
	*/
	void cHooks::CreateNewCommands(int localnum)
	{
		if (LocalClientIsInGame() && CG->PlayerState.iOtherFlags & 0x4000)
		{
			sUserCmd* pOldCmd = ClientActive->GetUserCmd(ClientActive->iCurrentCmd - 1);
			sUserCmd* pNewCmd = ClientActive->GetUserCmd(ClientActive->iCurrentCmd);

			if (_mainGui.GetKeyPress(VK_DELETE, true))
				*pNewCmd = *pOldCmd;

			*pOldCmd = *pNewCmd;
			--pOldCmd->iServerTime;

			_packets.CreateNewCommands(pOldCmd, pNewCmd);
		}
	}
	/*
	//=====================================================================================
	*/
	void cHooks::BulletFirePenetrate(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sGEntity* attacker, int servertime)
	{
		if (LocalClientIsInGame())
		{
			if (bp->iEntityNum == CG->PlayerState.iClientNum)
			{
				sUserCmd* pUserCmd = ClientActive->GetUserCmd(ClientActive->iCurrentCmd - !WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)));
				int iSeed = _removals.TransformSeed(WeaponIsAkimbo(GetViewmodelWeapon(&CG->PlayerState)) && pUserCmd->iButtons & (IsGamePadEnabled() ? BUTTON_FIRERIGHT : BUTTON_FIRELEFT), pUserCmd->iServerTime);

				Vector3 vAngles, vForward, vRight, vUp;
				VectorCopy(_aimBot.AimState.vAimAngles, vAngles);

				vAngles[0] += WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? CG->vRefDefViewAngles[0] : IsThirdPersonMode(&CG->PlayerState) ? CG->vThirdPersonViewAngles[0] : CG->vWeaponAngles[0];
				vAngles[1] += WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? CG->vRefDefViewAngles[1] : IsThirdPersonMode(&CG->PlayerState) ? CG->vThirdPersonViewAngles[1] : CG->vWeaponAngles[1];

				AngleVectors(_profiler.gSilentAim->Custom.bValue && _aimBot.AimState.bIsAutoAiming ? vAngles : WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? CG->vRefDefViewAngles : IsThirdPersonMode(&CG->PlayerState) ? CG->vThirdPersonViewAngles : CG->vWeaponAngles, vForward, vRight, vUp);
				BulletEndPosition(&iSeed, _removals.GetWeaponSpread() * _profiler.gSpreadFactor->Custom.flValue, bp->vStart, bp->vEnd, bp->vDir, vForward, vRight, vUp);
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
			if (_profiler.gThirdPersonAntiAim->Custom.bValue && _antiAim.IsAntiAiming() && !_mainGui.GetKeyPress(VK_DELETE, true))
			{
				if (entity->NextEntityState.iEntityNum == CG->PlayerState.iClientNum)
				{
					CharacterInfo[entity->NextEntityState.iEntityNum].vViewAngles[0] = _antiAim.vAntiAimAngles[0] + CG->PlayerState.vDeltaAngles[0];
					entity->vViewAngles[1] = _antiAim.vAntiAimAngles[1] + CG->PlayerState.vDeltaAngles[1];
				}
			}
		}
	}
	/*
	//=====================================================================================
	*/
	void cHooks::Obituary(int localnum, sEntityState* entitystate, int weapon)
	{
		if (LocalClientIsInGame())
		{
			if (entitystate->iAttackerEntityNum == CG->PlayerState.iClientNum)
			{
				if (_profiler.gTeaBag->Custom.bValue && *(int*)OFF_ISCURRENTHOST)
				{
					_packets.iTeaBagTime = clock();
					VectorCopy(PlayerState[entitystate->iOtherEntityNum].vOrigin, _packets.vTeaBagPos);

					std::string szTeaBag = _profiler.gTeaBagMessage->Custom.szValue;

					if (!szTeaBag.empty())
					{
						szTeaBag = acut::FindAndReplaceString(szTeaBag, "%attacker", ClientInfo[entitystate->iAttackerEntityNum].szName);
						szTeaBag = acut::FindAndReplaceString(szTeaBag, "%victim", ClientInfo[entitystate->iOtherEntityNum].szName);

						Say(&GEntity[entitystate->iOtherEntityNum], NULL, 0, szTeaBag);
					}
				}

				if (_profiler.gKillSpam->Custom.bValue)
				{
					std::string szKillSpam = _profiler.gKillSpamMessage->Custom.szValue;

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

				if (_profiler.gNameStealer->Custom.bValue)
				{
					strncpy_s((LPSTR)(*(DWORD_PTR*)OFF_STEAMAPI + OFF_STEAMNAME), 
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
	void cHooks::AddCmdDrawText(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, RGBA color, int flags)
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
		if (LocalClientIsInGame())
			_host.MassKill();
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
				ExceptionInfo->ContextRecord->Rax = dwThreadLocalStoragePointer;
				ExceptionInfo->ContextRecord->Rip += 0x4;

				return EXCEPTION_CONTINUE_EXECUTION;
			}
		}

		return EXCEPTION_CONTINUE_SEARCH;
	}
}

//=====================================================================================