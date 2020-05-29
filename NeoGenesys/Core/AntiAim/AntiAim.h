//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cAntiAim
	{
	public:

		ImVec3 vAntiAimAngles;

		void AntiAim(sUserCmd* usercmd);
		bool IsAntiAiming();
	} extern _antiAim;
}

//=====================================================================================
