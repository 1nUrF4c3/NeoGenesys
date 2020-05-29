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

		QWORD dwSysValue;
		LPVOID pVectoredExceptionHandler;

		void Refresh(int localnum);
		void WritePacket(int localnum);
		void PredictPlayerState(int localnum);
		void BulletFirePenetrate(int* seed, sBulletFireParams* bp, sBulletTraceResults* br, int weapon, bool alternate, sCEntity* attacker, int servertime);
		void BulletHitEvent(int localnum, int sourcenum, int targetnum, int weapon, bool alternate, ImVec3 start, ImVec3 position, ImVec3 normal, int surface, int _event, char param, int contents);
		void CalcEntityLerpPositions(int localnum, sCEntity* entity);
		void Obituary(int localnum, sEntityState* entitystate, int weapon);
		void AddCmdDrawText(LPSTR text, int length, LPVOID font, float x, float y, float w, float h, float angle, ImVec4 color, int flags);
		void ClientFrame(sGEntity* entity);

		LONG VectoredExceptionHandler(_In_ LPEXCEPTION_POINTERS ExceptionInfo);
		typedef LONG(CALLBACK* tVectoredExceptionHandler)(_In_ LPEXCEPTION_POINTERS ExceptionInfo);

		StdThunk<tVectoredExceptionHandler, cHooks> _thunkVectoredExceptionHandler;
	} extern _hooks;
}

//=====================================================================================