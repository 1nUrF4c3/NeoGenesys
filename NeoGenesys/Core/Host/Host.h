//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cHost
	{
	public:

		std::vector<sGEntity*> vSpawnedBots;

		void PlayerMods();
		void StartMatch();
		void MassKill();
		void SpawnBots(int count);

		std::mutex _mutex;
	} extern _host;
}

//=====================================================================================
