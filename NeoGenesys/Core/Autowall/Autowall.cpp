//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cAutowall _autoWall;

	float cAutowall::C_Autowall(Vector3 start, Vector3 end)
	{
		sBulletFireParams FP_Enter;
		sBulletTraceResults TR_Enter;

		ZeroMemory(&FP_Enter, sizeof(sBulletFireParams));
		ZeroMemory(&TR_Enter, sizeof(sBulletTraceResults));

		VectorSubtract(end, start, FP_Enter.vDir);
		VectorNormalize(FP_Enter.vDir);

		FP_Enter.iMaxEntNum = 2046;
		FP_Enter.iEntityNum = CG->PlayerState.iClientNum;
		FP_Enter.flPower = 1.0f;
		FP_Enter.iBulletType = (IsRifleBullet(CEntity[CG->PlayerState.iClientNum].NextEntityState.iWeapon, CEntity[CG->PlayerState.iClientNum].NextEntityState.iInAltWeaponMode) != 0) + 1;

		VectorCopy(start, FP_Enter.vViewOrigin);
		VectorCopy(start, FP_Enter.vStart);
		VectorCopy(end, FP_Enter.vEnd);

		bool bEnterHit = C_BulletTrace(&FP_Enter, &CEntity[CG->PlayerState.iClientNum], &TR_Enter, TRACE_HITTYPE_NONE);

		if (bEnterHit)
		{
			int iSurfaceCount = 0;
			float flEnterDepth = 0.0f, flExitDepth = 0.0f, flSurfaceDepth = 0.0f;

			sBulletFireParams FP_Exit;
			sBulletTraceResults TR_Exit;

			Vector3 vHitPos = { 0.0f };

			while (TRUE)
			{
				flEnterDepth = GetSurfacePenetrationDepth(CEntity[CG->PlayerState.iClientNum].NextEntityState.iWeapon, CEntity[CG->PlayerState.iClientNum].NextEntityState.iInAltWeaponMode, TR_Enter.iDepthSurfaceType);

				if (HasPerk(CG->PlayerState.iClientNum, PERK_EXTRABP))
					flEnterDepth *= FindVariable("perk_bulletPenetrationMultiplier")->Current.flValue;

				if (flEnterDepth <= 0.0f)
					return 0.0f;

				VectorCopy(TR_Enter.vHitPos, vHitPos);

				if (!AdvanceTrace(&FP_Enter, &TR_Enter, 0.13500001f))
					return 0.0f;

				bEnterHit = C_BulletTrace(&FP_Enter, &CEntity[CG->PlayerState.iClientNum], &TR_Enter, TR_Enter.iDepthSurfaceType);

				CopyMemory(&FP_Exit, &FP_Enter, sizeof(sBulletFireParams));
				VectorScale(FP_Enter.vDir, -1.0f, FP_Exit.vDir);

				VectorCopy(FP_Enter.vEnd, FP_Exit.vStart);
				VectorMA(vHitPos, 0.0099999998f, FP_Exit.vDir, FP_Exit.vEnd);

				CopyMemory(&TR_Exit, &TR_Enter, sizeof(sBulletTraceResults));
				VectorScale(TR_Exit.Trace.vNormal, -1.0f, TR_Exit.Trace.vNormal);

				if (bEnterHit)
					AdvanceTrace(&FP_Exit, &TR_Exit, 0.0099999998f);

				bool bExitHit = C_BulletTrace(&FP_Exit, &CEntity[CG->PlayerState.iClientNum], &TR_Exit, TR_Exit.iDepthSurfaceType);
				bool bSolid = (bExitHit && TR_Exit.Trace.bAllSolid) || (TR_Enter.Trace.bStartSolid && TR_Exit.Trace.bStartSolid);

				if (bExitHit || bSolid)
				{
					if (bSolid)
						flSurfaceDepth = _mathematics.CalculateDistance(FP_Exit.vEnd, FP_Exit.vStart);

					else
						flSurfaceDepth = _mathematics.CalculateDistance(vHitPos, TR_Exit.vHitPos);

					flSurfaceDepth = max(flSurfaceDepth, 1.0f);

					if (bExitHit)
					{
						flExitDepth = GetSurfacePenetrationDepth(CEntity[CG->PlayerState.iClientNum].NextEntityState.iWeapon, CEntity[CG->PlayerState.iClientNum].NextEntityState.iInAltWeaponMode, TR_Exit.iDepthSurfaceType);

						if (HasPerk(CG->PlayerState.iClientNum, PERK_EXTRABP))
							flExitDepth *= FindVariable("perk_bulletPenetrationMultiplier")->Current.flValue;

						flEnterDepth = min(flEnterDepth, flExitDepth);

						if (flEnterDepth <= 0.0f)
							return 0.0f;
					}

					FP_Enter.flPower -= flSurfaceDepth / flEnterDepth;

					if (FP_Enter.flPower <= 0.0f)
						return 0.0f;
				}

				else if (!bEnterHit)
					return FP_Enter.flPower;

				if (bEnterHit)
				{
					if (++iSurfaceCount < 5)
						continue;
				}

				return 0.0f;
			}
		}

		return FP_Enter.flPower;
	}
	/*
	//=====================================================================================
	*/
	bool cAutowall::C_TraceBullet(Vector3 start, Vector3 end, int entitynum)
	{
		sBulletFireParams FP_Enter;
		sBulletTraceResults TR_Enter;

		ZeroMemory(&FP_Enter, sizeof(sBulletFireParams));
		ZeroMemory(&TR_Enter, sizeof(sBulletTraceResults));

		VectorSubtract(end, start, FP_Enter.vDir);
		VectorNormalize(FP_Enter.vDir);

		FP_Enter.iMaxEntNum = 2046;
		FP_Enter.iEntityNum = CG->PlayerState.iClientNum;
		FP_Enter.flPower = 1.0f;
		FP_Enter.iBulletType = (IsRifleBullet(CEntity[CG->PlayerState.iClientNum].NextEntityState.iWeapon, CEntity[CG->PlayerState.iClientNum].NextEntityState.iInAltWeaponMode) != 0) + 1;

		VectorCopy(start, FP_Enter.vViewOrigin);
		VectorCopy(start, FP_Enter.vStart);
		VectorCopy(end, FP_Enter.vEnd);

		C_BulletTrace(&FP_Enter, &CEntity[CG->PlayerState.iClientNum], &TR_Enter, TRACE_HITTYPE_NONE);

		return (TR_Enter.Trace.wHitID == entitynum || TR_Enter.Trace.flFraction == 1.0f);
	}
	/*
	//=====================================================================================
	*/
	float cAutowall::G_Autowall(Vector3 start, Vector3 end)
	{
		sBulletFireParams FP_Enter;
		sBulletTraceResults TR_Enter;

		ZeroMemory(&FP_Enter, sizeof(sBulletFireParams));
		ZeroMemory(&TR_Enter, sizeof(sBulletTraceResults));

		VectorSubtract(end, start, FP_Enter.vDir);
		VectorNormalize(FP_Enter.vDir);

		FP_Enter.iMaxEntNum = 2046;
		FP_Enter.iEntityNum = CG->PlayerState.iClientNum;
		FP_Enter.flPower = 1.0f;
		FP_Enter.iBulletType = (IsRifleBullet(CEntity[CG->PlayerState.iClientNum].NextEntityState.iWeapon, CEntity[CG->PlayerState.iClientNum].NextEntityState.iInAltWeaponMode) != 0) + 1;

		VectorCopy(start, FP_Enter.vViewOrigin);
		VectorCopy(start, FP_Enter.vStart);
		VectorCopy(end, FP_Enter.vEnd);

		bool bEnterHit = G_BulletTrace(&FP_Enter, CEntity[CG->PlayerState.iClientNum].NextEntityState.iWeapon, CEntity[CG->PlayerState.iClientNum].NextEntityState.iInAltWeaponMode, &GEntity[CG->PlayerState.iClientNum], &TR_Enter, TRACE_HITTYPE_NONE);

		if (TR_Enter.Trace.wPartGroup == 19)
			return 0.0f;

		if (bEnterHit)
		{
			int iSurfaceCount = 0;
			float flEnterDepth = 0.0f, flExitDepth = 0.0f, flSurfaceDepth = 0.0f;

			sBulletFireParams FP_Exit;
			sBulletTraceResults TR_Exit;

			Vector3 vHitPos = { 0.0f };

			while (TRUE)
			{
				flEnterDepth = GetSurfacePenetrationDepth(CEntity[CG->PlayerState.iClientNum].NextEntityState.iWeapon, CEntity[CG->PlayerState.iClientNum].NextEntityState.iInAltWeaponMode, TR_Enter.iDepthSurfaceType);

				if (HasPerk(CG->PlayerState.iClientNum, PERK_EXTRABP))
					flEnterDepth *= FindVariable("perk_bulletPenetrationMultiplier")->Current.flValue;

				if (flEnterDepth <= 0.0f)
					return 0.0f;

				VectorCopy(TR_Enter.vHitPos, vHitPos);

				if (!AdvanceTrace(&FP_Enter, &TR_Enter, 0.13500001f))
					return 0.0f;

				bEnterHit = G_BulletTrace(&FP_Enter, CEntity[CG->PlayerState.iClientNum].NextEntityState.iWeapon, CEntity[CG->PlayerState.iClientNum].NextEntityState.iInAltWeaponMode, &GEntity[CG->PlayerState.iClientNum], &TR_Enter, TR_Enter.iDepthSurfaceType);

				if (TR_Enter.Trace.wPartGroup == 19)
					return 0.0f;

				CopyMemory(&FP_Exit, &FP_Enter, sizeof(sBulletFireParams));
				VectorScale(FP_Enter.vDir, -1.0f, FP_Exit.vDir);

				VectorCopy(FP_Enter.vEnd, FP_Exit.vStart);
				VectorMA(vHitPos, 0.0099999998f, FP_Exit.vDir, FP_Exit.vEnd);

				CopyMemory(&TR_Exit, &TR_Enter, sizeof(sBulletTraceResults));
				VectorScale(TR_Exit.Trace.vNormal, -1.0f, TR_Exit.Trace.vNormal);

				if (bEnterHit)
					AdvanceTrace(&FP_Exit, &TR_Exit, 0.0099999998f);

				bool bExitHit = G_BulletTrace(&FP_Exit, CEntity[CG->PlayerState.iClientNum].NextEntityState.iWeapon, CEntity[CG->PlayerState.iClientNum].NextEntityState.iInAltWeaponMode, &GEntity[CG->PlayerState.iClientNum], &TR_Exit, TR_Exit.iDepthSurfaceType);
				bool bSolid = (bExitHit && TR_Exit.Trace.bAllSolid) || (TR_Enter.Trace.bStartSolid && TR_Exit.Trace.bStartSolid);

				if (TR_Exit.Trace.wPartGroup == 19)
					return 0.0f;

				if (bExitHit || bSolid)
				{
					if (bSolid)
						flSurfaceDepth = _mathematics.CalculateDistance(FP_Exit.vEnd, FP_Exit.vStart);

					else
						flSurfaceDepth = _mathematics.CalculateDistance(vHitPos, TR_Exit.vHitPos);

					flSurfaceDepth = max(flSurfaceDepth, 1.0f);

					if (bExitHit)
					{
						flExitDepth = GetSurfacePenetrationDepth(CEntity[CG->PlayerState.iClientNum].NextEntityState.iWeapon, CEntity[CG->PlayerState.iClientNum].NextEntityState.iInAltWeaponMode, TR_Exit.iDepthSurfaceType);

						if (HasPerk(CG->PlayerState.iClientNum, PERK_EXTRABP))
							flExitDepth *= FindVariable("perk_bulletPenetrationMultiplier")->Current.flValue;

						flEnterDepth = min(flEnterDepth, flExitDepth);

						if (flEnterDepth <= 0.0f)
							return 0.0f;
					}

					FP_Enter.flPower -= flSurfaceDepth / flEnterDepth;

					if (FP_Enter.flPower <= 0.0f)
						return 0.0f;
				}

				else if (!bEnterHit)
					return GetRemainingDamage(&FP_Enter, &TR_Enter, TR_Enter.Trace.wPartGroup, CEntity[CG->PlayerState.iClientNum].NextEntityState.iWeapon, CEntity[CG->PlayerState.iClientNum].NextEntityState.iInAltWeaponMode);

				if (bEnterHit)
				{
					if (++iSurfaceCount < 5)
						continue;
				}

				return 0.0f;
			}
		}

		return GetRemainingDamage(&FP_Enter, &TR_Enter, TR_Enter.Trace.wPartGroup, CEntity[CG->PlayerState.iClientNum].NextEntityState.iWeapon, CEntity[CG->PlayerState.iClientNum].NextEntityState.iInAltWeaponMode);
	}
	/*
	//=====================================================================================
	*/
	bool cAutowall::G_TraceBullet(Vector3 start, Vector3 end, int entitynum)
	{
		sBulletFireParams FP_Enter;
		sBulletTraceResults TR_Enter;

		ZeroMemory(&FP_Enter, sizeof(sBulletFireParams));
		ZeroMemory(&TR_Enter, sizeof(sBulletTraceResults));

		VectorSubtract(end, start, FP_Enter.vDir);
		VectorNormalize(FP_Enter.vDir);

		FP_Enter.iMaxEntNum = 2046;
		FP_Enter.iEntityNum = CG->PlayerState.iClientNum;
		FP_Enter.flPower = 1.0f;
		FP_Enter.iBulletType = (IsRifleBullet(CEntity[CG->PlayerState.iClientNum].NextEntityState.iWeapon, CEntity[CG->PlayerState.iClientNum].NextEntityState.iInAltWeaponMode) != 0) + 1;

		VectorCopy(start, FP_Enter.vViewOrigin);
		VectorCopy(start, FP_Enter.vStart);
		VectorCopy(end, FP_Enter.vEnd);

		G_BulletTrace(&FP_Enter, CEntity[CG->PlayerState.iClientNum].NextEntityState.iWeapon, CEntity[CG->PlayerState.iClientNum].NextEntityState.iInAltWeaponMode, &GEntity[CG->PlayerState.iClientNum], &TR_Enter, TRACE_HITTYPE_NONE);

		return ((TR_Enter.Trace.wHitID == entitynum || TR_Enter.Trace.flFraction == 1.0f) && TR_Enter.Trace.wPartGroup != 19);
	}
	/*
	//=====================================================================================
	*/
	float cAutowall::GetRemainingDamage(sBulletFireParams* fireparams, sBulletTraceResults* traceresults, short partgroup, int weapon, bool alternate)
	{
		float flDamage = 0.0f;

		if (fireparams->flPower > 0.0f)
		{
			flDamage = (float)GetWeaponDamage(fireparams, traceresults, weapon, alternate, &GEntity[CG->PlayerState.iClientNum]);
			flDamage = GetWeaponHitLocationMultiplier(partgroup, weapon, alternate) * flDamage;

			if (flDamage <= 0.0f)
				flDamage = 1.0f;
		}

		return flDamage;
	}
	/*
	//=====================================================================================
	*/
	bool cAutowall::TraceLine(Vector3 start, Vector3 end, int entitynum)
	{
		sTrace Trace;

		LocationalTrace(&Trace, start, end, CG->PlayerState.iClientNum, MASK_CONTENTS);

		return (Trace.wHitID == entitynum || Trace.flFraction == 1.0f);
	}
}

//=====================================================================================