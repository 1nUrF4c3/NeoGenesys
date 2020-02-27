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

			if (!IsPlayerReloading() && !WeaponBothClipEmpty(&CG->PlayerState))
				_aimBot.StandardAim();

			_removals.RecoilCompensation();
		}

		_host.PlayerModThread();
	}
	/*
	//=====================================================================================
	*/
	void cHooks::WritePacket(int localnum)
	{
		if (LocalClientIsInGame() && CG->PlayerState.iOtherFlags & 0x4000)
		{
			sUserCmd* pUserCmd = ClientActive->GetUserCmd(ClientActive->iCurrentCmd);

			_packets.WritePacket(pUserCmd);
		}
	}
	/*
	//=====================================================================================
	*/
	void cHooks::PredictPlayerState(int localnum)
	{
		if (LocalClientIsInGame() && CG->PlayerState.iOtherFlags & 0x4000)
		{
			sUserCmd* pOldCmd = ClientActive->GetUserCmd(ClientActive->iCurrentCmd - 1);
			sUserCmd* pNewCmd = ClientActive->GetUserCmd(ClientActive->iCurrentCmd);

			if (_mainGui.GetKeyPress(VK_DELETE, true))
				*pNewCmd = *pOldCmd;

			*pOldCmd = *pNewCmd;
			--pOldCmd->iServerTime;

			_packets.PredictPlayerState(pOldCmd, pNewCmd);
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
}

//=====================================================================================