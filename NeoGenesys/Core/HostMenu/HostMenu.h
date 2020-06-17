//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cHostMenu
	{
	public:

		typedef enum
		{
			MASSKILL_OFF,
			MASSKILL_AXIS,
			MASSKILL_ALLIES,
			MASSKILL_ALL,
			MASSKILL_MAX
		} eMassKill;

		std::shared_ptr<sCvar> gSuperJump = std::make_shared<sCvar>("Super Jump", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gGravityGun = std::make_shared<sCvar>("Gravity Gun", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gMassKill = std::make_shared<sCvar>("Masskill", std::vector<std::string>(), MASSKILL_OFF, MASSKILL_OFF, MASSKILL_MAX - 1);

		struct sHostMenu
		{
			char szKickReason[64] = { NULL };
			bool bShowWindow = false, bWriteLog = false;

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
