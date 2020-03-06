//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cAntiAim _antiAim;

	void cAntiAim::ClientAntiAim(sUserCmd* usercmd)
	{
		if (GetViewmodelWeapon(&CG->PlayerState) && !WeaponIsVehicle(GetViewmodelWeapon(&CG->PlayerState)) &&
			!(CEntity[CG->PlayerState.iClientNum].NextEntityState.LerpEntityState.iEntityFlags & EF_PRONE) &&
			!(CEntity[CG->PlayerState.iClientNum].NextEntityState.LerpEntityState.iEntityFlags & EF_MANTLE))
		{
			if (_profiler.gAntiAim->Custom.iValue == cProfiler::ANTIAIM_SPINBOT)
			{
				static float flAngle = 0.0f;

				if (flAngle > 360.0f)
					flAngle -= 360.0f;

				usercmd->iViewAngles[0] += AngleToShort(70.0f - CG->vRefDefViewAngles[0]);
				usercmd->iViewAngles[1] += AngleToShort(flAngle - CG->vRefDefViewAngles[1]);

				flAngle += 40.0f;
			}

			else if (_profiler.gAntiAim->Custom.iValue == cProfiler::ANTIAIM_JITTERBOT)
			{
				static int iMode = 1;

				switch (iMode)
				{
				case 1:
					usercmd->iViewAngles[0] += AngleToShort(-80.0f - CG->vRefDefViewAngles[0]);
					usercmd->iViewAngles[1] += AngleToShort(0.0f - CG->vRefDefViewAngles[1]);
					iMode = 2;
					break;

				case 2:
					usercmd->iViewAngles[0] += AngleToShort(80.0f - CG->vRefDefViewAngles[0]);
					usercmd->iViewAngles[1] += AngleToShort(90.0f - CG->vRefDefViewAngles[1]);
					iMode = 3;
					break;

				case 3:
					usercmd->iViewAngles[0] += AngleToShort(-80.0f - CG->vRefDefViewAngles[0]);
					usercmd->iViewAngles[1] += AngleToShort(180.0f - CG->vRefDefViewAngles[1]);
					iMode = 4;
					break;

				case 4:
					usercmd->iViewAngles[0] += AngleToShort(80.0f - CG->vRefDefViewAngles[0]);
					usercmd->iViewAngles[1] += AngleToShort(270.0f - CG->vRefDefViewAngles[1]);
					iMode = 1;
					break;
				}
			}

			else if (_profiler.gAntiAim->Custom.iValue == cProfiler::ANTIAIM_RANDOMIZED)
			{
				std::random_device Device;
				std::uniform_real_distribution<float> RandomPitch(-80.0f, 80.0f), RandomYaw(0.0f, 360.0f);

				usercmd->iViewAngles[0] += AngleToShort(RandomPitch(Device) - CG->vRefDefViewAngles[0]);
				usercmd->iViewAngles[1] += AngleToShort(RandomYaw(Device) - CG->vRefDefViewAngles[1]);
			}

			else if (_profiler.gAntiAim->Custom.iValue == cProfiler::ANTIAIM_REVERSED)
			{
				if (_targetList.vIsTarget[_targetList.iRiotShieldTarget] && _targetList.iRiotShieldTarget != CG->PlayerState.iClientNum)
				{
					_mathematics.CalculateAngles(CG->PlayerState.vOrigin, CEntity[_targetList.iRiotShieldTarget].vOrigin, _targetList.vRiotShieldTarget);

					usercmd->iViewAngles[0] += AngleToShort(_targetList.vRiotShieldTarget[0] - 5.7f);
					usercmd->iViewAngles[1] += AngleToShort(_targetList.vRiotShieldTarget[1] - 180.0f);
				}

				else
				{
					if (_aimBot.AimState.bIsAutoAiming)
					{
						usercmd->iViewAngles[0] += AngleToShort(_aimBot.AimState.vAimbotAngles[0] - 5.7f);
						usercmd->iViewAngles[1] += AngleToShort(_aimBot.AimState.vAimbotAngles[1] - 180.0f);
					}

					else
					{
						usercmd->iViewAngles[0] += AngleToShort(-5.7f);
						usercmd->iViewAngles[1] += AngleToShort(-180.0f);
					}
				}
			}
		}
	}
	/*
	//=====================================================================================
	*/
	void cAntiAim::EntityAntiAim(Vector3 angles)
	{
		if (_profiler.gAntiAim->Custom.iValue == cProfiler::ANTIAIM_SPINBOT)
		{
			static float flAngle = 0.0f;

			if (flAngle > 360.0f)
				flAngle -= 360.0f;

			angles[0] = 70.0f;
			angles[1] = flAngle;

			flAngle += 40.0f;
		}

		else if (_profiler.gAntiAim->Custom.iValue == cProfiler::ANTIAIM_JITTERBOT)
		{
			static int iMode = 1;

			switch (iMode)
			{
			case 1:
				angles[0] = -80.0f;
				angles[1] = 0.0f;
				iMode = 2;
				break;

			case 2:
				angles[0] = 80.0f;
				angles[1] = 90.0f;
				iMode = 3;
				break;

			case 3:
				angles[0] = -80.0f;
				angles[1] = 180.0f;
				iMode = 4;
				break;

			case 4:
				angles[0] = 80.0f;
				angles[1] = 270.0f;
				iMode = 1;
				break;
			}
		}

		else if (_profiler.gAntiAim->Custom.iValue == cProfiler::ANTIAIM_RANDOMIZED)
		{
			std::random_device Device;
			std::uniform_real_distribution<float> RandomPitch(-80.0f, 80.0f), RandomYaw(0.0f, 360.0f);

			angles[0] = RandomPitch(Device);
			angles[1] = RandomYaw(Device);
		}

		else if (_profiler.gAntiAim->Custom.iValue == cProfiler::ANTIAIM_REVERSED)
		{
			if (_targetList.vIsTarget[_targetList.iRiotShieldTarget] && _targetList.iRiotShieldTarget != CG->PlayerState.iClientNum)
			{
				_mathematics.CalculateAngles(CG->PlayerState.vOrigin, CEntity[_targetList.iRiotShieldTarget].vOrigin, _targetList.vRiotShieldTarget);

				angles[0] = _targetList.vRiotShieldTarget[0] - 5.7f;
				angles[1] = _targetList.vRiotShieldTarget[1] - 180.0f;
			}

			else
			{
				if (_aimBot.AimState.bIsAutoAiming)
				{
					angles[0] = _aimBot.AimState.vAimbotAngles[0] - 5.7f;
					angles[1] = _aimBot.AimState.vAimbotAngles[1] - 180.0f;
				}

				else
				{
					angles[0] += -5.7f;
					angles[1] += -180.0f;
				}
			}
		}
	}
}

//=====================================================================================