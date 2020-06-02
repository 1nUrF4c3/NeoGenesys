//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cHostMenu
	{
	public:

		struct sHostMenu
		{
			char szKickReason[64] = { NULL };
			bool bShowWindow = false, bWriteLog = false;

			struct sPlayerMod
			{
				bool bGodMode = false, bNoClip = false, bInfiniteAmmo = false,
					bInvisibility = false, bSuperSpeed = false, bFreezePosition = false;

				ImVec3 szPosition;
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
		void StartMatch();
		void MassKill();
		void SpawnBots(int count);
		void DrawHostMenu();

		std::vector<sGEntity*> vSpawnedBots;
		std::mutex _mutex;
	} extern _hostMenu;
}

//=====================================================================================