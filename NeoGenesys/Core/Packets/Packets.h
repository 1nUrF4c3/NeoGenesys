//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cPackets
	{
	public:

		int iTeaBagTime;
		Vector3 vTeaBagPos;

		void WritePacket(sUserCmd* usercmd);
		void PredictPlayerState(sUserCmd* usercmd);
	} extern _packets;
}

//=====================================================================================