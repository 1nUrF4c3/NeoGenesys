//=====================================================================================

#include "../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cHooks _hooks;

	void cHooks::Refresh(int localnum)
	{
		if (LocalClientIsInGame() && CG->PlayerState.iOtherFlags & 0x4000)
		{
			_targetList.GetInformation();

			if (!WeaponBothClipEmpty(&CG->PlayerState))
			{
				_aimBot.StandardAim();
				_removals.RecoilCompensation();
			}
		}

		_host.PlayerModThread();
	}
	/*
	//=====================================================================================
	*/
	void cHooks::CreateNewCommands(int localnum)
	{
		if (LocalClientIsInGame() && CG->PlayerState.iOtherFlags & 0x4000)
		{
			sUserCMD* pOldCMD = ClientActive->GetUserCMD(ClientActive->iCurrentCMD - 1);
			sUserCMD* pNewCMD = ClientActive->GetUserCMD(ClientActive->iCurrentCMD);

			*pOldCMD = *pNewCMD;
			--pOldCMD->iServerTime;

			if (!WeaponBothClipEmpty(&CG->PlayerState))
			{
				_packets.OldCommand(pOldCMD);
				_aimBot.AutoFire(WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? pNewCMD : pOldCMD);
			}
		}
	}
	/*
	//=====================================================================================
	*/
	void cHooks::WritePacket(int localnum)
	{
		if (LocalClientIsInGame() && CG->PlayerState.iOtherFlags & 0x4000)
		{
			sUserCMD* pUserCMD = ClientActive->GetUserCMD(ClientActive->iCurrentCMD);
			_packets.NewCommand(pUserCMD);
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
				int iSeed = _removals.TransformSeed(WeaponIsAkimbo(GetViewmodelWeapon(&CG->PlayerState)) && 
					ClientActive->GetUserCMD(ClientActive->iCurrentCMD - !WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)))->iButtons & BUTTON_FIRELEFT, 
					ClientActive->GetUserCMD(ClientActive->iCurrentCMD - !WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)))->iServerTime);

				Vector3 vAngles, vForward, vRight, vUp;
				VectorCopy(_aimBot.AimState.vAimbotAngles, vAngles);

				vAngles[0] += FindVariable("camera_thirdPerson")->Current.bValue && !WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? CG->vThirdPersonViewAngles[0] : CG->vRefDefViewAngles[0];
				vAngles[1] += FindVariable("camera_thirdPerson")->Current.bValue && !WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? CG->vThirdPersonViewAngles[1] : CG->vRefDefViewAngles[1];

				AngleVectors(_profiler.gSilentAim->Custom.bValue && _aimBot.AimState.bIsAutoAiming ?
					vAngles : FindVariable("camera_thirdPerson")->Current.bValue && !WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? CG->vThirdPersonViewAngles : CG->vRefDefViewAngles, vForward, vRight, vUp);

				BulletEndPosition(&iSeed, _removals.GetWeaponSpread() * (1.0f - _profiler.gSpreadFactor->Custom.flValue), RefDef->vViewOrg,
					bp->vEnd, bp->vDir, vForward, vRight, vUp);
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
				if (_profiler.gKillSpam->Custom.bValue)
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

				if (_profiler.gNameStealer->Custom.bValue)
				{
					strncpy_s((LPSTR)(*(DWORD_PTR*)OFF_STEAMAPI + OFF_STEAMNAME), 
						strlen(ClientInfo[entitystate->iOtherEntityNum].szName) + 1, 
						ClientInfo[entitystate->iOtherEntityNum].szName, 32);
					
					Cbuf_AddText(VariadicText("name %s\n", ClientInfo[entitystate->iOtherEntityNum].szName));
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
			strcpy_s(szInvalidText, sizeof("crash"), "crash");
	}
	/*
	//=====================================================================================
	*/
	void cHooks::ClientFrame(sGEntity* entity)
	{
		if (LocalClientIsInGame())
			_host.MassKill();
	}
}

//=====================================================================================