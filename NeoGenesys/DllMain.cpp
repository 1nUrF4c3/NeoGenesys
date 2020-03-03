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

void HOOKCALL hWritePacket(int localnum);
typedef void(HOOKCALL* tWritePacket)(int localnum);
tWritePacket oWritePacket = (tWritePacket)OFF_WRITEPACKET;

void HOOKCALL hPredictPlayerState(int localnum);
typedef void(HOOKCALL* tPredictPlayerState)(int localnum);
tPredictPlayerState oPredictPlayerState = (tPredictPlayerState)OFF_PREDICTPLAYERSTATE;

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

	return oRefresh(localnum);
}

//=====================================================================================

void HOOKCALL hWritePacket(int localnum)
{
	_hooks.WritePacket(localnum);

	return oWritePacket(localnum);
}

//=====================================================================================

void HOOKCALL hPredictPlayerState(int localnum)
{
	_hooks.PredictPlayerState(localnum);

	return oPredictPlayerState(localnum);
}

//=====================================================================================

void HOOKCALL hBulletFirePenetrate(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sGEntity* attacker, int servertime)
{
	_hooks.BulletFirePenetrate(seed, bp, br, weapon, alternate, attacker, servertime);

	return oBulletFirePenetrate(seed, bp, br, weapon, alternate, attacker, servertime);
}

//=====================================================================================

void HOOKCALL hObituary(int localnum, sEntityState* entitystate, int weapon)
{
	_hooks.Obituary(localnum, entitystate, weapon);

	return oObituary(localnum, entitystate, weapon);
}

//=====================================================================================

void HOOKCALL hAddCmdDrawText(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, RGBA color, int flags)
{
	_hooks.AddCmdDrawText(text, length, font, x, y, w, h, angle, color, flags);

	return oAddCmdDrawText(text, length, font, x, y, w, h, angle, color, flags);
}

//=====================================================================================

void HOOKCALL hClientFrame(sGEntity* entity)
{
	_hooks.ClientFrame(entity);

	return oClientFrame(entity);
}

//=====================================================================================

void Initialize()
{
	Hook(oPresent, hPresent);
	Hook(oRefresh, hRefresh);
	Hook(oWritePacket, hWritePacket);
	Hook(oPredictPlayerState, hPredictPlayerState);
	Hook(oObituary, hObituary);
	Hook(oAddCmdDrawText, hAddCmdDrawText);
	Hook(oClientFrame, hClientFrame);
}

//=====================================================================================

void Deallocate()
{
	UnHook(oPresent, hPresent);
	UnHook(oRefresh, hRefresh);
	UnHook(oWritePacket, hWritePacket);
	UnHook(oPredictPlayerState, hPredictPlayerState);
	UnHook(oObituary, hObituary);
	UnHook(oAddCmdDrawText, hAddCmdDrawText);
	UnHook(oClientFrame, hClientFrame);

	_mainGui.pDevice->Release();
	_mainGui.pDeviceContext->Release();

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

	SetWindowLongPtr(_mainGui.hWindow, GWLP_WNDPROC, (LONG_PTR)_mainGui.oWindowProcess);
}

//=====================================================================================

BOOL APIENTRY DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		Initialize();
		return TRUE;

	case DLL_PROCESS_DETACH:
		Deallocate();
		return TRUE;

	default:
		return FALSE;
	}
}

//=====================================================================================