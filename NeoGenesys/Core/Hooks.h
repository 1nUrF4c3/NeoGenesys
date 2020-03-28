//=====================================================================================

#pragma once

#include "Engine/Engine.hpp"

//=====================================================================================

namespace NeoGenesys
{
	class cHooks
	{
	public:

		cHooks() : _thunkVectoredExceptionHandler(&cHooks::VectoredExceptionHandler, this) {}

		QWORD dwThreadLocalStoragePointer;
		LPVOID pVectoredExceptionHandler;

		void Refresh(int localnum);
		void WritePacket(int localnum);
		void CreateNewCommands(int localnum);
		void BulletFirePenetrate(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sGEntity* attacker, int servertime);
		void CalcEntityLerpPositions(int localnum, sCEntity* entity);
		void Obituary(int localnum, sEntityState* entitystate, int weapon);
		void AddCmdDrawText(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, RGBA color, int flags);
		void ClientFrame(sGEntity* entity);

		LONG VectoredExceptionHandler(_In_ LPEXCEPTION_POINTERS ExceptionInfo);
		typedef LONG(CALLBACK* tVectoredExceptionHandler)(_In_ LPEXCEPTION_POINTERS ExceptionInfo);

		cWin32Thunk<tVectoredExceptionHandler, cHooks> _thunkVectoredExceptionHandler;
	} extern _hooks;
}

//=====================================================================================