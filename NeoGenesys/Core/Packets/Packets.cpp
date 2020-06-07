//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cPackets _packets;

	void cPackets::WritePacket(sUserCmd* usercmd)
	{
		if (WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)))
		{
			_aimBot.SilentAim(usercmd);
			_aimBot.AutoFire(usercmd);
		}

		_antiAim.AntiAim(usercmd);

		if (gBunnyHop->Custom.bValue && _mainGui.GetKeyPress(VK_SPACE, true))
		{
			if (usercmd->iButtons & BUTTON_JUMP)
				usercmd->iButtons &= ~BUTTON_JUMP;

			else
				usercmd->iButtons |= BUTTON_JUMP;
		}

		if (Sys_Milliseconds() - iTeaBagTime < 3000)
		{
			PlayerState[CG->PredictedPlayerState.iClientNum].vOrigin = vTeaBagPos;

			if (usercmd->iButtons & BUTTON_CROUCH)
				usercmd->iButtons &= ~BUTTON_CROUCH;

			else
				usercmd->iButtons |= BUTTON_CROUCH;
		}
	}
	/*
	//=====================================================================================
	*/
	void cPackets::PredictPlayerState(sUserCmd* usercmd)
	{
		if (!WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) && !IsPlayerReloading() && !WeaponBothClipEmpty(&CG->PredictedPlayerState))
		{
			_aimBot.SilentAim(usercmd);
			_aimBot.AutoFire(usercmd);
		}

		if (_aimBot.gSilentAim->Custom.bValue)
			_removals.SpreadCompensationSilentAim(usercmd, WeaponIsAkimbo(GetViewmodelWeapon(&CG->PredictedPlayerState)) && usercmd->iButtons & (IsGamePadEnabled() ? BUTTON_FIRERIGHT : BUTTON_FIRELEFT));

		else
			_removals.SpreadCompensationStandardAim(usercmd, WeaponIsAkimbo(GetViewmodelWeapon(&CG->PredictedPlayerState)) && usercmd->iButtons & (IsGamePadEnabled() ? BUTTON_FIRERIGHT : BUTTON_FIRELEFT));
	}
}

//=====================================================================================