//=====================================================================================

#include "../StdAfx.hpp"

//=====================================================================================

namespace NeoGenesys
{
	cMainGUI _mainGui;

	void cMainGUI::InitInterface()
	{
		hWindow = *(HWND*)OFF_WINDOWHANDLE;

		if (!hWindow || !_device || !_deviceContext)
			return;

		oWindowProcess = (tWindowProcess)SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)_thunkWindowProcess.GetThunk());

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(hWindow);
		ImGui_ImplDX11_Init(_device, _deviceContext);

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

		if (ImGui::TabLabels(gMenuTabs->Domain.iMax, acut::StringVectorToCharPointerArray(gMenuTabs->szItems), gMenuTabs->Current.iValue, NULL, false, NULL, NULL, false, false, NULL, NULL, &ImVec2(94.0f, 25.0f)))
		{
			Menu.bWriteLog = true;
		}

		ImGui::BeginChild("ContentRegion", ImVec2(0.0f, 214.0f), true);
		ImGui::Separator();
		ImGui::NewLine();

		switch (gMenuTabs->Current.iValue)
		{
		case MENU_TAB_AIMBOT:
		{
			if (ImGui::RadioButton(_aimBot.gAimBotMode->szItems[cAimbot::AIMBOT_MODE_OFF].c_str(), &_aimBot.gAimBotMode->Current.iValue, cAimbot::AIMBOT_MODE_OFF))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(148.0f);

			if (ImGui::RadioButton(_aimBot.gAimBotMode->szItems[cAimbot::AIMBOT_MODE_MANUAL].c_str(), &_aimBot.gAimBotMode->Current.iValue, cAimbot::AIMBOT_MODE_MANUAL))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::RadioButton(_aimBot.gAimBotMode->szItems[cAimbot::AIMBOT_MODE_AUTO].c_str(), &_aimBot.gAimBotMode->Current.iValue, cAimbot::AIMBOT_MODE_AUTO))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Checkbox(_aimBot.gAutoZoom->szName.c_str(), &_aimBot.gAutoZoom->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_aimBot.gAutoFire->szName.c_str(), &_aimBot.gAutoFire->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_targetList.gAutoWall->szName.c_str(), &_targetList.gAutoWall->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_aimBot.gSilentAim->szName.c_str(), &_aimBot.gSilentAim->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_targetList.gTargetAgents->szName.c_str(), &_targetList.gTargetAgents->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_targetList.gTargetMissiles->szName.c_str(), &_targetList.gTargetMissiles->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Combo(_antiAim.gAntiAim->szName.c_str(), &_antiAim.gAntiAim->Current.iValue, acut::StringVectorToCharPointerArray(_antiAim.gAntiAim->szItems), _antiAim.gAntiAim->Domain.iMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Combo(_targetList.gBoneScan->szName.c_str(), &_targetList.gBoneScan->Current.iValue, acut::StringVectorToCharPointerArray(_targetList.gBoneScan->szItems), _targetList.gBoneScan->Domain.iMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Combo(_targetList.gSortMethod->szName.c_str(), &_targetList.gSortMethod->Current.iValue, acut::StringVectorToCharPointerArray(_targetList.gSortMethod->szItems), _targetList.gSortMethod->Domain.iMax))
			{
				Menu.bWriteLog = true;
			}
		} break;

		case MENU_TAB_WALLHACK:
		{
			if (ImGui::RadioButton(_drawing.gWallHackMode->szItems[cDrawing::WALLHACK_MODE_AXIS].c_str(), &_drawing.gWallHackMode->Current.iValue, cDrawing::WALLHACK_MODE_AXIS))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(148.0f);

			if (ImGui::RadioButton(_drawing.gWallHackMode->szItems[cDrawing::WALLHACK_MODE_ALLIES].c_str(), &_drawing.gWallHackMode->Current.iValue, cDrawing::WALLHACK_MODE_ALLIES))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::RadioButton(_drawing.gWallHackMode->szItems[cDrawing::WALLHACK_MODE_ALL].c_str(), &_drawing.gWallHackMode->Current.iValue, cDrawing::WALLHACK_MODE_ALL))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Combo(_drawing.gPlayerBoxes->szName.c_str(), &_drawing.gPlayerBoxes->Current.iValue, acut::StringVectorToCharPointerArray(_drawing.gPlayerBoxes->szItems), _drawing.gPlayerBoxes->Domain.iMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Combo(_drawing.gPlayerBones->szName.c_str(), &_drawing.gPlayerBones->Current.iValue, acut::StringVectorToCharPointerArray(_drawing.gPlayerBones->szItems), _drawing.gPlayerBones->Domain.iMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Combo(_drawing.gPlayerSnapLines->szName.c_str(), &_drawing.gPlayerSnapLines->Current.iValue, acut::StringVectorToCharPointerArray(_drawing.gPlayerSnapLines->szItems), _drawing.gPlayerSnapLines->Domain.iMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Checkbox(_drawing.gPlayerInformation->szName.c_str(), &_drawing.gPlayerInformation->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_drawing.gPlayerWeapons->szName.c_str(), &_drawing.gPlayerWeapons->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_hooks.gPlayerBulletTracers->szName.c_str(), &_hooks.gPlayerBulletTracers->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_drawing.gAgents->szName.c_str(), &_drawing.gAgents->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_drawing.gMissiles->szName.c_str(), &_drawing.gMissiles->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_drawing.gItems->szName.c_str(), &_drawing.gItems->Current.bValue))
			{
				Menu.bWriteLog = true;
			}
		} break;

		case MENU_TAB_MISCELLANEOUS:
		{
			if (ImGui::Checkbox(_hooks.gThirdPerson->szName.c_str(), &_hooks.gThirdPerson->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_packets.gLaserAttachment->szName.c_str(), &_packets.gLaserAttachment->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_hooks.gNameStealer->szName.c_str(), &_hooks.gNameStealer->Current.bValue))
			{
				Menu.bWriteLog = true;
			}  ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_drawing.gPlayerCrossHair->szName.c_str(), &_drawing.gPlayerCrossHair->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_drawing.gPlayerCompass->szName.c_str(), &_drawing.gPlayerCompass->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_drawing.gPlayerRadar->szName.c_str(), &_drawing.gPlayerRadar->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(gConsole->szName.c_str(), &gConsole->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(_playerList.gPlayerList->szName.c_str(), &_playerList.gPlayerList->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::Checkbox(_hostMenu.gHostMenu->szName.c_str(), &_hostMenu.gHostMenu->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::SameLine(296.0f);

			if (ImGui::Checkbox(gMemoryEditor->szName.c_str(), &gMemoryEditor->Current.bValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Combo(_targetList.gRiotShielders->szName.c_str(), &_targetList.gRiotShielders->Current.iValue, acut::StringVectorToCharPointerArray(_targetList.gRiotShielders->szItems), _targetList.gRiotShielders->Domain.iMax))
			{
				Menu.bWriteLog = true;
			}
		} break;

		case MENU_TAB_TWEAKS:
		{
			if (ImGui::SliderInt(_targetList.gAimBone->szName.c_str(), &_targetList.gAimBone->Current.iValue, _targetList.gAimBone->Domain.iMin, _targetList.gAimBone->Domain.iMax, vBones[_targetList.gAimBone->Current.iValue].second.first.c_str()))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_targetList.gAimAngle->szName.c_str(), &_targetList.gAimAngle->Current.iValue, _targetList.gAimAngle->Domain.iMin, _targetList.gAimAngle->Domain.iMax, "%d degrees"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_aimBot.gAimPower->szName.c_str(), &_aimBot.gAimPower->Current.iValue, _aimBot.gAimPower->Domain.iMin, _aimBot.gAimPower->Domain.iMax, "%d percent"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_aimBot.gAutoAimTime->szName.c_str(), &_aimBot.gAutoAimTime->Current.iValue, _aimBot.gAutoAimTime->Domain.iMin, _aimBot.gAutoAimTime->Domain.iMax, "%d ms"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_aimBot.gAutoAimDelay->szName.c_str(), &_aimBot.gAutoAimDelay->Current.iValue, _aimBot.gAutoAimDelay->Domain.iMin, _aimBot.gAutoAimDelay->Domain.iMax, "%d ms"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_aimBot.gAutoZoomDelay->szName.c_str(), &_aimBot.gAutoZoomDelay->Current.iValue, _aimBot.gAutoZoomDelay->Domain.iMin, _aimBot.gAutoZoomDelay->Domain.iMax, "%d ms"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderInt(_aimBot.gAutoFireDelay->szName.c_str(), &_aimBot.gAutoFireDelay->Current.iValue, _aimBot.gAutoFireDelay->Domain.iMin, _aimBot.gAutoFireDelay->Domain.iMax, "%d ms"))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderFloat(_removals.gRecoilFactor->szName.c_str(), &_removals.gRecoilFactor->Current.flValue, _removals.gRecoilFactor->Domain.flMin, _removals.gRecoilFactor->Domain.flMax))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			if (ImGui::SliderFloat(_removals.gSpreadFactor->szName.c_str(), &_removals.gSpreadFactor->Current.flValue, _removals.gSpreadFactor->Domain.flMin, _removals.gSpreadFactor->Domain.flMax))
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
				_targetList.gAimBone->Current.iValue = _targetList.gAimBone->Reset.iValue;
				_targetList.gAimAngle->Current.iValue = _targetList.gAimAngle->Reset.iValue;
				_aimBot.gAimPower->Current.iValue = _aimBot.gAimPower->Reset.iValue;
				_aimBot.gAutoAimTime->Current.iValue = _aimBot.gAutoAimTime->Reset.iValue;
				_aimBot.gAutoAimDelay->Current.iValue = _aimBot.gAutoAimDelay->Reset.iValue;
				_aimBot.gAutoZoomDelay->Current.iValue = _aimBot.gAutoZoomDelay->Reset.iValue;
				_aimBot.gAutoFireDelay->Current.iValue = _aimBot.gAutoFireDelay->Reset.iValue;
				_removals.gRecoilFactor->Current.flValue = _removals.gRecoilFactor->Reset.flValue;
				_removals.gSpreadFactor->Current.flValue = _removals.gSpreadFactor->Reset.flValue;

				FindVariable("cg_fov")->Current.flValue = FindVariable("cg_fov")->Reset.flValue;
				FindVariable("com_maxfps")->Current.iValue = FindVariable("com_maxfps")->Reset.iValue;

				Menu.bWriteLog = true;
			}
		} break;

		case MENU_TAB_STYLES:
		{
			_drawing.ColorPicker(_drawing.gColorAxisVisible->szName, _drawing.gColorAxisVisible->Current.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_drawing.gColorAxisVisible->szName.c_str(), _drawing.gColorAxisVisible->Current.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_drawing.gColorAxisInvisible->szName, _drawing.gColorAxisInvisible->Current.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_drawing.gColorAxisInvisible->szName.c_str(), _drawing.gColorAxisInvisible->Current.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_drawing.gColorAlliesVisible->szName, _drawing.gColorAlliesVisible->Current.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_drawing.gColorAlliesVisible->szName.c_str(), _drawing.gColorAlliesVisible->Current.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_drawing.gColorAlliesInvisible->szName, _drawing.gColorAlliesInvisible->Current.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_drawing.gColorAlliesInvisible->szName.c_str(), _drawing.gColorAlliesInvisible->Current.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_drawing.gColorAccents->szName, _drawing.gColorAccents->Current.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_drawing.gColorAccents->szName.c_str(), _drawing.gColorAccents->Current.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_drawing.gColorCrossHair->szName, _drawing.gColorCrossHair->Current.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_drawing.gColorCrossHair->szName.c_str(), _drawing.gColorCrossHair->Current.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_drawing.gColorText->szName, _drawing.gColorText->Current.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_drawing.gColorText->szName.c_str(), _drawing.gColorText->Current.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine();

			_drawing.ColorPicker(_drawing.gColorShadow->szName, _drawing.gColorShadow->Current.cValue);
			ImGui::SameLine(0.0f, 4.0f);

			if (ImGui::ColorEdit4(_drawing.gColorShadow->szName.c_str(), _drawing.gColorShadow->Current.cValue))
			{
				Menu.bWriteLog = true;
			} ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Button("Reset to Default", ImVec2(446.0f, 35.0f)))
			{
				_drawing.gColorAxisVisible->Current.cValue = _drawing.gColorAxisVisible->Reset.cValue;
				_drawing.gColorAxisInvisible->Current.cValue = _drawing.gColorAxisInvisible->Reset.cValue;
				_drawing.gColorAlliesVisible->Current.cValue = _drawing.gColorAlliesVisible->Reset.cValue;
				_drawing.gColorAlliesInvisible->Current.cValue = _drawing.gColorAlliesInvisible->Reset.cValue;
				_drawing.gColorAccents->Current.cValue = _drawing.gColorAccents->Reset.cValue;
				_drawing.gColorCrossHair->Current.cValue = _drawing.gColorCrossHair->Reset.cValue;
				_drawing.gColorText->Current.cValue = _drawing.gColorText->Reset.cValue;
				_drawing.gColorShadow->Current.cValue = _drawing.gColorShadow->Reset.cValue;

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
		LPCSTR szSavePath = Menu.SaveDialog.saveFileDialog(bSaveButton, acut::GetParent(Menu.szProfilePath).c_str(), NULL, ".xml", "SAVE PROFILE");

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
		LPCSTR szLoadPath = Menu.LoadDialog.chooseFileDialog(bLoadButton, acut::GetParent(Menu.szProfilePath).c_str(), ".xml", "LOAD PROFILE");

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

		case WM_MOUSEWHEEL:
			_hostMenu.HostMenu.flGravityGunDist += (float)GET_WHEEL_DELTA_WPARAM(wParam);
			break;
		}

		if (GetKeyPress(VK_INSERT, false))
			Menu.bShowWindow = !Menu.bShowWindow;

		if (LocalClientIsInGame())
			_packets.bIsAirStuck = GetKeyPress(VK_DELETE, true);

		else if (GetKeyPress(VK_DELETE, false))
			std::thread(&cHostMenu::StartMatch, &_hostMenu).detach();

		if (GetKeyPress(VK_HOME, false))
			_profiler.LoadProfile("");

		if (GetKeyPress(VK_END, false))
			_profiler.DisableAll();

		if (GetKeyPress(VK_PRIOR, false) && IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum))
			_hostMenu.HostMenu.vTeleport = PlayerState[CG->PredictedPlayerState.iClientNum].vOrigin;

		if (GetKeyPress(VK_NEXT, false) && IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum))
		{
			PlayerState[CG->PredictedPlayerState.iClientNum].vOrigin = _hostMenu.HostMenu.vTeleport;
			PlayerState[CG->PredictedPlayerState.iClientNum].vVelocity *= 0.0f;
			PlayerState[CG->PredictedPlayerState.iClientNum].iGravity = 0;
		}

		if (GetKeyPress(VK_NUMPAD5, false) && IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum))
			_hostMenu.TeleportAll();

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
			pSwapChain->GetDevice(__uuidof(_device), (void**)&_device);
			_device->GetImmediateContext(&_deviceContext);
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

				if (gMemoryEditor->Current.bValue)
					Menu.MemEdit.DrawMemoryEditor(&gMemoryEditor->Current.bValue, hIw6mp64_ship.lpBaseOfDll, hIw6mp64_ship.SizeOfImage, (size_t)hIw6mp64_ship.lpBaseOfDll);

				if (gConsole->Current.bValue)
					_console.DrawConsole(&gConsole->Current.bValue);

				if (_playerList.gPlayerList->Current.bValue && LocalClientIsInGame())
					_playerList.DrawPlayerList();

				if (_hostMenu.gHostMenu->Current.bValue && LocalClientIsInGame() && IsSessionHost(GetCurrentSession(), CG->PredictedPlayerState.iClientNum) && !*(bool*)OFF_ISALIENSMODE)
					_hostMenu.DrawHostMenu();
			}

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			if (hWindow != *(HWND*)OFF_WINDOWHANDLE)
			{
				_device->Release();
				_deviceContext->Release();

				ImGui_ImplWin32_Shutdown();
				ImGui_ImplDX11_Shutdown();
				ImGui::DestroyContext();

				bInitialized = false;
			}

			if (SpamTimer.Ready())
			{
				if (LocalClientIsInGame() && !IsMigrating() && CG->PredictedPlayerState.iOtherFlags & 0x4000)
				{
					if (gNameSpam->Current.bValue)
					{
						std::string szUserName = acut::RandomANString(0);

						strncpy_s((LPSTR)FindDmaAddy(OFF_STEAMAPI, std::vector<DWORD_PTR>({ OFF_STEAMNAME })), strlen(szUserName.c_str()) + 1, szUserName.c_str(), 32);
						Cbuf_AddText(VariadicText("name \"%s\"\n", szUserName.c_str()));
					}

					if (gChatSpam->Current.bValue)
					{
						Cbuf_AddText(VariadicText("say \"%s\"\n", gChatSpamMessage->Current.szValue));
					}
				}

				SpamTimer.Wait(max(*(int*)OFF_FRAMETIME, *(int*)OFF_PING));
			}
		}
	}
	/*
	//=====================================================================================
	*/
	void WINAPI cMainGUI::DrawIndexed(_In_ ID3D11DeviceContext* pContext, _In_ UINT IndexCount, _In_ UINT StartIndexLocation, _In_ INT BaseVertexLocation)
	{
		return;
	}
	/*
	//=====================================================================================
	*/
	void WINAPI cMainGUI::ClearRenderTargetView(_In_ ID3D11DeviceContext* pContext, _In_ ID3D11RenderTargetView* pRenderTargetView, _In_ const FLOAT ColorRGBA[4])
	{
		return;
	}
}

//=====================================================================================