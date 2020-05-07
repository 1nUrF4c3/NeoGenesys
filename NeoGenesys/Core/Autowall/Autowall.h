//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cAutowall
	{
	public:

		float C_Autowall(sCEntity* entity, Vector3 start, Vector3 end, short hitloc);
		float C_TraceBullet(sCEntity* entity, Vector3 start, Vector3 end, short hitloc);
		float G_Autowall(sCEntity* entity, Vector3 start, Vector3 end);
		float G_TraceBullet(sCEntity* entity, Vector3 start, Vector3 end);
		float GetRemainingDamage(sBulletFireParams* fireparams, sBulletTraceResults* traceresults, short partgroup, int weapon, bool alternate);
		bool TraceLine(sCEntity* entity, Vector3 start, Vector3 end);
	} extern _autoWall;
}

//=====================================================================================
