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

		_profiler.gMenuColor->Custom.iValue = GetPrivateProfileInt("MenuStyle", "COLOR", cProfiler::MENU_COLOR_NEUTRAL, (acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_CFG, " ", "")).c_str());
		_profiler.gMenuCursor->Custom.iValue = GetPrivateProfileInt("MenuStyle", "CURSOR", cProfiler::MENU_CURSOR_BLACK, (acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_CFG, " ", "")).c_str());
		_profiler.gMenuFont->Custom.iValue = GetPrivateProfileInt("MenuStyle", "FONT", cProfiler::MENU_FONT_LIGHT, (acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_CFG, " ", "")).c_str());

		RefreshInterface(_profiler.gMenuColor->Custom.iValue, _profiler.gMenuCursor->Custom.iValue, _profiler.gMenuFont->Custom.iValue);

		bInitialized = true;
	}
	/*
	//=====================================================================================
	*/
	void cMainGUI::SetMenuColor(int index)
	{
		switch (index)
		{
		case cProfiler::MENU_COLOR_NEUTRAL:
			ImGui::StyleColorsNeutral();
			break;

		case cProfiler::MENU_COLOR_NEUTRAL_NEON:
			ImGui::StyleColorsNeutralNeon();
			break;

		case cProfiler::MENU_COLOR_RED:
			ImGui::StyleColorsRed();
			break;

		case cProfiler::MENU_COLOR_RED_NEON:
			ImGui::StyleColorsRedNeon();
			break;

		case cProfiler::MENU_COLOR_ORANGE:
			ImGui::StyleColorsOrange();
			break;

		case cProfiler::MENU_COLOR_ORANGE_NEON:
			ImGui::StyleColorsOrangeNeon();
			break;

		case cProfiler::MENU_COLOR_YELLOW:
			ImGui::StyleColorsYellow();
			break;

		case cProfiler::MENU_COLOR_YELLOW_NEON:
			ImGui::StyleColorsYellowNeon();
			break;

		case cProfiler::MENU_COLOR_GREEN:
			ImGui::StyleColorsGreen();
			break;

		case cProfiler::MENU_COLOR_GREEN_NEON:
			ImGui::StyleColorsGreenNeon();
			break;

		case cProfiler::MENU_COLOR_BLUE:
			ImGui::StyleColorsBlue();
			break;

		case cProfiler::MENU_COLOR_BLUE_NEON:
			ImGui::StyleColorsBlueNeon();
			break;

		case cProfiler::MENU_COLOR_PURPLE:
			ImGui::StyleColorsPurple();
			break;

		case cProfiler::MENU_COLOR_PURPLE_NEON:
			ImGui::StyleColorsPurpleNeon();
			break;

		case cProfiler::MENU_COLOR_RAINBOW:
			ImGui::StyleColorsRed();
			break;

		case cProfiler::MENU_COLOR_RAINBOW_NEON:
			ImGui::StyleColorsRedNeon();
			break;

		default:
			ImGui::StyleColorsNeutral();
			break;
		}
	}
	/*
	//=====================================================================================
	*/
	void cMainGUI::SetMenuCursor(int index)
	{
		switch (index)
		{
		case cProfiler::MENU_CURSOR_BLACK:
			ImGui::StyleCursorsBlack();
			break;

		case cProfiler::MENU_CURSOR_WHITE:
			ImGui::StyleCursorsWhite();
			break;

		default:
			ImGui::StyleCursorsBlack();
			break;
		}
	}
	/*
	//=====================================================================================
	*/
	void cMainGUI::SetMenuFont(int index)
	{
		switch (index)
		{
		case cProfiler::MENU_FONT_LIGHT:
			ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->Fonts[cProfiler::MENU_FONT_LIGHT];
			break;

		case cProfiler::MENU_FONT_MEDIUM:
			ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->Fonts[cProfiler::MENU_FONT_MEDIUM];
			break;

		case cProfiler::MENU_FONT_BOLD:
			ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->Fonts[cProfiler::MENU_FONT_BOLD];
			break;

		default:
			ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->Fonts[cProfiler::MENU_FONT_LIGHT];
			break;
		}
	}
	/*
	//=====================================================================================
	*/
	void cMainGUI::RefreshInterface(int color, int cursor, int font)
	{
		SetMenuColor(color);

		ImGui::GetIO().Fonts->Clear();

		SetMenuCursor(cursor);

		ImGui::GetIO().Fonts->AddFontLight();
		ImGui::GetIO().Fonts->AddFontMedium();
		ImGui::GetIO().Fonts->AddFontBold();

		Eurostile_Bold = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(eurostile_bold_compressed_data_base85, flEurostile_Bold = Window->iHeight / 80.0f);
		Eurostile_Extended = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(eurostile_extended_compressed_data_base85, flEurostile_Extended = Window->iHeight / 60.0f);
		Eurostile_Regular = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(eurostile_regular_compressed_data_base85, flEurostile_Regular = Window->iHeight / 80.0f);

		ImGui_ImplDX11_CreateDeviceObjects();

		SetMenuFont(font);
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
				std::thread(&cHost::StartMatch, &_host).detach();

		if (GetKeyPress(VK_HOME, false))
			_profiler.LoadProfile("");

		if (GetKeyPress(VK_END, false))
			_profiler.DisableAll();

		if (GetKeyPress(VK_PRIOR, false) && *(int*)OFF_ISCURRENTHOST)
			VectorCopy(PlayerState[CG->PlayerState.iClientNum].vOrigin, Menu.HostMenu.vTeleport);

		if (GetKeyPress(VK_NEXT, false) && *(int*)OFF_ISCURRENTHOST)
			VectorCopy(Menu.HostMenu.vTeleport, PlayerState[CG->PlayerState.iClientNum].vOrigin);

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
			if (_profiler.gMenuColor->Custom.iValue == cProfiler::MENU_COLOR_RAINBOW || _profiler.gMenuColor->Custom.iValue == cProfiler::MENU_COLOR_RAINBOW_NEON)
			{
				static float flHue = 0.0f;

				if (flHue > 255.0f)
					flHue = 0.0f;

				for (int i = 0; i < ImGuiCol_COUNT; i++)
				{
					Vector3 vHSV;

					ImGui::ColorConvertRGBtoHSV(ImGui::GetStyle().Colors[i].x, ImGui::GetStyle().Colors[i].y, ImGui::GetStyle().Colors[i].z, vHSV[0], vHSV[1], vHSV[2]);
					ImGui::ColorConvertHSVtoRGB(ByteToFloat(flHue), vHSV[1], vHSV[2], ImGui::GetStyle().Colors[i].x, ImGui::GetStyle().Colors[i].y, ImGui::GetStyle().Colors[i].z);
				}

				for (int i = 0; i < ImGui::TabLabelStyle::Col_TabLabel_Count; i++)
				{
					Vector3 vHSV;
					ImVec4 vColor = ImGui::ColorConvertU32ToFloat4(ImGui::TabLabelStyle::Get().colors[i]);

					ImGui::ColorConvertRGBtoHSV(vColor.x, vColor.y, vColor.z, vHSV[0], vHSV[1], vHSV[2]);
					ImGui::ColorConvertHSVtoRGB(ByteToFloat(flHue), vHSV[1], vHSV[2], vColor.x, vColor.y, vColor.z);

					ImGui::TabLabelStyle::Get().colors[i] = ImGui::ColorConvertFloat4ToU32(vColor);
				}

				flHue += (60.0f / ImGui::GetIO().Framerate);
			}

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

			if (LocalClientIsInGame() && CG->PlayerState.iOtherFlags & 0x4000)
			{
				_drawing.DrawESP();
				_drawing.DrawCompass();
				_drawing.DrawRadar();
				_drawing.DrawCrosshair();
			}

			std::string szWatermark(VariadicText("%s for Call of Duty: Ghosts | Frametime: %s, Ping: %s", acut::ToUpper(PROGRAM_NAME).c_str(),
				LocalClientIsInGame() ? VariadicText("%i ms", *(int*)OFF_FRAMETIME).c_str() : "N/A",
				LocalClientIsInGame() ? VariadicText("%i ms", *(int*)OFF_PING).c_str() : "N/A"));

			ImVec2 vWatermark(Eurostile_Extended->CalcTextSizeA(flEurostile_Extended, FLT_MAX, 0.0f, szWatermark.c_str()));

			ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
				ImVec2(vWatermark.y + 1.0f, flEurostile_Extended - vWatermark.y + 1.0f),
				0xFF000000, szWatermark.c_str());

			ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
				ImVec2(vWatermark.y, flEurostile_Extended - vWatermark.y),
				0xFFFFFFFF, szWatermark.c_str());

			if (LocalClientIsInGame())
			{
				std::string szConnection(*(int*)OFF_ISCURRENTHOST ? "Hosting" : "Not Hosting");
				ImVec2 vConnection(Eurostile_Extended->CalcTextSizeA(flEurostile_Extended, FLT_MAX, 0.0f, szConnection.c_str()));

				ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
					ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - vConnection.x / 2.0f + 1.0f, flEurostile_Extended - vConnection.y + 1.0f),
					0xFF000000, szConnection.c_str());

				ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
					ImVec2(ImGui::GetIO().DisplaySize.x / 2.0f - vConnection.x / 2.0f, flEurostile_Extended - vConnection.y),
					*(int*)OFF_ISCURRENTHOST ? 0xFF00FF00 : 0xFF0000FF, szConnection.c_str());
			}

			std::string szFramesPerSecond(VariadicText("%i", (int)ImGui::GetIO().Framerate));
			ImVec2 vFramesPerSecond(Eurostile_Extended->CalcTextSizeA(flEurostile_Extended, FLT_MAX, 0.0f, szFramesPerSecond.c_str()));

			ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
				ImVec2(ImGui::GetIO().DisplaySize.x - vFramesPerSecond.x - vFramesPerSecond.y + 1.0f, flEurostile_Extended - vFramesPerSecond.y + 1.0f),
				0xFF000000, szFramesPerSecond.c_str());

			ImGui::GetWindowDrawList()->AddText(Eurostile_Extended, flEurostile_Extended,
				ImVec2(ImGui::GetIO().DisplaySize.x - vFramesPerSecond.x - vFramesPerSecond.y, flEurostile_Extended - vFramesPerSecond.y),
				0xFF00FFFF, szFramesPerSecond.c_str());
			
			ImGui::End();

			if (Menu.bShowWindow)
			{
				if (Menu.bWriteLog)
				{
					ImGui::LogToFile();
					Menu.bWriteLog = false;
				}

				ImGui::SetNextWindowSize(ImVec2(490.0f, 324.0f));
				ImGui::Begin(acut::ToUpper(PROGRAM_NAME).c_str(), &Menu.bShowWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
				ImGui::SetColorEditOptions(ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
 
				if (ImGui::TabLabels(_profiler.gMenuTabs->MaxValue.iMax, acut::StringVectorToCharPointerArray(_profiler.gMenuTabs->szItems), _profiler.gMenuTabs->Custom.iValue, NULL, false, NULL, NULL, false, false, NULL, NULL, &ImVec2(94.0f, 25.0f)))
				{
					Menu.bWriteLog = true;
				}

				ImGui::BeginChild("ContentRegion", ImVec2(0.0f, 204.0f), true);
				ImGui::Separator();
				ImGui::NewLine();

				switch (_profiler.gMenuTabs->Custom.iValue)
				{
				case cProfiler::MENU_TAB_AIMBOT:
				{
					if (ImGui::RadioButton(_profiler.gAimBotMode->szItems[cProfiler::AIMBOT_MODE_OFF].c_str(), &_profiler.gAimBotMode->Custom.iValue, cProfiler::AIMBOT_MODE_OFF))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(148.0f);

					if (ImGui::RadioButton(_profiler.gAimBotMode->szItems[cProfiler::AIMBOT_MODE_MANUAL].c_str(), &_profiler.gAimBotMode->Custom.iValue, cProfiler::AIMBOT_MODE_MANUAL))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::RadioButton(_profiler.gAimBotMode->szItems[cProfiler::AIMBOT_MODE_AUTO].c_str(), &_profiler.gAimBotMode->Custom.iValue, cProfiler::AIMBOT_MODE_AUTO))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gAutoZoom->szLabel.c_str(), &_profiler.gAutoZoom->Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gAutoFire->szLabel.c_str(), &_profiler.gAutoFire->Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gAutoWall->szLabel.c_str(), &_profiler.gAutoWall->Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gSilentAim->szLabel.c_str(), &_profiler.gSilentAim->Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gTargetAgents->szLabel.c_str(), &_profiler.gTargetAgents->Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gTargetMissiles->szLabel.c_str(), &_profiler.gTargetMissiles->Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Combo(_profiler.gAntiAim->szLabel.c_str(), &_profiler.gAntiAim->Custom.iValue, acut::StringVectorToCharPointerArray(_profiler.gAntiAim->szItems), _profiler.gAntiAim->MaxValue.iMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Combo(_profiler.gBoneScan->szLabel.c_str(), &_profiler.gBoneScan->Custom.iValue, acut::StringVectorToCharPointerArray(_profiler.gBoneScan->szItems), _profiler.gBoneScan->MaxValue.iMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Combo(_profiler.gSortMethod->szLabel.c_str(), &_profiler.gSortMethod->Custom.iValue, acut::StringVectorToCharPointerArray(_profiler.gSortMethod->szItems), _profiler.gSortMethod->MaxValue.iMax))
					{
						Menu.bWriteLog = true;
					}
				} break;

				case cProfiler::MENU_TAB_WALLHACK:
				{
					if (ImGui::RadioButton(_profiler.gWallHackMode->szItems[cProfiler::WALLHACK_MODE_AXIS].c_str(), &_profiler.gWallHackMode->Custom.iValue, cProfiler::WALLHACK_MODE_AXIS))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(148.0f);

					if (ImGui::RadioButton(_profiler.gWallHackMode->szItems[cProfiler::WALLHACK_MODE_ALLIES].c_str(), &_profiler.gWallHackMode->Custom.iValue, cProfiler::WALLHACK_MODE_ALLIES))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::RadioButton(_profiler.gWallHackMode->szItems[cProfiler::WALLHACK_MODE_ALL].c_str(), &_profiler.gWallHackMode->Custom.iValue, cProfiler::WALLHACK_MODE_ALL))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Combo(_profiler.gPlayerBoxes->szLabel.c_str(), &_profiler.gPlayerBoxes->Custom.iValue, acut::StringVectorToCharPointerArray(_profiler.gPlayerBoxes->szItems), _profiler.gPlayerBoxes->MaxValue.iMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Combo(_profiler.gPlayerBones->szLabel.c_str(), &_profiler.gPlayerBones->Custom.iValue, acut::StringVectorToCharPointerArray(_profiler.gPlayerBones->szItems), _profiler.gPlayerBones->MaxValue.iMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Combo(_profiler.gPlayerSnapLines->szLabel.c_str(), &_profiler.gPlayerSnapLines->Custom.iValue, acut::StringVectorToCharPointerArray(_profiler.gPlayerSnapLines->szItems), _profiler.gPlayerSnapLines->MaxValue.iMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gPlayerDistances->szLabel.c_str(), &_profiler.gPlayerDistances->Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gPlayerNames->szLabel.c_str(), &_profiler.gPlayerNames->Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gPlayerWeapons->szLabel.c_str(), &_profiler.gPlayerWeapons->Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gAgents->szLabel.c_str(), &_profiler.gAgents->Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gMissiles->szLabel.c_str(), &_profiler.gMissiles->Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gItems->szLabel.c_str(), &_profiler.gItems->Custom.bValue))
					{
						Menu.bWriteLog = true;
					}
				} break;

				case cProfiler::MENU_TAB_MISCELLANEOUS:
				{
					if (ImGui::Checkbox(_profiler.gKillSpam->szLabel.c_str(), &_profiler.gKillSpam->Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gNameStealer->szLabel.c_str(), &_profiler.gNameStealer->Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gPlayerCrossHair->szLabel.c_str(), &_profiler.gPlayerCrossHair->Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox(_profiler.gPlayerCompass->szLabel.c_str(), &_profiler.gPlayerCompass->Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox(_profiler.gPlayerRadar->szLabel.c_str(), &_profiler.gPlayerRadar->Custom.bValue))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox("Console", &Menu.bShowConsole))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::Checkbox("Player List", &Menu.PlayerList.bShowWindow))
					{
						Menu.bWriteLog = true;
					} ImGui::SameLine(296.0f);

					if (ImGui::Checkbox("Host Menu", &Menu.HostMenu.bShowWindow))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Combo(_profiler.gRiotShield->szLabel.c_str(), &_profiler.gRiotShield->Custom.iValue, acut::StringVectorToCharPointerArray(_profiler.gRiotShield->szItems), _profiler.gRiotShield->MaxValue.iMax))
					{
						Menu.bWriteLog = true;
					}
				} break;

				case cProfiler::MENU_TAB_TWEAKS:
				{
					if (ImGui::SliderInt(_profiler.gAimBone->szLabel.c_str(), &_profiler.gAimBone->Custom.iValue, _profiler.gAimBone->MinValue.iMin, _profiler.gAimBone->MaxValue.iMax, vBones[_profiler.gAimBone->Custom.iValue].first.c_str()))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderFloat(_profiler.gAimAngle->szLabel.c_str(), &_profiler.gAimAngle->Custom.flValue, _profiler.gAimAngle->MinValue.flMin, _profiler.gAimAngle->MaxValue.flMax, "%.0f degrees"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAimPower->szLabel.c_str(), &_profiler.gAimPower->Custom.iValue, _profiler.gAimPower->MinValue.iMin, _profiler.gAimPower->MaxValue.iMax, "%d%%"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAutoAimTime->szLabel.c_str(), &_profiler.gAutoAimTime->Custom.iValue, _profiler.gAutoAimTime->MinValue.iMin, _profiler.gAutoAimTime->MaxValue.iMax, "%d ms"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAutoAimDelay->szLabel.c_str(), &_profiler.gAutoAimDelay->Custom.iValue, _profiler.gAutoAimDelay->MinValue.iMin, _profiler.gAutoAimDelay->MaxValue.iMax, "%d ms"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAutoZoomDelay->szLabel.c_str(), &_profiler.gAutoZoomDelay->Custom.iValue, _profiler.gAutoZoomDelay->MinValue.iMin, _profiler.gAutoZoomDelay->MaxValue.iMax, "%d ms"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt(_profiler.gAutoFireDelay->szLabel.c_str(), &_profiler.gAutoFireDelay->Custom.iValue, _profiler.gAutoFireDelay->MinValue.iMin, _profiler.gAutoFireDelay->MaxValue.iMax, "%d ms"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderFloat(_profiler.gRecoilFactor->szLabel.c_str(), &_profiler.gRecoilFactor->Custom.flValue, _profiler.gRecoilFactor->MinValue.flMin, _profiler.gRecoilFactor->MaxValue.flMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderFloat(_profiler.gSpreadFactor->szLabel.c_str(), &_profiler.gSpreadFactor->Custom.flValue, _profiler.gSpreadFactor->MinValue.flMin, _profiler.gSpreadFactor->MaxValue.flMax))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderFloat("Field of View", &FindVariable("cg_fov")->Current.flValue, 65.0f, 120.0f, "%.0f fov"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::SliderInt("Frames per Second", &FindVariable("com_maxfps")->Current.iValue, 0, 240, "%d fps"))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Button("Reset to Default", ImVec2(446.0f, 35.0f)))
					{
						_profiler.gAimBone->Custom.iValue = _profiler.gAimBone->Default.iValue;
						_profiler.gAimAngle->Custom.flValue = _profiler.gAimAngle->Default.flValue;
						_profiler.gAimPower->Custom.iValue = _profiler.gAimPower->Default.iValue;
						_profiler.gAutoAimTime->Custom.iValue = _profiler.gAutoAimTime->Default.iValue;
						_profiler.gAutoAimDelay->Custom.iValue = _profiler.gAutoAimDelay->Default.iValue;
						_profiler.gAutoZoomDelay->Custom.iValue = _profiler.gAutoZoomDelay->Default.iValue;
						_profiler.gAutoFireDelay->Custom.iValue = _profiler.gAutoFireDelay->Default.iValue;
						_profiler.gRecoilFactor->Custom.flValue = _profiler.gRecoilFactor->Default.flValue;
						_profiler.gSpreadFactor->Custom.flValue = _profiler.gSpreadFactor->Default.flValue;

						FindVariable("cg_fov")->Current.flValue = FindVariable("cg_fov")->Reset.flValue;
						FindVariable("com_maxfps")->Current.iValue = FindVariable("com_maxfps")->Reset.iValue;

						Menu.bWriteLog = true;
					}
				} break;

				case cProfiler::MENU_TAB_STYLES:
				{
					if (ImGui::ColorEdit3(_profiler.gColorAxis->szLabel.c_str(), _profiler.gColorAxis->Custom.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::ColorEdit3(_profiler.gColorAllies->szLabel.c_str(), _profiler.gColorAllies->Custom.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::ColorEdit3(_profiler.gColorRiotShield->szLabel.c_str(), _profiler.gColorRiotShield->Custom.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::ColorEdit3(_profiler.gColorCrossHair->szLabel.c_str(), _profiler.gColorCrossHair->Custom.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::ColorEdit3(_profiler.gColorText->szLabel.c_str(), _profiler.gColorText->Custom.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine();

					if (ImGui::ColorEdit3(_profiler.gColorShadow->szLabel.c_str(), _profiler.gColorShadow->Custom.cValue))
					{
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();
					
					if (ImGui::Combo(_profiler.gMenuColor->szLabel.c_str(), &_profiler.gMenuColor->Custom.iValue, acut::StringVectorToCharPointerArray(_profiler.gMenuColor->szItems), _profiler.gMenuColor->MaxValue.iMax))
					{
						WritePrivateProfileString("MenuStyle", "COLOR", std::to_string(_profiler.gMenuColor->Custom.iValue).c_str(), (acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_CFG, " ", "")).c_str());

						Menu.bStyleChanged = true;
						Menu.bWriteLog = true;
					}

					if (ImGui::Combo(_profiler.gMenuCursor->szLabel.c_str(), &_profiler.gMenuCursor->Custom.iValue, acut::StringVectorToCharPointerArray(_profiler.gMenuCursor->szItems), _profiler.gMenuCursor->MaxValue.iMax))
					{
						WritePrivateProfileString("MenuStyle", "CURSOR", std::to_string(_profiler.gMenuCursor->Custom.iValue).c_str(), (acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_CFG, " ", "")).c_str());

						Menu.bStyleChanged = true;
						Menu.bWriteLog = true;
					}

					if (ImGui::Combo(_profiler.gMenuFont->szLabel.c_str(), &_profiler.gMenuFont->Custom.iValue, acut::StringVectorToCharPointerArray(_profiler.gMenuFont->szItems), _profiler.gMenuFont->MaxValue.iMax))
					{
						WritePrivateProfileString("MenuStyle", "FONT", std::to_string(_profiler.gMenuFont->Custom.iValue).c_str(), (acut::GetExeDirectory() + acut::FindAndReplaceString(DEFAULT_CFG, " ", "")).c_str());

						Menu.bStyleChanged = true;
						Menu.bWriteLog = true;
					} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

					if (ImGui::Button("Reset to Default", ImVec2(446.0f, 35.0f)))
					{
						_profiler.gColorAxis->Custom.cValue = _profiler.gColorAxis->Default.cValue;
						_profiler.gColorAllies->Custom.cValue = _profiler.gColorAllies->Default.cValue;
						_profiler.gColorRiotShield->Custom.cValue = _profiler.gColorRiotShield->Default.cValue;
						_profiler.gColorCrossHair->Custom.cValue = _profiler.gColorCrossHair->Default.cValue;
						_profiler.gColorText->Custom.cValue = _profiler.gColorText->Default.cValue;
						_profiler.gColorShadow->Custom.cValue = _profiler.gColorShadow->Default.cValue;

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
				LPCSTR szSavePath = Menu.SaveDialog.saveFileDialog(bSaveButton, Menu.SaveDialog.getLastDirectory(), NULL, ".xml", "Save Profile");

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
				LPCSTR szLoadPath = Menu.LoadDialog.chooseFileDialog(bLoadButton, Menu.LoadDialog.getLastDirectory(), ".xml", "Load Profile");

				if (strlen(szLoadPath))
				{
					_profiler.LoadProfile(szLoadPath);
					strcpy_s(Menu.szProfilePath, szLoadPath);
				}

				ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
				ImGui::InputText("", Menu.szProfilePath, sizeof(Menu.szProfilePath), ImGuiInputTextFlags_ReadOnly);
				ImGui::PopItemWidth();
				ImGui::End();

				if (Menu.bShowConsole)
				{
					_console.Init();
					_console.Draw("CONSOLE", &Menu.bShowConsole);
				}
			}

			if (Menu.bShowWindow && Menu.PlayerList.bShowWindow && LocalClientIsInGame())
			{
				if (Menu.PlayerList.bWriteLog)
				{
					ImGui::LogToFile();
					Menu.PlayerList.bWriteLog = false;
				}

				ImGui::SetNextWindowSize(ImVec2(400.0f, 480.0f));
				ImGui::Begin("PLAYER LIST", &Menu.PlayerList.bShowWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
				ImGui::BeginChild("ContentRegion", ImVec2(0.0f, 0.0f), true);

				for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
				{
					if (CharacterInfo[i].iInfoValid)
					{
						GetPlayerAddr(&Menu.PlayerList.NetAddr[i], GetCurrentSession(), ClientInfo[i].iClientNum);

						ImGui::PushID(i);

						if (ImGui::RadioButton("", &_targetList.iRiotShieldTarget, i))
						{
							Menu.PlayerList.bWriteLog = true;
						} ImGui::PopID(); ImGui::SameLine();

						ImGui::PushID(i + FindVariable("sv_maxclients")->Current.iValue);
						
						if (ImGui::Checkbox("", (bool*)&_targetList.vIsTarget[i]))
						{
							Menu.PlayerList.bWriteLog = true;
						} ImGui::PopID(); ImGui::SameLine();

						if (*(int*)OFF_ISCURRENTHOST)
						{
							ImGui::PushID(i + FindVariable("sv_maxclients")->Current.iValue * 2);

							if (ImGui::Button("Crash", ImVec2(50.0f, 0.0f)))
							{
								Say(&GEntity[CG->PlayerState.iClientNum], &GEntity[i], 0, "\x5E\x01\x3D\x3D\xFF");
								Menu.PlayerList.bWriteLog = true;
							} ImGui::PopID(); ImGui::SameLine();
						}

						ImGui::PushItemWidth(150.0f);
						ImGui::InputText(VariadicText("%i: %s", ClientInfo[i].iClientNum, ClientInfo[i].szName).c_str(),
							(LPSTR)VariadicText("%u.%u.%u.%u",
								(BYTE)Menu.PlayerList.NetAddr[i].szIP[0],
								(BYTE)Menu.PlayerList.NetAddr[i].szIP[1],
								(BYTE)Menu.PlayerList.NetAddr[i].szIP[2],
								(BYTE)Menu.PlayerList.NetAddr[i].szIP[3]).c_str(),
							1024, ImGuiInputTextFlags_ReadOnly);
						ImGui::PopItemWidth();
					}
				}

				ImGui::EndChild();
				ImGui::End();
			}

			if (Menu.bShowWindow && Menu.HostMenu.bShowWindow && LocalClientIsInGame() && *(int*)OFF_ISCURRENTHOST && !*(bool*)OFF_ISALIENSMODE)
			{
				if (Menu.HostMenu.bWriteLog)
				{
					ImGui::LogToFile();
					Menu.HostMenu.bWriteLog = false;
				}

				ImGui::SetNextWindowSize(ImVec2(484.0f, 616.0f));
				ImGui::Begin("HOST MENU", &Menu.HostMenu.bShowWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

				for (int i = 0; i < FindVariable("sv_maxclients")->Current.iValue; i++)
					if (CharacterInfo[i].iInfoValid)
						Menu.HostMenu.vPlayers.push_back(std::make_pair(ClientInfo[i].iClientNum, ClientInfo[i].szName));

				ImGui::PushItemWidth(ImGui::GetWindowContentRegionWidth());
				if (ImGui::BeginCombo("", ClientInfo[Menu.HostMenu.iPlayer].szName))
				{
					Menu.HostMenu.GetPlayerSelection();
					ImGui::EndCombo();

					Menu.HostMenu.bWriteLog = true;
				} ImGui::PopItemWidth();

				ImGui::BeginChild("ContentRegion", ImVec2(0.0f, 0.0f), true);
				ImGui::Separator();
				ImGui::NewLine();

				if (ImGui::Checkbox("God Mode", &Menu.HostMenu.PlayerMod[Menu.HostMenu.iPlayer].bGodMode))
				{
					Menu.HostMenu.bWriteLog = true;
				} ImGui::SameLine(292.0f);
				
				if (ImGui::Checkbox("No Clip", &Menu.HostMenu.PlayerMod[Menu.HostMenu.iPlayer].bNoClip))
				{
					Menu.HostMenu.bWriteLog = true;
				} ImGui::NewLine();
				
				if (ImGui::Checkbox("Infinite Ammo", &Menu.HostMenu.PlayerMod[Menu.HostMenu.iPlayer].bInfiniteAmmo))
				{
					Menu.HostMenu.bWriteLog = true;
				} ImGui::SameLine(292.0f);

				if (ImGui::Checkbox("Explosive Bullets", &Menu.HostMenu.PlayerMod[Menu.HostMenu.iPlayer].bExplosiveBullets))
				{
					Menu.HostMenu.bWriteLog = true;
				} ImGui::NewLine();
				
				if (ImGui::Checkbox("Super Speed", &Menu.HostMenu.PlayerMod[Menu.HostMenu.iPlayer].bSuperSpeed))
				{
					Menu.HostMenu.bWriteLog = true;
				} ImGui::SameLine(292.0f);
				
				if (ImGui::Checkbox("Freeze Position", &Menu.HostMenu.PlayerMod[Menu.HostMenu.iPlayer].bFreezePosition))
				{
					Menu.HostMenu.bWriteLog = true;
				} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

				if (ImGui::InputText("Name", PlayerState[Menu.HostMenu.iPlayer].ClientState.szName, 16))
				{
					Menu.HostMenu.bWriteLog = true;
				}

				if (ImGui::Combo("Team", (int*)&PlayerState[Menu.HostMenu.iPlayer].ClientState.iTeam, Menu.HostMenu.szTeam, TEAM_MAX))
				{
					TeamChanged(ClientInfo[Menu.HostMenu.iPlayer].iClientNum);
					Menu.HostMenu.bWriteLog = true;
				}

				if (ImGui::InputInt("Health", &GEntity[Menu.HostMenu.iPlayer].iHealth))
				{
					Menu.HostMenu.bWriteLog = true;
				} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

				for (auto& WeaponID : vWeaponIDs)
				{
					char szComplete[1024] = { NULL }, szDisplay[1024] = { NULL };

					GetWeaponNameComplete(WeaponID, false, szComplete, 1024);
					GetWeaponDisplayName(WeaponID, false, szDisplay, 1024);

					Menu.HostMenu.vWeaponCompleteNames.push_back(_strdup(acut::StripColorCodes(szComplete).c_str()));
					Menu.HostMenu.vWeaponDisplayNames.push_back(_strdup(acut::StripColorCodes(szDisplay).c_str()));
				}

				if (ImGui::Combo("Weapon", &Menu.HostMenu.iWeaponID, Menu.HostMenu.vWeaponDisplayNames.data(), (int)vWeaponIDs.size()))
				{
					Menu.HostMenu.bWriteLog = true;
				} Menu.HostMenu.vWeaponCompleteNames.clear(); Menu.HostMenu.vWeaponDisplayNames.clear();

				if (ImGui::Button("Give Weapon", ImVec2(150.0f, 25.0f)))
				{
					TakePlayerWeapon(&PlayerState[Menu.HostMenu.iPlayer], GetViewmodelWeapon(&PlayerState[Menu.HostMenu.iPlayer]));
					GivePlayerWeapon(&PlayerState[Menu.HostMenu.iPlayer], vWeaponIDs[Menu.HostMenu.iWeaponID], false, false, true);
					GameSendServerCommand(Menu.HostMenu.iPlayer, SV_CMD_RELIABLE, VariadicText("a %i", vWeaponIDs[Menu.HostMenu.iWeaponID]));
					AddAmmo(&PlayerState[Menu.HostMenu.iPlayer], vWeaponIDs[Menu.HostMenu.iWeaponID], false, 255, true);

					Menu.HostMenu.bWriteLog = true;
				} ImGui::SameLine(0.0f, 4.0f);

				if (ImGui::Button("Take Weapon", ImVec2(150.0f, 25.0f)))
				{
					TakePlayerWeapon(&PlayerState[Menu.HostMenu.iPlayer], GetViewmodelWeapon(&PlayerState[Menu.HostMenu.iPlayer]));
					GameSendServerCommand(Menu.HostMenu.iPlayer, SV_CMD_RELIABLE, VariadicText("a %i", 0));

					Menu.HostMenu.bWriteLog = true;
				} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

				if (ImGui::SliderFloat3("Position", PlayerState[Menu.HostMenu.iPlayer].vOrigin, -8192.0f, 8192.0f, "%.0f"))
				{
					Menu.HostMenu.bWriteLog = true;
				}

				if (ImGui::Button("Teleport To", ImVec2(150.0f, 25.0f)))
				{
					VectorCopy(PlayerState[Menu.HostMenu.iPlayer].vOrigin, PlayerState[CG->PlayerState.iClientNum].vOrigin);
					Menu.HostMenu.bWriteLog = true;
				} ImGui::SameLine(0.0f, 4.0f);

				if (ImGui::Button("Teleport From", ImVec2(150.0f, 25.0f)))
				{
					VectorCopy(PlayerState[CG->PlayerState.iClientNum].vOrigin, PlayerState[Menu.HostMenu.iPlayer].vOrigin);
					Menu.HostMenu.bWriteLog = true;
				} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

				if (ImGui::Button("Kick", ImVec2(304.0f, 25.0f)))
				{
					KickClient(ClientInfo[Menu.HostMenu.iPlayer].iClientNum, Menu.HostMenu.szKickReason);
					Menu.HostMenu.bWriteLog = true;
				}

				if (ImGui::InputText("Reason", Menu.HostMenu.szKickReason, 64))
				{
					Menu.HostMenu.bWriteLog = true;
				} ImGui::NewLine(); ImGui::Separator(); ImGui::EndChild();

				Menu.HostMenu.vPlayers.clear();

				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			
			if (Menu.bStyleChanged)
			{
				RefreshInterface(_profiler.gMenuColor->Custom.iValue, _profiler.gMenuCursor->Custom.iValue, _profiler.gMenuFont->Custom.iValue);
				Menu.bStyleChanged = false;
			}

			if (hWindow != *(HWND*)OFF_WINDOWHANDLE)
			{
				pDevice->Release();
				pDeviceContext->Release();

				ImGui_ImplWin32_Shutdown();
				ImGui_ImplDX11_Shutdown();
				ImGui::DestroyContext();

				bInitialized = false;
			}

			static int iTime = clock();

			if (clock() - iTime > max(*(int*)OFF_FRAMETIME, *(int*)OFF_PING))
			{
				if (LocalClientIsInGame() && !IsMigrating() && CG->PlayerState.iOtherFlags & 0x4000)
				{
					if (_profiler.gNameSpam->Custom.bValue)
					{
						std::string szUserName = acut::RandomANString(0);

						strncpy_s((LPSTR)(*(DWORD_PTR*)OFF_STEAMAPI + OFF_STEAMNAME), strlen(szUserName.c_str()) + 1, szUserName.c_str(), 32);
						Cbuf_AddText(VariadicText("name \"%s\"\n", szUserName.c_str()));
					}

					if (_profiler.gChatSpam->Custom.bValue)
					{
						Cbuf_AddText(VariadicText("say \"%s\"\n", _profiler.gChatSpamMessage->Custom.szValue));
					}
				}

				iTime = clock();
			}
		}
	}
}

//=====================================================================================