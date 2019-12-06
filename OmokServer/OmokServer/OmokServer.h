#pragma once


//#include "../Common/common.h"
//#include <Windows.h>
#include <memory>
#include <unordered_map>

#include "Omok.h"


class Session;

class OmokServer
{
public:
	OmokServer();
	OmokServer(const OmokServer&) = delete;
	OmokServer(OmokServer&&) = delete;
	~OmokServer();

	bool init();
	void update();
	void fin();

	// 家南 包府 窃荐
	BOOL AddSocketInfo(SOCKET sock);
	void RemoveSocketInfo(int nIndex);

	void assignStoneType(Session* session);
	void requestGameStart(Session* session);
	void process_accpet();

	void broadcast(Packet* packet);
	std::unique_ptr<Omok>& get_game();

	size_t get_timer_tick();
	void make_packet_game_result(Packet* packet);
	void make_packet_game_server_time(Packet* packet);
//{
//	PacketHeader header;
//
//	header.type = PT_SC_NOTIFY_GAME_RESULT;
//	header.length = sizeof(header);
//	size_t time_tick = owner->get_timer_tick();
//
//	inner_packet->write(&header, sizeof(header));
//	inner_packet->write(&winner, sizeof(winner));
//	inner_packet->write(&time_tick, sizeof(time_tick));
//}

private:
	static DWORD WINAPI timer_thread_func(LPVOID param);
	static DWORD WINAPI kick_thread_func(LPVOID param);
	bool apply_sock_opt();
	void update_network();
	void insert_session(SOCKETINFO*);
	void remove_session(SOCKETINFO*);
	void process_accpet(SOCKETINFO*);

private:
	enum
	{
		MAX_KICK_THREAD = 256
	};
private:
	int			nTotalSockets;				
	SOCKETINFO* SocketInfoArray[FD_SETSIZE];

	SOCKET listen_sock;
	const int max_connection_count;

	HANDLE timer_thread;

	HANDLE kick_threads[MAX_KICK_THREAD];
	size_t kick_thread_index;

	std::unique_ptr<Omok> omok;

	size_t server_last_error;
	std::unordered_map<SOCKETINFO*, Session*> session_map;

	size_t time_tick;
	SOCKET udp_sock;

	size_t timer_event;
};

