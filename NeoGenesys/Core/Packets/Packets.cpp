//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cPackets _packets;

	void cPackets::ModifyCommands()
	{
		sUserCMD* pOldCMD = ClientActive->GetUserCMD(ClientActive->iCurrentCMD);
		++ClientActive->iCurrentCMD;

		sUserCMD* pNewCMD = ClientActive->GetUserCMD(ClientActive->iCurrentCMD);
		*pNewCMD = *pOldCMD;
		--pOldCMD->iServerTime;

		OldCommand(pOldCMD);
		NewCommand(pNewCMD);
	}
	/*
	//=====================================================================================
	*/
	void cPackets::OldCommand(sUserCMD* usercmd)
	{
		_aimBot.SilentAim(usercmd);

		if (_profiler.gSilentAim->Custom.bValue && !WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)))
			_removals.SpreadCompensationSilentAim(usercmd, WeaponIsAkimbo(GetViewmodelWeapon(&CG->PlayerState)) && usercmd->iButtons & BUTTON_FIRELEFT);

		else
			_removals.SpreadCompensationStandardAim(usercmd, WeaponIsAkimbo(GetViewmodelWeapon(&CG->PlayerState)) && usercmd->iButtons & BUTTON_FIRELEFT);
	}
	/*
	//=====================================================================================
	*/
	void cPackets::NewCommand(sUserCMD* usercmd)
	{
		_antiAim.AntiAim(usercmd);
	}
}

//=====================================================================================