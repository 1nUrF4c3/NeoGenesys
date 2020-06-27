//=====================================================================================

#pragma once

#include "../Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cAimbot
	{
	public:

		enum eAimBotMode
		{
			AIMBOT_MODE_OFF,
			AIMBOT_MODE_MANUAL,
			AIMBOT_MODE_AUTO,
			AIMBOT_MODE_MAX
		};

		std::shared_ptr<sCvar> gAimBotMode = std::make_shared<sCvar>("Mode", std::vector<std::string>({ "Off", "Manual", "Auto" }), AIMBOT_MODE_OFF, AIMBOT_MODE_OFF, AIMBOT_MODE_MAX);
		std::shared_ptr<sCvar> gAutoZoom = std::make_shared<sCvar>("Autozoom", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gAutoFire = std::make_shared<sCvar>("Autofire", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gSilentAim = std::make_shared<sCvar>("Silent-Aim", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gAimPower = std::make_shared<sCvar>("Aimpower", std::vector<std::string>(), 100, 1, 100);
		std::shared_ptr<sCvar> gAutoAimTime = std::make_shared<sCvar>("Autoaim Time", std::vector<std::string>(), 0, 0, 1000);
		std::shared_ptr<sCvar> gAutoAimDelay = std::make_shared<sCvar>("Autoaim Delay", std::vector<std::string>(), 0, 0, 1000);
		std::shared_ptr<sCvar> gAutoZoomDelay = std::make_shared<sCvar>("Autozoom Delay", std::vector<std::string>(), 0, 0, 1000);
		std::shared_ptr<sCvar> gAutoFireDelay = std::make_shared<sCvar>("Autofire Delay", std::vector<std::string>(), 0, 0, 1000);

		struct sAimState
		{
			bool bAkimbo, bTargetAcquired, bAntiAimTargetAcquired, bIsZooming, bLockonTarget, bIsAutoAiming, bIsAutoFiring;
			int iFireTMR, iCurrentAimTime, iCurrentAimDelay, iCurrentZoomDelay, iCurrentFireDelay, iDeltaTMR, iTargetNum, iLastTargetNum, iAntiAimTargetNum;
			ImVec3 vAimPosition, vAimAngles, vAntiAimAngles;
		} AimState;

		void StandardAim();
		void SilentAim(sUserCmd* usercmd);
		void AutoFire(sUserCmd* usercmd);
		void SetAimState();
	} extern _aimBot;
}

//=====================================================================================
