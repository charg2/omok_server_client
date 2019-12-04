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

	// 소켓 관리 함수
	BOOL AddSocketInfo(SOCKET sock);
	void RemoveSocketInfo(int nIndex);

private:
	// timer 또는 출력용?
	static DWORD WINAPI tiemr_thread_func(LPVOID param);
	bool apply_sock_opt();

private:
	int nTotalSockets = 0;
	SOCKETINFO* SocketInfoArray[FD_SETSIZE];

	std::unique_ptr<Omok> omok;

	HANDLE timer_thread;
	SOCKET listen_sock;
	size_t server_last_error;
	std::unordered_map<SOCKETINFO*, Session*> session_map;

	size_t time_tick;
};

