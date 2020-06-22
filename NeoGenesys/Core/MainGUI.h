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

		typedef enum
		{
			MENU_TAB_AIMBOT,
			MENU_TAB_WALLHACK,
			MENU_TAB_MISCELLANEOUS,
			MENU_TAB_TWEAKS,
			MENU_TAB_STYLES,
			MENU_TAB_MAX
		} eMenuTab;

		std::shared_ptr<sCvar> gMenuTabs = std::make_shared<sCvar>("Menu Tab", std::vector<std::string>({ "Aimbot", "ESP", "Misc.", "Tweaks", "Styles" }), MENU_TAB_AIMBOT, MENU_TAB_AIMBOT, MENU_TAB_MAX);
		std::shared_ptr<sCvar> gNameSpam = std::make_shared<sCvar>("Namespam", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gChatSpam = std::make_shared<sCvar>("Chatspam", std::vector<std::string>(), false);
		std::shared_ptr<sCvar> gChatSpamMessage = std::make_shared<sCvar>("Chatspam Message", std::vector<std::string>(), VariadicText(""));

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
		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;

		void InitInterface();
		void DrawMainGUI();
		bool GetKeyPress(int vkey, bool immediate);

		void WINAPI Present(_In_ IDXGISwapChain* pSwapChain, _In_ UINT SyncInterval, _In_ UINT Flags);
		void WINAPI DrawIndexed(_In_ ID3D11DeviceContext* pContext, _In_ UINT IndexCount, _In_ UINT StartIndexLocation, _In_ INT BaseVertexLocation);
		void WINAPI ClearRenderTargetView(_In_ ID3D11DeviceContext* pContext, _In_ ID3D11RenderTargetView* pRenderTargetView, _In_ const FLOAT ColorRGBA[4]);

		LRESULT CALLBACK WindowProcess(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
		typedef LRESULT(CALLBACK* tWindowProcess)(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
		tWindowProcess oWindowProcess;

		StdThunk<tWindowProcess, cMainGUI> _thunkWindowProcess;
	} extern _mainGui;
}

//=====================================================================================
