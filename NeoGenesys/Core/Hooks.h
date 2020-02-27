//=====================================================================================

#pragma once

#include "Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cHooks
	{
	public:

		void Refresh(int localnum);
		void WritePacket(int localnum);
		void PredictPlayerState(int localnum);
		void Obituary(int localnum, sEntityState* entitystate, int weapon);
		void AddCmdDrawText(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, RGBA color, int flags);
		void ClientFrame(sGEntity* entity);
	} extern _hooks;
}

//=====================================================================================