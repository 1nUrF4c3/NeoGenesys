//=====================================================================================

#include "StdAfx.hpp"

using namespace NeoGenesys;

//=====================================================================================

#define HOOKCALL __fastcall

#define AttachHook(original, hook) (DetourTransactionBegin(), DetourUpdateThread(GetCurrentThread()), DetourAttach((LPVOID*)&original, (LPVOID)hook), DetourTransactionCommit())
#define DetachHook(original, hook) (DetourTransactionBegin(), DetourUpdateThread(GetCurrentThread()), DetourDetach((LPVOID*)&original, (LPVOID)hook), DetourTransactionCommit())

//=====================================================================================

HRESULT WINAPI hPresent(_In_ IDXGISwapChain* pSwapChain, _In_ UINT SyncInterval, _In_ UINT Flags);
typedef HRESULT(WINAPI* tPresent)(_In_ IDXGISwapChain* pSwapChain, _In_ UINT SyncInterval, _In_ UINT Flags);
tPresent oPresent;

void WINAPI hDrawIndexed(_In_ ID3D11DeviceContext* pContext, _In_ UINT IndexCount, _In_ UINT StartIndexLocation, _In_ INT BaseVertexLocation);
typedef void(WINAPI* tDrawIndexed)(_In_ ID3D11DeviceContext* pContext, _In_ UINT IndexCount, _In_ UINT StartIndexLocation, _In_ INT BaseVertexLocation);
tDrawIndexed oDrawIndexed;

void WINAPI hClearRenderTargetView(_In_ ID3D11DeviceContext* pContext, _In_ ID3D11RenderTargetView* pRenderTargetView, _In_ const FLOAT ColorRGBA[4]);
typedef void(WINAPI* tClearRenderTargetView)(_In_ ID3D11DeviceContext* pContext, _In_ ID3D11RenderTargetView* pRenderTargetView, _In_ const FLOAT ColorRGBA[4]);
tClearRenderTargetView oClearRenderTargetView;

void HOOKCALL hRefresh(int localnum);
typedef void(HOOKCALL* tRefresh)(int localnum);
tRefresh oRefresh = (tRefresh)OFF_REFRESH;

void HOOKCALL hWritePacket(int localnum);
typedef void(HOOKCALL* tWritePacket)(int localnum);
tWritePacket oWritePacket = (tWritePacket)OFF_WRITEPACKET;

void HOOKCALL hPredictPlayerState(int localnum);
typedef void(HOOKCALL* tPredictPlayerState)(int localnum);
tPredictPlayerState oPredictPlayerState = (tPredictPlayerState)OFF_PREDICTPLAYERSTATE;

void HOOKCALL hBulletFirePenetrate(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sCEntity* attacker, int servertime);
typedef void(HOOKCALL* tBulletFirePenetrate)(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sCEntity* attacker, int servertime);
tBulletFirePenetrate oBulletFirePenetrate = (tBulletFirePenetrate)OFF_BULLETFIREPENETRATE;

void HOOKCALL hBulletHitEvent(int localnum, int sourcenum, int targetnum, int weapon, bool alternate, ImVec3 start, ImVec3 position, ImVec3 normal, int surface, int _event, char param, int contents);
typedef void(HOOKCALL* tBulletHitEvent)(int localnum, int sourcenum, int targetnum, int weapon, bool alternate, ImVec3 start, ImVec3 position, ImVec3 normal, int surface, int _event, char param, int contents);
tBulletHitEvent oBulletHitEvent = (tBulletHitEvent)OFF_BULLETHITEVENT;

void HOOKCALL hCalcEntityLerpPositions(int localnum, sCEntity* entity);
typedef void(HOOKCALL* tCalcEntityLerpPositions)(int localnum, sCEntity* entity);
tCalcEntityLerpPositions oCalcEntityLerpPositions = (tCalcEntityLerpPositions)OFF_CALCENTITYLERPPOSITIONS;

void HOOKCALL hObituary(int localnum, sEntityState* entitystate, int weapon);
typedef void(HOOKCALL* tObituary)(int localnum, sEntityState* entitystate, int weapon);
tObituary oObituary = (tObituary)OFF_OBITUARY;

void HOOKCALL hAddCmdDrawText(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, ImVec4 color, int flags);
typedef void(HOOKCALL* tAddCmdDrawText)(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, ImVec4 color, int flags);
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

void WINAPI hDrawIndexed(_In_ ID3D11DeviceContext* pContext, _In_ UINT IndexCount, _In_ UINT StartIndexLocation, _In_ INT BaseVertexLocation)
{
	_mainGui.DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);

	oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
}

//=====================================================================================

void WINAPI hClearRenderTargetView(_In_ ID3D11DeviceContext* pContext, _In_ ID3D11RenderTargetView* pRenderTargetView, _In_ const FLOAT ColorRGBA[4])
{
	_mainGui.ClearRenderTargetView(pContext, pRenderTargetView, ColorRGBA);

	oClearRenderTargetView(pContext, pRenderTargetView, ColorRGBA);
}

//=====================================================================================

void HOOKCALL hRefresh(int localnum)
{
	_hooks.Refresh(localnum);

	oRefresh(localnum);
}

//=====================================================================================

void HOOKCALL hWritePacket(int localnum)
{
	_hooks.WritePacket(localnum);

	oWritePacket(localnum);
}

//=====================================================================================

void HOOKCALL hPredictPlayerState(int localnum)
{
	_hooks.PredictPlayerState(localnum);

	oPredictPlayerState(localnum);
}

//=====================================================================================

void HOOKCALL hBulletFirePenetrate(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sCEntity* attacker, int servertime)
{
	_hooks.BulletFirePenetrate(seed, bp, br, weapon, alternate, attacker, servertime);

	oBulletFirePenetrate(seed, bp, br, weapon, alternate, attacker, servertime);
}

//=====================================================================================

void HOOKCALL hBulletHitEvent(int localnum, int sourcenum, int targetnum, int weapon, bool alternate, ImVec3 start, ImVec3 position, ImVec3 normal, int surface, int _event, char param, int contents)
{
	_hooks.BulletHitEvent(localnum, sourcenum, targetnum, weapon, alternate, start, position, normal, surface, _event, param, contents);

	oBulletHitEvent(localnum, sourcenum, targetnum, weapon, alternate, start, position, normal, surface, _event, param, contents);
}

//=====================================================================================

void HOOKCALL hCalcEntityLerpPositions(int localnum, sCEntity* entity)
{
	oCalcEntityLerpPositions(localnum, entity);

	_hooks.CalcEntityLerpPositions(localnum, entity);
}

//=====================================================================================

void HOOKCALL hObituary(int localnum, sEntityState* entitystate, int weapon)
{
	_hooks.Obituary(localnum, entitystate, weapon);

	oObituary(localnum, entitystate, weapon);
}

//=====================================================================================

void HOOKCALL hAddCmdDrawText(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, ImVec4 color, int flags)
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
	_hooks.pVectoredExceptionHandler = AddVectoredExceptionHandler(TRUE, _hooks._thunkVectoredExceptionHandler.GetThunk());

	oPresent = (tPresent)SwapVMT(_mainGui._swapChain, &hPresent, 8);
	oDrawIndexed = (tDrawIndexed)SwapVMT(_mainGui._deviceContext, &hDrawIndexed, 12);
	oClearRenderTargetView = (tClearRenderTargetView)SwapVMT(_mainGui._deviceContext, &hClearRenderTargetView, 50);

	AttachHook(oRefresh, hRefresh);
	AttachHook(oWritePacket, hWritePacket);
	AttachHook(oPredictPlayerState, hPredictPlayerState);
	AttachHook(oBulletFirePenetrate, hBulletFirePenetrate);
	AttachHook(oBulletHitEvent, hBulletHitEvent);
	AttachHook(oCalcEntityLerpPositions, hCalcEntityLerpPositions);
	AttachHook(oObituary, hObituary);
	AttachHook(oAddCmdDrawText, hAddCmdDrawText);
	AttachHook(oClientFrame, hClientFrame);
}

//=====================================================================================

void Deallocate()
{
	RemoveVectoredExceptionHandler(_hooks.pVectoredExceptionHandler);

	SwapVMT(_mainGui._swapChain, oPresent, 8);
	SwapVMT(_mainGui._deviceContext, oDrawIndexed, 12);
	SwapVMT(_mainGui._deviceContext, oClearRenderTargetView, 50);

	DetachHook(oRefresh, hRefresh);
	DetachHook(oWritePacket, hWritePacket);
	DetachHook(oPredictPlayerState, hPredictPlayerState);
	DetachHook(oBulletFirePenetrate, hBulletFirePenetrate);
	DetachHook(oBulletHitEvent, hBulletHitEvent);
	DetachHook(oCalcEntityLerpPositions, hCalcEntityLerpPositions);
	DetachHook(oObituary, hObituary);
	DetachHook(oAddCmdDrawText, hAddCmdDrawText);
	DetachHook(oClientFrame, hClientFrame);

	_mainGui._device->Release();
	_mainGui._deviceContext->Release();

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

	SetWindowLongPtr(_mainGui.hWindow, GWLP_WNDPROC, (LONG_PTR)_mainGui.oWindowProcess);
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

	default:
		return FALSE;
	}
}

//=====================================================================================