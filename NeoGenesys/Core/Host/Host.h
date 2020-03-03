//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cHost
	{
	public:

		void PlayerMods();
		void StartMatch();
		void MassKill();

		std::mutex _mutex;
	} extern _host;
}

//=====================================================================================
