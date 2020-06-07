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

			XML.set("NeoGenesys.AimBot.Mode", _aimBot.gAimBotMode->Custom.iValue);
			XML.set("NeoGenesys.AimBot.AutoZoom", _aimBot.gAutoZoom->Custom.bValue);
			XML.set("NeoGenesys.AimBot.AutoFire", _aimBot.gAutoFire->Custom.bValue);
			XML.set("NeoGenesys.AimBot.AutoWall", _targetList.gAutoWall->Custom.bValue);
			XML.set("NeoGenesys.AimBot.SilentAim", _aimBot.gSilentAim->Custom.bValue);
			XML.set("NeoGenesys.AimBot.TargetAgents", _targetList.gTargetAgents->Custom.bValue);
			XML.set("NeoGenesys.AimBot.TargetMissiles", _targetList.gTargetMissiles->Custom.bValue);
			XML.set("NeoGenesys.AimBot.AntiAim", _antiAim.gAntiAim->Custom.iValue);
			XML.set("NeoGenesys.AimBot.BoneScan", _targetList.gBoneScan->Custom.iValue);
			XML.set("NeoGenesys.AimBot.SortMethod", _targetList.gSortMethod->Custom.iValue);

			XML.set("NeoGenesys.WallHack.Mode", _drawing.gWallHackMode->Custom.iValue);
			XML.set("NeoGenesys.WallHack.Boxes", _drawing.gPlayerBoxes->Custom.iValue);
			XML.set("NeoGenesys.WallHack.Bones", _drawing.gPlayerBones->Custom.iValue);
			XML.set("NeoGenesys.WallHack.SnapLines", _drawing.gPlayerSnapLines->Custom.iValue);
			XML.set("NeoGenesys.WallHack.Information", _drawing.gPlayerInformation->Custom.bValue);
			XML.set("NeoGenesys.WallHack.Weapons", _drawing.gPlayerWeapons->Custom.bValue);
			XML.set("NeoGenesys.WallHack.BulletTracers", _hooks.gPlayerBulletTracers->Custom.bValue);
			XML.set("NeoGenesys.WallHack.Agents", _drawing.gAgents->Custom.bValue);
			XML.set("NeoGenesys.WallHack.Missiles", _drawing.gMissiles->Custom.bValue);
			XML.set("NeoGenesys.WallHack.Items", _drawing.gItems->Custom.bValue);

			XML.set("NeoGenesys.Miscellaneous.KillSpam", _hooks.gKillSpam->Custom.bValue);
			XML.set("NeoGenesys.Miscellaneous.NameStealer", _hooks.gNameStealer->Custom.bValue);
			XML.set("NeoGenesys.Miscellaneous.ThirdPerson", _hooks.gThirdPerson->Custom.bValue);
			XML.set("NeoGenesys.Miscellaneous.CrossHair", _drawing.gPlayerCrossHair->Custom.bValue);
			XML.set("NeoGenesys.Miscellaneous.Compass", _drawing.gPlayerCompass->Custom.bValue);
			XML.set("NeoGenesys.Miscellaneous.Radar", _drawing.gPlayerRadar->Custom.bValue);
			XML.set("NeoGenesys.Miscellaneous.RiotShield", _targetList.gRiotShielders->Custom.iValue);

			XML.set("NeoGenesys.Tweaks.AimBone", _targetList.gAimBone->Custom.iValue);
			XML.set("NeoGenesys.Tweaks.AimAngle", _targetList.gAimAngle->Custom.iValue);
			XML.set("NeoGenesys.Tweaks.AimPower", _aimBot.gAimPower->Custom.iValue);
			XML.set("NeoGenesys.Tweaks.AutoAimTime", _aimBot.gAutoAimTime->Custom.iValue);
			XML.set("NeoGenesys.Tweaks.AutoAimDelay", _aimBot.gAutoAimDelay->Custom.iValue);
			XML.set("NeoGenesys.Tweaks.AutoZoomDelay", _aimBot.gAutoZoomDelay->Custom.iValue);
			XML.set("NeoGenesys.Tweaks.AutoFireDelay", _aimBot.gAutoFireDelay->Custom.iValue);
			XML.set("NeoGenesys.Tweaks.RecoilFactor", _removals.gRecoilFactor->Custom.flValue);
			XML.set("NeoGenesys.Tweaks.SpreadFactor", _removals.gSpreadFactor->Custom.flValue);

			XML.set("NeoGenesys.Tweaks.FieldOfView", FindVariable("cg_fov")->Current.flValue);
			XML.set("NeoGenesys.Tweaks.FramesPerSecond", FindVariable("com_maxfps")->Current.iValue);

			XML.set("NeoGenesys.Colors.Axis.Red", _drawing.gColorAxis->Custom.cValue.x);
			XML.set("NeoGenesys.Colors.Axis.Green", _drawing.gColorAxis->Custom.cValue.y);
			XML.set("NeoGenesys.Colors.Axis.Blue", _drawing.gColorAxis->Custom.cValue.z);
			XML.set("NeoGenesys.Colors.Axis.Alpha", _drawing.gColorAxis->Custom.cValue.w);

			XML.set("NeoGenesys.Colors.Allies.Red", _drawing.gColorAllies->Custom.cValue.x);
			XML.set("NeoGenesys.Colors.Allies.Green", _drawing.gColorAllies->Custom.cValue.y);
			XML.set("NeoGenesys.Colors.Allies.Blue", _drawing.gColorAllies->Custom.cValue.z);
			XML.set("NeoGenesys.Colors.Allies.Alpha", _drawing.gColorAllies->Custom.cValue.w);

			XML.set("NeoGenesys.Colors.Accents.Red", _drawing.gColorAccents->Custom.cValue.x);
			XML.set("NeoGenesys.Colors.Accents.Green", _drawing.gColorAccents->Custom.cValue.y);
			XML.set("NeoGenesys.Colors.Accents.Blue", _drawing.gColorAccents->Custom.cValue.z);
			XML.set("NeoGenesys.Colors.Accents.Alpha", _drawing.gColorAccents->Custom.cValue.w);

			XML.set("NeoGenesys.Colors.CrossHair.Red", _drawing.gColorCrossHair->Custom.cValue.x);
			XML.set("NeoGenesys.Colors.CrossHair.Green", _drawing.gColorCrossHair->Custom.cValue.y);
			XML.set("NeoGenesys.Colors.CrossHair.Blue", _drawing.gColorCrossHair->Custom.cValue.z);
			XML.set("NeoGenesys.Colors.CrossHair.Alpha", _drawing.gColorCrossHair->Custom.cValue.w);

			XML.set("NeoGenesys.Colors.Text.Red", _drawing.gColorText->Custom.cValue.x);
			XML.set("NeoGenesys.Colors.Text.Green", _drawing.gColorText->Custom.cValue.y);
			XML.set("NeoGenesys.Colors.Text.Blue", _drawing.gColorText->Custom.cValue.z);
			XML.set("NeoGenesys.Colors.Text.Alpha", _drawing.gColorText->Custom.cValue.w);

			XML.set("NeoGenesys.Colors.Shadow.Red", _drawing.gColorShadow->Custom.cValue.x);
			XML.set("NeoGenesys.Colors.Shadow.Green", _drawing.gColorShadow->Custom.cValue.y);
			XML.set("NeoGenesys.Colors.Shadow.Blue", _drawing.gColorShadow->Custom.cValue.z);
			XML.set("NeoGenesys.Colors.Shadow.Alpha", _drawing.gColorShadow->Custom.cValue.w);

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

			XML.get_if_present("NeoGenesys.AimBot.Mode", _aimBot.gAimBotMode->Custom.iValue);
			XML.get_if_present("NeoGenesys.AimBot.AutoZoom", _aimBot.gAutoZoom->Custom.bValue);
			XML.get_if_present("NeoGenesys.AimBot.AutoFire", _aimBot.gAutoFire->Custom.bValue);
			XML.get_if_present("NeoGenesys.AimBot.AutoWall", _targetList.gAutoWall->Custom.bValue);
			XML.get_if_present("NeoGenesys.AimBot.SilentAim", _aimBot.gSilentAim->Custom.bValue);
			XML.get_if_present("NeoGenesys.AimBot.TargetAgents", _targetList.gTargetAgents->Custom.bValue);
			XML.get_if_present("NeoGenesys.AimBot.TargetMissiles", _targetList.gTargetMissiles->Custom.bValue);
			XML.get_if_present("NeoGenesys.AimBot.AntiAim", _antiAim.gAntiAim->Custom.iValue);
			XML.get_if_present("NeoGenesys.AimBot.BoneScan", _targetList.gBoneScan->Custom.iValue);
			XML.get_if_present("NeoGenesys.AimBot.SortMethod", _targetList.gSortMethod->Custom.iValue);

			XML.get_if_present("NeoGenesys.WallHack.Mode", _drawing.gWallHackMode->Custom.iValue);
			XML.get_if_present("NeoGenesys.WallHack.Boxes", _drawing.gPlayerBoxes->Custom.iValue);
			XML.get_if_present("NeoGenesys.WallHack.Bones", _drawing.gPlayerBones->Custom.iValue);
			XML.get_if_present("NeoGenesys.WallHack.SnapLines", _drawing.gPlayerSnapLines->Custom.iValue);
			XML.get_if_present("NeoGenesys.WallHack.Information", _drawing.gPlayerInformation->Custom.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Weapons", _drawing.gPlayerWeapons->Custom.bValue);
			XML.get_if_present("NeoGenesys.WallHack.BulletTracers", _hooks.gPlayerBulletTracers->Custom.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Agents", _drawing.gAgents->Custom.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Missiles", _drawing.gMissiles->Custom.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Items", _drawing.gItems->Custom.bValue);

			XML.get_if_present("NeoGenesys.Miscellaneous.KillSpam", _hooks.gKillSpam->Custom.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.NameStealer", _hooks.gNameStealer->Custom.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.ThirdPerson", _hooks.gThirdPerson->Custom.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.CrossHair", _drawing.gPlayerCrossHair->Custom.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.Compass", _drawing.gPlayerCompass->Custom.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.Radar", _drawing.gPlayerRadar->Custom.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.RiotShield", _targetList.gRiotShielders->Custom.iValue);

			XML.get_if_present("NeoGenesys.Tweaks.AimBone", _targetList.gAimBone->Custom.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AimAngle", _targetList.gAimAngle->Custom.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AimPower", _aimBot.gAimPower->Custom.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AutoAimTime", _aimBot.gAutoAimTime->Custom.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AutoAimDelay", _aimBot.gAutoAimDelay->Custom.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AutoZoomDelay", _aimBot.gAutoZoomDelay->Custom.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AutoFireDelay", _aimBot.gAutoFireDelay->Custom.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.RecoilFactor", _removals.gRecoilFactor->Custom.flValue);
			XML.get_if_present("NeoGenesys.Tweaks.SpreadFactor", _removals.gSpreadFactor->Custom.flValue);

			XML.get_if_present("NeoGenesys.Tweaks.FieldOfView", FindVariable("cg_fov")->Current.flValue);
			XML.get_if_present("NeoGenesys.Tweaks.FramesPerSecond", FindVariable("com_maxfps")->Current.iValue);

			XML.get_if_present("NeoGenesys.Colors.Axis.Red", _drawing.gColorAxis->Custom.cValue.x);
			XML.get_if_present("NeoGenesys.Colors.Axis.Green", _drawing.gColorAxis->Custom.cValue.y);
			XML.get_if_present("NeoGenesys.Colors.Axis.Blue", _drawing.gColorAxis->Custom.cValue.z);
			XML.get_if_present("NeoGenesys.Colors.Axis.Alpha", _drawing.gColorAxis->Custom.cValue.w);

			XML.get_if_present("NeoGenesys.Colors.Allies.Red", _drawing.gColorAllies->Custom.cValue.x);
			XML.get_if_present("NeoGenesys.Colors.Allies.Green", _drawing.gColorAllies->Custom.cValue.y);
			XML.get_if_present("NeoGenesys.Colors.Allies.Blue", _drawing.gColorAllies->Custom.cValue.z);
			XML.get_if_present("NeoGenesys.Colors.Allies.Alpha", _drawing.gColorAllies->Custom.cValue.w);

			XML.get_if_present("NeoGenesys.Colors.Accents.Red", _drawing.gColorAccents->Custom.cValue.x);
			XML.get_if_present("NeoGenesys.Colors.Accents.Green", _drawing.gColorAccents->Custom.cValue.y);
			XML.get_if_present("NeoGenesys.Colors.Accents.Blue", _drawing.gColorAccents->Custom.cValue.z);
			XML.get_if_present("NeoGenesys.Colors.Accents.Alpha", _drawing.gColorAccents->Custom.cValue.w);

			XML.get_if_present("NeoGenesys.Colors.CrossHair.Red", _drawing.gColorCrossHair->Custom.cValue.x);
			XML.get_if_present("NeoGenesys.Colors.CrossHair.Green", _drawing.gColorCrossHair->Custom.cValue.y);
			XML.get_if_present("NeoGenesys.Colors.CrossHair.Blue", _drawing.gColorCrossHair->Custom.cValue.z);
			XML.get_if_present("NeoGenesys.Colors.CrossHair.Alpha", _drawing.gColorCrossHair->Custom.cValue.w);

			XML.get_if_present("NeoGenesys.Colors.Text.Red", _drawing.gColorText->Custom.cValue.x);
			XML.get_if_present("NeoGenesys.Colors.Text.Green", _drawing.gColorText->Custom.cValue.y);
			XML.get_if_present("NeoGenesys.Colors.Text.Blue", _drawing.gColorText->Custom.cValue.z);
			XML.get_if_present("NeoGenesys.Colors.Text.Alpha", _drawing.gColorText->Custom.cValue.w);

			XML.get_if_present("NeoGenesys.Colors.Shadow.Red", _drawing.gColorShadow->Custom.cValue.x);
			XML.get_if_present("NeoGenesys.Colors.Shadow.Green", _drawing.gColorShadow->Custom.cValue.y);
			XML.get_if_present("NeoGenesys.Colors.Shadow.Blue", _drawing.gColorShadow->Custom.cValue.z);
			XML.get_if_present("NeoGenesys.Colors.Shadow.Alpha", _drawing.gColorShadow->Custom.cValue.w);

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
		_aimBot.gAimBotMode->Custom.iValue = _aimBot.gAimBotMode->Default.iValue;
		_aimBot.gAutoZoom->Custom.bValue = _aimBot.gAutoZoom->Default.bValue;
		_aimBot.gAutoFire->Custom.bValue = _aimBot.gAutoFire->Default.bValue;
		_targetList.gAutoWall->Custom.bValue = _targetList.gAutoWall->Default.bValue;
		_aimBot.gSilentAim->Custom.bValue = _aimBot.gSilentAim->Default.bValue;
		_targetList.gTargetAgents->Custom.bValue = _targetList.gTargetAgents->Default.bValue;
		_targetList.gTargetMissiles->Custom.bValue = _targetList.gTargetMissiles->Default.bValue;
		_antiAim.gAntiAim->Custom.iValue = _antiAim.gAntiAim->Default.iValue;
		_targetList.gBoneScan->Custom.iValue = _targetList.gBoneScan->Default.iValue;
		_targetList.gSortMethod->Custom.iValue = _targetList.gSortMethod->Default.iValue;

		_drawing.gWallHackMode->Custom.iValue = _drawing.gWallHackMode->Default.iValue;
		_drawing.gPlayerBoxes->Custom.iValue = _drawing.gPlayerBoxes->Default.iValue;
		_drawing.gPlayerBones->Custom.iValue = _drawing.gPlayerBones->Default.iValue;
		_drawing.gPlayerSnapLines->Custom.iValue = _drawing.gPlayerSnapLines->Default.iValue;
		_drawing.gPlayerInformation->Custom.bValue = _drawing.gPlayerInformation->Default.bValue;
		_drawing.gPlayerWeapons->Custom.bValue = _drawing.gPlayerWeapons->Default.bValue;
		_hooks.gPlayerBulletTracers->Custom.bValue = _hooks.gPlayerBulletTracers->Default.bValue;
		_drawing.gAgents->Custom.bValue = _drawing.gAgents->Default.bValue;
		_drawing.gMissiles->Custom.bValue = _drawing.gMissiles->Default.bValue;
		_drawing.gItems->Custom.bValue = _drawing.gItems->Default.bValue;

		_hooks.gNameStealer->Custom.bValue = _hooks.gNameStealer->Default.bValue;
		_hooks.gThirdPerson->Custom.bValue = _hooks.gThirdPerson->Default.bValue;
		_drawing.gPlayerCrossHair->Custom.bValue = _drawing.gPlayerCrossHair->Default.bValue;
		_drawing.gPlayerCompass->Custom.bValue = _drawing.gPlayerCompass->Default.bValue;
		_drawing.gPlayerRadar->Custom.bValue = _drawing.gPlayerRadar->Default.bValue;
		_targetList.gRiotShielders->Custom.iValue = _targetList.gRiotShielders->Default.iValue;

		_mainGui.gNameSpam->Custom.bValue = _mainGui.gNameSpam->Default.bValue;
		_mainGui.gChatSpam->Custom.bValue = _mainGui.gChatSpam->Default.bValue;
		_hooks.gKillSpam->Custom.bValue = _hooks.gKillSpam->Default.bValue;
		_hostMenu.gSuperJump->Custom.bValue = _hostMenu.gSuperJump->Default.bValue;
		_hostMenu.gMassKill->Custom.iValue = _hostMenu.gMassKill->Default.iValue;
		_packets.gBunnyHop->Custom.bValue = _packets.gBunnyHop->Default.bValue;
		_hooks.gTeaBag->Custom.bValue = _hooks.gTeaBag->Default.bValue;
	}
}

//=====================================================================================