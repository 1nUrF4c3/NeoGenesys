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

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.Mode", gAimBotMode->Custom.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.AutoZoom", gAutoZoom->Custom.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.AutoFire", gAutoFire->Custom.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.AutoWall", gAutoWall->Custom.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.SilentAim", gSilentAim->Custom.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.TargetAgents", gTargetAgents->Custom.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.TargetMissiles", gTargetMissiles->Custom.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.AntiAim", gAntiAim->Custom.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.BoneScan", gBoneScan->Custom.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.SortMethod", gSortMethod->Custom.iValue);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Mode", gWallHackMode->Custom.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Boxes", gPlayerBoxes->Custom.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Bones", gPlayerBones->Custom.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.SnapLines", gPlayerSnapLines->Custom.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Distances", gPlayerDistances->Custom.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Names", gPlayerNames->Custom.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Weapons", gPlayerWeapons->Custom.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Agents", gAgents->Custom.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Missiles", gMissiles->Custom.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Items", gItems->Custom.bValue);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.KillSpam", gKillSpam->Custom.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.NameStealer", gNameStealer->Custom.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.CrossHair", gPlayerCrossHair->Custom.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.Compass", gPlayerCompass->Custom.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.Radar", gPlayerRadar->Custom.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.RiotShield", gRiotShield->Custom.iValue);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AimBone", gAimBone->Custom.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AimAngle", gAimAngle->Custom.flValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AimPower", gAimPower->Custom.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AutoAimTime", gAutoAimTime->Custom.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AutoAimDelay", gAutoAimDelay->Custom.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AutoZoomDelay", gAutoZoomDelay->Custom.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AutoFireDelay", gAutoFireDelay->Custom.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.RecoilFactor", gRecoilFactor->Custom.flValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.SpreadFactor", gSpreadFactor->Custom.flValue);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.FieldOfView", FindVariable("cg_fov")->Current.flValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.FramesPerSecond", FindVariable("com_maxfps")->Current.iValue);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Axis.Red", gColorAxis->Custom.cValue[0]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Axis.Green", gColorAxis->Custom.cValue[1]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Axis.Blue", gColorAxis->Custom.cValue[2]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Axis.Alpha", gColorAxis->Custom.cValue[3]);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Allies.Red", gColorAllies->Custom.cValue[0]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Allies.Green", gColorAllies->Custom.cValue[1]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Allies.Blue", gColorAllies->Custom.cValue[2]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Allies.Alpha", gColorAllies->Custom.cValue[3]);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.RiotShield.Red", gColorRiotShield->Custom.cValue[0]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.RiotShield.Green", gColorRiotShield->Custom.cValue[1]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.RiotShield.Blue", gColorRiotShield->Custom.cValue[2]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.RiotShield.Alpha", gColorRiotShield->Custom.cValue[3]);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.CrossHair.Red", gColorCrossHair->Custom.cValue[0]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.CrossHair.Green", gColorCrossHair->Custom.cValue[1]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.CrossHair.Blue", gColorCrossHair->Custom.cValue[2]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.CrossHair.Alpha", gColorCrossHair->Custom.cValue[3]);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Text.Red", gColorText->Custom.cValue[0]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Text.Green", gColorText->Custom.cValue[1]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Text.Blue", gColorText->Custom.cValue[2]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Text.Alpha", gColorText->Custom.cValue[3]);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Shadow.Red", gColorShadow->Custom.cValue[0]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Shadow.Green", gColorShadow->Custom.cValue[1]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Shadow.Blue", gColorShadow->Custom.cValue[2]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Shadow.Alpha", gColorShadow->Custom.cValue[3]);

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
				szFilePath = acut::AnsiToWstring(acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_XML, " ", ""), CP_ACP);

			else
				szFilePath = acut::AnsiToWstring(path, CP_ACP);

			if (GetFileAttributes(acut::WstringToAnsi(szFilePath, CP_ACP).c_str()) == INVALID_FILE_ATTRIBUTES)
				return false;

			XML.read_from_file(szFilePath);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.Mode", gAimBotMode->Custom.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.AutoZoom", gAutoZoom->Custom.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.AutoFire", gAutoFire->Custom.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.AutoWall", gAutoWall->Custom.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.SilentAim", gSilentAim->Custom.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.TargetAgents", gTargetAgents->Custom.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.TargetMissiles", gTargetMissiles->Custom.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.AntiAim", gAntiAim->Custom.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.BoneScan", gBoneScan->Custom.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.SortMethod", gSortMethod->Custom.iValue);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Mode", gWallHackMode->Custom.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Boxes", gPlayerBoxes->Custom.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Bones", gPlayerBones->Custom.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.SnapLines", gPlayerSnapLines->Custom.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Distances", gPlayerDistances->Custom.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Names", gPlayerNames->Custom.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Weapons", gPlayerWeapons->Custom.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Agents", gAgents->Custom.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Missiles", gMissiles->Custom.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Items", gItems->Custom.bValue);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.KillSpam", gKillSpam->Custom.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.NameStealer", gNameStealer->Custom.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.CrossHair", gPlayerCrossHair->Custom.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.Compass", gPlayerCompass->Custom.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.Radar", gPlayerRadar->Custom.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.RiotShield", gRiotShield->Custom.iValue);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AimBone", gAimBone->Custom.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AimAngle", gAimAngle->Custom.flValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AimPower", gAimPower->Custom.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AutoAimTime", gAutoAimTime->Custom.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AutoAimDelay", gAutoAimDelay->Custom.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AutoZoomDelay", gAutoZoomDelay->Custom.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AutoFireDelay", gAutoFireDelay->Custom.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.RecoilFactor", gRecoilFactor->Custom.flValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.SpreadFactor", gSpreadFactor->Custom.flValue);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.FieldOfView", FindVariable("cg_fov")->Current.flValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.FramesPerSecond", FindVariable("com_maxfps")->Current.iValue);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Axis.Red", gColorAxis->Custom.cValue[0]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Axis.Green", gColorAxis->Custom.cValue[1]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Axis.Blue", gColorAxis->Custom.cValue[2]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Axis.Alpha", gColorAxis->Custom.cValue[3]);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Allies.Red", gColorAllies->Custom.cValue[0]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Allies.Green", gColorAllies->Custom.cValue[1]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Allies.Blue", gColorAllies->Custom.cValue[2]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Allies.Alpha", gColorAllies->Custom.cValue[3]);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.RiotShield.Red", gColorRiotShield->Custom.cValue[0]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.RiotShield.Green", gColorRiotShield->Custom.cValue[1]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.RiotShield.Blue", gColorRiotShield->Custom.cValue[2]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.RiotShield.Alpha", gColorRiotShield->Custom.cValue[3]);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.CrossHair.Red", gColorCrossHair->Custom.cValue[0]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.CrossHair.Green", gColorCrossHair->Custom.cValue[1]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.CrossHair.Blue", gColorCrossHair->Custom.cValue[2]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.CrossHair.Alpha", gColorCrossHair->Custom.cValue[3]);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Text.Red", gColorText->Custom.cValue[0]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Text.Green", gColorText->Custom.cValue[1]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Text.Blue", gColorText->Custom.cValue[2]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Text.Alpha", gColorText->Custom.cValue[3]);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Shadow.Red", gColorShadow->Custom.cValue[0]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Shadow.Green", gColorShadow->Custom.cValue[1]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Shadow.Blue", gColorShadow->Custom.cValue[2]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Shadow.Alpha", gColorShadow->Custom.cValue[3]);

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
		gSortMethod->Custom.iValue = gSortMethod->Default.iValue;

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

		gNameStealer->Custom.bValue = gNameStealer->Default.bValue;
		gThirdPersonAntiAim->Custom.bValue = gThirdPersonAntiAim->Default.bValue;
		gPlayerCrossHair->Custom.bValue = gPlayerCrossHair->Default.bValue;
		gPlayerCompass->Custom.bValue = gPlayerCompass->Default.bValue;
		gPlayerRadar->Custom.bValue = gPlayerRadar->Default.bValue;
		gRiotShield->Custom.iValue = gRiotShield->Default.iValue;

		gNameSpam->Custom.bValue = gNameSpam->Default.bValue;
		gChatSpam->Custom.bValue = gChatSpam->Default.bValue;
		gKillSpam->Custom.bValue = gKillSpam->Default.bValue;
		gSuperJump->Custom.bValue = gSuperJump->Default.bValue;
		gMassKill->Custom.iValue = gMassKill->Default.iValue;
		gBunnyHop->Custom.bValue = gBunnyHop->Default.bValue;
		gTeaBag->Custom.bValue = gTeaBag->Default.bValue;
	}
}

//=====================================================================================