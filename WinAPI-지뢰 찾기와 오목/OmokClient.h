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

	// 현재 완성된 패킷이 있는지 검사하고 
	// 최대 패킷 길이 만큼만 버퍼를 땅기면 됨.
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

	// 생각해보니 어차피 버퍼 내용을 최대한 처리하고 다시 받을건데
	// 버퍼 사이즈를 64kb + 최대 패킷 길이 만큼만 잡아 놓으면 문제 없을거 같다.
	PacketVector<65536>*	send_buffer;
	PacketVector<65536>*	recv_buffer;

	Packet*					inner_packet;

	size_t					last_error;
	OmokScene*				game;
};

