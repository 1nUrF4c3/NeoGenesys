//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cPlayerList
	{
	public:

		std::shared_ptr<sCvar> gPlayerList = std::make_shared<sCvar>("Player List", std::vector<std::string>(), false);

		struct sPlayerList
		{
			bool bWriteLog = false;
			sNetAddr NetAddr[MAX_CLIENTS];
		} PlayerList;

		void DrawPlayerList();
	} extern _playerList;
}

//=====================================================================================
