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
			VectorCopy(PlayerState[CG->PredictedPlayerState.iClientNum].vOrigin, _hostMenu.HostMenu.vTeleport);

		if (GetKeyPress(VK_NEXT, false) && IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum))
			VectorCopy(_hostMenu.HostMenu.vTeleport, PlayerState[CG->PredictedPlayerState.iClientNum].vOrigin);

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
					if (_profiler.gNameSpam->Current.bValue)
					{
						std::string szUserName = acut::RandomANString(0);

						strncpy_s((LPSTR)FindDmaAddy(OFF_STEAMAPI, std::vector<DWORD_PTR>({ OFF_STEAMNAME })), strlen(szUserName.c_str()) + 1, szUserName.c_str(), 32);
						Cbuf_AddText(VariadicText("name \"%s\"\n", szUserName.c_str()));
					}

					if (_profiler.gChatSpam->Current.bValue)
					{
						Cbuf_AddText(VariadicText("say \"%s\"\n", _profiler.gChatSpamMessage->Current.szValue));
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

		if (ImGui::TabLabels(_profiler.gMenuTabs->Domain.iMax, acut::StringVectorToCharPointerArray(_profiler.gMenuTabs->szItems), _profiler.gMenuTabs->Current.iValue, NULL, false, NULL, NULL, false, false, NULL, NULL, &ImVec2(94.0f, 25.0f)))
		{
			Menu.bWriteLog = true;
		}

		ImGui::BeginChild("ContentRegion", ImVec2(0.0f, 214.0f), true);
		ImGui::Separator();
		ImGui::NewLine();

		switch (_profiler.gMenuTabs->Current.iValue)
		{
		case cProfiler::MENU_TAB_AIMBOT:
		{
			if (ImGui::RadioButton(_profiler.gAimBotMode->szItems[cProfiler::AIMBOT_MODE_OFF].c_str(), &_profiler.gAimBotMode->Current.iValue, cProfiler::AIMBOT_MODE_OFF))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(148.0f);

			if (ImGui::RadioButton(_profiler.gAimBotMode->szItems[cProfiler::AIMBOT_MODE_MANUAL].c_str(), &_profiler.gAimBotMode->Current.iValue, cProfiler::AIMBOT_MODE_MANUAL))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::RadioButton(_profiler.gAimBotMode->szItems[cProfiler::AIMBOT_MODE_AUTO].c_str(), &_profiler.gAimBotMode->Current.iValue, cProfiler::AIMBOT_MODE_AUTO))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Checkbox(_profiler.gAutoZoom->szName.c_str(), &_profiler.gAutoZoom->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_profiler.gAutoFire->szName.c_str(), &_profiler.gAutoFire->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_profiler.gAutoWall->szName.c_str(), &_profiler.gAutoWall->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_profiler.gSilentAim->szName.c_str(), &_profiler.gSilentAim->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_profiler.gTargetAgents->szName.c_str(), &_profiler.gTargetAgents->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_profiler.gTargetMissiles->szName.c_str(), &_profiler.gTargetMissiles->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Combo(_profiler.gAntiAim->szName.c_str(), &_profiler.gAntiAim->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gAntiAim->szItems), _profiler.gAntiAim->Domain.iMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Combo(_profiler.gBoneScan->szName.c_str(), &_profiler.gBoneScan->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gBoneScan->szItems), _profiler.gBoneScan->Domain.iMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Combo(_profiler.gSortMethod->szName.c_str(), &_profiler.gSortMethod->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gSortMethod->szItems), _profiler.gSortMethod->Domain.iMax))
			{
				Menu.bWriteLog = true;
			}
		} break;

		case cProfiler::MENU_TAB_WALLHACK:
		{
			if (ImGui::RadioButton(_profiler.gWallHackMode->szItems[cProfiler::WALLHACK_MODE_AXIS].c_str(), &_profiler.gWallHackMode->Current.iValue, cProfiler::WALLHACK_MODE_AXIS))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(148.0f);

			if (ImGui::RadioButton(_profiler.gWallHackMode->szItems[cProfiler::WALLHACK_MODE_ALLIES].c_str(), &_profiler.gWallHackMode->Current.iValue, cProfiler::WALLHACK_MODE_ALLIES))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::RadioButton(_profiler.gWallHackMode->szItems[cProfiler::WALLHACK_MODE_ALL].c_str(), &_profiler.gWallHackMode->Current.iValue, cProfiler::WALLHACK_MODE_ALL))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Combo(_profiler.gPlayerBoxes->szName.c_str(), &_profiler.gPlayerBoxes->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gPlayerBoxes->szItems), _profiler.gPlayerBoxes->Domain.iMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Combo(_profiler.gPlayerBones->szName.c_str(), &_profiler.gPlayerBones->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gPlayerBones->szItems), _profiler.gPlayerBones->Domain.iMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Combo(_profiler.gPlayerSnapLines->szName.c_str(), &_profiler.gPlayerSnapLines->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gPlayerSnapLines->szItems), _profiler.gPlayerSnapLines->Domain.iMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Checkbox(_profiler.gPlayerInformation->szName.c_str(), &_profiler.gPlayerInformation->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_profiler.gPlayerWeapons->szName.c_str(), &_profiler.gPlayerWeapons->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_profiler.gPlayerBulletTracers->szName.c_str(), &_profiler.gPlayerBulletTracers->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_profiler.gAgents->szName.c_str(), &_profiler.gAgents->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_profiler.gMissiles->szName.c_str(), &_profiler.gMissiles->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_profiler.gItems->szName.c_str(), &_profiler.gItems->Current.bValue))
			{
				Menu.bWriteLog = true;
			}
		} break;

		case cProfiler::MENU_TAB_MISCELLANEOUS:
		{
			if (ImGui::Checkbox(_profiler.gKillSpam->szName.c_str(), &_profiler.gKillSpam->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_profiler.gNameStealer->szName.c_str(), &_profiler.gNameStealer->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_profiler.gThirdPersonAntiAim->szName.c_str(), &_profiler.gThirdPersonAntiAim->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_profiler.gPlayerCrossHair->szName.c_str(), &_profiler.gPlayerCrossHair->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_profiler.gPlayerCompass->szName.c_str(), &_profiler.gPlayerCompass->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_profiler.gPlayerRadar->szName.c_str(), &_profiler.gPlayerRadar->Current.bValue))
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

			if (ImGui::Combo(_profiler.gRiotShield->szName.c_str(), &_profiler.gRiotShield->Current.iValue, acut::StringVectorToCharPointerArray(_profiler.gRiotShield->szItems), _profiler.gRiotShield->Domain.iMax))
			{
				Menu.bWriteLog = true;
			}
		} break;

		case cProfiler::MENU_TAB_TWEAKS:
		{
			if (ImGui::SliderInt(_profiler.gAimBone->szName.c_str(), &_profiler.gAimBone->Current.iValue, _profiler.gAimBone->Domain.iMin, _profiler.gAimBone->Domain.iMax, szBones[_profiler.gAimBone->Current.iValue].first.c_str()))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_profiler.gAimAngle->szName.c_str(), &_profiler.gAimAngle->Current.iValue, _profiler.gAimAngle->Domain.iMin, _profiler.gAimAngle->Domain.iMax, "%d degrees"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_profiler.gAimPower->szName.c_str(), &_profiler.gAimPower->Current.iValue, _profiler.gAimPower->Domain.iMin, _profiler.gAimPower->Domain.iMax, "%d percent"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_profiler.gAutoAimTime->szName.c_str(), &_profiler.gAutoAimTime->Current.iValue, _profiler.gAutoAimTime->Domain.iMin, _profiler.gAutoAimTime->Domain.iMax, "%d ms"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_profiler.gAutoAimDelay->szName.c_str(), &_profiler.gAutoAimDelay->Current.iValue, _profiler.gAutoAimDelay->Domain.iMin, _profiler.gAutoAimDelay->Domain.iMax, "%d ms"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_profiler.gAutoZoomDelay->szName.c_str(), &_profiler.gAutoZoomDelay->Current.iValue, _profiler.gAutoZoomDelay->Domain.iMin, _profiler.gAutoZoomDelay->Domain.iMax, "%d ms"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_profiler.gAutoFireDelay->szName.c_str(), &_profiler.gAutoFireDelay->Current.iValue, _profiler.gAutoFireDelay->Domain.iMin, _profiler.gAutoFireDelay->Domain.iMax, "%d ms"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderFloat(_profiler.gRecoilFactor->szName.c_str(), &_profiler.gRecoilFactor->Current.flValue, _profiler.gRecoilFactor->Domain.flMin, _profiler.gRecoilFactor->Domain.flMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderFloat(_profiler.gSpreadFactor->szName.c_str(), &_profiler.gSpreadFactor->Current.flValue, _profiler.gSpreadFactor->Domain.flMin, _profiler.gSpreadFactor->Domain.flMax))
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
				_profiler.gAimBone->Current.iValue = _profiler.gAimBone->Reset.iValue;
				_profiler.gAimAngle->Current.iValue = _profiler.gAimAngle->Reset.iValue;
				_profiler.gAimPower->Current.iValue = _profiler.gAimPower->Reset.iValue;
				_profiler.gAutoAimTime->Current.iValue = _profiler.gAutoAimTime->Reset.iValue;
				_profiler.gAutoAimDelay->Current.iValue = _profiler.gAutoAimDelay->Reset.iValue;
				_profiler.gAutoZoomDelay->Current.iValue = _profiler.gAutoZoomDelay->Reset.iValue;
				_profiler.gAutoFireDelay->Current.iValue = _profiler.gAutoFireDelay->Reset.iValue;
				_profiler.gRecoilFactor->Current.flValue = _profiler.gRecoilFactor->Reset.flValue;
				_profiler.gSpreadFactor->Current.flValue = _profiler.gSpreadFactor->Reset.flValue;

				FindVariable("cg_fov")->Current.flValue = FindVariable("cg_fov")->Reset.flValue;
				FindVariable("com_maxfps")->Current.iValue = FindVariable("com_maxfps")->Reset.iValue;

				Menu.bWriteLog = true;
			}
		} break;

		case cProfiler::MENU_TAB_STYLES:
		{
			_drawing.ColorPicker(_profiler.gColorAxis->szName, _profiler.gColorAxis->Current.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_profiler.gColorAxis->szName.c_str(), _profiler.gColorAxis->Current.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_profiler.gColorAllies->szName, _profiler.gColorAllies->Current.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_profiler.gColorAllies->szName.c_str(), _profiler.gColorAllies->Current.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_profiler.gColorAccents->szName, _profiler.gColorAccents->Current.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_profiler.gColorAccents->szName.c_str(), _profiler.gColorAccents->Current.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_profiler.gColorCrossHair->szName, _profiler.gColorCrossHair->Current.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_profiler.gColorCrossHair->szName.c_str(), _profiler.gColorCrossHair->Current.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_profiler.gColorText->szName, _profiler.gColorText->Current.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_profiler.gColorText->szName.c_str(), _profiler.gColorText->Current.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_profiler.gColorShadow->szName, _profiler.gColorShadow->Current.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_profiler.gColorShadow->szName.c_str(), _profiler.gColorShadow->Current.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Button("Reset to Default", ImVec2(446.0f, 35.0f)))
			{
				_profiler.gColorAxis->Current.cValue = _profiler.gColorAxis->Reset.cValue;
				_profiler.gColorAllies->Current.cValue = _profiler.gColorAllies->Reset.cValue;
				_profiler.gColorAccents->Current.cValue = _profiler.gColorAccents->Reset.cValue;
				_profiler.gColorCrossHair->Current.cValue = _profiler.gColorCrossHair->Reset.cValue;
				_profiler.gColorText->Current.cValue = _profiler.gColorText->Reset.cValue;
				_profiler.gColorShadow->Current.cValue = _profiler.gColorShadow->Reset.cValue;

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
		LPCSTR szSavePath = Menu.SaveDialog.saveFileDialog(bSaveButton, Menu.szProfilePath, NULL, ".xml", "Save Profile");

		if (strlen(szSavePath))
		{
			_profiler.SaveProfile(szSavePath);
			strcpy_s(Menu.szProfilePath, Menu.SaveDialog.getLastDirectory());
		}

		static bool bLoadButton;
		if ((bLoadButton = ImGui::Button("Load Profile", ImVec2(156.0f, 30.0f))))
		{
			Menu.bWriteLog = true;
		}
		LPCSTR szLoadPath = Menu.LoadDialog.chooseFileDialog(bLoadButton, Menu.szProfilePath, ".xml", "Load Profile");

		if (strlen(szLoadPath))
		{
			_profiler.LoadProfile(szLoadPath);
			strcpy_s(Menu.szProfilePath, Menu.LoadDialog.getLastDirectory());
		}

		ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
		ImGui::InputText("", Menu.szProfilePath, IM_ARRAYSIZE(Menu.szProfilePath), ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
		ImGui::End();
	}
}

//=====================================================================================