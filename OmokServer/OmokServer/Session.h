#pragma once

#include "stdafx.h"
#include "OmokServer.h"

class Session 
{
public:
	Session();
	~Session();

	size_t pre_send(const Packet* out_packet);

	/*void disconnect();*/

	// recv packet handling....
	void parse_packet(); //
	void process_packet(PacketHeader* header); //
	void process_packet(PacketHeader* header, Packet* packet); //

	void makePacketGameResult(OmokStatus winner, Packet* packet);

	SOCKETINFO*				sock_info;
	// state
	size_t					state;
	SOCKADDR_IN				sock_addr;

	// 생각해보니 어차피 버퍼 내용을 최대한 처리하고 다시 받을건데
	// 버퍼 사이즈를 64kb + 최대 패킷 길이 만큼만 잡아 놓으면 문제 없을거 같다.
	PacketVector<65536>*	send_buffer;
	PacketVector<65536>*	recv_buffer;

	Packet*					inner_packet;

	OmokServer*				owner;
	size_t					last_error;
};

