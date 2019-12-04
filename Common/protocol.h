#pragma once

// pair - request response  
// notify - 단방향.
// SC : server to client
// CS : client to server
enum PakcetType
{
	PT_NONE,
	
	PT_SC_ASSIGN_STONE,
	 
	PT_SC_NOTIFY_KICK,			// 더이상 서버에 접속을못함을 	
	PT_CS_NOTIFY_KICK,			// 더이상 서버에 접속을못함을 알려줌 응답용.

	PT_SC_NOTIFY_SERVER_TIME,	/// 시간 알림 일방통보

// content logic packet
	PT_CS_DROP_STONE_REQUEST,   // 서버에게 보냄. 
	PT_SC_DROP_STONE_RESPONSE,  // 맞으면 모두에게 브로드 캐스팅 , 잘못된 위치에 돌을 놓는것은 응답을 안해줌.

////////////////////////////
	PT_SC_NOTIFY_GAME_RESULT,	// 승부
	
	PT_MAX, 
};

// common packetHeader 이것을 무조건 상속 받아서 사용하시오.
struct PacketHeader
{
	unsigned short length;
	unsigned short type;
};

