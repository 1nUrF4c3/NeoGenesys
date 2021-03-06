//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cMathematics _mathematics;

	float cMathematics::CalculateFOV(ImVec3 start, ImVec3 end, ImVec3 angles)
	{
		ImVec3 vDirection = start - end, vAngles, vAimAngles;

		VectorNormalize(vDirection);
		VectorAngles(vDirection, vAimAngles);

		MakeVector(angles, vAngles);
		MakeVector(vAimAngles, vAimAngles);

		ImVec2 vLengthDot
		(
			VectorLength3D(vAngles, vAngles),
			DotProduct3D(vAngles, vAimAngles)
		);

		float flReturn = RadiansToDegrees(acosf(vLengthDot.y / powf(vLengthDot.x, 2.0f)));

		if (isnan(flReturn))
			flReturn = 0.0f;

		return flReturn;
	}
	/*
	//=====================================================================================
	*/
	float cMathematics::DotProduct3D(ImVec3 left, ImVec3 right)
	{
		return (left.x * right.x + left.y * right.y + left.z * right.z);
	}
	/*
	//=====================================================================================
	*/
	float cMathematics::DotProduct2D(ImVec2 left, ImVec2 right)
	{
		return (left.x * right.x + left.y * right.y);
	}
	/*
	//=====================================================================================
	*/
	float cMathematics::VectorLength3D(ImVec3 left, ImVec3 right)
	{
		return sqrtf(DotProduct3D(left, right));
	}
	/*
	//=====================================================================================
	*/
	float cMathematics::VectorLength2D(ImVec2 left, ImVec2 right)
	{
		return sqrtf(DotProduct2D(left, right));
	}
	/*
	//=====================================================================================
	*/
	float cMathematics::CalculateDistance3D(ImVec3 start, ImVec3 end)
	{
		ImVec3 vDirection;

		vDirection = start - end;

		return VectorLength3D(vDirection, vDirection);
	}
	/*
	//=====================================================================================
	*/
	float cMathematics::CalculateDistance2D(ImVec2 start, ImVec2 end)
	{
		ImVec2 vDirection;

		vDirection = start - end;

		return VectorLength2D(vDirection, vDirection);
	}
	/*
	//=====================================================================================
	*/
	ImVec3 cMathematics::AngleToForward(ImVec3 position, ImVec3 angles, float distance)
	{
		ImVec3 vForward, vRight, vUp;

		AngleVectors(angles, vForward, vRight, vUp);

		return (position + (vForward * distance));
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::VectorAngles(ImVec3 direction, ImVec3& angles)
	{
		float flTemp, flYaw, flPitch;

		if (direction.x == 0.0f && direction.y == 0.0f)
		{
			flYaw = 0.0f;

			if (direction.z > 0.0f)
				flPitch = 90.0f;

			else
				flPitch = 270.0f;
		}

		else
		{
			flYaw = RadiansToDegrees(atan2f(direction.y, direction.x));

			if (flYaw < 0.0f)
				flYaw += 360.0f;

			flTemp = sqrtf(direction.x * direction.x + direction.y * direction.y);
			flPitch = RadiansToDegrees(atan2f(direction.z, flTemp));

			if (flPitch < 0.0f)
				flPitch += 360.0f;
		}

		angles.x = -flPitch;
		angles.y = flYaw;
		angles.z = 0.0f;
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::AngleVectors(ImVec3 angles, ImVec3& forward, ImVec3& right, ImVec3& up)
	{
		float flAngle, flSinRoll, flSinPitch, flSinYaw, flCosRoll, flCosPitch, flCosYaw;

		flAngle = DegreesToRadians(angles.y);
		flSinYaw = sinf(flAngle);
		flCosYaw = cosf(flAngle);

		flAngle = DegreesToRadians(angles.x);
		flSinPitch = sinf(flAngle);
		flCosPitch = cosf(flAngle);

		flAngle = DegreesToRadians(angles.z);
		flSinRoll = sinf(flAngle);
		flCosRoll = cosf(flAngle);

		if (forward)
		{
			forward.x = flCosPitch * flCosYaw;
			forward.y = flCosPitch * flSinYaw;
			forward.z = -flSinPitch;
		}

		if (right)
		{
			right.x = (-1.0f * flSinRoll * flSinPitch * flCosYaw + -1.0f * flCosRoll * -flSinYaw);
			right.y = (-1.0f * flSinRoll * flSinPitch * flSinYaw + -1.0f * flCosRoll * flCosYaw);
			right.z = -1.0f * flSinRoll * flCosPitch;
		}

		if (up)
		{
			up.x = (flCosRoll * flSinPitch * flCosYaw + -flSinRoll * -flSinYaw);
			up.y = (flCosRoll * flSinPitch * flSinYaw + -flSinRoll * flCosYaw);
			up.z = flCosRoll * flCosPitch;
		}
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::VectorNormalize(ImVec3& direction)
	{
		float flLen = VectorLength3D(direction, direction);

		if (flLen == 0.0f)
		{
			direction.x = 0.0f;
			direction.y = 0.0f;
			direction.z = 1.0f;
		}

		else
		{
			flLen = 1.0f / flLen;

			direction *= flLen;
		}
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::ClampAngles(ImVec3& angles)
	{
		while (angles.x > 180.0f)
			angles.x -= 360.0f;

		while (angles.x < -180.0f)
			angles.x += 360.0f;

		while (angles.y > 180.0f)
			angles.y -= 360.0f;

		while (angles.y < -180.0f)
			angles.y += 360.0f;

		if (angles.z != 0.0f)
			angles.z = 0.0f;
	}
	/*
	//=====================================================================================
	*/
	char cMathematics::ClampMove(char value)
	{
		if (value < -128)
			return -128;

		if (value > 127)
			return 127;

		return value;
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::CalculateAimAngles(ImVec3 start, ImVec3 end, ImVec3& angles)
	{
		ImVec3 vDirection;
		vDirection = start - end;

		VectorNormalize(vDirection);
		VectorAngles(vDirection, angles);

		ClampAngles(angles);

		angles -= GetViewAngles();

		ClampAngles(angles);
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::CalculateAntiAimAngles(ImVec3 start, ImVec3 end, ImVec3& angles)
	{
		ImVec3 vDirection;
		vDirection = start - end;

		VectorNormalize(vDirection);
		VectorAngles(vDirection, angles);

		ClampAngles(angles);

		angles.x = -45.0f - angles.x;
		angles.y = angles.y - 180.0f;

		angles -= CG->PredictedPlayerState.vDeltaAngles;

		ClampAngles(angles);
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::MakeVector(ImVec3 angles, ImVec3& out)
	{
		ImVec2 vAngles
		(
			DegreesToRadians(angles.x),
			DegreesToRadians(angles.y)
		);

		out.x = -cosf(vAngles.x) * -cosf(vAngles.y);
		out.y = sinf(vAngles.y) * cosf(vAngles.x);
		out.z = -sinf(vAngles.x);
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::MovementFix(sUserCmd* usercmd, float yaw, float oldyaw, float forward, float right)
	{
		float flDelta = DegreesToRadians(yaw - oldyaw);

		usercmd->szForwardMove = ClampMove((char)(cosf(flDelta) * forward - sinf(flDelta) * right));
		usercmd->szRightMove = ClampMove((char)(sinf(flDelta) * forward + cosf(flDelta) * right));
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::WorldToCompass(ImVec3 world, ImVec2 compasspos, float compasssize, ImVec2& screen)
	{
		float flAngle;

		ImVec3 vDirection, vAngles;

		vDirection = GetViewOrigin() - world;

		VectorNormalize(vDirection);
		VectorAngles(vDirection, vAngles);

		vAngles = GetViewAngles() - vAngles;

		ClampAngles(vAngles);

		flAngle = ((vAngles.y + 180.0f) / 360.0f - 0.25f) * M_PI_DOUBLE;

		screen.x = compasspos.x + (cosf(flAngle) * (compasssize / 2.0f));
		screen.y = compasspos.y + (sinf(flAngle) * (compasssize / 2.0f));
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::WorldToRadar(ImVec3 world, ImVec2 radarpos, float scale, float radarsize, float blipsize, ImVec2& screen)
	{
		ImVec2 vDelta, vLocation;
		ImVec2 vCosSin
		(
			cosf(DegreesToRadians(GetViewAngles().y)),
			sinf(DegreesToRadians(GetViewAngles().y))
		);

		vDelta.x = world.x - GetViewOrigin().x;
		vDelta.y = world.y - GetViewOrigin().y;

		vLocation.x = (vDelta.y * vCosSin.x - vDelta.x * vCosSin.y) / scale;
		vLocation.y = (vDelta.x * vCosSin.x + vDelta.y * vCosSin.y) / scale;

		if (vLocation.x < -(radarsize / 2.0f - blipsize / 2.0f - 1.0f))
			vLocation.x = -(radarsize / 2.0f - blipsize / 2.0f - 1.0f);

		else if (vLocation.x > (radarsize / 2.0f - blipsize / 2.0f - 1.0f))
			vLocation.x = (radarsize / 2.0f - blipsize / 2.0f - 1.0f);

		if (vLocation.y < -(radarsize / 2.0f - blipsize / 2.0f - 1.0f))
			vLocation.y = -(radarsize / 2.0f - blipsize / 2.0f - 1.0f);

		else if (vLocation.y > (radarsize / 2.0f - blipsize / 2.0f))
			vLocation.y = (radarsize / 2.0f - blipsize / 2.0f);

		screen = (vLocation * -1.0f) + radarpos;
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::RotatePoint(ImVec3 point, ImVec3 center, float yaw, ImVec3& out)
	{
		ImVec2 vDelta;
		ImVec2 vCosSin
		(
			cosf(((-yaw + 180.0f) / 360.0f - 0.25f) * M_PI_DOUBLE),
			sinf(((-yaw + 180.0f) / 360.0f - 0.25f) * M_PI_DOUBLE)
		);

		vDelta.x = point.x - center.x;
		vDelta.y = point.y - center.y;

		out.x = center.x + vCosSin.y * vDelta.x - vCosSin.x * vDelta.y;
		out.y = center.y + vCosSin.x * vDelta.x + vCosSin.y * vDelta.y;
		out.z = point.z;
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::ApplyPositionPrediction(sCEntity* entity)
	{
		ImVec3 vOldPosition, vNewPosition, vDeltaPosition;

		EvaluateTrajectory(&entity->CurrentEntityState.PositionTrajectory, CG->PredictedPlayerState.OldSnapShot->iServerTime, &vOldPosition);
		EvaluateTrajectory(&entity->NextEntityState.LerpEntityState.PositionTrajectory, CG->PredictedPlayerState.NewSnapShot->iServerTime, &vNewPosition);

		vDeltaPosition = vNewPosition - vOldPosition;
		
		vDeltaPosition.x = (float)GetSign(vDeltaPosition.x);
		vDeltaPosition.y = (float)GetSign(vDeltaPosition.y);
		vDeltaPosition.z = (float)GetSign(vDeltaPosition.z);
		
		entity->vOrigin += (vDeltaPosition * (*(float*)OFF_FRAMEINTERPOLATION));
		entity->vOrigin += (vDeltaPosition * (*(int*)OFF_FRAMETIME / 1000.0f));
		entity->vOrigin += (vDeltaPosition * (*(int*)OFF_PING / 1000.0f));
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::ApplyAnglePrediction(sCEntity* entity)
	{
		ImVec3 vOldAngles, vNewAngles, vDeltaAngles;

		EvaluateTrajectory(&entity->CurrentEntityState.AngleTrajectory, CG->PredictedPlayerState.OldSnapShot->iServerTime, &vOldAngles);
		EvaluateTrajectory(&entity->NextEntityState.LerpEntityState.AngleTrajectory, CG->PredictedPlayerState.NewSnapShot->iServerTime, &vNewAngles);

		vDeltaAngles.x = AngleNormalize180(vNewAngles.x, vOldAngles.x);
		vDeltaAngles.y = AngleNormalize180(vNewAngles.y, vOldAngles.y);
		vDeltaAngles.z = AngleNormalize180(vNewAngles.z, vOldAngles.z);
		
		vDeltaAngles.x = (float)GetSign(vDeltaAngles.x);
		vDeltaAngles.y = (float)GetSign(vDeltaAngles.y);
		vDeltaAngles.z = (float)GetSign(vDeltaAngles.z);
		
		entity->vViewAngles += (vDeltaAngles * (*(float*)OFF_FRAMEINTERPOLATION));
		entity->vViewAngles += (vDeltaAngles * (*(int*)OFF_FRAMETIME / 1000.0f));
		entity->vViewAngles += (vDeltaAngles * (*(int*)OFF_PING / 1000.0f));
	}
}

//=====================================================================================