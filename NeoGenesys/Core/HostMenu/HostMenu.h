//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cHostMenu
	{
	public:

		enum eGravityGun
		{
			GRAVITY_GUN_OFF,
			GRAVITY_GUN_EXECUTE,
			GRAVITY_GUN_LAUNCH,
			GRAVITY_GUN_MAX
		};

		enum eMassKill
		{
			MASSKILL_OFF,
			MASSKILL_AXIS,
			MASSKILL_ALLIES,
			MASSKILL_ALL,
			MASSKILL_MAX
		};

		std::shared_ptr<sCvar> gHostMenu = std::make_shared<sCvar>("Host Menu", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gGravityGun = std::make_shared<sCvar>("Gravity Gun", std::vector<std::string>({ "Off", "Execute", "Launch" }), GRAVITY_GUN_OFF, GRAVITY_GUN_OFF, GRAVITY_GUN_MAX - 1);
		std::shared_ptr<sCvar> gMassKill = std::make_shared<sCvar>("Masskill", std::vector<std::string>({ "Off", "Axis", "Allies", "All" }), MASSKILL_OFF, MASSKILL_OFF, MASSKILL_MAX - 1);

		struct sHostMenu
		{
			char szKickReason[64] = { NULL };
			bool bWriteLog = false, bAkimbo;

			int iGravityGunNum;
			float flGravityGunDist;

			struct sPlayerMod
			{
				bool bGodMode = false, bNoClip = false, bInfiniteAmmo = false,
					bInvisibility = false, bSuperSpeed = false, bFreezePosition = false;

				ImVec3 vPosition;
			} PlayerMod[MAX_CLIENTS];

			std::vector<std::pair<int, LPSTR>> vPlayers;
			std::vector<LPCSTR> szTeam = { "Free", "Federation", "Ghosts", "Spectator" };
			std::vector<LPCSTR> vWeaponCompleteNames, vWeaponDisplayNames;
			ImVec3 vTeleport;

			int iPlayer = 0, iWeaponID = 0, iPerkID = 0;

			void GetPlayerSelection()
			{
				for (auto& Player : vPlayers)
				{
					bool bSelected = (iPlayer == Player.first);

					ImGui::PushID(Player.first);

					if (ImGui::Selectable(Player.second, bSelected))
						iPlayer = Player.first;

					if (bSelected)
						ImGui::SetItemDefaultFocus();

					ImGui::PopID();
				}
			}
		} HostMenu;

		void PlayerMods();
		void GravityGun();
		void TeleportAll();
		void StartMatch();
		void MassKill();
		void SpawnBots(int count);
		void DrawHostMenu();

		std::vector<sGEntity*> vSpawnedBots;
		std::mutex _mutex;
	} extern _hostMenu;
}

//=====================================================================================
