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

		void WritePacket(sUserCmd* currentcmd);
		void CreateNewCommands(sUserCmd* oldcmd, sUserCmd* newcmd);
	} extern _packets;
}

//=====================================================================================