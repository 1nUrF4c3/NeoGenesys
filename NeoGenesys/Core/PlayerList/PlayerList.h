//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cPlayerList
	{
	public:

		struct sPlayerList
		{
			bool bShowWindow = false, bWriteLog = false;
			sNetAddr NetAddr[MAX_CLIENTS];
		} PlayerList;

		void DrawPlayerList();
	} extern _playerList;
}

//=====================================================================================
