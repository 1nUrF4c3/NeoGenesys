//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cProfiler _profiler;

	bool cProfiler::SaveProfile(const std::string& path)
	{
		try
		{
			XML.create_document();
			std::wstring szFilePath;

			if (path.empty())
				szFilePath = acut::AnsiToWstring(acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_XML, " ", ""), CP_ACP);

			else
				szFilePath = acut::AnsiToWstring(path, CP_ACP);

			XML.set("NeoGenesys.AimBot.Mode", _aimBot.gAimBotMode->Current.iValue);
			XML.set("NeoGenesys.AimBot.AutoZoom", _aimBot.gAutoZoom->Current.bValue);
			XML.set("NeoGenesys.AimBot.AutoFire", _aimBot.gAutoFire->Current.bValue);
			XML.set("NeoGenesys.AimBot.AutoWall", _targetList.gAutoWall->Current.bValue);
			XML.set("NeoGenesys.AimBot.SilentAim", _aimBot.gSilentAim->Current.bValue);
			XML.set("NeoGenesys.AimBot.TargetAgents", _targetList.gTargetAgents->Current.bValue);
			XML.set("NeoGenesys.AimBot.TargetMissiles", _targetList.gTargetMissiles->Current.bValue);
			XML.set("NeoGenesys.AimBot.AntiAim", _antiAim.gAntiAim->Current.iValue);
			XML.set("NeoGenesys.AimBot.BoneScan", _targetList.gBoneScan->Current.iValue);
			XML.set("NeoGenesys.AimBot.SortMethod", _targetList.gSortMethod->Current.iValue);

			XML.set("NeoGenesys.WallHack.Mode", _drawing.gWallHackMode->Current.iValue);
			XML.set("NeoGenesys.WallHack.Boxes", _drawing.gPlayerBoxes->Current.iValue);
			XML.set("NeoGenesys.WallHack.Bones", _drawing.gPlayerBones->Current.iValue);
			XML.set("NeoGenesys.WallHack.SnapLines", _drawing.gPlayerSnapLines->Current.iValue);
			XML.set("NeoGenesys.WallHack.Information", _drawing.gPlayerInformation->Current.bValue);
			XML.set("NeoGenesys.WallHack.Weapons", _drawing.gPlayerWeapons->Current.bValue);
			XML.set("NeoGenesys.WallHack.BulletTracers", _hooks.gPlayerBulletTracers->Current.bValue);
			XML.set("NeoGenesys.WallHack.Agents", _drawing.gAgents->Current.bValue);
			XML.set("NeoGenesys.WallHack.Missiles", _drawing.gMissiles->Current.bValue);
			XML.set("NeoGenesys.WallHack.Items", _drawing.gItems->Current.bValue);

			XML.set("NeoGenesys.Miscellaneous.KillSpam", _hooks.gKillSpam->Current.bValue);
			XML.set("NeoGenesys.Miscellaneous.NameStealer", _hooks.gNameStealer->Current.bValue);
			XML.set("NeoGenesys.Miscellaneous.ThirdPerson", _hooks.gThirdPerson->Current.bValue);
			XML.set("NeoGenesys.Miscellaneous.CrossHair", _drawing.gPlayerCrossHair->Current.bValue);
			XML.set("NeoGenesys.Miscellaneous.Compass", _drawing.gPlayerCompass->Current.bValue);
			XML.set("NeoGenesys.Miscellaneous.Radar", _drawing.gPlayerRadar->Current.bValue);
			XML.set("NeoGenesys.Miscellaneous.Console", _mainGui.gConsole->Current.bValue);
			XML.set("NeoGenesys.Miscellaneous.PlayerList", _playerList.gPlayerList->Current.bValue);
			XML.set("NeoGenesys.Miscellaneous.HostMenu", _hostMenu.gHostMenu->Current.bValue);
			XML.set("NeoGenesys.Miscellaneous.MemoryEditor", _mainGui.gMemoryEditor->Current.bValue);
			XML.set("NeoGenesys.Miscellaneous.RiotShield", _targetList.gRiotShielders->Current.iValue);

			XML.set("NeoGenesys.Tweaks.AimBone", _targetList.gAimBone->Current.iValue);
			XML.set("NeoGenesys.Tweaks.AimAngle", _targetList.gAimAngle->Current.iValue);
			XML.set("NeoGenesys.Tweaks.AimPower", _aimBot.gAimPower->Current.iValue);
			XML.set("NeoGenesys.Tweaks.AutoAimTime", _aimBot.gAutoAimTime->Current.iValue);
			XML.set("NeoGenesys.Tweaks.AutoAimDelay", _aimBot.gAutoAimDelay->Current.iValue);
			XML.set("NeoGenesys.Tweaks.AutoZoomDelay", _aimBot.gAutoZoomDelay->Current.iValue);
			XML.set("NeoGenesys.Tweaks.AutoFireDelay", _aimBot.gAutoFireDelay->Current.iValue);
			XML.set("NeoGenesys.Tweaks.RecoilFactor", _removals.gRecoilFactor->Current.flValue);
			XML.set("NeoGenesys.Tweaks.SpreadFactor", _removals.gSpreadFactor->Current.flValue);

			XML.set("NeoGenesys.Tweaks.FieldOfView", FindVariable("cg_fov")->Current.flValue);
			XML.set("NeoGenesys.Tweaks.FramesPerSecond", FindVariable("com_maxfps")->Current.iValue);

			XML.set("NeoGenesys.Colors.Axis.Red", _drawing.gColorAxis->Current.cValue.x);
			XML.set("NeoGenesys.Colors.Axis.Green", _drawing.gColorAxis->Current.cValue.y);
			XML.set("NeoGenesys.Colors.Axis.Blue", _drawing.gColorAxis->Current.cValue.z);
			XML.set("NeoGenesys.Colors.Axis.Alpha", _drawing.gColorAxis->Current.cValue.w);

			XML.set("NeoGenesys.Colors.Allies.Red", _drawing.gColorAllies->Current.cValue.x);
			XML.set("NeoGenesys.Colors.Allies.Green", _drawing.gColorAllies->Current.cValue.y);
			XML.set("NeoGenesys.Colors.Allies.Blue", _drawing.gColorAllies->Current.cValue.z);
			XML.set("NeoGenesys.Colors.Allies.Alpha", _drawing.gColorAllies->Current.cValue.w);

			XML.set("NeoGenesys.Colors.Accents.Red", _drawing.gColorAccents->Current.cValue.x);
			XML.set("NeoGenesys.Colors.Accents.Green", _drawing.gColorAccents->Current.cValue.y);
			XML.set("NeoGenesys.Colors.Accents.Blue", _drawing.gColorAccents->Current.cValue.z);
			XML.set("NeoGenesys.Colors.Accents.Alpha", _drawing.gColorAccents->Current.cValue.w);

			XML.set("NeoGenesys.Colors.CrossHair.Red", _drawing.gColorCrossHair->Current.cValue.x);
			XML.set("NeoGenesys.Colors.CrossHair.Green", _drawing.gColorCrossHair->Current.cValue.y);
			XML.set("NeoGenesys.Colors.CrossHair.Blue", _drawing.gColorCrossHair->Current.cValue.z);
			XML.set("NeoGenesys.Colors.CrossHair.Alpha", _drawing.gColorCrossHair->Current.cValue.w);

			XML.set("NeoGenesys.Colors.Text.Red", _drawing.gColorText->Current.cValue.x);
			XML.set("NeoGenesys.Colors.Text.Green", _drawing.gColorText->Current.cValue.y);
			XML.set("NeoGenesys.Colors.Text.Blue", _drawing.gColorText->Current.cValue.z);
			XML.set("NeoGenesys.Colors.Text.Alpha", _drawing.gColorText->Current.cValue.w);

			XML.set("NeoGenesys.Colors.Shadow.Red", _drawing.gColorShadow->Current.cValue.x);
			XML.set("NeoGenesys.Colors.Shadow.Green", _drawing.gColorShadow->Current.cValue.y);
			XML.set("NeoGenesys.Colors.Shadow.Blue", _drawing.gColorShadow->Current.cValue.z);
			XML.set("NeoGenesys.Colors.Shadow.Alpha", _drawing.gColorShadow->Current.cValue.w);

			XML.write_document(szFilePath);

			return true;
		}

		catch (const std::runtime_error& error)
		{
			MessageBox(_mainGui.hWindow, error.what(), "Error", MB_OK | MB_ICONERROR);

			return false;
		}
	}
	/*
	//=====================================================================================
	*/
	bool cProfiler::LoadProfile(const std::string& path)
	{
		try
		{
			std::wstring szFilePath;

			if (path.empty())
				szFilePath = acut::AnsiToWstring(acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_XML, " ", ""), CP_ACP);

			else
				szFilePath = acut::AnsiToWstring(path, CP_ACP);

			if (GetFileAttributes(acut::WstringToAnsi(szFilePath, CP_ACP).c_str()) == INVALID_FILE_ATTRIBUTES)
				return false;

			XML.read_from_file(szFilePath);

			XML.get_if_present("NeoGenesys.AimBot.Mode", _aimBot.gAimBotMode->Current.iValue);
			XML.get_if_present("NeoGenesys.AimBot.AutoZoom", _aimBot.gAutoZoom->Current.bValue);
			XML.get_if_present("NeoGenesys.AimBot.AutoFire", _aimBot.gAutoFire->Current.bValue);
			XML.get_if_present("NeoGenesys.AimBot.AutoWall", _targetList.gAutoWall->Current.bValue);
			XML.get_if_present("NeoGenesys.AimBot.SilentAim", _aimBot.gSilentAim->Current.bValue);
			XML.get_if_present("NeoGenesys.AimBot.TargetAgents", _targetList.gTargetAgents->Current.bValue);
			XML.get_if_present("NeoGenesys.AimBot.TargetMissiles", _targetList.gTargetMissiles->Current.bValue);
			XML.get_if_present("NeoGenesys.AimBot.AntiAim", _antiAim.gAntiAim->Current.iValue);
			XML.get_if_present("NeoGenesys.AimBot.BoneScan", _targetList.gBoneScan->Current.iValue);
			XML.get_if_present("NeoGenesys.AimBot.SortMethod", _targetList.gSortMethod->Current.iValue);

			XML.get_if_present("NeoGenesys.WallHack.Mode", _drawing.gWallHackMode->Current.iValue);
			XML.get_if_present("NeoGenesys.WallHack.Boxes", _drawing.gPlayerBoxes->Current.iValue);
			XML.get_if_present("NeoGenesys.WallHack.Bones", _drawing.gPlayerBones->Current.iValue);
			XML.get_if_present("NeoGenesys.WallHack.SnapLines", _drawing.gPlayerSnapLines->Current.iValue);
			XML.get_if_present("NeoGenesys.WallHack.Information", _drawing.gPlayerInformation->Current.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Weapons", _drawing.gPlayerWeapons->Current.bValue);
			XML.get_if_present("NeoGenesys.WallHack.BulletTracers", _hooks.gPlayerBulletTracers->Current.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Agents", _drawing.gAgents->Current.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Missiles", _drawing.gMissiles->Current.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Items", _drawing.gItems->Current.bValue);

			XML.get_if_present("NeoGenesys.Miscellaneous.KillSpam", _hooks.gKillSpam->Current.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.NameStealer", _hooks.gNameStealer->Current.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.ThirdPerson", _hooks.gThirdPerson->Current.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.CrossHair", _drawing.gPlayerCrossHair->Current.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.Compass", _drawing.gPlayerCompass->Current.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.Radar", _drawing.gPlayerRadar->Current.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.Console", _mainGui.gConsole->Current.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.PlayerList", _playerList.gPlayerList->Current.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.HostMenu", _hostMenu.gHostMenu->Current.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.MemoryEditor", _mainGui.gMemoryEditor->Current.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.RiotShield", _targetList.gRiotShielders->Current.iValue);

			XML.get_if_present("NeoGenesys.Tweaks.AimBone", _targetList.gAimBone->Current.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AimAngle", _targetList.gAimAngle->Current.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AimPower", _aimBot.gAimPower->Current.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AutoAimTime", _aimBot.gAutoAimTime->Current.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AutoAimDelay", _aimBot.gAutoAimDelay->Current.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AutoZoomDelay", _aimBot.gAutoZoomDelay->Current.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AutoFireDelay", _aimBot.gAutoFireDelay->Current.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.RecoilFactor", _removals.gRecoilFactor->Current.flValue);
			XML.get_if_present("NeoGenesys.Tweaks.SpreadFactor", _removals.gSpreadFactor->Current.flValue);

			XML.get_if_present("NeoGenesys.Tweaks.FieldOfView", FindVariable("cg_fov")->Current.flValue);
			XML.get_if_present("NeoGenesys.Tweaks.FramesPerSecond", FindVariable("com_maxfps")->Current.iValue);

			XML.get_if_present("NeoGenesys.Colors.Axis.Red", _drawing.gColorAxis->Current.cValue.x);
			XML.get_if_present("NeoGenesys.Colors.Axis.Green", _drawing.gColorAxis->Current.cValue.y);
			XML.get_if_present("NeoGenesys.Colors.Axis.Blue", _drawing.gColorAxis->Current.cValue.z);
			XML.get_if_present("NeoGenesys.Colors.Axis.Alpha", _drawing.gColorAxis->Current.cValue.w);

			XML.get_if_present("NeoGenesys.Colors.Allies.Red", _drawing.gColorAllies->Current.cValue.x);
			XML.get_if_present("NeoGenesys.Colors.Allies.Green", _drawing.gColorAllies->Current.cValue.y);
			XML.get_if_present("NeoGenesys.Colors.Allies.Blue", _drawing.gColorAllies->Current.cValue.z);
			XML.get_if_present("NeoGenesys.Colors.Allies.Alpha", _drawing.gColorAllies->Current.cValue.w);

			XML.get_if_present("NeoGenesys.Colors.Accents.Red", _drawing.gColorAccents->Current.cValue.x);
			XML.get_if_present("NeoGenesys.Colors.Accents.Green", _drawing.gColorAccents->Current.cValue.y);
			XML.get_if_present("NeoGenesys.Colors.Accents.Blue", _drawing.gColorAccents->Current.cValue.z);
			XML.get_if_present("NeoGenesys.Colors.Accents.Alpha", _drawing.gColorAccents->Current.cValue.w);

			XML.get_if_present("NeoGenesys.Colors.CrossHair.Red", _drawing.gColorCrossHair->Current.cValue.x);
			XML.get_if_present("NeoGenesys.Colors.CrossHair.Green", _drawing.gColorCrossHair->Current.cValue.y);
			XML.get_if_present("NeoGenesys.Colors.CrossHair.Blue", _drawing.gColorCrossHair->Current.cValue.z);
			XML.get_if_present("NeoGenesys.Colors.CrossHair.Alpha", _drawing.gColorCrossHair->Current.cValue.w);

			XML.get_if_present("NeoGenesys.Colors.Text.Red", _drawing.gColorText->Current.cValue.x);
			XML.get_if_present("NeoGenesys.Colors.Text.Green", _drawing.gColorText->Current.cValue.y);
			XML.get_if_present("NeoGenesys.Colors.Text.Blue", _drawing.gColorText->Current.cValue.z);
			XML.get_if_present("NeoGenesys.Colors.Text.Alpha", _drawing.gColorText->Current.cValue.w);

			XML.get_if_present("NeoGenesys.Colors.Shadow.Red", _drawing.gColorShadow->Current.cValue.x);
			XML.get_if_present("NeoGenesys.Colors.Shadow.Green", _drawing.gColorShadow->Current.cValue.y);
			XML.get_if_present("NeoGenesys.Colors.Shadow.Blue", _drawing.gColorShadow->Current.cValue.z);
			XML.get_if_present("NeoGenesys.Colors.Shadow.Alpha", _drawing.gColorShadow->Current.cValue.w);

			return true;
		}

		catch (const std::runtime_error& error)
		{
			MessageBox(_mainGui.hWindow, error.what(), "Error", MB_OK | MB_ICONERROR);

			return false;
		}
	}
	/*
	//=====================================================================================
	*/
	void cProfiler::DisableAll()
	{
		_aimBot.gAimBotMode->Current.iValue = _aimBot.gAimBotMode->Reset.iValue;
		_aimBot.gAutoZoom->Current.bValue = _aimBot.gAutoZoom->Reset.bValue;
		_aimBot.gAutoFire->Current.bValue = _aimBot.gAutoFire->Reset.bValue;
		_targetList.gAutoWall->Current.bValue = _targetList.gAutoWall->Reset.bValue;
		_aimBot.gSilentAim->Current.bValue = _aimBot.gSilentAim->Reset.bValue;
		_targetList.gTargetAgents->Current.bValue = _targetList.gTargetAgents->Reset.bValue;
		_targetList.gTargetMissiles->Current.bValue = _targetList.gTargetMissiles->Reset.bValue;
		_antiAim.gAntiAim->Current.iValue = _antiAim.gAntiAim->Reset.iValue;
		_targetList.gBoneScan->Current.iValue = _targetList.gBoneScan->Reset.iValue;
		_targetList.gSortMethod->Current.iValue = _targetList.gSortMethod->Reset.iValue;

		_drawing.gWallHackMode->Current.iValue = _drawing.gWallHackMode->Reset.iValue;
		_drawing.gPlayerBoxes->Current.iValue = _drawing.gPlayerBoxes->Reset.iValue;
		_drawing.gPlayerBones->Current.iValue = _drawing.gPlayerBones->Reset.iValue;
		_drawing.gPlayerSnapLines->Current.iValue = _drawing.gPlayerSnapLines->Reset.iValue;
		_drawing.gPlayerInformation->Current.bValue = _drawing.gPlayerInformation->Reset.bValue;
		_drawing.gPlayerWeapons->Current.bValue = _drawing.gPlayerWeapons->Reset.bValue;
		_hooks.gPlayerBulletTracers->Current.bValue = _hooks.gPlayerBulletTracers->Reset.bValue;
		_drawing.gAgents->Current.bValue = _drawing.gAgents->Reset.bValue;
		_drawing.gMissiles->Current.bValue = _drawing.gMissiles->Reset.bValue;
		_drawing.gItems->Current.bValue = _drawing.gItems->Reset.bValue;

		_hooks.gNameStealer->Current.bValue = _hooks.gNameStealer->Reset.bValue;
		_hooks.gThirdPerson->Current.bValue = _hooks.gThirdPerson->Reset.bValue;
		_drawing.gPlayerCrossHair->Current.bValue = _drawing.gPlayerCrossHair->Reset.bValue;
		_drawing.gPlayerCompass->Current.bValue = _drawing.gPlayerCompass->Reset.bValue;
		_drawing.gPlayerRadar->Current.bValue = _drawing.gPlayerRadar->Reset.bValue;
		_mainGui.gConsole->Current.bValue = _mainGui.gConsole->Reset.bValue;
		_playerList.gPlayerList->Current.bValue = _playerList.gPlayerList->Reset.bValue;
		_hostMenu.gHostMenu->Current.bValue = _hostMenu.gHostMenu->Reset.bValue;
		_mainGui.gMemoryEditor->Current.bValue = _mainGui.gMemoryEditor->Reset.bValue;
		_targetList.gRiotShielders->Current.iValue = _targetList.gRiotShielders->Reset.iValue;

		_mainGui.gNameSpam->Current.bValue = _mainGui.gNameSpam->Reset.bValue;
		_mainGui.gChatSpam->Current.bValue = _mainGui.gChatSpam->Reset.bValue;
		_hooks.gKillSpam->Current.bValue = _hooks.gKillSpam->Reset.bValue;
		_hostMenu.gMassKill->Current.iValue = _hostMenu.gMassKill->Reset.iValue;
		_packets.gBunnyHop->Current.bValue = _packets.gBunnyHop->Reset.bValue;
		_hooks.gTeaBag->Current.bValue = _hooks.gTeaBag->Reset.bValue;
	}
}

//=====================================================================================