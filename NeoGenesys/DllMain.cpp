//=====================================================================================

#include "StdAfx.hpp"

using namespace NeoGenesys;

//=====================================================================================

#define HOOKCALL __fastcall

#define Hook(original, hook) (DetourTransactionBegin(), DetourUpdateThread(GetCurrentThread()), DetourAttach((LPVOID*)&original, (LPVOID)hook), DetourTransactionCommit())
#define UnHook(original, hook) (DetourTransactionBegin(), DetourUpdateThread(GetCurrentThread()), DetourDetach((LPVOID*)&original, (LPVOID)hook), DetourTransactionCommit())

//=====================================================================================

HRESULT WINAPI hPresent(_In_ IDXGISwapChain* pSwapChain, _In_ UINT SyncInterval, _In_ UINT Flags);
typedef HRESULT(WINAPI* tPresent)(_In_ IDXGISwapChain* pSwapChain, _In_ UINT SyncInterval, _In_ UINT Flags);
tPresent oPresent = *(tPresent*)dwPresent;

void HOOKCALL hRefresh(int localnum);
typedef void(HOOKCALL* tRefresh)(int localnum);
tRefresh oRefresh = (tRefresh)OFF_REFRESH;

void HOOKCALL hCreateNewCommands(int localnum);
typedef void(HOOKCALL* tCreateNewCommands)(int localnum);
tCreateNewCommands oCreateNewCommands = (tCreateNewCommands)OFF_CREATENEWCOMMANDS;

void HOOKCALL hWritePacket(int localnum);
typedef void(HOOKCALL* tWritePacket)(int localnum);
tWritePacket oWritePacket = (tWritePacket)OFF_WRITEPACKET;

void HOOKCALL hBulletFirePenetrate(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sGEntity* attacker, int servertime);
typedef void(HOOKCALL* tBulletFirePenetrate)(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sGEntity* attacker, int servertime);
tBulletFirePenetrate oBulletFirePenetrate = (tBulletFirePenetrate)OFF_BULLETFIREPENETRATE;

void HOOKCALL hObituary(int localnum, sEntityState* entitystate, int weapon);
typedef void(HOOKCALL* tObituary)(int localnum, sEntityState* entitystate, int weapon);
tObituary oObituary = (tObituary)OFF_OBITUARY;

void HOOKCALL hAddCmdDrawText(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, RGBA color, int flags);
typedef void(HOOKCALL* tAddCmdDrawText)(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, RGBA color, int flags);
tAddCmdDrawText oAddCmdDrawText = (tAddCmdDrawText)OFF_ADDCMDDRAWTEXT;

void HOOKCALL hClientFrame(sGEntity* entity);
typedef void(HOOKCALL* tClientFrame)(sGEntity* entity);
tClientFrame oClientFrame = (tClientFrame)OFF_CLIENTFRAME;

//=====================================================================================

HRESULT WINAPI hPresent(_In_ IDXGISwapChain* swapchain, _In_ UINT syncinterval, _In_ UINT flags)
{
	_mainGui.Present(swapchain, syncinterval, flags);

	return oPresent(swapchain, syncinterval, flags);
}

//=====================================================================================

void HOOKCALL hRefresh(int localnum)
{
	_hooks.Refresh(localnum);

	oRefresh(localnum);
}

//=====================================================================================

void HOOKCALL hCreateNewCommands(int localnum)
{
	oCreateNewCommands(localnum);

	_hooks.CreateNewCommands(localnum);
}

//=====================================================================================

void HOOKCALL hWritePacket(int localnum)
{
	_hooks.WritePacket(localnum);

	oWritePacket(localnum);
}

//=====================================================================================

void HOOKCALL hBulletFirePenetrate(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sGEntity* attacker, int servertime)
{
	_hooks.BulletFirePenetrate(seed, bp, br, weapon, alternate, attacker, servertime);

	oBulletFirePenetrate(seed, bp, br, weapon, alternate, attacker, servertime);
}

//=====================================================================================

void HOOKCALL hObituary(int localnum, sEntityState* entitystate, int weapon)
{
	_hooks.Obituary(localnum, entitystate, weapon);

	oObituary(localnum, entitystate, weapon);
}

//=====================================================================================

void HOOKCALL hAddCmdDrawText(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, RGBA color, int flags)
{
	_hooks.AddCmdDrawText(text, length, font, x, y, w, h, angle, color, flags);

	oAddCmdDrawText(text, length, font, x, y, w, h, angle, color, flags);
}

//=====================================================================================

void HOOKCALL hClientFrame(sGEntity* entity)
{
	_hooks.ClientFrame(entity);

	oClientFrame(entity);
}

//=====================================================================================

void Initialize()
{
	Hook(oPresent, hPresent);
	Hook(oRefresh, hRefresh);
	Hook(oCreateNewCommands, hCreateNewCommands);
	Hook(oWritePacket, hWritePacket);
	Hook(oBulletFirePenetrate, hBulletFirePenetrate);
	Hook(oObituary, hObituary);
	Hook(oAddCmdDrawText, hAddCmdDrawText);
	Hook(oClientFrame, hClientFrame);
}

//=====================================================================================

void Deallocate()
{
	UnHook(oPresent, hPresent);
	UnHook(oRefresh, hRefresh);
	UnHook(oCreateNewCommands, hCreateNewCommands);
	UnHook(oWritePacket, hWritePacket);
	UnHook(oBulletFirePenetrate, hBulletFirePenetrate);
	UnHook(oObituary, hObituary);
	UnHook(oAddCmdDrawText, hAddCmdDrawText);
	UnHook(oClientFrame, hClientFrame);

	_mainGui.pDevice->Release();
	_mainGui.pDeviceContext->Release();

	SetWindowLongPtr(_mainGui.hWindow, GWLP_WNDPROC, (LONG_PTR)_mainGui.oWindowProcess);

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

//=====================================================================================

BOOL APIENTRY DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
	DisableThreadLibraryCalls(hinstDLL);

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		Initialize();
		return TRUE;

	case DLL_PROCESS_DETACH:
		Deallocate();
		return TRUE;
	}

	return FALSE;
}

//=====================================================================================