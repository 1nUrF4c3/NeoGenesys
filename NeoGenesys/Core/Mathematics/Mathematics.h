//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cMathematics
	{
	public:

		void VectorAngles(Vector3 direction, Vector3 angles);
		void AngleVectors(Vector3 angles, Vector3 forward, Vector3 right, Vector3 up);
		void VectorNormalize(Vector3 direction);
		void NormalizeAngles(Vector3 angles);
		void CalculateAngles(Vector3 position, Vector3 angles);
		float CalculateFOV(Vector3 position);
		void MakeVector(Vector3 angles, Vector3 out);
		void MovementFix(sUserCMD* usercmd, float yaw);
		void WorldToCompass(Vector3 world, ImVec2 compasspos, float compasssize, ImVec2& screen);
		void WorldToRadar(Vector3 world, ImVec2 radarpos, float scale, float radarsize, float blipsize, ImVec2& screen);
		void RotatePoint(Vector3 point, Vector3 center, float yaw, Vector3 out);
	} extern _mathematics;
}

//=====================================================================================
