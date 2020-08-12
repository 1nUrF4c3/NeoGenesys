//=====================================================================================

#pragma once
#include "../Engine/Engine.hpp"

//=====================================================================================

typedef int ControllerIndex_t;

enum netsrc_t
{
	NS_CLIENT1 = 0x0,
	NS_MAXCLIENTS = 0x1,
	NS_SERVER = 0x2,
	NS_PACKET = 0x3,
	NS_INVALID_NETSRC = 0x4,
};

struct msg_t
{
	int overflowed;
	int readOnly;
	char* data;
	char* splitData;
	int maxsize;
	int cursize;
	int splitSize;
	int readcount;
	int bit;
	int lastEntityRef;
	netsrc_t targetLocalNetID;
	int useZlib;
};

enum netadrtype_t
{
	NA_BOT = 0x0,
	NA_BAD = 0x1,
	NA_LOOPBACK = 0x2,
	NA_BROADCAST = 0x3,
	NA_IP = 0x4,
};

struct netadr_s
{
	netadrtype_t type;
	char ip[4];
	unsigned __int16 port;
	netsrc_t localNetID;
	unsigned int addrHandleIndex;
};

struct __declspec(align(8)) SessionStaticData
{
	const char* sessionName;
	bool registerUsersWithVoice;
};

struct bdSecurityID
{
	char ab[8];
};

struct XNADDR
{
	char addrBuff[37];
};

struct bdSecurityKey
{
	char ab[16];
};

struct XSESSION_INFO
{
	bdSecurityID sessionID;
	XNADDR hostAddress;
	bdSecurityKey keyExchangeKey;
};

struct __declspec(align(8)) RegisteredUser
{
	bool active;
	unsigned __int64 xuid;
	bool privateSlot;
};

struct __declspec(align(8)) ClientInfo
{
	bool registered;
	bool voiceRegistered;
	unsigned __int64 xuid;
	int natType;
	netadr_s addr;
	int usrVoiceConnectivityBits;
	int nextConnectivityTestTime[1];
	bool muted;
	bool privateSlot;
};

struct SessionDynamicData
{
	bool sessionHandle;
	XSESSION_INFO sessionInfo;
	bool keysGenerated;
	bool sessionStartCalled;
	unsigned __int64 sessionNonce;
	int privateSlots;
	int publicSlots;
	int flags;
	bool qosListenEnabled;
	ClientInfo users[18];
	int localVoiceConnectivityBits;
	int sessionCreateController;
	int sessionDeleteTime;
	bool allowJoining;
	RegisteredUser internalRegisteredUsers[18];
};

struct SessionData
{
	SessionStaticData staticData;
	SessionDynamicData dyn;
};

//=====================================================================================

namespace t6
{
	namespace offsets
	{
		constexpr auto g_partyData = OFF_PARTYDATA;
		constexpr auto g_lobbyData = OFF_LOBBYDATA;

		constexpr auto atoi_CL_HandleRelayPacket = OFF_ATOICLHANDLERELAYPACKET;

		constexpr auto MSG_Init = OFF_MSGINIT;
		constexpr auto MSG_WriteString = OFF_MSGWRITESTRING;
		constexpr auto NET_OutOfBandData = OFF_NETOUTOFBANDDATA;

		constexpr auto CL_SendPeerData = OFF_CLSENDPEERDATA;
		constexpr auto cls_state = OFF_CLSSTATE;
		constexpr auto MSG_WriteByte = OFF_MSGWRITEBYTE;
		constexpr auto CL_CanWeConnectToClient = OFF_CLCANWECONNECTTOCLIENT;
		constexpr auto Party_FindMemberByXUID = OFF_PARTYFINDMEMBERBYXUID;
		constexpr auto Live_GetXuid = OFF_LIVEGETXUID;
	}

	const static auto atoi_CL_HandleRelayPacket = reinterpret_cast<int(*)(const char* str)>(offsets::atoi_CL_HandleRelayPacket);

	const static auto MSG_Init = reinterpret_cast<void(*)(msg_t * buf, char* data, int length)>(offsets::MSG_Init);
	const static auto MSG_WriteString = reinterpret_cast<void(*)(msg_t * sb, const char* s)>(offsets::MSG_WriteString);
	const static auto NET_OutOfBandData = reinterpret_cast<bool(*)(netsrc_t sock, netadr_s adr, const char* format, int len)>(offsets::NET_OutOfBandData);

	const static auto CL_SendPeerData = reinterpret_cast<bool(*)(void* sessionData, int localClientNum, netsrc_t sock, int clientNum, msg_t * msg, int dataType)>(offsets::CL_SendPeerData);
	const static auto MSG_WriteByte = reinterpret_cast<void(*)(msg_t * msg, int c)>(offsets::MSG_WriteByte);
	const static auto CL_CanWeConnectToClient = reinterpret_cast<int(*)(void* sessionData, int ourClientNum, int theirClientNum)>(offsets::CL_CanWeConnectToClient);
	const static auto Party_FindMemberByXUID = reinterpret_cast<int(*)(SessionData * party, std::uint64_t player)>(offsets::Party_FindMemberByXUID);
	const static auto Live_GetXuid = reinterpret_cast<std::uint64_t(*)(ControllerIndex_t controllerIndex)>(offsets::Live_GetXuid);

	static auto g_partyData = reinterpret_cast<SessionData*>(offsets::g_partyData);
	static auto g_lobbyData = reinterpret_cast<SessionData*>(offsets::g_lobbyData);
}

//=====================================================================================

namespace furtive_crash
{
	int atoi_CL_HandleRelayPacket(const char* str);
	void init();
	void free();

	void send_crash(int client_num);
	bool send_connectivity_test(int client_num);
}

//=====================================================================================