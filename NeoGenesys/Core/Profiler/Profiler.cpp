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

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.Mode", gAimBotMode->Current.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.AutoZoom", gAutoZoom->Current.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.AutoFire", gAutoFire->Current.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.AutoWall", gAutoWall->Current.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.SilentAim", gSilentAim->Current.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.TargetAgents", gTargetAgents->Current.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.TargetMissiles", gTargetMissiles->Current.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.AntiAim", gAntiAim->Current.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.BoneScan", gBoneScan->Current.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.SortMethod", gSortMethod->Current.iValue);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Mode", gWallHackMode->Current.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Boxes", gPlayerBoxes->Current.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Bones", gPlayerBones->Current.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.SnapLines", gPlayerSnapLines->Current.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Information", gPlayerInformation->Current.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Weapons", gPlayerWeapons->Current.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.BulletTracers", gPlayerBulletTracers->Current.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Agents", gAgents->Current.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Missiles", gMissiles->Current.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Items", gItems->Current.bValue);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.KillSpam", gKillSpam->Current.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.NameStealer", gNameStealer->Current.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.CrossHair", gPlayerCrossHair->Current.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.Compass", gPlayerCompass->Current.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.Radar", gPlayerRadar->Current.bValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.RiotShield", gRiotShield->Current.iValue);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AimBone", gAimBone->Current.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AimAngle", gAimAngle->Current.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AimPower", gAimPower->Current.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AutoAimTime", gAutoAimTime->Current.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AutoAimDelay", gAutoAimDelay->Current.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AutoZoomDelay", gAutoZoomDelay->Current.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AutoFireDelay", gAutoFireDelay->Current.iValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.RecoilFactor", gRecoilFactor->Current.flValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.SpreadFactor", gSpreadFactor->Current.flValue);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.FieldOfView", FindVariable("cg_fov")->Current.flValue);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.FramesPerSecond", FindVariable("com_maxfps")->Current.iValue);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Axis.Red", gColorAxis->Current.cValue[0]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Axis.Green", gColorAxis->Current.cValue[1]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Axis.Blue", gColorAxis->Current.cValue[2]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Axis.Alpha", gColorAxis->Current.cValue[3]);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Allies.Red", gColorAllies->Current.cValue[0]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Allies.Green", gColorAllies->Current.cValue[1]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Allies.Blue", gColorAllies->Current.cValue[2]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Allies.Alpha", gColorAllies->Current.cValue[3]);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Accents.Red", gColorAccents->Current.cValue[0]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Accents.Green", gColorAccents->Current.cValue[1]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Accents.Blue", gColorAccents->Current.cValue[2]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Accents.Alpha", gColorAccents->Current.cValue[3]);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.CrossHair.Red", gColorCrossHair->Current.cValue[0]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.CrossHair.Green", gColorCrossHair->Current.cValue[1]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.CrossHair.Blue", gColorCrossHair->Current.cValue[2]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.CrossHair.Alpha", gColorCrossHair->Current.cValue[3]);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Text.Red", gColorText->Current.cValue[0]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Text.Green", gColorText->Current.cValue[1]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Text.Blue", gColorText->Current.cValue[2]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Text.Alpha", gColorText->Current.cValue[3]);

			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Shadow.Red", gColorShadow->Current.cValue[0]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Shadow.Green", gColorShadow->Current.cValue[1]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Shadow.Blue", gColorShadow->Current.cValue[2]);
			XML.set(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Shadow.Alpha", gColorShadow->Current.cValue[3]);

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

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.Mode", gAimBotMode->Current.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.AutoZoom", gAutoZoom->Current.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.AutoFire", gAutoFire->Current.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.AutoWall", gAutoWall->Current.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.SilentAim", gSilentAim->Current.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.TargetAgents", gTargetAgents->Current.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.TargetMissiles", gTargetMissiles->Current.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.AntiAim", gAntiAim->Current.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.BoneScan", gBoneScan->Current.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".AimBot.SortMethod", gSortMethod->Current.iValue);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Mode", gWallHackMode->Current.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Boxes", gPlayerBoxes->Current.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Bones", gPlayerBones->Current.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.SnapLines", gPlayerSnapLines->Current.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Information", gPlayerInformation->Current.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Weapons", gPlayerWeapons->Current.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.BulletTracers", gPlayerBulletTracers->Current.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Agents", gAgents->Current.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Missiles", gMissiles->Current.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".WallHack.Items", gItems->Current.bValue);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.KillSpam", gKillSpam->Current.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.NameStealer", gNameStealer->Current.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.CrossHair", gPlayerCrossHair->Current.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.Compass", gPlayerCompass->Current.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.Radar", gPlayerRadar->Current.bValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Miscellaneous.RiotShield", gRiotShield->Current.iValue);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AimBone", gAimBone->Current.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AimAngle", gAimAngle->Current.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AimPower", gAimPower->Current.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AutoAimTime", gAutoAimTime->Current.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AutoAimDelay", gAutoAimDelay->Current.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AutoZoomDelay", gAutoZoomDelay->Current.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.AutoFireDelay", gAutoFireDelay->Current.iValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.RecoilFactor", gRecoilFactor->Current.flValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.SpreadFactor", gSpreadFactor->Current.flValue);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.FieldOfView", FindVariable("cg_fov")->Current.flValue);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Tweaks.FramesPerSecond", FindVariable("com_maxfps")->Current.iValue);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Axis.Red", gColorAxis->Current.cValue[0]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Axis.Green", gColorAxis->Current.cValue[1]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Axis.Blue", gColorAxis->Current.cValue[2]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Axis.Alpha", gColorAxis->Current.cValue[3]);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Allies.Red", gColorAllies->Current.cValue[0]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Allies.Green", gColorAllies->Current.cValue[1]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Allies.Blue", gColorAllies->Current.cValue[2]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Allies.Alpha", gColorAllies->Current.cValue[3]);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Accents.Red", gColorAccents->Current.cValue[0]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Accents.Green", gColorAccents->Current.cValue[1]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Accents.Blue", gColorAccents->Current.cValue[2]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Accents.Alpha", gColorAccents->Current.cValue[3]);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.CrossHair.Red", gColorCrossHair->Current.cValue[0]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.CrossHair.Green", gColorCrossHair->Current.cValue[1]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.CrossHair.Blue", gColorCrossHair->Current.cValue[2]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.CrossHair.Alpha", gColorCrossHair->Current.cValue[3]);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Text.Red", gColorText->Current.cValue[0]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Text.Green", gColorText->Current.cValue[1]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Text.Blue", gColorText->Current.cValue[2]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Text.Alpha", gColorText->Current.cValue[3]);

			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Shadow.Red", gColorShadow->Current.cValue[0]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Shadow.Green", gColorShadow->Current.cValue[1]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Shadow.Blue", gColorShadow->Current.cValue[2]);
			XML.get_if_present(acut::FindAndReplaceString(PROGRAM_NAME, " ", "") + ".Colors.Shadow.Alpha", gColorShadow->Current.cValue[3]);

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
		gAimBotMode->Current.iValue = gAimBotMode->Reset.iValue;
		gAutoZoom->Current.bValue = gAutoZoom->Reset.bValue;
		gAutoFire->Current.bValue = gAutoFire->Reset.bValue;
		gAutoWall->Current.bValue = gAutoWall->Reset.bValue;
		gSilentAim->Current.bValue = gSilentAim->Reset.bValue;
		gTargetAgents->Current.bValue = gTargetAgents->Reset.bValue;
		gTargetMissiles->Current.bValue = gTargetMissiles->Reset.bValue;
		gAntiAim->Current.iValue = gAntiAim->Reset.iValue;
		gBoneScan->Current.iValue = gBoneScan->Reset.iValue;
		gSortMethod->Current.iValue = gSortMethod->Reset.iValue;

		gWallHackMode->Current.iValue = gWallHackMode->Reset.iValue;
		gPlayerBoxes->Current.iValue = gPlayerBoxes->Reset.iValue;
		gPlayerBones->Current.iValue = gPlayerBones->Reset.iValue;
		gPlayerSnapLines->Current.iValue = gPlayerSnapLines->Reset.iValue;
		gPlayerInformation->Current.bValue = gPlayerInformation->Reset.bValue;
		gPlayerWeapons->Current.bValue = gPlayerWeapons->Reset.bValue;
		gPlayerBulletTracers->Current.bValue = gPlayerBulletTracers->Reset.bValue;
		gAgents->Current.bValue = gAgents->Reset.bValue;
		gMissiles->Current.bValue = gMissiles->Reset.bValue;
		gItems->Current.bValue = gItems->Reset.bValue;

		gNameStealer->Current.bValue = gNameStealer->Reset.bValue;
		gThirdPersonAntiAim->Current.bValue = gThirdPersonAntiAim->Reset.bValue;
		gPlayerCrossHair->Current.bValue = gPlayerCrossHair->Reset.bValue;
		gPlayerCompass->Current.bValue = gPlayerCompass->Reset.bValue;
		gPlayerRadar->Current.bValue = gPlayerRadar->Reset.bValue;
		gRiotShield->Current.iValue = gRiotShield->Reset.iValue;

		gNameSpam->Current.bValue = gNameSpam->Reset.bValue;
		gChatSpam->Current.bValue = gChatSpam->Reset.bValue;
		gKillSpam->Current.bValue = gKillSpam->Reset.bValue;
		gSuperJump->Current.bValue = gSuperJump->Reset.bValue;
		gMassKill->Current.iValue = gMassKill->Reset.iValue;
		gBunnyHop->Current.bValue = gBunnyHop->Reset.bValue;
		gTeaBag->Current.bValue = gTeaBag->Reset.bValue;
	}
}

//=====================================================================================