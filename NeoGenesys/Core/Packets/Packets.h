//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cPackets
	{
	public:

		std::shared_ptr<sCvar> gBunnyHop = std::make_shared<sCvar>("Bunny Hop", std::vector<std::string>(), false);

		int iTeaBagTime = -3000;
		ImVec3 vTeaBagPos;

		void WritePacket(sUserCmd* usercmd);
		void PredictPlayerState(sUserCmd* usercmd);
	} extern _packets;
}

//=====================================================================================