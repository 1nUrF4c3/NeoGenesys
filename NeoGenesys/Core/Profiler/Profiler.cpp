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

			XML.set("NeoGenesys.AimBot.Mode", gAimBotMode->Current.iValue);
			XML.set("NeoGenesys.AimBot.AutoZoom", gAutoZoom->Current.bValue);
			XML.set("NeoGenesys.AimBot.AutoFire", gAutoFire->Current.bValue);
			XML.set("NeoGenesys.AimBot.AutoWall", gAutoWall->Current.bValue);
			XML.set("NeoGenesys.AimBot.SilentAim", gSilentAim->Current.bValue);
			XML.set("NeoGenesys.AimBot.TargetAgents", gTargetAgents->Current.bValue);
			XML.set("NeoGenesys.AimBot.TargetMissiles", gTargetMissiles->Current.bValue);
			XML.set("NeoGenesys.AimBot.AntiAim", gAntiAim->Current.iValue);
			XML.set("NeoGenesys.AimBot.BoneScan", gBoneScan->Current.iValue);
			XML.set("NeoGenesys.AimBot.SortMethod", gSortMethod->Current.iValue);

			XML.set("NeoGenesys.WallHack.Mode", gWallHackMode->Current.iValue);
			XML.set("NeoGenesys.WallHack.Boxes", gPlayerBoxes->Current.iValue);
			XML.set("NeoGenesys.WallHack.Bones", gPlayerBones->Current.iValue);
			XML.set("NeoGenesys.WallHack.SnapLines", gPlayerSnapLines->Current.iValue);
			XML.set("NeoGenesys.WallHack.Information", gPlayerInformation->Current.bValue);
			XML.set("NeoGenesys.WallHack.Weapons", gPlayerWeapons->Current.bValue);
			XML.set("NeoGenesys.WallHack.BulletTracers", gPlayerBulletTracers->Current.bValue);
			XML.set("NeoGenesys.WallHack.Agents", gAgents->Current.bValue);
			XML.set("NeoGenesys.WallHack.Missiles", gMissiles->Current.bValue);
			XML.set("NeoGenesys.WallHack.Items", gItems->Current.bValue);

			XML.set("NeoGenesys.Miscellaneous.KillSpam", gKillSpam->Current.bValue);
			XML.set("NeoGenesys.Miscellaneous.NameStealer", gNameStealer->Current.bValue);
			XML.set("NeoGenesys.Miscellaneous.ThirdPerson", gThirdPerson->Current.bValue);
			XML.set("NeoGenesys.Miscellaneous.CrossHair", gPlayerCrossHair->Current.bValue);
			XML.set("NeoGenesys.Miscellaneous.Compass", gPlayerCompass->Current.bValue);
			XML.set("NeoGenesys.Miscellaneous.Radar", gPlayerRadar->Current.bValue);
			XML.set("NeoGenesys.Miscellaneous.RiotShield", gRiotShield->Current.iValue);

			XML.set("NeoGenesys.Tweaks.AimBone", gAimBone->Current.iValue);
			XML.set("NeoGenesys.Tweaks.AimAngle", gAimAngle->Current.iValue);
			XML.set("NeoGenesys.Tweaks.AimPower", gAimPower->Current.iValue);
			XML.set("NeoGenesys.Tweaks.AutoAimTime", gAutoAimTime->Current.iValue);
			XML.set("NeoGenesys.Tweaks.AutoAimDelay", gAutoAimDelay->Current.iValue);
			XML.set("NeoGenesys.Tweaks.AutoZoomDelay", gAutoZoomDelay->Current.iValue);
			XML.set("NeoGenesys.Tweaks.AutoFireDelay", gAutoFireDelay->Current.iValue);
			XML.set("NeoGenesys.Tweaks.RecoilFactor", gRecoilFactor->Current.flValue);
			XML.set("NeoGenesys.Tweaks.SpreadFactor", gSpreadFactor->Current.flValue);

			XML.set("NeoGenesys.Tweaks.FieldOfView", FindVariable("cg_fov")->Current.flValue);
			XML.set("NeoGenesys.Tweaks.FramesPerSecond", FindVariable("com_maxfps")->Current.iValue);

			XML.set("NeoGenesys.Colors.Axis.Red", gColorAxis->Current.cValue[0]);
			XML.set("NeoGenesys.Colors.Axis.Green", gColorAxis->Current.cValue[1]);
			XML.set("NeoGenesys.Colors.Axis.Blue", gColorAxis->Current.cValue[2]);
			XML.set("NeoGenesys.Colors.Axis.Alpha", gColorAxis->Current.cValue[3]);

			XML.set("NeoGenesys.Colors.Allies.Red", gColorAllies->Current.cValue[0]);
			XML.set("NeoGenesys.Colors.Allies.Green", gColorAllies->Current.cValue[1]);
			XML.set("NeoGenesys.Colors.Allies.Blue", gColorAllies->Current.cValue[2]);
			XML.set("NeoGenesys.Colors.Allies.Alpha", gColorAllies->Current.cValue[3]);

			XML.set("NeoGenesys.Colors.Accents.Red", gColorAccents->Current.cValue[0]);
			XML.set("NeoGenesys.Colors.Accents.Green", gColorAccents->Current.cValue[1]);
			XML.set("NeoGenesys.Colors.Accents.Blue", gColorAccents->Current.cValue[2]);
			XML.set("NeoGenesys.Colors.Accents.Alpha", gColorAccents->Current.cValue[3]);

			XML.set("NeoGenesys.Colors.CrossHair.Red", gColorCrossHair->Current.cValue[0]);
			XML.set("NeoGenesys.Colors.CrossHair.Green", gColorCrossHair->Current.cValue[1]);
			XML.set("NeoGenesys.Colors.CrossHair.Blue", gColorCrossHair->Current.cValue[2]);
			XML.set("NeoGenesys.Colors.CrossHair.Alpha", gColorCrossHair->Current.cValue[3]);

			XML.set("NeoGenesys.Colors.Text.Red", gColorText->Current.cValue[0]);
			XML.set("NeoGenesys.Colors.Text.Green", gColorText->Current.cValue[1]);
			XML.set("NeoGenesys.Colors.Text.Blue", gColorText->Current.cValue[2]);
			XML.set("NeoGenesys.Colors.Text.Alpha", gColorText->Current.cValue[3]);

			XML.set("NeoGenesys.Colors.Shadow.Red", gColorShadow->Current.cValue[0]);
			XML.set("NeoGenesys.Colors.Shadow.Green", gColorShadow->Current.cValue[1]);
			XML.set("NeoGenesys.Colors.Shadow.Blue", gColorShadow->Current.cValue[2]);
			XML.set("NeoGenesys.Colors.Shadow.Alpha", gColorShadow->Current.cValue[3]);

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

			XML.get_if_present("NeoGenesys.AimBot.Mode", gAimBotMode->Current.iValue);
			XML.get_if_present("NeoGenesys.AimBot.AutoZoom", gAutoZoom->Current.bValue);
			XML.get_if_present("NeoGenesys.AimBot.AutoFire", gAutoFire->Current.bValue);
			XML.get_if_present("NeoGenesys.AimBot.AutoWall", gAutoWall->Current.bValue);
			XML.get_if_present("NeoGenesys.AimBot.SilentAim", gSilentAim->Current.bValue);
			XML.get_if_present("NeoGenesys.AimBot.TargetAgents", gTargetAgents->Current.bValue);
			XML.get_if_present("NeoGenesys.AimBot.TargetMissiles", gTargetMissiles->Current.bValue);
			XML.get_if_present("NeoGenesys.AimBot.AntiAim", gAntiAim->Current.iValue);
			XML.get_if_present("NeoGenesys.AimBot.BoneScan", gBoneScan->Current.iValue);
			XML.get_if_present("NeoGenesys.AimBot.SortMethod", gSortMethod->Current.iValue);

			XML.get_if_present("NeoGenesys.WallHack.Mode", gWallHackMode->Current.iValue);
			XML.get_if_present("NeoGenesys.WallHack.Boxes", gPlayerBoxes->Current.iValue);
			XML.get_if_present("NeoGenesys.WallHack.Bones", gPlayerBones->Current.iValue);
			XML.get_if_present("NeoGenesys.WallHack.SnapLines", gPlayerSnapLines->Current.iValue);
			XML.get_if_present("NeoGenesys.WallHack.Information", gPlayerInformation->Current.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Weapons", gPlayerWeapons->Current.bValue);
			XML.get_if_present("NeoGenesys.WallHack.BulletTracers", gPlayerBulletTracers->Current.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Agents", gAgents->Current.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Missiles", gMissiles->Current.bValue);
			XML.get_if_present("NeoGenesys.WallHack.Items", gItems->Current.bValue);

			XML.get_if_present("NeoGenesys.Miscellaneous.KillSpam", gKillSpam->Current.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.NameStealer", gNameStealer->Current.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.ThirdPerson", gThirdPerson->Current.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.CrossHair", gPlayerCrossHair->Current.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.Compass", gPlayerCompass->Current.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.Radar", gPlayerRadar->Current.bValue);
			XML.get_if_present("NeoGenesys.Miscellaneous.RiotShield", gRiotShield->Current.iValue);

			XML.get_if_present("NeoGenesys.Tweaks.AimBone", gAimBone->Current.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AimAngle", gAimAngle->Current.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AimPower", gAimPower->Current.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AutoAimTime", gAutoAimTime->Current.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AutoAimDelay", gAutoAimDelay->Current.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AutoZoomDelay", gAutoZoomDelay->Current.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.AutoFireDelay", gAutoFireDelay->Current.iValue);
			XML.get_if_present("NeoGenesys.Tweaks.RecoilFactor", gRecoilFactor->Current.flValue);
			XML.get_if_present("NeoGenesys.Tweaks.SpreadFactor", gSpreadFactor->Current.flValue);

			XML.get_if_present("NeoGenesys.Tweaks.FieldOfView", FindVariable("cg_fov")->Current.flValue);
			XML.get_if_present("NeoGenesys.Tweaks.FramesPerSecond", FindVariable("com_maxfps")->Current.iValue);

			XML.get_if_present("NeoGenesys.Colors.Axis.Red", gColorAxis->Current.cValue[0]);
			XML.get_if_present("NeoGenesys.Colors.Axis.Green", gColorAxis->Current.cValue[1]);
			XML.get_if_present("NeoGenesys.Colors.Axis.Blue", gColorAxis->Current.cValue[2]);
			XML.get_if_present("NeoGenesys.Colors.Axis.Alpha", gColorAxis->Current.cValue[3]);

			XML.get_if_present("NeoGenesys.Colors.Allies.Red", gColorAllies->Current.cValue[0]);
			XML.get_if_present("NeoGenesys.Colors.Allies.Green", gColorAllies->Current.cValue[1]);
			XML.get_if_present("NeoGenesys.Colors.Allies.Blue", gColorAllies->Current.cValue[2]);
			XML.get_if_present("NeoGenesys.Colors.Allies.Alpha", gColorAllies->Current.cValue[3]);

			XML.get_if_present("NeoGenesys.Colors.Accents.Red", gColorAccents->Current.cValue[0]);
			XML.get_if_present("NeoGenesys.Colors.Accents.Green", gColorAccents->Current.cValue[1]);
			XML.get_if_present("NeoGenesys.Colors.Accents.Blue", gColorAccents->Current.cValue[2]);
			XML.get_if_present("NeoGenesys.Colors.Accents.Alpha", gColorAccents->Current.cValue[3]);

			XML.get_if_present("NeoGenesys.Colors.CrossHair.Red", gColorCrossHair->Current.cValue[0]);
			XML.get_if_present("NeoGenesys.Colors.CrossHair.Green", gColorCrossHair->Current.cValue[1]);
			XML.get_if_present("NeoGenesys.Colors.CrossHair.Blue", gColorCrossHair->Current.cValue[2]);
			XML.get_if_present("NeoGenesys.Colors.CrossHair.Alpha", gColorCrossHair->Current.cValue[3]);

			XML.get_if_present("NeoGenesys.Colors.Text.Red", gColorText->Current.cValue[0]);
			XML.get_if_present("NeoGenesys.Colors.Text.Green", gColorText->Current.cValue[1]);
			XML.get_if_present("NeoGenesys.Colors.Text.Blue", gColorText->Current.cValue[2]);
			XML.get_if_present("NeoGenesys.Colors.Text.Alpha", gColorText->Current.cValue[3]);

			XML.get_if_present("NeoGenesys.Colors.Shadow.Red", gColorShadow->Current.cValue[0]);
			XML.get_if_present("NeoGenesys.Colors.Shadow.Green", gColorShadow->Current.cValue[1]);
			XML.get_if_present("NeoGenesys.Colors.Shadow.Blue", gColorShadow->Current.cValue[2]);
			XML.get_if_present("NeoGenesys.Colors.Shadow.Alpha", gColorShadow->Current.cValue[3]);

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
		gThirdPerson->Current.bValue = gThirdPerson->Reset.bValue;
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