//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cConsole _console;

	cConsole::~cConsole()
	{
		ClearLog();

		for (int i = 0; i < vHistory.Size; i++)
			free(vHistory[i]);
	}
	/*
	//=====================================================================================
	*/
	void cConsole::InitInterface()
	{
		static bool bFirstTime = true;

		if (bFirstTime)
		{
			ClearLog();
			ZeroMemory(szInput, sizeof(szInput));
			iHistoryPos = -1;

			vCommands.push_back("clear");
			vCommands.push_back("history");
			vCommands.push_back("quit");

			vCommands.push_back("neo_name");
			vCommands.push_back("neo_namespam");
			vCommands.push_back("neo_end");
			vCommands.push_back("neo_crash");
			vCommands.push_back("neo_minimap");
			vCommands.push_back("neo_hostawall");
			vCommands.push_back("neo_rapidfire");
			vCommands.push_back("neo_superjump");
			vCommands.push_back("neo_gravitygun");
			vCommands.push_back("neo_masskill");
			vCommands.push_back("neo_antileave");
			vCommands.push_back("neo_bhop");
			vCommands.push_back("neo_tbag");
			vCommands.push_back("neo_experience");
			vCommands.push_back("neo_prestige");
			vCommands.push_back("neo_squadpoints");
			vCommands.push_back("neo_unlockall");
			vCommands.push_back("neo_resetstats");
			vCommands.push_back("neo_hostdvar");
			vCommands.push_back("neo_message");
			vCommands.push_back("neo_chatspam");
			vCommands.push_back("neo_killspam");
			vCommands.push_back("neo_spawnbot");
			vCommands.push_back("neo_enableai");
			vCommands.push_back("neo_infinite");
			vCommands.push_back("neo_godmode");
			vCommands.push_back("neo_noclip");
			vCommands.push_back("neo_infammo");
			vCommands.push_back("neo_invisibile");
			vCommands.push_back("neo_superspeed");
			vCommands.push_back("neo_freeze");
			vCommands.push_back("neo_disconnect");

			AddLog("Ready.");

			bFirstTime = false;
		}
	}
	/*
	//=====================================================================================
	*/
	void cConsole::ClearLog()
	{
		for (int i = 0; i < vItems.Size; i++)
			free(vItems[i]);

		vItems.clear();
		bScrollToBottom = true;
	}
	/*
	//=====================================================================================
	*/
	void cConsole::AddLog(LPCSTR format, ...) IM_FMTARGS(2)
	{
		char szBuffer[1024];
		va_list Args;
		va_start(Args, format);
		vsnprintf(szBuffer, IM_ARRAYSIZE(szBuffer), format, Args);
		szBuffer[IM_ARRAYSIZE(szBuffer) - 1] = 0;
		va_end(Args);
		vItems.push_back(Strdup(szBuffer));
		bScrollToBottom = true;
	}
	/*
	//=====================================================================================
	*/
	void cConsole::DrawConsole(bool* open)
	{
		if (bWriteLog)
		{
			ImGui::LogToFile();
			bWriteLog = false;
		}

		InitInterface();
		ImGui::SetNextWindowSize(ImVec2(510.0f, 350.0f));

		if (!ImGui::Begin("CONSOLE", open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::End();
			return;
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Close"))
			{
				*open = false;
				bWriteLog = true;
			}

			ImGui::EndPopup();
			bWriteLog = true;
		}

		ImGui::TextWrapped("\t\t\tNeoGenesys");
		ImGui::Spacing();
		ImGui::TextWrapped("Press Help for details, press Tab to use text completion.");

		if (ImGui::Button("Help", ImVec2(50, 0)))
		{
			AddLog("1. neo_name <name>\n\t\tChange your name.");
			AddLog("2. neo_namespam <on|off>\n\t\tEnable/disable random name spam.");
			AddLog("3. neo_end\n\t\tForce the current match to end.");
			AddLog("4. neo_crash\n\t\tCrash everyone in the lobby except yourself.");
			AddLog("5. neo_minimap <on|off>\n\t\tEnable/disable enemy blips on the minimap.");
			AddLog("6. neo_hostawall <on|off>\n\t\tEnable/disable host autowall (as host).");
			AddLog("7. neo_rapidfire <on|off>\n\t\tEnable/disable rapidfire weapon rate (as host).");
			AddLog("8. neo_superjump <on|off>\n\t\tEnable/disable super high jump (as host).");
			AddLog("9. neo_gravitygun <on|off>\n\t\tEnable/disable gravity gun (as host).");
			AddLog("10. neo_masskill <off|axis|allies|all>\n\t\tEnable/disable player masskill (as host).");
			AddLog("11. neo_antileave <off|on>\n\t\tEnable/disable player antileave (as host).");
			AddLog("12. neo_bhop <on|off>\n\t\tEnable/disable auto bunny hop on jump.");
			AddLog("13. neo_tbag <on|off> <message>\n\t\tEnable/disable auto tea bag on kill with optional message (as host).");
			AddLog("14. neo_experience <all|index> <max|experience>\n\t\tSet your experience.");
			AddLog("15. neo_prestige <max|number>\n\t\tSet your prestige.");
			AddLog("16. neo_squadpoints <max|squadpoints>\n\t\tSet your squadpoints.");
			AddLog("17. neo_unlockall\n\t\tUnlock everything in the game.");
			AddLog("18. neo_resetstats\n\t\tCompletely erase your save game.");
			AddLog("19. neo_hostdvar <dvar> <value>\n\t\tSet DVAR value for all clients (as host).");
			AddLog("20. neo_message <self|index> <all|index> <lobby|team|private> <message>\n\t\tSend a message (as host).");
			AddLog("21. neo_chatspam <on|off> <message>\n\t\tEnable/disable custom chatspam message.");
			AddLog("22. neo_killspam <on|off> <message>\n\t\tEnable/disable custom killspam message.");
			AddLog("23. neo_spawnbot <max|number>\n\t\tSpawn bots into the current match (as host).");
			AddLog("24. neo_enableai <on|off>\n\t\tEnable/disable AI system for bots in public match (as host).");
			AddLog("25. neo_infinite\n\t\tSet scorelimit and timelimit to unlimited (as host).");
			AddLog("26. neo_godmode <on|off> <all|index>\n\t\tEnable/disable player god mode (as host).");
			AddLog("27. neo_noclip <on|off> <all|index>\n\t\tEnable/disable player no clip (as host).");
			AddLog("28. neo_infammo <on|off> <all|index>\n\t\tEnable/disable player infinite ammo (as host).");
			AddLog("29. neo_invisibile <on|off> <all|index>\n\t\tEnable/disable player invisibility (as host).");
			AddLog("30. neo_superspeed <on|off> <all|index>\n\t\tEnable/disable player super speed (as host).");
			AddLog("31. neo_freeze <on|off> <all|index>\n\t\tEnable/disable player freeze position (as host).");
			AddLog("32. neo_disconnect\n\t\tDisconnect from the current session.");

			bWriteLog = true;
		} ImGui::SameLine();

		if (ImGui::Button("Clear", ImVec2(50, 0)))
		{
			ClearLog();
			bWriteLog = true;
		} ImGui::SameLine();

		bool bCopyToClipboard = ImGui::Button("Copy", ImVec2(50, 0));
		const float flFooterHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

		ImGui::Separator();
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -flFooterHeightToReserve), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Selectable("Clear"))
			{
				ClearLog();
				bWriteLog = true;
			}

			if (ImGui::Selectable("Copy"))
			{
				bCopyToClipboard = true;
				bWriteLog = true;
			}

			ImGui::EndPopup();
			bWriteLog = true;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

		if (bCopyToClipboard)
			ImGui::LogToClipboard();

		ImVec4 cDefaultText = ImGui::GetStyleColorVec4(ImGuiCol_Text);

		for (int i = 0; i < vItems.Size; i++)
		{
			LPCSTR szItem = vItems[i];
			ImVec4 cTemp = cDefaultText;

			if (strncmp(szItem, PREFIX_ERROR, strlen(PREFIX_ERROR)) == 0)
				cTemp = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);

			else if (strncmp(szItem, PREFIX_COMMAND, strlen(PREFIX_COMMAND)) == 0)
				cTemp = ImVec4(1.0f, 0.5f, 0.3f, 1.0f);

			else if (strncmp(szItem, PREFIX_WARNING, strlen(PREFIX_WARNING)) == 0)
				cTemp = ImVec4(1.0f, 1.0f, 0.3f, 1.0f);

			else if (strncmp(szItem, PREFIX_LOG, strlen(PREFIX_LOG)) == 0)
				cTemp = ImVec4(0.3f, 0.5f, 1.0f, 1.0f);

			ImGui::PushStyleColor(ImGuiCol_Text, cTemp);
			ImGui::TextUnformatted(szItem);
			ImGui::PopStyleColor();
		}

		if (bCopyToClipboard)
			ImGui::LogFinish();

		if (bScrollToBottom)
			ImGui::SetScrollHere();

		bScrollToBottom = false;

		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::Separator();

		bool bReclaimFocus = false;
		ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());

		if (ImGui::InputText("", szInput, IM_ARRAYSIZE(szInput), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, _thunkTextCallback.GetThunk(), this))
		{
			LPSTR szInputEnd = szInput + strlen(szInput);

			while (szInputEnd > szInput && szInputEnd[-1] == ' ')
			{
				szInputEnd--;
			} *szInputEnd = 0;

			if (szInput[0])
				ExecCommand(szInput);

			ZeroMemory(szInput, sizeof(szInput));
			bReclaimFocus = true;

			bWriteLog = true;
		}

		ImGui::PopItemWidth();
		ImGui::SetItemDefaultFocus();

		if (bReclaimFocus)
			ImGui::SetKeyboardFocusHere(-1);

		ImGui::End();
	}
	/*
	//=====================================================================================
	*/
	void cConsole::SplitCommandLine(LPCSTR in, sCmdLine* out)
	{
		ZeroMemory(out, sizeof(sCmdLine));

		int iLength = (int)strlen(in) + 1;
		LPSTR szCmdBuffer = new char[iLength];

		strcpy_s(szCmdBuffer, iLength, in);

		LPSTR szToken = strtok(szCmdBuffer, " ");

		while (szToken != nullptr)
		{
			if (out->iArgNum == 0)
				strcpy_s(out->szCmdName, szToken);

			else
				strcpy_s(out->szCmdArgs[out->iArgNum - 1], szToken);

			szToken = strtok(nullptr, " ");

			if (szToken != nullptr)
				++out->iArgNum;
		}

		delete[] szCmdBuffer;
	}
	/*
	//=====================================================================================
	*/
	void cConsole::ExecCommand(LPCSTR command)
	{
		AddLog("%s %s\n", PREFIX_COMMAND, command);

		sCmdLine CmdLine;
		SplitCommandLine(command, &CmdLine);

		iHistoryPos = -1;

		for (int i = vHistory.Size - 1; i >= 0; i--)
		{
			if (Stricmp(vHistory[i], command) == 0)
			{
				free(vHistory[i]);
				vHistory.erase(vHistory.begin() + i);
				break;
			}
		}

		vHistory.push_back(Strdup(command));

		if (!Stricmp(CmdLine.szCmdName, "clear"))
		{
			ClearLog();
		}

		else if (!Stricmp(CmdLine.szCmdName, "history"))
		{
			int iFirst = vHistory.Size - 10;

			for (int i = iFirst > 0 ? iFirst : 0; i < vHistory.Size; i++)
				AddLog("%3d: %s\n", i, vHistory[i]);
		}

		else if (!Stricmp(CmdLine.szCmdName, "quit"))
		{
			exit(EXIT_SUCCESS);
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_name"))
		{
			char szArgBuff[512] = { NULL };

			for (int i = 0; i < CmdLine.iArgNum; i++)
				strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]));

			LPSTR szUsername = strtok(szArgBuff, "\n");

			if (szUsername)
			{
				AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

				strncpy_s((LPSTR)FindDmaAddy(OFF_STEAMAPI, std::vector<DWORD_PTR>({ OFF_STEAMNAME })), strlen(szUsername) + 1, szUsername, 32);
				Cbuf_AddText(VariadicText("name \"%s\"\n", szUsername));

				AddLog("Name has been changed to \"%s.\"", szUsername);
				AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
			}

			else
			{
				AddLog("%s Null argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_namespam"))
		{
			if (CmdLine.iArgNum > 0)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_mainGui.gNameSpam->Current.bValue = true;

					AddLog("Spamming random names has been enabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_mainGui.gNameSpam->Current.bValue = false;

					AddLog("Spamming random names has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_end"))
		{
			AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

			Cbuf_AddText(VariadicText("cmd lui 9 1 %i\n", *(int*)OFF_SERVERID));

			AddLog("Game lobby has been ended.");
			AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_crash"))
		{
			AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

			Cbuf_AddText("say \"\x5E\x01\x3D\x3D\xFF\"\n");

			AddLog("Game lobby has been crashed.");
			AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_minimap"))
		{
			if (CmdLine.iArgNum > 0)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					FindVariable("bg_compassShowEnemies")->Current.bValue = true;

					AddLog("Enemy minimap blips have been enabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					FindVariable("bg_compassShowEnemies")->Current.bValue = false;

					AddLog("Enemy minimap blips have been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_hostawall"))
		{
			if (CmdLine.iArgNum > 0)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					FindVariable("perk_bulletPenetrationMultiplier")->Current.flValue = 30.0f;
					EnablePerk(CG->PredictedPlayerState.iClientNum, PERK_EXTRABP);

					AddLog("Host autowall has been enabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					FindVariable("perk_bulletPenetrationMultiplier")->Current.flValue = FindVariable("perk_bulletPenetrationMultiplier")->Reset.flValue;
					DisablePerk(CG->PredictedPlayerState.iClientNum, PERK_EXTRABP);

					AddLog("Host autowall has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_rapidfire"))
		{
			if (CmdLine.iArgNum > 0)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					FindVariable("perk_weapRateMultiplier")->Current.flValue = 0.0f;
					EnablePerk(CG->PredictedPlayerState.iClientNum, PERK_RATEOFFIRE);

					AddLog("Rapidfire has been enabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					FindVariable("perk_weapRateMultiplier")->Current.flValue = FindVariable("perk_weapRateMultiplier")->Reset.flValue;
					DisablePerk(CG->PredictedPlayerState.iClientNum, PERK_RATEOFFIRE);

					AddLog("Rapidfire has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_superjump"))
		{
			if (CmdLine.iArgNum > 0)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					WriteMemoryProtected((LPVOID)OFF_ALTJUMPHEIGHT, 3000.0f);

					AddLog("Super jump has been enabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					WriteMemoryProtected((LPVOID)OFF_ALTJUMPHEIGHT, 39.0f);

					AddLog("Super jump has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_gravitygun"))
		{
			if (CmdLine.iArgNum > 0)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_hostMenu.gGravityGun->Current.bValue = true;

					AddLog("Gravity gun has been enabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_hostMenu.gGravityGun->Current.bValue = false;

					AddLog("Gravity gun has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_masskill"))
		{
			if (CmdLine.iArgNum > 0)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_hostMenu.gMassKill->Current.iValue = cHostMenu::MASSKILL_OFF;

					AddLog("Masskill has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "axis"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_hostMenu.gMassKill->Current.iValue = cHostMenu::MASSKILL_AXIS;

					AddLog("Masskill has been set to %s.", acut::ToLower(CmdLine.szCmdArgs[0]));
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "allies"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_hostMenu.gMassKill->Current.iValue = cHostMenu::MASSKILL_ALLIES;

					AddLog("Masskill has been set to %s.", acut::ToLower(CmdLine.szCmdArgs[0]));
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "all"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_hostMenu.gMassKill->Current.iValue = cHostMenu::MASSKILL_ALL;

					AddLog("Masskill has been set to %s.", acut::ToLower(CmdLine.szCmdArgs[0]));
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_antileave"))
		{
			if (CmdLine.iArgNum > 0)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_hooks.gAntiLeave->Current.bValue = true;

					AddLog("Anti-leave has been enabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_hooks.gAntiLeave->Current.bValue = false;

					AddLog("Anti-leave has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_bhop"))
		{
			if (CmdLine.iArgNum > 0)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_packets.gBunnyHop->Current.bValue = true;

					AddLog("Auto bunny hop has been enabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_packets.gBunnyHop->Current.bValue = false;

					AddLog("Auto bunny hop has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_tbag"))
		{
			if (CmdLine.iArgNum > 0)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					char szArgBuff[512] = { NULL };

					for (int i = 1; i < CmdLine.iArgNum; i++)
						strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]));

					LPSTR szTeaBagMessage = strtok(szArgBuff, "\n");

					if (szTeaBagMessage)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						_hooks.gTeaBagMessage->Current.szValue = VariadicText(szTeaBagMessage);
						_hooks.gTeaBag->Current.bValue = true;

						AddLog("Auto tea bag has been enabled with message \"%s.\"", szTeaBagMessage);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						_hooks.gTeaBag->Current.bValue = true;

						AddLog("Auto tea bag has been enabled.");
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_hooks.gTeaBagMessage->Current.szValue = VariadicText("");
					_hooks.gTeaBag->Current.bValue = false;

					AddLog("Auto tea bag has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_experience"))
		{
			if (CmdLine.iArgNum > 1)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "all"))
				{
					if (!Stricmp(CmdLine.szCmdArgs[1], "max"))
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						for (int i = 0; i < 10; i++)
							*(int*)(OFF_SQUADMEMBEREXP + (OFF_SQUADMEMBERSIZE * i)) = 4805;

						AddLog("All squad members' experience have been set to %s.", acut::ToLower(CmdLine.szCmdArgs[1]));
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) <= 4805)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						for (int i = 0; i < 10; i++)
							*(int*)(OFF_SQUADMEMBEREXP + (OFF_SQUADMEMBERSIZE * i)) = atoi(CmdLine.szCmdArgs[1]);

						AddLog("All squad members' experience have been set to %i.", atoi(CmdLine.szCmdArgs[1]));
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s Invalid argument(s).", PREFIX_ERROR);
					}
				}

				else if (atoi(CmdLine.szCmdArgs[0]) >= 1 && atoi(CmdLine.szCmdArgs[0]) <= 10)
				{
					if (!Stricmp(CmdLine.szCmdArgs[1], "max"))
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						*(int*)(OFF_SQUADMEMBEREXP + (OFF_SQUADMEMBERSIZE * (atoi(CmdLine.szCmdArgs[0]) - 1))) = 4805;

						AddLog("%s's experience has been set to %s.", (LPSTR)(OFF_SQUADMEMBERNAME + (OFF_SQUADMEMBERSIZE * (atoi(CmdLine.szCmdArgs[0]) - 1))), acut::ToLower(CmdLine.szCmdArgs[1]));
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) <= 4805)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						*(int*)(OFF_SQUADMEMBEREXP + (OFF_SQUADMEMBERSIZE * (atoi(CmdLine.szCmdArgs[0]) - 1))) = atoi(CmdLine.szCmdArgs[1]);

						AddLog("%s's experience has been set to %i.", (LPSTR)(OFF_SQUADMEMBERNAME + (OFF_SQUADMEMBERSIZE * (atoi(CmdLine.szCmdArgs[0]) - 1))), atoi(CmdLine.szCmdArgs[1]));
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s Invalid argument(s).", PREFIX_ERROR);
					}
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_prestige"))
		{
			if (CmdLine.iArgNum > 0)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "max"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					*(int*)OFF_PRESTIGE = 10;

					AddLog("Prestige has been set to %s.", acut::ToLower(CmdLine.szCmdArgs[0]));
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (atoi(CmdLine.szCmdArgs[0]) >= 0 && atoi(CmdLine.szCmdArgs[0]) <= 10)
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					*(int*)OFF_PRESTIGE = atoi(CmdLine.szCmdArgs[0]);

					AddLog("Prestige has been set to %i.", atoi(CmdLine.szCmdArgs[0]));
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_squadpoints"))
		{
			if (CmdLine.iArgNum > 0)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "max"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					*(int*)OFF_SQUADPOINTS = 99000000;

					AddLog("Squad points have been set to %s.", acut::ToLower(CmdLine.szCmdArgs[0]));
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (atoi(CmdLine.szCmdArgs[0]) >= 0 && atoi(CmdLine.szCmdArgs[0]) <= 99000000)
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					*(int*)OFF_SQUADPOINTS = atoi(CmdLine.szCmdArgs[0]);

					AddLog("Squad points have been set to %i.", atoi(CmdLine.szCmdArgs[0]));
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_unlockall"))
		{
			AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

			for (int i = 0; i < OFF_CHALLENGESIZE; i++)
				*(BYTE*)(OFF_CHALLENGES + i) = 0xFF;

			for (auto& Achievement : szAchievements)
				GameSendServerCommand(CG->PredictedPlayerState.iClientNum, SV_CMD_RELIABLE, VariadicText("3 %s", Achievement.c_str()));

			AddLog("All challenges/achievements have been unlocked.");
			AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_resetstats"))
		{
			AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

			Cbuf_AddText("statsReset\n");

			AddLog("All stats have been reset.");
			AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_hostdvar"))
		{
			if (CmdLine.iArgNum > 1)
			{
				DWORD dwDvar;
				if (GetVariableIndex(FindVariable(CmdLine.szCmdArgs[0]), &dwDvar))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					GameSendServerCommand(-1, SV_CMD_RELIABLE, VariadicText("%c %i \"%s\"", 0x71, dwDvar, CmdLine.szCmdArgs[1]));

					AddLog("DVAR \"%s\" has been set to \"%s.\"", CmdLine.szCmdArgs[0], CmdLine.szCmdArgs[1]);
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_message"))
		{
			if (CmdLine.iArgNum > 2)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "self"))
				{
					if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
					{
						if (!Stricmp(CmdLine.szCmdArgs[2], "lobby"))
						{
							char szArgBuff[512] = { NULL };

							for (int i = 3; i < CmdLine.iArgNum; i++)
								strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]));

							LPSTR szMessage = strtok(szArgBuff, "\n");

							if (szMessage)
							{
								AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

								Say(&GEntity[CG->PredictedPlayerState.iClientNum], NULL, 0, szMessage);

								AddLog("Message \"%s\" has been sent from %s to %s in %s mode.", szMessage, CmdLine.szCmdArgs[0], CmdLine.szCmdArgs[1], CmdLine.szCmdArgs[2]);
								AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
							}

							else
							{
								AddLog("%s Null argument(s).", PREFIX_ERROR);
							}
						}

						else if (!Stricmp(CmdLine.szCmdArgs[2], "team"))
						{
							char szArgBuff[512] = { NULL };

							for (int i = 3; i < CmdLine.iArgNum; i++)
								strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]));

							LPSTR szMessage = strtok(szArgBuff, "\n");

							if (szMessage)
							{
								AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

								Say(&GEntity[CG->PredictedPlayerState.iClientNum], NULL, 1, szMessage);

								AddLog("Message \"%s\" has been sent from %s to %s in %s mode.", szMessage, CmdLine.szCmdArgs[0], CmdLine.szCmdArgs[1], CmdLine.szCmdArgs[2]);
								AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
							}

							else
							{
								AddLog("%s Null argument(s).", PREFIX_ERROR);
							}
						}

						else if (!Stricmp(CmdLine.szCmdArgs[2], "private"))
						{
							char szArgBuff[512] = { NULL };

							for (int i = 3; i < CmdLine.iArgNum; i++)
								strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]));

							LPSTR szMessage = strtok(szArgBuff, "\n");

							if (szMessage)
							{
								AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

								Say(&GEntity[CG->PredictedPlayerState.iClientNum], NULL, 2, szMessage);

								AddLog("Message \"%s\" has been sent from %s to %s in %s mode.", szMessage, CmdLine.szCmdArgs[0], CmdLine.szCmdArgs[1], CmdLine.szCmdArgs[2]);
								AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
							}

							else
							{
								AddLog("%s Null argument(s).", PREFIX_ERROR);
							}
						}

						else
						{
							AddLog("%s Invalid argument(s).", PREFIX_ERROR);
						}
					}

					else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) < FindVariable("sv_maxclients")->Current.iValue)
					{
						if (!Stricmp(CmdLine.szCmdArgs[2], "lobby"))
						{
							char szArgBuff[512] = { NULL };

							for (int i = 3; i < CmdLine.iArgNum; i++)
								strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]));

							LPSTR szMessage = strtok(szArgBuff, "\n");

							if (szMessage)
							{
								AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

								Say(&GEntity[CG->PredictedPlayerState.iClientNum], &GEntity[atoi(CmdLine.szCmdArgs[1])], 0, szMessage);

								AddLog("Message \"%s\" has been sent from %s to %s in %s mode.", szMessage, CmdLine.szCmdArgs[0], ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName, CmdLine.szCmdArgs[2]);
								AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
							}

							else
							{
								AddLog("%s Null argument(s).", PREFIX_ERROR);
							}
						}

						else if (!Stricmp(CmdLine.szCmdArgs[2], "team"))
						{
							char szArgBuff[512] = { NULL };

							for (int i = 3; i < CmdLine.iArgNum; i++)
								strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]));

							LPSTR szMessage = strtok(szArgBuff, "\n");

							if (szMessage)
							{
								AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

								Say(&GEntity[CG->PredictedPlayerState.iClientNum], &GEntity[atoi(CmdLine.szCmdArgs[1])], 1, szMessage);

								AddLog("Message \"%s\" has been sent from %s to %s in %s mode.", szMessage, CmdLine.szCmdArgs[0], ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName, CmdLine.szCmdArgs[2]);
								AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
							}

							else
							{
								AddLog("%s Null argument(s).", PREFIX_ERROR);
							}
						}

						else if (!Stricmp(CmdLine.szCmdArgs[2], "private"))
						{
							char szArgBuff[512] = { NULL };

							for (int i = 3; i < CmdLine.iArgNum; i++)
								strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]));

							LPSTR szMessage = strtok(szArgBuff, "\n");

							if (szMessage)
							{
								AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

								Say(&GEntity[CG->PredictedPlayerState.iClientNum], &GEntity[atoi(CmdLine.szCmdArgs[1])], 2, szMessage);

								AddLog("Message \"%s\" has been sent from %s to %s in %s mode.", szMessage, CmdLine.szCmdArgs[0], ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName, CmdLine.szCmdArgs[2]);
								AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
							}

							else
							{
								AddLog("%s Null argument(s).", PREFIX_ERROR);
							}
						}

						else
						{
							AddLog("%s Invalid argument(s).", PREFIX_ERROR);
						}
					}

					else
					{
						AddLog("%s Invalid argument(s).", PREFIX_ERROR);
					}
				}

				else if (atoi(CmdLine.szCmdArgs[0]) >= 0 && atoi(CmdLine.szCmdArgs[0]) < FindVariable("sv_maxclients")->Current.iValue)
				{
					if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
					{
						if (!Stricmp(CmdLine.szCmdArgs[2], "lobby"))
						{
							char szArgBuff[512] = { NULL };

							for (int i = 3; i < CmdLine.iArgNum; i++)
								strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]));

							LPSTR szMessage = strtok(szArgBuff, "\n");

							if (szMessage)
							{
								AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

								Say(&GEntity[atoi(CmdLine.szCmdArgs[0])], NULL, 0, szMessage);

								AddLog("Message \"%s\" has been sent from %s to %s in %s mode.", szMessage, ClientInfo[atoi(CmdLine.szCmdArgs[0])].szName, CmdLine.szCmdArgs[1], CmdLine.szCmdArgs[2]);
								AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
							}

							else
							{
								AddLog("%s Null argument(s).", PREFIX_ERROR);
							}
						}

						else if (!Stricmp(CmdLine.szCmdArgs[2], "team"))
						{
							char szArgBuff[512] = { NULL };

							for (int i = 3; i < CmdLine.iArgNum; i++)
								strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]));

							LPSTR szMessage = strtok(szArgBuff, "\n");

							if (szMessage)
							{
								AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

								Say(&GEntity[atoi(CmdLine.szCmdArgs[0])], NULL, 1, szMessage);

								AddLog("Message \"%s\" has been sent from %s to %s in %s mode.", szMessage, ClientInfo[atoi(CmdLine.szCmdArgs[0])].szName, CmdLine.szCmdArgs[1], CmdLine.szCmdArgs[2]);
								AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
							}

							else
							{
								AddLog("%s Null argument(s).", PREFIX_ERROR);
							}
						}

						else if (!Stricmp(CmdLine.szCmdArgs[2], "private"))
						{
							char szArgBuff[512] = { NULL };

							for (int i = 3; i < CmdLine.iArgNum; i++)
								strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]));

							LPSTR szMessage = strtok(szArgBuff, "\n");

							if (szMessage)
							{
								AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

								Say(&GEntity[atoi(CmdLine.szCmdArgs[0])], NULL, 2, szMessage);

								AddLog("Message \"%s\" has been sent from %s to %s in %s mode.", szMessage, ClientInfo[atoi(CmdLine.szCmdArgs[0])].szName, CmdLine.szCmdArgs[1], CmdLine.szCmdArgs[2]);
								AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
							}

							else
							{
								AddLog("%s Null argument(s).", PREFIX_ERROR);
							}
						}

						else
						{
							AddLog("%s Invalid argument(s).", PREFIX_ERROR);
						}
					}

					else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) < FindVariable("sv_maxclients")->Current.iValue)
					{
						if (!Stricmp(CmdLine.szCmdArgs[2], "lobby"))
						{
							char szArgBuff[512] = { NULL };

							for (int i = 3; i < CmdLine.iArgNum; i++)
								strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]));

							LPSTR szMessage = strtok(szArgBuff, "\n");

							if (szMessage)
							{
								AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

								Say(&GEntity[atoi(CmdLine.szCmdArgs[0])], &GEntity[atoi(CmdLine.szCmdArgs[1])], 0, szMessage);

								AddLog("Message \"%s\" has been sent from %s to %s in %s mode.", szMessage, ClientInfo[atoi(CmdLine.szCmdArgs[0])].szName, ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName, CmdLine.szCmdArgs[2]);
								AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
							}

							else
							{
								AddLog("%s Null argument(s).", PREFIX_ERROR);
							}
						}

						else if (!Stricmp(CmdLine.szCmdArgs[2], "team"))
						{
							char szArgBuff[512] = { NULL };

							for (int i = 3; i < CmdLine.iArgNum; i++)
								strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]));

							LPSTR szMessage = strtok(szArgBuff, "\n");

							if (szMessage)
							{
								AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

								Say(&GEntity[atoi(CmdLine.szCmdArgs[0])], &GEntity[atoi(CmdLine.szCmdArgs[1])], 1, szMessage);

								AddLog("Message \"%s\" has been sent from %s to %s in %s mode.", szMessage, ClientInfo[atoi(CmdLine.szCmdArgs[0])].szName, ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName, CmdLine.szCmdArgs[2]);
								AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
							}

							else
							{
								AddLog("%s Null argument(s).", PREFIX_ERROR);
							}
						}

						else if (!Stricmp(CmdLine.szCmdArgs[2], "private"))
						{
							char szArgBuff[512] = { NULL };

							for (int i = 3; i < CmdLine.iArgNum; i++)
								strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]));

							LPSTR szMessage = strtok(szArgBuff, "\n");

							if (szMessage)
							{
								AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

								Say(&GEntity[atoi(CmdLine.szCmdArgs[0])], &GEntity[atoi(CmdLine.szCmdArgs[1])], 2, szMessage);

								AddLog("Message \"%s\" has been sent from %s to %s in %s mode.", szMessage, ClientInfo[atoi(CmdLine.szCmdArgs[0])].szName, ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName, CmdLine.szCmdArgs[2]);
								AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
							}

							else
							{
								AddLog("%s Null argument(s).", PREFIX_ERROR);
							}
						}

						else
						{
							AddLog("%s Invalid argument(s).", PREFIX_ERROR);
						}
					}

					else
					{
						AddLog("%s Invalid argument(s).", PREFIX_ERROR);
					}
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_chatspam"))
		{
			if (CmdLine.iArgNum > 0)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					char szArgBuff[512] = { NULL };

					for (int i = 1; i < CmdLine.iArgNum; i++)
						strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]));

					LPSTR szChatSpam = strtok(szArgBuff, "\n");

					if (szChatSpam)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						_mainGui.gChatSpamMessage->Current.szValue = VariadicText(szChatSpam);
						_mainGui.gChatSpam->Current.bValue = true;

						AddLog("Custom chatspam message \"%s\" has been enabled.", szChatSpam);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s Null argument(s).", PREFIX_ERROR);
					}
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_mainGui.gChatSpamMessage->Current.szValue = VariadicText("");
					_mainGui.gChatSpam->Current.bValue = false;

					AddLog("Custom chatspam message has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_killspam"))
		{
			if (CmdLine.iArgNum > 0)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					char szArgBuff[512] = { NULL };

					for (int i = 1; i < CmdLine.iArgNum; i++)
						strcat_s(szArgBuff, VariadicText(i == CmdLine.iArgNum - 1 ? "%s" : "%s ", CmdLine.szCmdArgs[i]));

					LPSTR szKillSpam = strtok(szArgBuff, "\n");

					if (szKillSpam)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						_hooks.gKillSpamMessage->Current.szValue = VariadicText(szKillSpam);
						_hooks.gKillSpam->Current.bValue = true;

						AddLog("Custom killspam message \"%s\" has been enabled.", szKillSpam);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s Null argument(s).", PREFIX_ERROR);
					}
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_hooks.gKillSpamMessage->Current.szValue = VariadicText("");
					_hooks.gKillSpam->Current.bValue = false;

					AddLog("Custom killspam message has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_spawnbot"))
		{
			if (CmdLine.iArgNum > 0)
			{
				int iCurrentPlayers = 0;

				for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
					if (CharacterInfo[i].iInfoValid)
						iCurrentPlayers++;

				if (!Stricmp(CmdLine.szCmdArgs[0], "max"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_hostMenu.SpawnBots(FindVariable("sv_maxclients")->Current.iValue - iCurrentPlayers);

					AddLog("Spawned %i bots into the match.", FindVariable("sv_maxclients")->Current.iValue - iCurrentPlayers);
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (atoi(CmdLine.szCmdArgs[0]) >= 0 && atoi(CmdLine.szCmdArgs[0]) <= FindVariable("sv_maxclients")->Current.iValue - iCurrentPlayers)
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					_hostMenu.SpawnBots(atoi(CmdLine.szCmdArgs[0]));

					AddLog("Spawned %i bot(s) into the match.", atoi(CmdLine.szCmdArgs[0]));
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_enableai"))
		{
			if (CmdLine.iArgNum > 0)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					FindVariable("xblive_privatematch")->Current.bValue = true;

					AddLog("AI system has been enabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

					FindVariable("xblive_privatematch")->Current.bValue = false;

					AddLog("AI system has been disabled.");
					AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_infinite"))
		{
			AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

			Cbuf_AddText(VariadicText("scr_%s_scorelimit 0\n", FindVariable("g_gametype")->Current.szValue));
			Cbuf_AddText(VariadicText("scr_%s_timelimit 0\n", FindVariable("g_gametype")->Current.szValue));

			AddLog("Score/time limit has been set to unlimited.");
			AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_godmode"))
		{
			if (CmdLine.iArgNum > 1)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
							_hostMenu.HostMenu.PlayerMod[i].bGodMode = true;

						AddLog("God mode has been enabled for %s.", CmdLine.szCmdArgs[1]);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) < FindVariable("sv_maxclients")->Current.iValue)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						_hostMenu.HostMenu.PlayerMod[atoi(CmdLine.szCmdArgs[1])].bGodMode = true;

						AddLog("God mode has been enabled for %s.", ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s Invalid argument(s).", PREFIX_ERROR);
					}
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
							_hostMenu.HostMenu.PlayerMod[i].bGodMode = false;

						AddLog("God mode has been disabled for %s.", CmdLine.szCmdArgs[1]);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) < FindVariable("sv_maxclients")->Current.iValue)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						_hostMenu.HostMenu.PlayerMod[atoi(CmdLine.szCmdArgs[1])].bGodMode = false;

						AddLog("God mode has been disabled for %s.", ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s Invalid argument(s).", PREFIX_ERROR);
					}
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_noclip"))
		{
			if (CmdLine.iArgNum > 1)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
							_hostMenu.HostMenu.PlayerMod[i].bNoClip = true;

						AddLog("No clip has been enabled for %s.", CmdLine.szCmdArgs[1]);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) < FindVariable("sv_maxclients")->Current.iValue)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						_hostMenu.HostMenu.PlayerMod[atoi(CmdLine.szCmdArgs[1])].bNoClip = true;

						AddLog("No clip has been enabled for %s.", ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s Invalid argument(s).", PREFIX_ERROR);
					}
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
							_hostMenu.HostMenu.PlayerMod[i].bNoClip = false;

						AddLog("No clip has been disabled for %s.", CmdLine.szCmdArgs[1]);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) < FindVariable("sv_maxclients")->Current.iValue)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						_hostMenu.HostMenu.PlayerMod[atoi(CmdLine.szCmdArgs[1])].bNoClip = false;

						AddLog("No clip has been disabled for %s.", ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s Invalid argument(s).", PREFIX_ERROR);
					}
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_infammo"))
		{
			if (CmdLine.iArgNum > 1)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
							_hostMenu.HostMenu.PlayerMod[i].bInfiniteAmmo = true;

						AddLog("Infinite ammo has been enabled for %s.", CmdLine.szCmdArgs[1]);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) < FindVariable("sv_maxclients")->Current.iValue)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						_hostMenu.HostMenu.PlayerMod[atoi(CmdLine.szCmdArgs[1])].bInfiniteAmmo = true;

						AddLog("Infinite ammo has been enabled for %s.", ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s Invalid argument(s).", PREFIX_ERROR);
					}
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
							_hostMenu.HostMenu.PlayerMod[i].bInfiniteAmmo = false;

						AddLog("Infinite ammo been disabled for %s.", CmdLine.szCmdArgs[1]);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) < FindVariable("sv_maxclients")->Current.iValue)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						_hostMenu.HostMenu.PlayerMod[atoi(CmdLine.szCmdArgs[1])].bInfiniteAmmo = false;

						AddLog("Infinite ammo has been disabled for %s.", ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s Invalid argument(s).", PREFIX_ERROR);
					}
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_invisible"))
		{
			if (CmdLine.iArgNum > 1)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
							_hostMenu.HostMenu.PlayerMod[i].bInvisibility = true;

						AddLog("Invisibility has been enabled for %s.", CmdLine.szCmdArgs[1]);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) < FindVariable("sv_maxclients")->Current.iValue)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						_hostMenu.HostMenu.PlayerMod[atoi(CmdLine.szCmdArgs[1])].bInvisibility = true;

						AddLog("Invisibility has been enabled for %s.", ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s Invalid argument(s).", PREFIX_ERROR);
					}
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
							_hostMenu.HostMenu.PlayerMod[i].bInvisibility = false;

						AddLog("Invisibility has been disabled for %s.", CmdLine.szCmdArgs[1]);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) < FindVariable("sv_maxclients")->Current.iValue)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						_hostMenu.HostMenu.PlayerMod[atoi(CmdLine.szCmdArgs[1])].bInvisibility = false;

						AddLog("Invisibility has been disabled for %s.", ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s Invalid argument(s).", PREFIX_ERROR);
					}
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_superspeed"))
		{
			if (CmdLine.iArgNum > 1)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
							_hostMenu.HostMenu.PlayerMod[i].bSuperSpeed = true;

						AddLog("Super speed has been enabled for %s.", CmdLine.szCmdArgs[1]);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) < FindVariable("sv_maxclients")->Current.iValue)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						_hostMenu.HostMenu.PlayerMod[atoi(CmdLine.szCmdArgs[1])].bSuperSpeed = true;

						AddLog("Super speed has been enabled for %s.", ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s Invalid argument(s).", PREFIX_ERROR);
					}
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
							_hostMenu.HostMenu.PlayerMod[i].bSuperSpeed = false;

						AddLog("Super speed has been disabled for %s.", CmdLine.szCmdArgs[1]);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) < FindVariable("sv_maxclients")->Current.iValue)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						_hostMenu.HostMenu.PlayerMod[atoi(CmdLine.szCmdArgs[1])].bSuperSpeed = false;

						AddLog("Super speed has been disabled for %s.", ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s Invalid argument(s).", PREFIX_ERROR);
					}
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_freeze"))
		{
			if (CmdLine.iArgNum > 1)
			{
				if (!Stricmp(CmdLine.szCmdArgs[0], "on"))
				{
					if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
							_hostMenu.HostMenu.PlayerMod[i].bFreezePosition = true;

						AddLog("Freeze position has been enabled for %s.", CmdLine.szCmdArgs[1]);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) < FindVariable("sv_maxclients")->Current.iValue)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						_hostMenu.HostMenu.PlayerMod[atoi(CmdLine.szCmdArgs[1])].bFreezePosition = true;

						AddLog("Freeze position has been enabled for %s.", ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s Invalid argument(s).", PREFIX_ERROR);
					}
				}

				else if (!Stricmp(CmdLine.szCmdArgs[0], "off"))
				{
					if (!Stricmp(CmdLine.szCmdArgs[1], "all"))
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
							_hostMenu.HostMenu.PlayerMod[i].bFreezePosition = false;

						AddLog("Freeze position has been disabled for %s.", CmdLine.szCmdArgs[1]);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else if (atoi(CmdLine.szCmdArgs[1]) >= 0 && atoi(CmdLine.szCmdArgs[1]) < FindVariable("sv_maxclients")->Current.iValue)
					{
						AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

						_hostMenu.HostMenu.PlayerMod[atoi(CmdLine.szCmdArgs[1])].bFreezePosition = false;

						AddLog("Freeze position has been disabled for %s.", ClientInfo[atoi(CmdLine.szCmdArgs[1])].szName);
						AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
					}

					else
					{
						AddLog("%s Invalid argument(s).", PREFIX_ERROR);
					}
				}

				else
				{
					AddLog("%s Invalid argument(s).", PREFIX_ERROR);
				}
			}

			else
			{
				AddLog("%s Missing argument(s).", PREFIX_ERROR);
			}
		}

		else if (!Stricmp(CmdLine.szCmdName, "neo_disconnect"))
		{
			AddLog("%s executing.", acut::ToLower(CmdLine.szCmdName).c_str());

			Cbuf_AddText("disconnect\n");

			AddLog("Disconnected from the game session.");
			AddLog("%s executed.", acut::ToLower(CmdLine.szCmdName).c_str());
		}

		else
		{
			Cbuf_AddText(command);
		}
	}
	/*
	//=====================================================================================
	*/
	int cConsole::TextEditCallbackStub(ImGuiTextEditCallbackData* data)
	{
		cConsole* Console = (cConsole*)data->UserData;
		return Console->TextEditCallback(data);
	}
	/*
	//=====================================================================================
	*/
	int cConsole::TextEditCallback(ImGuiTextEditCallbackData* data)
	{
		switch (data->EventFlag)
		{
		case ImGuiInputTextFlags_CallbackCompletion:
		{
			LPCSTR szEnd = data->Buf + data->CursorPos, szStart = szEnd;

			while (szStart > data->Buf)
			{
				const char c = szStart[-1];

				if (c == ' ' || c == '\t' || c == ',' || c == ';')
					break;

				szStart--;
			}

			ImVector<LPCSTR> vCandidates;

			for (int i = 0; i < vCommands.Size; i++)
				if (Strnicmp(vCommands[i], szStart, (int)(szEnd - szStart)) == 0)
					vCandidates.push_back(vCommands[i]);

			if (vCandidates.Size == 0)
				AddLog("No match for \"%.*s\"!\n", (int)(szEnd - szStart), szStart);

			else if (vCandidates.Size == 1)
			{
				data->DeleteChars((int)(szStart - data->Buf), (int)(szEnd - szStart));
				data->InsertChars(data->CursorPos, vCandidates[0]);
				data->InsertChars(data->CursorPos, " ");
			}

			else
			{
				int iMatchLength = (int)(szEnd - szStart);

				for (;;)
				{
					int c = 0;

					bool bAllCandidatesMatches = true;

					for (int i = 0; i < vCandidates.Size && bAllCandidatesMatches; i++)
						if (i == 0)
							c = tolower(vCandidates[i][iMatchLength]);

						else if (c == 0 || c != tolower(vCandidates[i][iMatchLength]))
							bAllCandidatesMatches = false;

					if (!bAllCandidatesMatches)
						break;

					iMatchLength++;
				}

				if (iMatchLength > 0)
				{
					data->DeleteChars((int)(szStart - data->Buf), (int)(szEnd - szStart));
					data->InsertChars(data->CursorPos, vCandidates[0], vCandidates[0] + iMatchLength);
				}

				AddLog("Possible matches:\n");

				for (int i = 0; i < vCandidates.Size; i++)
					AddLog("- %s\n", vCandidates[i]);
			}

			break;
		}
		case ImGuiInputTextFlags_CallbackHistory:
		{
			const int iPreviousHistoryPos = iHistoryPos;

			if (data->EventKey == ImGuiKey_UpArrow)
			{
				if (iHistoryPos == -1)
					iHistoryPos = vHistory.Size - 1;

				else if (iHistoryPos > 0)
					iHistoryPos--;
			}

			else if (data->EventKey == ImGuiKey_DownArrow)
			{
				if (iHistoryPos != -1)
					if (++iHistoryPos >= vHistory.Size)
						iHistoryPos = -1;
			}

			if (iPreviousHistoryPos != iHistoryPos)
			{
				data->CursorPos = data->SelectionStart = data->SelectionEnd = data->BufTextLen = (int)snprintf(data->Buf, (size_t)data->BufSize, "%s", (iHistoryPos >= 0) ? vHistory[iHistoryPos] : "");
				data->BufDirty = true;
			}
		}
		}
		return 0;
	}
}

//=====================================================================================