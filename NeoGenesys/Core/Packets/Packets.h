//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cPackets
	{
	public:

		void WritePacket(sUserCmd* usercmd);
		void PredictPlayerState(sUserCmd* oldcmd, sUserCmd* newcmd);
	} extern _packets;
}

//=====================================================================================