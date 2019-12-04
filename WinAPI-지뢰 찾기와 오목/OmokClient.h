#pragma once

#include "OmokScene.h"
class OmokScene;
class OmokClient
{
public:
	OmokClient();
	~OmokClient();

	bool init();
	void update();
	void fin();

	// ���� �ϼ��� ��Ŷ�� �ִ��� �˻��ϰ� 
	// �ִ� ��Ŷ ���� ��ŭ�� ���۸� ����� ��.
	size_t recv_packet();
	size_t send_packet();
	size_t pre_send(const Packet* out_packet);

	void disconnect();
	void connect();
	void requset_to_drop_stone(int x, int y, OmokStatus type);
	
	
	void set_game(OmokScene* game);
private:
	// recv packet handling....
	void parse_packet(); //
	void process_packet(PacketHeader* header); //
	void process_packet(PacketHeader* header, Packet* packet); //
	void apply_sockopt();


	// state
	size_t					state;
	size_t					tick;
	SOCKADDR_IN				sock_addr;
	SOCKET					sock;

	// �����غ��� ������ ���� ������ �ִ��� ó���ϰ� �ٽ� �����ǵ�
	// ���� ����� 64kb + �ִ� ��Ŷ ���� ��ŭ�� ��� ������ ���� ������ ����.
	PacketVector<65536>*	send_buffer;
	PacketVector<65536>*	recv_buffer;

	Packet*					inner_packet;

	size_t					last_error;
	OmokScene*				game;
};

