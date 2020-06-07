//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cMathematics _mathematics;

	float cMathematics::CalculateFOV(ImVec3 position)
	{
		ImVec3 vViewOrigin, vDirection, vAngles, vAimAngles;

		GetPlayerViewOrigin(&CG->PredictedPlayerState, &vViewOrigin);
		vDirection = position - vViewOrigin;

		VectorNormalize(vDirection);
		VectorAngles(vDirection, vAimAngles);

		MakeVector(CG->vRefDefViewAngles, vAngles);
		MakeVector(vAimAngles, vAimAngles);

		float flMag = VectorLength(vAngles, vAngles),
			flDot = DotProduct(vAngles, vAimAngles),
			flReturn = RadiansToDegrees(acosf(flDot / powf(flMag, 2.0f)));

		if (isnan(flReturn))
			flReturn = 0.0f;

		return flReturn;
	}
	/*
	//=====================================================================================
	*/
	float cMathematics::CalculateDistance(ImVec3 start, ImVec3 end)
	{
		ImVec3 vDirection;

		vDirection = start - end;

		return VectorLength(vDirection, vDirection);
	}
	/*
	//=====================================================================================
	*/
	float cMathematics::DotProduct(ImVec3 left, ImVec3 right)
	{
		return (left.x * right.x + left.y * right.y + left.z * right.z);
	}
	/*
	//=====================================================================================
	*/
	float cMathematics::VectorLength(ImVec3 left, ImVec3 right)
	{
		return sqrtf(DotProduct(left, right));
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
		float flLen = VectorLength(direction, direction);

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
	void cMathematics::ClampMove(char value[])
	{
		while (value[0] > 127)
			value[0] = 127;

		while (value[0] < -128)
			value[0] = -128;

		while (value[1] > 127)
			value[1] = 127;

		while (value[1] < -128)
			value[1] = -128;

		if (value[2] != 0)
			value[2] = 0;
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

		angles -= WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? CG->vRefDefViewAngles : IsThirdPersonMode(&CG->PredictedPlayerState) ? CG->vThirdPersonViewAngles : CG->vWeaponAngles;

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

		angles.x = -40.0f - angles.x;
		angles.y = angles.y - 180.0f;

		angles -= CG->PredictedPlayerState.vDeltaAngles;

		ClampAngles(angles);
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::MakeVector(ImVec3 angles, ImVec3& out)
	{
		float flPitch = DegreesToRadians(angles.x),
			flYaw = DegreesToRadians(angles.y);

		out.x = -cosf(flPitch) * -cosf(flYaw);
		out.y = sinf(flYaw) * cosf(flPitch);
		out.z = -sinf(flPitch);
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::MovementFix(sUserCmd* usercmd, float yaw, float oldyaw, float forward, float right)
	{
		float flDelta = DegreesToRadians(yaw - oldyaw);

		usercmd->szMove[0] = (char)(cosf(flDelta) * forward - sinf(flDelta) * right);
		usercmd->szMove[1] = (char)(sinf(flDelta) * forward + cosf(flDelta) * right);

		ClampMove(usercmd->szMove);
	}
	/*
	//=====================================================================================
	*/
	void cMathematics::WorldToCompass(ImVec3 world, ImVec2 compasspos, float compasssize, ImVec2& screen)
	{
		float flAngle;

		ImVec3 vViewOrigin, vDirection, vAngles;

		GetPlayerViewOrigin(&CG->PredictedPlayerState, &vViewOrigin);
		vDirection = (WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? RefDef->vViewOrigin : vViewOrigin) - world;
		
		VectorNormalize(vDirection);
		VectorAngles(vDirection, vAngles);

		vAngles = (WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? CG->vRefDefViewAngles : IsThirdPersonMode(&CG->PredictedPlayerState) ? CG->vThirdPersonViewAngles : CG->vWeaponAngles) - vAngles;

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
		ImVec3 vViewOrigin;
		ImVec2 vDelta, vLocation;

		GetPlayerViewOrigin(&CG->PredictedPlayerState, &vViewOrigin);

		float flCosYaw = cosf(DegreesToRadians(WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? CG->vRefDefViewAngles.y : IsThirdPersonMode(&CG->PredictedPlayerState) ? CG->vThirdPersonViewAngles.y : CG->vWeaponAngles.y)),
			flSinYaw = sinf(DegreesToRadians(WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? CG->vRefDefViewAngles.y : IsThirdPersonMode(&CG->PredictedPlayerState) ? CG->vThirdPersonViewAngles.y : CG->vWeaponAngles.y));
		
		vDelta.x = world.x - (WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? RefDef->vViewOrigin.x : vViewOrigin.x);
		vDelta.y = world.y - (WeaponIsVehicle(GetViewmodelWeapon(&CG->PredictedPlayerState)) ? RefDef->vViewOrigin.y : vViewOrigin.y);

		vLocation.x = (vDelta.y * flCosYaw - vDelta.x * flSinYaw) / scale;
		vLocation.y = (vDelta.x * flCosYaw + vDelta.y * flSinYaw) / scale;

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
		ImVec2 vDifference;

		float flAngleCos = cosf(((-yaw + 180.0f) / 360.0f - 0.25f) * M_PI_DOUBLE),
			flAngleSin = sinf(((-yaw + 180.0f) / 360.0f - 0.25f) * M_PI_DOUBLE);

		vDifference.x = point.x - center.x;
		vDifference.y = point.y - center.y;

		out.x = center.x + flAngleSin * vDifference.x - flAngleCos * vDifference.y;
		out.y = center.y + flAngleCos * vDifference.x + flAngleSin * vDifference.y;
		out.z = point.z;
	}
}

//=====================================================================================