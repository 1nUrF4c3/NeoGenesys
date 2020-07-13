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

		bool bIsAirStuck;
		ImVec3 vTeaBagPos;

		sTimer TeaBagTimer;

		void WritePacket(sUserCmd* usercmd);
		void PredictPlayerState(sUserCmd* usercmd);
	} extern _packets;
}

//=====================================================================================