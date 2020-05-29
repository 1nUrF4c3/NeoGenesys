//=====================================================================================

#pragma once

#include "Engine/Engine.hpp"

//=====================================================================================

#define MAX_VIRTUALKEYS 0x100

#define DEFAULT_CFG "\\NeoGenesys.cfg"
#define DEFAULT_INI "\\NeoGenesys.ini"
#define DEFAULT_LOG "\\NeoGenesys.log"
#define DEFAULT_XML "\\NeoGenesys.xml"

//=====================================================================================

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=====================================================================================

namespace NeoGenesys
{
	class cMainGUI
	{
	public:

		cMainGUI() : _thunkWindowProcess(&cMainGUI::WindowProcess, this) {}

		bool bInitialized = false;

		ImFont* Bank_Gothic_Pro_Light, * Eurostile_Extended;
		float flBank_Gothic_Pro_Light, flEurostile_Extended;

		struct sMenu
		{
			char szProfilePath[1024] = { NULL };
			bool bShowWindow = true, bShowConsole = false, bShowMemoryEditor = false, bWriteLog = false;

			std::string szIniFileName, szLogFileName;

			ImGuiFs::Dialog SaveDialog, LoadDialog;
			MemoryEditor MemEdit;
		} Menu;

		struct sVirtualKeys
		{
			bool bKey, bDown, bUp;
		} VirtualKeys[MAX_VIRTUALKEYS];

		HWND hWindow;
		ID3D11Device* pDevice;
		ID3D11DeviceContext* pDeviceContext;

		void InitInterface();
		bool GetKeyPress(int vkey, bool immediate);
		void DrawMainGUI();

		void WINAPI Present(_In_ IDXGISwapChain* pSwapChain, _In_ UINT SyncInterval, _In_ UINT Flags);
		LRESULT CALLBACK WindowProcess(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
		typedef LRESULT(CALLBACK* tWindowProcess)(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
		tWindowProcess oWindowProcess;

		StdThunk<tWindowProcess, cMainGUI> _thunkWindowProcess;
	} extern _mainGui;
}

//=====================================================================================
