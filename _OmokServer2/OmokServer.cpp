#include "stdafx.h"
#include "../Common/common.h"
#include "OmokServer.h"

OmokServer::OmokServer() 
{
	this->omok.reset(new Omok);
}

OmokServer::~OmokServer()
{
	this->omok.reset();
}

bool OmokServer::init()
{
	this->timer_thread =  CreateThread(NULL, 0, tiemr_thread_func, &this->time_tick,  0, NULL);

	this->omok->init();

	int retval;
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}

	// socket()
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
		return false;
	}

	// sockopt
	if (false == this->apply_sock_opt())
	{
		return false; 
	}

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("bind()");
		return false;
	}

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		err_quit("listen()");
		return false;
	}

	// 넌블로킹 소켓으로 전환
	u_long on = 1;
	retval = ioctlsocket(listen_sock, FIONBIO, &on);
	if (retval == SOCKET_ERROR)
	{
		err_display("ioctlsocket()");
		return false;
	}

	return true;
}

bool OmokServer::apply_sock_opt()
{
	// nagle opt
	BOOL optval = TRUE; // Nagle 알고리즘 중지
	int retval = setsockopt(this->listen_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));
	

	if (retval == SOCKET_ERROR)
		err_quit("setsockopt()");

	// linger opt
	linger ling_optval;
	ling_optval.l_onoff = 1;
	ling_optval.l_linger = 0;

	retval = setsockopt(this->listen_sock, SOL_SOCKET, SO_LINGER,
		(char*)&ling_optval, sizeof(ling_optval));

	if (retval == SOCKET_ERROR) 
		err_quit("setsockopt()");

	return true;
}


// 세션 별로 데이터 가져와서  // 서버가
// 매 루프마다 세션별로 받은 데이터가 있으면 데이터 처리 // 클라이언트가


void OmokServer::update()
{
	// 서버 초기화
	//init(); // 인젭 부터 들어 올 수 있음.
	//apply_sock_opt(listen_sock); // linger opt on, nagle opt off, keep alive opt on

////////////////////////////// update server 
	for (;;)
	{
		int retval;
		FD_SET rset, wset;
		SOCKET client_sock;
		SOCKADDR_IN clientaddr;
		int addrlen, i;

		while (false == omok->is_gameover()) // 게임이 종료 되지 않았으면 근야 처리함.
		{
			// 소켓 셋 초기화
			FD_ZERO(&rset);
			FD_ZERO(&wset);
			FD_SET(this->listen_sock, &rset);

			// 이거 왜하는거야??
			// 버퍼가 하나여서그런가?
			for (i = 0; i < nTotalSockets; i++)
			{
				if (SocketInfoArray[i]->recvbytes > SocketInfoArray[i]->sendbytes)
					FD_SET(SocketInfoArray[i]->sock, &wset);
				else
					FD_SET(SocketInfoArray[i]->sock, &rset);
			}

			// select()
			retval = select(0, &rset, &wset, NULL, NULL);
			if (retval == SOCKET_ERROR)
			{
				err_quit("select()");
			}

			// 소켓 셋 검사(1): 클라이언트 접속 수용
			if (FD_ISSET(listen_sock, &rset))
			{
				addrlen = sizeof(clientaddr);
				client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
				if (client_sock == INVALID_SOCKET)
				{
					err_display("accept()");
				}
				else
				{
					printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
						inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
					// 소켓 정보 추가
					AddSocketInfo(client_sock);
					// 세션 정보 추가.
					// 등등
				}
			}

			// 소켓 셋 검사(2): 데이터 통신
			// read를 할 수 있는지 검사.
			for (i = 0; i < nTotalSockets; i++)
			{
				SOCKETINFO* ptr = SocketInfoArray[i];
				if (FD_ISSET(ptr->sock, &rset))
				{
					// 데이터 받기
					retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);

					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						RemoveSocketInfo(i);
						continue;
					}
					else if (retval == 0)
					{
						RemoveSocketInfo(i);
						continue;
					}
					ptr->recvbytes = retval;
					// 받은 데이터 출력
					addrlen = sizeof(clientaddr);
					getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);
					ptr->buf[retval] = '\0';
					printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
						ntohs(clientaddr.sin_port), ptr->buf);
				}

				if (FD_ISSET(ptr->sock, &wset))
				{
					// 데이터 보내기
					retval = send(ptr->sock, ptr->buf + ptr->sendbytes,
						ptr->recvbytes - ptr->sendbytes, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						RemoveSocketInfo(i);
						continue;
					}
					ptr->sendbytes += retval;
					if (ptr->recvbytes == ptr->sendbytes)
					{
						ptr->recvbytes = ptr->sendbytes = 0;
					}
				}
			}
		}

		// game update()
		omok->update();
	}


}

void OmokServer::fin()
{
	WSACleanup();
}

BOOL OmokServer::AddSocketInfo(SOCKET sock)
{
	if (nTotalSockets >= FD_SETSIZE) {
		printf("[오류] 소켓 정보를 추가할 수 없습니다!\n");
		return FALSE;
	}

	SOCKETINFO* ptr = new SOCKETINFO;
	if (ptr == NULL) {
		printf("[오류] 메모리가 부족합니다!\n");
		return FALSE;
	}

	ptr->sock = sock;
	ptr->recvbytes = 0;
	ptr->sendbytes = 0;
	SocketInfoArray[nTotalSockets++] = ptr;

	return TRUE;
}

void OmokServer::RemoveSocketInfo(int nIndex)
{
	SOCKETINFO* ptr = SocketInfoArray[nIndex];

	// 클라이언트 정보 얻기
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	closesocket(ptr->sock);

	delete ptr;

	if (nIndex != (nTotalSockets - 1))
		SocketInfoArray[nIndex] = SocketInfoArray[nTotalSockets - 1];

	--nTotalSockets;
}

DWORD __stdcall OmokServer::tiemr_thread_func(LPVOID param)
{
	/*size_t* shraed_timer_tick = (size_t*)param;*/
	OmokServer* server	= reinterpret_cast<OmokServer*>(param);
	Omok*		game	= reinterpret_cast<Omok*>(param);

	for (;;)
	{
		Sleep(200);
		// event 
		
		if (true == game->is_gameover())
			break;

		//*shraed_timer_tick = GetTickCount64();
		server->time_tick = GetTickCount64();
	}

	return 0;
}

