//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cPackets
	{
	public:

		void ModifyCommands();
		void OldCommand(sUserCMD* usercmd);
		void NewCommand(sUserCMD* usercmd);
	} extern _packets;
}

//=====================================================================================