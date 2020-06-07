//=====================================================================================

#include "../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cMainGUI _mainGui;

	void cMainGUI::InitInterface()
	{
		hWindow = *(HWND*)OFF_WINDOWHANDLE;

		if (!hWindow || !pDevice || !pDeviceContext)
			return;

		oWindowProcess = (tWindowProcess)SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)_thunkWindowProcess.GetThunk());

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(hWindow);
		ImGui_ImplDX11_Init(pDevice, pDeviceContext);

		Menu.szIniFileName = acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_INI, " ", "");
		Menu.szLogFileName = acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_LOG, " ", "");

		ImGui::GetIO().IniFilename = Menu.szIniFileName.c_str();
		ImGui::GetIO().LogFilename = Menu.szLogFileName.c_str();

		ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->AddFontDefault();
		Bank_Gothic_Pro_Light = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(bank_gothic_pro_light_otf_compressed_data_base85, flBank_Gothic_Pro_Light = 14.0f);
		Eurostile_Extended = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(eurostile_extended_compressed_data_base85, flEurostile_Extended = Window->iHeight / 60.0f);

		bInitialized = true;
	}
	/*
	//=====================================================================================
	*/
	bool cMainGUI::GetKeyPress(int vkey, bool immediate)
	{
		if (VirtualKeys[vkey].bKey)
		{
			VirtualKeys[vkey].bUp = false;
			VirtualKeys[vkey].bDown = true;
		}

		else if (!VirtualKeys[vkey].bKey && VirtualKeys[vkey].bDown)
		{
			VirtualKeys[vkey].bUp = true;
			VirtualKeys[vkey].bDown = false;
		}

		else
		{
			VirtualKeys[vkey].bUp = false;
			VirtualKeys[vkey].bDown = false;
		}

		if (immediate)
			return VirtualKeys[vkey].bDown;

		else
			return VirtualKeys[vkey].bUp;
	}
	/*
	//=====================================================================================
	*/
	LRESULT CALLBACK cMainGUI::WindowProcess(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			VirtualKeys[VK_LBUTTON].bKey = true;
			break;

		case WM_LBUTTONUP:
			VirtualKeys[VK_LBUTTON].bKey = false;
			break;

		case WM_RBUTTONDOWN:
			VirtualKeys[VK_RBUTTON].bKey = true;
			break;

		case WM_RBUTTONUP:
			VirtualKeys[VK_RBUTTON].bKey = false;
			break;

		case WM_KEYDOWN:
			VirtualKeys[wParam].bKey = true;
			break;

		case WM_KEYUP:
			VirtualKeys[wParam].bKey = false;
			break;
		}

		if (GetKeyPress(VK_INSERT, false))
			Menu.bShowWindow = !Menu.bShowWindow;

		if (GetKeyPress(VK_DELETE, false))
			if (!LocalClientIsInGame())
				std::thread(&cHostMenu::StartMatch, &_hostMenu).detach();

		if (GetKeyPress(VK_HOME, false))
			_profiler.LoadProfile("");

		if (GetKeyPress(VK_END, false))
			_profiler.DisableAll();

		if (GetKeyPress(VK_PRIOR, false) && IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum))
			_hostMenu.HostMenu.vTeleport = PlayerState[CG->PredictedPlayerState.iClientNum].vOrigin;

		if (GetKeyPress(VK_NEXT, false) && IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum))
			PlayerState[CG->PredictedPlayerState.iClientNum].vOrigin = _hostMenu.HostMenu.vTeleport;

		*(bool*)OFF_MOUSEINPUT = !Menu.bShowWindow;
		FindVariable("cl_bypassMouseInput")->Current.iValue = Menu.bShowWindow;

		if (bInitialized && Menu.bShowWindow && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return TRUE;

		return CallWindowProc(oWindowProcess, hWnd, uMsg, wParam, lParam);
	}
	/*
	//=====================================================================================
	*/
	void WINAPI cMainGUI::Present(_In_ IDXGISwapChain* pSwapChain, _In_ UINT SyncInterval, _In_ UINT Flags)
	{
		if (!bInitialized)
		{
			pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
			pDevice->GetImmediateContext(&pDeviceContext);
			InitInterface();
		}

		else
		{
			ImGui::GetIO().MouseDrawCursor = Menu.bShowWindow;

			ImGui_ImplWin32_NewFrame();
			ImGui_ImplDX11_NewFrame();
			ImGui::NewFrame();

			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

			ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

			ImGui::Begin("INVISIBLE", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);
			ImGui::GetWindowDrawList()->PushClipRectFullScreen();

			ImGui::PopStyleColor(2);

			if (LocalClientIsInGame() && CG->PredictedPlayerState.iOtherFlags & 0x4000)
			{
				_drawing.DrawESP();
				_drawing.DrawCompass();
				_drawing.DrawRadar();
				_drawing.DrawCrosshair();
				_drawing.DrawTracers();
			}

			ImU32 cShadow = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

			std::string szWatermark(VariadicText("NEOGENESYS - COD GHOSTS by: InUrFace | Frametime: %s, Ping: %s",
				LocalClientIsInGame() ? VariadicText("%i ms", *(int*)OFF_FRAMETIME).c_str() : "N/A",
				LocalClientIsInGame() ? VariadicText("%i ms", *(int*)OFF_PING).c_str() : "N/A"));

			ImVec2 vWatermark(Eurostile_Extended->CalcTextSizeA(flEurostile_Extended, FLT_MAX, 0.0f, szWatermark.c_str()));
			ImU32 cWatermark = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

			ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
				ImVec2(vWatermark.y + 1.0f, flEurostile_Extended - vWatermark.y + 1.0f),
				cShadow, szWatermark.c_str());

			ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
				ImVec2(vWatermark.y, flEurostile_Extended - vWatermark.y),
				cWatermark, szWatermark.c_str());

			if (LocalClientIsInGame())
			{
				std::string szConnection(IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum) ? "Hosting" : "Not Hosting");
				ImVec2 vConnection(Eurostile_Extended->CalcTextSizeA(flEurostile_Extended, FLT_MAX, 0.0f, szConnection.c_str()));
				ImU32 cConnection = ImGui::ColorConvertFloat4ToU32(IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum) ? ImVec4(0.3f, 1.0f, 0.3f, 1.0f) : ImVec4(1.0f, 0.4f, 0.4f, 1.0f));

				ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
					ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - vConnection.x / 2.0f + 1.0f, flEurostile_Extended - vConnection.y + 1.0f),
					cShadow, szConnection.c_str());

				ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
					ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - vConnection.x / 2.0f, flEurostile_Extended - vConnection.y),
					cConnection, szConnection.c_str());
			}

			std::string szFramesPerSecond(VariadicText("%i", (int)ImGui::GetIO().Framerate));
			ImVec2 vFramesPerSecond(Eurostile_Extended->CalcTextSizeA(flEurostile_Extended, FLT_MAX, 0.0f, szFramesPerSecond.c_str()));
			ImU32 cFramesPerSecond = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 0.3f, 1.0f));

			ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
				ImVec2(ImGui::GetIO().DisplaySize.x - vFramesPerSecond.x - vFramesPerSecond.y + 1.0f, flEurostile_Extended - vFramesPerSecond.y + 1.0f),
				cShadow, szFramesPerSecond.c_str());

			ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
				ImVec2(ImGui::GetIO().DisplaySize.x - vFramesPerSecond.x - vFramesPerSecond.y, flEurostile_Extended - vFramesPerSecond.y),
				cFramesPerSecond, szFramesPerSecond.c_str());
			
			ImGui::End();

			if (Menu.bShowWindow)
			{
				DrawMainGUI();

				if (Menu.bShowMemoryEditor)
					Menu.MemEdit.DrawMemoryEditor(&Menu.bShowMemoryEditor, hIw6mp64_ship.lpBaseOfDll, hIw6mp64_ship.SizeOfImage, (size_t)hIw6mp64_ship.lpBaseOfDll);

				if (Menu.bShowConsole)
					_console.DrawConsole(&Menu.bShowConsole);

				if (_playerList.PlayerList.bShowWindow && LocalClientIsInGame())
					_playerList.DrawPlayerList();

				if (_hostMenu.HostMenu.bShowWindow && LocalClientIsInGame() && IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum) && !*(bool*)OFF_ISALIENSMODE)
					_hostMenu.DrawHostMenu();
			}

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			if (hWindow != *(HWND*)OFF_WINDOWHANDLE)
			{
				pDevice->Release();
				pDeviceContext->Release();

				ImGui_ImplWin32_Shutdown();
				ImGui_ImplDX11_Shutdown();
				ImGui::DestroyContext();

				bInitialized = false;
			}

			static int iTime = Sys_Milliseconds();

			if (Sys_Milliseconds() - iTime > max(*(int*)OFF_FRAMETIME, *(int*)OFF_PING))
			{
				if (LocalClientIsInGame() && !IsMigrating() && CG->PredictedPlayerState.iOtherFlags & 0x4000)
				{
					if (gNameSpam->Custom.bValue)
					{
						std::string szUserName = acut::RandomANString(0);

						strncpy_s((LPSTR)FindDmaAddy(OFF_STEAMAPI, std::vector<DWORD_PTR>({ OFF_STEAMNAME })), strlen(szUserName.c_str()) + 1, szUserName.c_str(), 32);
						Cbuf_AddText(VariadicText("name \"%s\"\n", szUserName.c_str()));
					}

					if (gChatSpam->Custom.bValue)
					{
						Cbuf_AddText(VariadicText("say \"%s\"\n", gChatSpamMessage->Custom.szValue));
					}
				}

				iTime = Sys_Milliseconds();
			}
		}
	}
	/*
	//=====================================================================================
	*/
	void cMainGUI::DrawMainGUI()
	{
		if (Menu.bWriteLog)
		{
			ImGui::LogToFile();
			Menu.bWriteLog = false;
		}

		ImGui::SetNextWindowSize(ImVec2(490.0f, 324.0f));
		ImGui::Begin("NEOGENESYS", &Menu.bShowWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
		ImGui::SetColorEditOptions(ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);

		if (ImGui::TabLabels(gMenuTabs->Limits.iMax, acut::StringVectorToCharPointerArray(gMenuTabs->szOptions), gMenuTabs->Custom.iValue, NULL, false, NULL, NULL, false, false, NULL, NULL, &ImVec2(94.0f, 25.0f)))
		{
			Menu.bWriteLog = true;
		}

		ImGui::BeginChild("ContentRegion", ImVec2(0.0f, 214.0f), true);
		ImGui::Separator();
		ImGui::NewLine();

		switch (gMenuTabs->Custom.iValue)
		{
		case MENU_TAB_AIMBOT:
		{
			if (ImGui::RadioButton(_aimBot.gAimBotMode->szOptions[cAimbot::AIMBOT_MODE_OFF].c_str(), &_aimBot.gAimBotMode->Custom.iValue, cAimbot::AIMBOT_MODE_OFF))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(148.0f);

			if (ImGui::RadioButton(_aimBot.gAimBotMode->szOptions[cAimbot::AIMBOT_MODE_MANUAL].c_str(), &_aimBot.gAimBotMode->Custom.iValue, cAimbot::AIMBOT_MODE_MANUAL))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::RadioButton(_aimBot.gAimBotMode->szOptions[cAimbot::AIMBOT_MODE_AUTO].c_str(), &_aimBot.gAimBotMode->Custom.iValue, cAimbot::AIMBOT_MODE_AUTO))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Checkbox(_aimBot.gAutoZoom->szLabel.c_str(), &_aimBot.gAutoZoom->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_aimBot.gAutoFire->szLabel.c_str(), &_aimBot.gAutoFire->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_targetList.gAutoWall->szLabel.c_str(), &_targetList.gAutoWall->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_aimBot.gSilentAim->szLabel.c_str(), &_aimBot.gSilentAim->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_targetList.gTargetAgents->szLabel.c_str(), &_targetList.gTargetAgents->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_targetList.gTargetMissiles->szLabel.c_str(), &_targetList.gTargetMissiles->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Combo(_antiAim.gAntiAim->szLabel.c_str(), &_antiAim.gAntiAim->Custom.iValue, acut::StringVectorToCharPointerArray(_antiAim.gAntiAim->szOptions), _antiAim.gAntiAim->Limits.iMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Combo(_targetList.gBoneScan->szLabel.c_str(), &_targetList.gBoneScan->Custom.iValue, acut::StringVectorToCharPointerArray(_targetList.gBoneScan->szOptions), _targetList.gBoneScan->Limits.iMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Combo(_targetList.gSortMethod->szLabel.c_str(), &_targetList.gSortMethod->Custom.iValue, acut::StringVectorToCharPointerArray(_targetList.gSortMethod->szOptions), _targetList.gSortMethod->Limits.iMax))
			{
				Menu.bWriteLog = true;
			}
		} break;

		case MENU_TAB_WALLHACK:
		{
			if (ImGui::RadioButton(_drawing.gWallHackMode->szOptions[cDrawing::WALLHACK_MODE_AXIS].c_str(), &_drawing.gWallHackMode->Custom.iValue, cDrawing::WALLHACK_MODE_AXIS))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(148.0f);

			if (ImGui::RadioButton(_drawing.gWallHackMode->szOptions[cDrawing::WALLHACK_MODE_ALLIES].c_str(), &_drawing.gWallHackMode->Custom.iValue, cDrawing::WALLHACK_MODE_ALLIES))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::RadioButton(_drawing.gWallHackMode->szOptions[cDrawing::WALLHACK_MODE_ALL].c_str(), &_drawing.gWallHackMode->Custom.iValue, cDrawing::WALLHACK_MODE_ALL))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Combo(_drawing.gPlayerBoxes->szLabel.c_str(), &_drawing.gPlayerBoxes->Custom.iValue, acut::StringVectorToCharPointerArray(_drawing.gPlayerBoxes->szOptions), _drawing.gPlayerBoxes->Limits.iMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Combo(_drawing.gPlayerBones->szLabel.c_str(), &_drawing.gPlayerBones->Custom.iValue, acut::StringVectorToCharPointerArray(_drawing.gPlayerBones->szOptions), _drawing.gPlayerBones->Limits.iMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Combo(_drawing.gPlayerSnapLines->szLabel.c_str(), &_drawing.gPlayerSnapLines->Custom.iValue, acut::StringVectorToCharPointerArray(_drawing.gPlayerSnapLines->szOptions), _drawing.gPlayerSnapLines->Limits.iMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Checkbox(_drawing.gPlayerInformation->szLabel.c_str(), &_drawing.gPlayerInformation->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_drawing.gPlayerWeapons->szLabel.c_str(), &_drawing.gPlayerWeapons->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_hooks.gPlayerBulletTracers->szLabel.c_str(), &_hooks.gPlayerBulletTracers->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_drawing.gAgents->szLabel.c_str(), &_drawing.gAgents->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_drawing.gMissiles->szLabel.c_str(), &_drawing.gMissiles->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_drawing.gItems->szLabel.c_str(), &_drawing.gItems->Custom.bValue))
			{
				Menu.bWriteLog = true;
			}
		} break;

		case MENU_TAB_MISCELLANEOUS:
		{
			if (ImGui::Checkbox(_hooks.gKillSpam->szLabel.c_str(), &_hooks.gKillSpam->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_hooks.gNameStealer->szLabel.c_str(), &_hooks.gNameStealer->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_hooks.gThirdPerson->szLabel.c_str(), &_hooks.gThirdPerson->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_drawing.gPlayerCrossHair->szLabel.c_str(), &_drawing.gPlayerCrossHair->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_drawing.gPlayerCompass->szLabel.c_str(), &_drawing.gPlayerCompass->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_drawing.gPlayerRadar->szLabel.c_str(), &_drawing.gPlayerRadar->Custom.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox("Console", &Menu.bShowConsole))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox("Player List", &_playerList.PlayerList.bShowWindow))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox("Host Menu", &_hostMenu.HostMenu.bShowWindow))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox("Memory Editor", &Menu.bShowMemoryEditor))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Combo(_targetList.gRiotShielders->szLabel.c_str(), &_targetList.gRiotShielders->Custom.iValue, acut::StringVectorToCharPointerArray(_targetList.gRiotShielders->szOptions), _targetList.gRiotShielders->Limits.iMax))
			{
				Menu.bWriteLog = true;
			}
		} break;

		case MENU_TAB_TWEAKS:
		{
			if (ImGui::SliderInt(_targetList.gAimBone->szLabel.c_str(), &_targetList.gAimBone->Custom.iValue, _targetList.gAimBone->Limits.iMin, _targetList.gAimBone->Limits.iMax, szBones[_targetList.gAimBone->Custom.iValue].first.c_str()))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_targetList.gAimAngle->szLabel.c_str(), &_targetList.gAimAngle->Custom.iValue, _targetList.gAimAngle->Limits.iMin, _targetList.gAimAngle->Limits.iMax, "%d degrees"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_aimBot.gAimPower->szLabel.c_str(), &_aimBot.gAimPower->Custom.iValue, _aimBot.gAimPower->Limits.iMin, _aimBot.gAimPower->Limits.iMax, "%d percent"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_aimBot.gAutoAimTime->szLabel.c_str(), &_aimBot.gAutoAimTime->Custom.iValue, _aimBot.gAutoAimTime->Limits.iMin, _aimBot.gAutoAimTime->Limits.iMax, "%d ms"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_aimBot.gAutoAimDelay->szLabel.c_str(), &_aimBot.gAutoAimDelay->Custom.iValue, _aimBot.gAutoAimDelay->Limits.iMin, _aimBot.gAutoAimDelay->Limits.iMax, "%d ms"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_aimBot.gAutoZoomDelay->szLabel.c_str(), &_aimBot.gAutoZoomDelay->Custom.iValue, _aimBot.gAutoZoomDelay->Limits.iMin, _aimBot.gAutoZoomDelay->Limits.iMax, "%d ms"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_aimBot.gAutoFireDelay->szLabel.c_str(), &_aimBot.gAutoFireDelay->Custom.iValue, _aimBot.gAutoFireDelay->Limits.iMin, _aimBot.gAutoFireDelay->Limits.iMax, "%d ms"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderFloat(_removals.gRecoilFactor->szLabel.c_str(), &_removals.gRecoilFactor->Custom.flValue, _removals.gRecoilFactor->Limits.flMin, _removals.gRecoilFactor->Limits.flMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderFloat(_removals.gSpreadFactor->szLabel.c_str(), &_removals.gSpreadFactor->Custom.flValue, _removals.gSpreadFactor->Limits.flMin, _removals.gSpreadFactor->Limits.flMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderFloat("Field Of View", &FindVariable("cg_fov")->Current.flValue, FindVariable("cg_fov")->Domain.flMin, FindVariable("cg_fov")->Domain.flMax, "%.0f fov"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt("Frames Per Second", (int*)&FindVariable("com_maxfps")->Current.dwValue, FindVariable("com_maxfps")->Domain.dwMin, FindVariable("com_maxfps")->Domain.dwMax, "%d fps"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Button("Reset to Default", ImVec2(446.0f, 35.0f)))
			{
				_targetList.gAimBone->Custom.iValue = _targetList.gAimBone->Default.iValue;
				_targetList.gAimAngle->Custom.iValue = _targetList.gAimAngle->Default.iValue;
				_aimBot.gAimPower->Custom.iValue = _aimBot.gAimPower->Default.iValue;
				_aimBot.gAutoAimTime->Custom.iValue = _aimBot.gAutoAimTime->Default.iValue;
				_aimBot.gAutoAimDelay->Custom.iValue = _aimBot.gAutoAimDelay->Default.iValue;
				_aimBot.gAutoZoomDelay->Custom.iValue = _aimBot.gAutoZoomDelay->Default.iValue;
				_aimBot.gAutoFireDelay->Custom.iValue = _aimBot.gAutoFireDelay->Default.iValue;
				_removals.gRecoilFactor->Custom.flValue = _removals.gRecoilFactor->Default.flValue;
				_removals.gSpreadFactor->Custom.flValue = _removals.gSpreadFactor->Default.flValue;

				FindVariable("cg_fov")->Current.flValue = FindVariable("cg_fov")->Reset.flValue;
				FindVariable("com_maxfps")->Current.iValue = FindVariable("com_maxfps")->Reset.iValue;

				Menu.bWriteLog = true;
			}
		} break;

		case MENU_TAB_STYLES:
		{
			_drawing.ColorPicker(_drawing.gColorAxis->szLabel, _drawing.gColorAxis->Custom.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_drawing.gColorAxis->szLabel.c_str(), _drawing.gColorAxis->Custom.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_drawing.gColorAllies->szLabel, _drawing.gColorAllies->Custom.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_drawing.gColorAllies->szLabel.c_str(), _drawing.gColorAllies->Custom.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_drawing.gColorAccents->szLabel, _drawing.gColorAccents->Custom.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_drawing.gColorAccents->szLabel.c_str(), _drawing.gColorAccents->Custom.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_drawing.gColorCrossHair->szLabel, _drawing.gColorCrossHair->Custom.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_drawing.gColorCrossHair->szLabel.c_str(), _drawing.gColorCrossHair->Custom.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_drawing.gColorText->szLabel, _drawing.gColorText->Custom.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_drawing.gColorText->szLabel.c_str(), _drawing.gColorText->Custom.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_drawing.gColorShadow->szLabel, _drawing.gColorShadow->Custom.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_drawing.gColorShadow->szLabel.c_str(), _drawing.gColorShadow->Custom.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Button("Reset to Default", ImVec2(446.0f, 35.0f)))
			{
				_drawing.gColorAxis->Custom.cValue = _drawing.gColorAxis->Default.cValue;
				_drawing.gColorAllies->Custom.cValue = _drawing.gColorAllies->Default.cValue;
				_drawing.gColorAccents->Custom.cValue = _drawing.gColorAccents->Default.cValue;
				_drawing.gColorCrossHair->Custom.cValue = _drawing.gColorCrossHair->Default.cValue;
				_drawing.gColorText->Custom.cValue = _drawing.gColorText->Default.cValue;
				_drawing.gColorShadow->Custom.cValue = _drawing.gColorShadow->Default.cValue;

				Menu.bWriteLog = true;
			}
		} break;
		}

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::EndChild();

		if (ImGui::Button("Set as Default", ImVec2(156.0f, 30.0f)))
		{
			_profiler.SaveProfile("");
			Menu.bWriteLog = true;
		} ImGui::SameLine(0.0f, 3.0f);

		static bool bSaveButton;
		if ((bSaveButton = ImGui::Button("Save Profile", ImVec2(156.0f, 30.0f))))
		{
			Menu.bWriteLog = true;
		} ImGui::SameLine(0.0f, 3.0f);
		LPCSTR szSavePath = Menu.SaveDialog.saveFileDialog(bSaveButton, acut::GetParent(Menu.szProfilePath).c_str(), NULL, ".xml", "Save Profile");

		if (strlen(szSavePath))
		{
			_profiler.SaveProfile(szSavePath);
			strcpy_s(Menu.szProfilePath, szSavePath);
		}

		static bool bLoadButton;
		if ((bLoadButton = ImGui::Button("Load Profile", ImVec2(156.0f, 30.0f))))
		{
			Menu.bWriteLog = true;
		}
		LPCSTR szLoadPath = Menu.LoadDialog.chooseFileDialog(bLoadButton, acut::GetParent(Menu.szProfilePath).c_str(), ".xml", "Load Profile");

		if (strlen(szLoadPath))
		{
			_profiler.LoadProfile(szLoadPath);
			strcpy_s(Menu.szProfilePath, szLoadPath);
		}

		ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
		ImGui::InputText("", Menu.szProfilePath, IM_ARRAYSIZE(Menu.szProfilePath), ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
		ImGui::End();
	}
}

//=====================================================================================