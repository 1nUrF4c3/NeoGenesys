//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cMathematics
	{
	public:

		float CalculateFOV(ImVec3 position);
		float CalculateDistance(ImVec3 start, ImVec3 end);
		ImVec3 AngleToForward(ImVec3 position, ImVec3 angles, float distance);
		float DotProduct(ImVec3 left, ImVec3 right);
		float VectorLength(ImVec3 left, ImVec3 right);
		void VectorAngles(ImVec3 direction, ImVec3& angles);
		void AngleVectors(ImVec3 angles, ImVec3& forward, ImVec3& right, ImVec3& up);
		void VectorNormalize(ImVec3& direction);
		void ClampAngles(ImVec3& angles);
		void ClampMove(char value[]);
		void CalculateAimAngles(ImVec3 start, ImVec3 end, ImVec3& angles);
		void CalculateAntiAimAngles(ImVec3 start, ImVec3 end, ImVec3& angles);
		void MakeVector(ImVec3 angles, ImVec3& out);
		void MovementFix(sUserCmd* usercmd, float yaw, float oldyaw, float forward, float right);
		void WorldToCompass(ImVec3 world, ImVec2 compasspos, float compasssize, ImVec2& screen);
		void WorldToRadar(ImVec3 world, ImVec2 radarpos, float scale, float radarsize, float blipsize, ImVec2& screen);
		void RotatePoint(ImVec3 point, ImVec3 center, float yaw, ImVec3& out);
		void ApplyPositionPrediction(sCEntity* entity);
		void ApplyAnglePrediction(sCEntity* entity);
	} extern _mathematics;
}

//=====================================================================================
