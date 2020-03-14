//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cPackets _packets;

	void cPackets::WritePacket(sUserCmd* currentcmd)
	{
		_antiAim.ClientAntiAim(currentcmd);

		if (_profiler.gBunnyHop->Custom.bValue && _mainGui.GetKeyPress(VK_SPACE, true))
		{
			if (currentcmd->iButtons & BUTTON_JUMP)
				currentcmd->iButtons &= ~BUTTON_JUMP;

			else
				currentcmd->iButtons |= BUTTON_JUMP;
		}

		if (clock() - iTeaBagTime < 3000)
		{
			VectorCopy(vTeaBagPos, PlayerState[CG->PlayerState.iClientNum].vOrigin);

			if (currentcmd->iButtons & BUTTON_CROUCH)
				currentcmd->iButtons &= ~BUTTON_CROUCH;

			else
				currentcmd->iButtons |= BUTTON_CROUCH;
		}
	}
	/*
	//=====================================================================================
	*/
	void cPackets::PredictPlayerState(sUserCmd* oldcmd, sUserCmd* newcmd)
	{
		if (!IsPlayerReloading() && !WeaponBothClipEmpty(&CG->PlayerState))
		{
			_aimBot.SilentAim(oldcmd);

			if (!WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)))
				_aimBot.AutoFire(oldcmd);

			else
				_aimBot.AutoFire(newcmd);
		}

		if (_profiler.gSilentAim->Custom.bValue)
			_removals.SpreadCompensationSilentAim(oldcmd, WeaponIsAkimbo(GetViewmodelWeapon(&CG->PlayerState)) && oldcmd->iButtons & (IsGamePadEnabled() ? BUTTON_FIRERIGHT : BUTTON_FIRELEFT));

		else
			_removals.SpreadCompensationStandardAim(oldcmd, WeaponIsAkimbo(GetViewmodelWeapon(&CG->PlayerState)) && oldcmd->iButtons & (IsGamePadEnabled() ? BUTTON_FIRERIGHT : BUTTON_FIRELEFT));
	}
}

//=====================================================================================