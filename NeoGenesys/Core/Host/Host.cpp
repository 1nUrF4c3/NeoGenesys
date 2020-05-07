//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cHost _host;

	void cHost::PlayerMods()
	{
		for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
		{
			if (LocalClientIsInGame() && *(int*)OFF_ISCURRENTHOST)
			{
				if (_mainGui.Menu.HostMenu.PlayerMod[i].bGodMode)
				{
					if (GEntity[i].iHealth != -10000000)
						GEntity[i].iHealth = -10000000;
				}

				else
				{
					if (GEntity[i].iHealth == -10000000)
						GEntity[i].iHealth = 100;
				}

				if (_mainGui.Menu.HostMenu.PlayerMod[i].bNoClip)
					PlayerState[i].iMoveType = 2;

				else
					PlayerState[i].iMoveType = 0;

				if (_mainGui.Menu.HostMenu.PlayerMod[i].bInfiniteAmmo)
				{
					for (int j = 0; j < 15; j++)
					{
						if (PlayerState[i].AmmoNotInClip[j].iAmmoCount != 600)
							PlayerState[i].AmmoNotInClip[j].iAmmoCount = 600;

						if (PlayerState[i].AmmoInClip[j].iAmmoCount[0] != 200)
							PlayerState[i].AmmoInClip[j].iAmmoCount[0] = 200;

						if (PlayerState[i].AmmoInClip[j].iAmmoCount[1] != 200)
							PlayerState[i].AmmoInClip[j].iAmmoCount[1] = 200;
					}
				}

				if (_mainGui.Menu.HostMenu.PlayerMod[i].bInvisibility)
					PlayerState[i].iEntityFlags |= 0x20;

				else
					PlayerState[i].iEntityFlags &= ~0x20;

				if (_mainGui.Menu.HostMenu.PlayerMod[i].bSuperSpeed)
					PlayerState[i].flSpeedMultiplier = 3.0f;

				if (_mainGui.Menu.HostMenu.PlayerMod[i].bFreezePosition)
					VectorCopy(_mainGui.Menu.HostMenu.PlayerMod[i].szPosition, PlayerState[i].vOrigin);

				else
					VectorCopy(PlayerState[i].vOrigin, _mainGui.Menu.HostMenu.PlayerMod[i].szPosition);

				if (_profiler.gAntiLeave->Current.bValue)
					if (i != CG->PredictedPlayerState.iClientNum)
						GameSendServerCommand(i, SV_CMD_RELIABLE, "o 11 1");
			}

			if (!LocalClientIsInGame() || !CharacterInfo[i].iInfoValid)
			{
				_mainGui.Menu.HostMenu.PlayerMod[i].bGodMode = false;
				_mainGui.Menu.HostMenu.PlayerMod[i].bNoClip = false;
				_mainGui.Menu.HostMenu.PlayerMod[i].bInfiniteAmmo = false;
				_mainGui.Menu.HostMenu.PlayerMod[i].bInvisibility = false;
				_mainGui.Menu.HostMenu.PlayerMod[i].bSuperSpeed = false;
				_mainGui.Menu.HostMenu.PlayerMod[i].bFreezePosition = false;

				_targetList.vIsTarget[i] = TRUE;
			}
		}

		static bool bSuperJump = false;

		if (_profiler.gSuperJump->Current.bValue && !bSuperJump)
		{
			WriteMemoryProtected((LPVOID)OFF_ALTJUMPHEIGHT, 3000.0f);
			bSuperJump = true;
		}

		else if (!_profiler.gSuperJump->Current.bValue && bSuperJump)
		{
			WriteMemoryProtected((LPVOID)OFF_ALTJUMPHEIGHT, 39.0f);
			bSuperJump = false;
		}
	}
	/*
	//=====================================================================================
	*/
	void cHost::StartMatch()
	{
		if (!_mutex.try_lock())
			return;

		bool bTeamBased = FindVariable("party_teambased")->Current.bValue;
		PBYTE pResult = VariadicCall<PBYTE>(0x1402D6880);

		if (pResult[78])
		{
			LPVOID* v1 = VariadicCall<LPVOID*>(0x1402D6870);
			VariadicCall<PBYTE>(0x1402E9A60, v1);
		}

		if (!pResult[78] || !bTeamBased)
		{
			_mutex.unlock();
			return;
		}

		for (int i = clock(); !LocalClientIsInGame(); Sleep(100))
		{
			if (clock() - i > 60000)
			{
				_mutex.unlock();
				return;
			}
		}

		if (PlayerState[CG->PredictedPlayerState.iClientNum].ClientState.iTeam == TEAM_FREE)
		{
			PlayerState[CG->PredictedPlayerState.iClientNum].ClientState.iTeam = TEAM_ALLIES;
			TeamChanged(CG->PredictedPlayerState.iClientNum);
		}

		_mutex.unlock();
	}
	/*
	//=====================================================================================
	*/
	void cHost::MassKill()
	{
		if (!*(int*)OFF_ISCURRENTHOST)
			return;

		static int iCounter = 0;
		int iTargetNum = iCounter % FindVariable("sv_maxclients")->Current.iValue;

		if (iTargetNum != CG->PredictedPlayerState.iClientNum && CEntity[iTargetNum].NextEntityState.iWeapon)
		{
			if (CharacterInfo[iTargetNum].iInfoValid && CharacterInfo[iTargetNum].iNextValid)
			{
				if ((_profiler.gMassKill->Current.iValue == cProfiler::MASSKILL_AXIS && _targetList.EntityIsEnemy(iTargetNum)) ||
					(_profiler.gMassKill->Current.iValue == cProfiler::MASSKILL_ALLIES && !_targetList.EntityIsEnemy(iTargetNum)) ||
					_profiler.gMassKill->Current.iValue == cProfiler::MASSKILL_ALL)
				{
					PlayerKill(&GEntity[iTargetNum],
						_targetList.EntityIsEnemy(iTargetNum) ? NULL : &GEntity[iTargetNum],
						_targetList.EntityIsEnemy(iTargetNum) ? &GEntity[CG->PredictedPlayerState.iClientNum] : &GEntity[iTargetNum],
						_targetList.EntityIsEnemy(iTargetNum) ? 9 : 14,
						_targetList.EntityIsEnemy(iTargetNum) ? GetViewmodelWeapon(&CG->PredictedPlayerState) : 0);
				}
			}
		}

		iCounter++;
	}
	/*
	//=====================================================================================
	*/
	void cHost::SpawnBots(int count)
	{
		int iCurrentIndex = 0;

		auto AddEntities = [&]()
		{
			for (auto& SpawnedBot : vSpawnedBots)
			{
				AddEntity(SpawnedBot);
			}
		};

		auto SpawnTestClients = [&]()
		{
			for (auto& SpawnedBot : vSpawnedBots)
			{
				SpawnTestClient(SpawnedBot);
			}
		};

		auto AddTestClients = [&]()
		{
			vSpawnedBots.clear();

			for (int i = 0; i < count; iCurrentIndex++)
			{
				if (CharacterInfo[iCurrentIndex].iInfoValid)
					continue;

				vSpawnedBots.push_back(AddTestClient(TC_NONE, TEAM_FREE, iCurrentIndex, sEntRef(iCurrentIndex, 0)));

				i++;
			}
		};

		AddTestClients();
		AddEntities();
		SpawnTestClients();
	}
}

//=====================================================================================