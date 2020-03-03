//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cPackets _packets;

	void cPackets::WritePacket(sUserCmd* currentcmd)
	{
		_antiAim.AntiAim(currentcmd);
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