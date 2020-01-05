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
				szFilePath = acut::AnsiToWstring(acut::GetExeDirectory() + DEFAULT_XML, CP_ACP);

			else
				szFilePath = acut::AnsiToWstring(path, CP_ACP);

			XML.set("NeoGenesys.AimBot.Mode", gAimBotMode->Custom.iValue);
			XML.set("NeoGenesys.AimBot.AutoZoom", gAutoZoom->Custom.bValue);
			XML.set("NeoGenesys.AimBot.AutoFire", gAutoFire->Custom.bValue);
			XML.set("NeoGenesys.AimBot.AutoWall", gAutoWall->Custom.bValue);
			XML.set("NeoGenesys.AimBot.SilentAim", gSilentAim->Custom.bValue);
			XML.set("NeoGenesys.AimBot.TargetAgents", gTargetAgents->Custom.bValue);
			XML.set("NeoGenesys.AimBot.TargetMissiles", gTargetMissiles->Custom.bValue);
			XML.set("NeoGenesys.AimBot.AntiAim", gAntiAim->Custom.iValue);
			XML.set("NeoGenesys.AimBot.BoneScan", gBoneScan->Custom.iValue);

			XML.set("NeoGenesys.WallHack.Mode", gWallHackMode->Custom.iValue);
			XML.set("NeoGenesys.WallHack.Boxes", gPlayerBoxes->Custom.iValue);
			XML.set("NeoGenesys.WallHack.Bones", gPlayerBones->Custom.iValue);
			XML.set("NeoGenesys.WallHack.SnapLines", gPlayerSnapLines->Custom.iValue);
			XML.set("NeoGenesys.WallHack.Distances", gPlayerDistances->Custom.bValue);
			XML.set("NeoGenesys.WallHack.Names", gPlayerNames->Custom.bValue);
			XML.set("NeoGenesys.WallHack.Weapons", gPlayerWeapons->Custom.bValue);
			XML.set("NeoGenesys.WallHack.Agents", gAgents->Custom.bValue);
			XML.set("NeoGenesys.WallHack.Missiles", gMissiles->Custom.bValue);
			XML.set("NeoGenesys.WallHack.Items", gItems->Custom.bValue);

			XML.set("NeoGenesys.Miscellaneous.KillSpam", gKillSpam->Custom.bValue);
			XML.set("NeoGenesys.Miscellaneous.NameStealer", gNameStealer->Custom.bValue);
			XML.set("NeoGenesys.Miscellaneous.CrossHair", gPlayerCrossHair->Custom.bValue);
			XML.set("NeoGenesys.Miscellaneous.Compass", gPlayerCompass->Custom.bValue);
			XML.set("NeoGenesys.Miscellaneous.Radar", gPlayerRadar->Custom.bValue);
			XML.set("NeoGenesys.Miscellaneous.RiotShield", gRiotShield->Custom.iValue);

			XML.set("NeoGenesys.Tweaks.AimBone", gAimBone->Custom.iValue);
			XML.set("NeoGenesys.Tweaks.AimAngle", gAimAngle->Custom.flValue);
			XML.set("NeoGenesys.Tweaks.AimPower", gAimPower->Custom.iValue);
			XML.set("NeoGenesys.Tweaks.AutoAimTime", gAutoAimTime->Custom.iValue);
			XML.set("NeoGenesys.Tweaks.AutoAimDelay", gAutoAimDelay->Custom.iValue);
			XML.set("NeoGenesys.Tweaks.AutoZoomDelay", gAutoZoomDelay->Custom.iValue);
			XML.set("NeoGenesys.Tweaks.AutoFireDelay", gAutoFireDelay->Custom.iValue);
			XML.set("NeoGenesys.Tweaks.RecoilFactor", gRecoilFactor->Custom.flValue);
			XML.set("NeoGenesys.Tweaks.SpreadFactor", gSpreadFactor->Custom.flValue);

			XML.set("NeoGenesys.Tweaks.FieldOfView", FindVariable("cg_fov")->Current.flValue);
			XML.set("NeoGenesys.Tweaks.FramesPerSecond", FindVariable("com_maxfps")->Current.iValue);

			XML.set("NeoGenesys.Colors.Axis.Red", gColorAxis->Custom.cValue[0]);
			XML.set("NeoGenesys.Colors.Axis.Green", gColorAxis->Custom.cValue[1]);
			XML.set("NeoGenesys.Colors.Axis.Blue", gColorAxis->Custom.cValue[2]);

			XML.set("NeoGenesys.Colors.Allies.Red", gColorAllies->Custom.cValue[0]);
			XML.set("NeoGenesys.Colors.Allies.Green", gColorAllies->Custom.cValue[1]);
			XML.set("NeoGenesys.Colors.Allies.Blue", gColorAllies->Custom.cValue[2]);

			XML.set("NeoGenesys.Colors.RiotShield.Red", gColorRiotShield->Custom.cValue[0]);
			XML.set("NeoGenesys.Colors.RiotShield.Green", gColorRiotShield->Custom.cValue[1]);
			XML.set("NeoGenesys.Colors.RiotShield.Blue", gColorRiotShield->Custom.cValue[2]);

			XML.set("NeoGenesys.Colors.CrossHair.Red", gColorCrossHair->Custom.cValue[0]);
			XML.set("NeoGenesys.Colors.CrossHair.Green", gColorCrossHair->Custom.cValue[1]);
			XML.set("NeoGenesys.Colors.CrossHair.Blue", gColorCrossHair->Custom.cValue[2]);

			XML.set("NeoGenesys.Colors.Text.Red", gColorText->Custom.cValue[0]);
			XML.set("NeoGenesys.Colors.Text.Green", gColorText->Custom.cValue[1]);
			XML.set("NeoGenesys.Colors.Text.Blue", gColorText->Custom.cValue[2]);

			XML.set("NeoGenesys.Colors.Shadow.Red", gColorShadow->Custom.cValue[0]);
			XML.set("NeoGenesys.Colors.Shadow.Green", gColorShadow->Custom.cValue[1]);
			XML.set("NeoGenesys.Colors.Shadow.Blue", gColorShadow->Custom.cValue[2]);

			XML.write_document(szFilePath);

			return true;
		}

		catch (const std::runtime_error& e)
		{
			MessageBox(_mainGui.hWindow, e.what(), "Error", MB_OK | MB_ICONERROR);

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
				szFilePath = acut::AnsiToWstring(acut::GetExeDirectory() + DEFAULT_XML, CP_ACP);

			else
				szFilePath = acut::AnsiToWstring(path, CP_ACP);

			if (GetFileAttributes(acut::WstringToAnsi(szFilePath, CP_ACP).c_str()) == INVALID_FILE_ATTRIBUTES)
				return false;

			XML.read_from_file(szFilePath);

			XML.get_if_present("NeoGenesys.AimBot.Mode", gAimBotMode->Custom.iValue);
			XML.get_if_present("NeoGenesys.AimBot.AutoZoom", gAutoZoom->Custom.bValue);
			XML.get_if_present("NeoGenesys.AimBot.AutoFire", gAutoFire->Custom.bValue);
			XML.get_if_present("NeoGenesys.AimBot.AutoWall", gAutoWall->Custom.bValue);
			XML.get_if_present("NeoGenesys.AimBot.SilentAim", gSilentAim->Custom.bValue);
			XML.get_if_present("NeoGenesys.AimBot.TargetAgents", gTargetAgents->Custom.bValue);
			XML.get_if_present("NeoGenesys.AimBot.TargetMissiles", gTargetMissiles->Custom.bValue);
			XML.get_if_present("NeoGenesys.AimBot.AntiAim", gAntiAim->Custom.iValue);
			XML.get_if_present("NeoGenesys.AimBot.BoneScan", gBoneScan->Custom.iValue);

			XML.get_if_present("NeoGenesys.WallHack.Mode", gWallHackMode->Custom.iValue);
			XML.get_if_present("NeoGenesys.WallHack.Boxes", gPlayerBoxes->Custom.iValue);
			XML.get_if_present("NeoGenesys.WallHack.Bones", gPlayerBones->Custom.iValue);
			XML.get_if_present("NeoGenesys.WallHack.SnapLines", gPlayerSnapLines->Custom.iValue);
			XML.get_if_present("NeoGenesys.WallHack.Distances", gPlayerDistances->Custom.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Names", gPlayerNames->Custom.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Weapons", gPlayerWeapons->Custom.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Agents", gAgents->Custom.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Missiles", gMissiles->Custom.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Items", gItems->Custom.bValue);

			XML.get_if_present("NeoGenesys.Miscellaneous.KillSpam", gKillSpam->Custom.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.NameStealer", gNameStealer->Custom.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.CrossHair", gPlayerCrossHair->Custom.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.Compass", gPlayerCompass->Custom.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.Radar", gPlayerRadar->Custom.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.RiotShield", gRiotShield->Custom.iValue);

			XML.get_if_present("NeoGenesys.Tweaks.AimBone", gAimBone->Custom.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AimAngle", gAimAngle->Custom.flValue);
			XML.get_if_present("NeoGenesys.Tweaks.AimPower", gAimPower->Custom.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AutoAimTime", gAutoAimTime->Custom.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AutoAimDelay", gAutoAimDelay->Custom.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AutoZoomDelay", gAutoZoomDelay->Custom.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AutoFireDelay", gAutoFireDelay->Custom.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.RecoilFactor", gRecoilFactor->Custom.flValue);
			XML.get_if_present("NeoGenesys.Tweaks.SpreadFactor", gSpreadFactor->Custom.flValue);

			XML.get_if_present("NeoGenesys.Tweaks.FieldOfView", FindVariable("cg_fov")->Current.flValue);
			XML.get_if_present("NeoGenesys.Tweaks.FramesPerSecond", FindVariable("com_maxfps")->Current.iValue);

			XML.get_if_present("NeoGenesys.Colors.Axis.Red", gColorAxis->Custom.cValue[0]);
			XML.get_if_present("NeoGenesys.Colors.Axis.Green", gColorAxis->Custom.cValue[1]);
			XML.get_if_present("NeoGenesys.Colors.Axis.Blue", gColorAxis->Custom.cValue[2]);

			XML.get_if_present("NeoGenesys.Colors.Allies.Red", gColorAllies->Custom.cValue[0]);
			XML.get_if_present("NeoGenesys.Colors.Allies.Green", gColorAllies->Custom.cValue[1]);
			XML.get_if_present("NeoGenesys.Colors.Allies.Blue", gColorAllies->Custom.cValue[2]);

			XML.get_if_present("NeoGenesys.Colors.RiotShield.Red", gColorRiotShield->Custom.cValue[0]);
			XML.get_if_present("NeoGenesys.Colors.RiotShield.Green", gColorRiotShield->Custom.cValue[1]);
			XML.get_if_present("NeoGenesys.Colors.RiotShield.Blue", gColorRiotShield->Custom.cValue[2]);

			XML.get_if_present("NeoGenesys.Colors.CrossHair.Red", gColorCrossHair->Custom.cValue[0]);
			XML.get_if_present("NeoGenesys.Colors.CrossHair.Green", gColorCrossHair->Custom.cValue[1]);
			XML.get_if_present("NeoGenesys.Colors.CrossHair.Blue", gColorCrossHair->Custom.cValue[2]);

			XML.get_if_present("NeoGenesys.Colors.Text.Red", gColorText->Custom.cValue[0]);
			XML.get_if_present("NeoGenesys.Colors.Text.Green", gColorText->Custom.cValue[1]);
			XML.get_if_present("NeoGenesys.Colors.Text.Blue", gColorText->Custom.cValue[2]);

			XML.get_if_present("NeoGenesys.Colors.Shadow.Red", gColorShadow->Custom.cValue[0]);
			XML.get_if_present("NeoGenesys.Colors.Shadow.Green", gColorShadow->Custom.cValue[1]);
			XML.get_if_present("NeoGenesys.Colors.Shadow.Blue", gColorShadow->Custom.cValue[2]);

			return true;
		}

		catch (const std::runtime_error& e)
		{
			MessageBox(_mainGui.hWindow, e.what(), "Error", MB_OK | MB_ICONERROR);

			return false;
		}
	}
	/*
	//=====================================================================================
	*/
	void cProfiler::DisableAll()
	{
		gAimBotMode->Custom.iValue = gAimBotMode->Default.iValue;
		gAutoZoom->Custom.bValue = gAutoZoom->Default.bValue;
		gAutoFire->Custom.bValue = gAutoFire->Default.bValue;
		gAutoWall->Custom.bValue = gAutoWall->Default.bValue;
		gSilentAim->Custom.bValue = gSilentAim->Default.bValue;
		gTargetAgents->Custom.bValue = gTargetAgents->Default.bValue;
		gTargetMissiles->Custom.bValue = gTargetMissiles->Default.bValue;
		gAntiAim->Custom.iValue = gAntiAim->Default.iValue;
		gBoneScan->Custom.iValue = gBoneScan->Default.iValue;

		gWallHackMode->Custom.iValue = gWallHackMode->Default.iValue;
		gPlayerBoxes->Custom.iValue = gPlayerBoxes->Default.iValue;
		gPlayerBones->Custom.iValue = gPlayerBones->Default.iValue;
		gPlayerSnapLines->Custom.iValue = gPlayerSnapLines->Default.iValue;
		gPlayerDistances->Custom.bValue = gPlayerDistances->Default.bValue;
		gPlayerNames->Custom.bValue = gPlayerNames->Default.bValue;
		gPlayerWeapons->Custom.bValue = gPlayerWeapons->Default.bValue;
		gAgents->Custom.bValue = gAgents->Default.bValue;
		gMissiles->Custom.bValue = gMissiles->Default.bValue;
		gItems->Custom.bValue = gItems->Default.bValue;

		gKillSpam->Custom.bValue = gKillSpam->Default.bValue;
		gNameStealer->Custom.bValue = gNameStealer->Default.bValue;
		gPlayerCrossHair->Custom.bValue = gPlayerCrossHair->Default.bValue;
		gPlayerCompass->Custom.bValue = gPlayerCompass->Default.bValue;
		gPlayerRadar->Custom.bValue = gPlayerRadar->Default.bValue;
		gRiotShield->Custom.iValue = gRiotShield->Default.iValue;

		gMassKill->Custom.iValue = gMassKill->Default.iValue;
	}
}

//=====================================================================================