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

	// �����غ��� ������ ���� ������ �ִ��� ó���ϰ� �ٽ� �����ǵ�
	// ���� ����� 64kb + �ִ� ��Ŷ ���� ��ŭ�� ��� ������ ���� ������ ����.
	PacketVector<65536>*	send_buffer;
	PacketVector<65536>*	recv_buffer;

	Packet*					inner_packet;

	OmokServer*				owner;
	size_t					last_error;
};

