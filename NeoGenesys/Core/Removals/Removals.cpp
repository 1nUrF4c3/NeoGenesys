//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cRemovals _removals;

	void cRemovals::RecoilCompensation()
	{
		Punch->vPunchAngles[0] *= _profiler.gRecoilFactor->Custom.flValue;
		Punch->vPunchAngles[1] *= _profiler.gRecoilFactor->Custom.flValue;
		Punch->vPunchAngles[2] *= _profiler.gRecoilFactor->Custom.flValue;

		ViewMatrix->vRecoilAngles[0] *= _profiler.gRecoilFactor->Custom.flValue;
		ViewMatrix->vRecoilAngles[1] *= _profiler.gRecoilFactor->Custom.flValue;
		ViewMatrix->vRecoilAngles[2] *= _profiler.gRecoilFactor->Custom.flValue;

		Punch->vWeaponPunchAngles[0][0] *= _profiler.gRecoilFactor->Custom.flValue;
		Punch->vWeaponPunchAngles[0][1] *= _profiler.gRecoilFactor->Custom.flValue;
		Punch->vWeaponPunchAngles[0][2] *= _profiler.gRecoilFactor->Custom.flValue;

		Punch->vWeaponPunchAngles[1][0] *= _profiler.gRecoilFactor->Custom.flValue;
		Punch->vWeaponPunchAngles[1][1] *= _profiler.gRecoilFactor->Custom.flValue;
		Punch->vWeaponPunchAngles[1][2] *= _profiler.gRecoilFactor->Custom.flValue;

		Punch->vWeaponPunchAngles[2][0] *= _profiler.gRecoilFactor->Custom.flValue;
		Punch->vWeaponPunchAngles[2][1] *= _profiler.gRecoilFactor->Custom.flValue;
		Punch->vWeaponPunchAngles[2][2] *= _profiler.gRecoilFactor->Custom.flValue;
	}
	/*
	//=====================================================================================
	*/
	void cRemovals::SpreadCompensationStandardAim(sUserCMD* usercmd, bool akimbo)
	{
		FirstBulletFix();

		Vector3 vAngles;

		GetSpreadAngles(akimbo, usercmd->iServerTime, GetWeaponSpread(), vAngles);

		usercmd->iViewAngles[0] += AngleToShort(((WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? CG->vRefDefViewAngles[0] : CG->vWeaponAngles[0]) - vAngles[0]) * _profiler.gSpreadFactor->Custom.flValue);
		usercmd->iViewAngles[1] += AngleToShort(((WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? CG->vRefDefViewAngles[1] : CG->vWeaponAngles[1]) - vAngles[1]) * _profiler.gSpreadFactor->Custom.flValue);
	}
	/*
	//=====================================================================================
	*/
	void cRemovals::SpreadCompensationSilentAim(sUserCMD* usercmd, bool akimbo)
	{
		FirstBulletFix();

		int iSeed = TransformSeed(akimbo, usercmd->iServerTime);
		float flSpread = GetWeaponSpread(), flSpreadX, flSpreadY;

		GetRandomFloats(&iSeed, &flSpreadX, &flSpreadY);

		flSpreadX *= flSpread;
		flSpreadY *= flSpread;

		usercmd->iViewAngles[0] += AngleToShort(flSpreadY * _profiler.gSpreadFactor->Custom.flValue);
		usercmd->iViewAngles[1] += AngleToShort(flSpreadX * _profiler.gSpreadFactor->Custom.flValue);
	}
	/*
	//=====================================================================================
	*/
	void cRemovals::GetSpreadAngles(bool akimbo, int servertime, float spread, Vector3 angles)
	{
		Vector3 vForward, vRight, vUp, vEnd, vDir;
		int iSeed = TransformSeed(akimbo, servertime);

		AngleVectors(WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) ? CG->vRefDefViewAngles : CG->vWeaponAngles, vForward, vRight, vUp);
		BulletEndPosition(&iSeed, spread, RefDef->vViewOrg, vEnd, vDir, vForward, vRight, vUp);

		VectorAngles(vDir, angles);
		_mathematics.NormalizeAngles(angles);
	}
	/*
	//=====================================================================================
	*/
	void cRemovals::GetRandomFloats(int* seed, float* spreadx, float* spready)
	{
		float flRandom1 = RandomFloat(seed), 
			flRandom2 = RandomFloat(seed);

		flRandom1 *= M_PI_DOUBLE;

		*spreadx = cosf(flRandom1) * flRandom2;
		*spready = sinf(flRandom1) * flRandom2;
	}
	/*
	//=====================================================================================
	*/
	int cRemovals::TransformSeed(bool akimbo, int servertime)
	{
		int iServerTime = servertime;

		if (akimbo)
			iServerTime += 10;

		return SeedRandom(&iServerTime);
	}
	/*
	//=====================================================================================
	*/
	float cRemovals::GetWeaponSpread()
	{
		float flSpreadMultiplier = ByteToFloat(*(float*)OFF_SPREADMULTIPLIER),
			flZoomSpread = GetZoomSpreadForWeapon(&CG->PlayerState, CEntity[CG->PlayerState.iClientNum].NextEntityState.iWeapon, CEntity[CG->PlayerState.iClientNum].NextEntityState.iInAltWeaponMode),
			flMinSpread,
			flMaxSpread,
			flSpread;

		GetHipfireSpreadForWeapon(&CG->PlayerState, GetViewmodelWeapon(&CG->PlayerState), &flMinSpread, &flMaxSpread);

		if (CEntity[CG->PlayerState.iClientNum].NextEntityState.LerpEntityState.iEntityFlags & EF_ZOOM)
		{
			if (*(float*)OFF_ZOOMMULTIPLIER == 1.0f)
				flSpreadMultiplier = 0.0f;

			flSpread = ((((flMaxSpread - flMinSpread) * flSpreadMultiplier) + flMinSpread) * (1.0f - *(float*)OFF_ZOOMMULTIPLIER))
				+ ((((flMaxSpread - flZoomSpread) * flSpreadMultiplier) + flZoomSpread) * *(float*)OFF_ZOOMMULTIPLIER);
		}

		else
			flSpread = ((flMaxSpread - flMinSpread) * flSpreadMultiplier) + flMinSpread;

		return flSpread;
	}
	/*
	//=====================================================================================
	*/
	void cRemovals::FirstBulletFix()
	{
		float v1, v2, v3, v4;

		v1 = *(float*)0x141820718;

		if (*(float*)0x141823F20 != 0.0)
			v1 = *(float*)0x141820718 * *(float*)0x141823F20;

		*(float*)0x1419E51D8 = v1;

		v2 = *(float*)0x141823E80 + *(float*)0x141823E74;
		v3 = *(float*)0x141823E84 + *(float*)0x141823E78;
		v4 = *(float*)0x141823E88 + *(float*)0x141823E7C;

		*(float*)0x1419E51E4 = v2;
		*(float*)0x1419E51E8 = v3;
		*(float*)0x1419E51EC = v4;

		*(int*)0x1419E51CC = *(int*)0x141821368;
		*(int*)0x1419E51D0 = *(int*)0x141823B0C;
		*(int*)0x1419E51D4 = *(int*)0x14182136C != 0;

		*(int*)0x1419E5278 |= *(int*)0x14187D42C;
		*(int*)0x14187D42C = 0;
	}
}

//=====================================================================================