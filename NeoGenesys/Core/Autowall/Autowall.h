//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cAutowall
	{
	public:

		float C_Autowall(sCEntity* entity, ImVec3 start, ImVec3 end, eHitLocation hitloc);
		float C_TraceBullet(sCEntity* entity, ImVec3 start, ImVec3 end, eHitLocation hitloc);
		float G_Autowall(sGEntity* entity, ImVec3 start, ImVec3 end);
		float G_TraceBullet(sGEntity* entity, ImVec3 start, ImVec3 end);
		float GetRemainingDamage(sBulletFireParams* fireparams, sBulletTraceResults* traceresults, eHitLocation hitloc, int weapon, bool alternate);
		bool TraceLine(sCEntity* entity, ImVec3 start, ImVec3 end);
	} extern _autoWall;
}

//=====================================================================================
