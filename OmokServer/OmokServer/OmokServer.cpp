#include "stdafx.h"
#include "../../Common/common.h"
#include "OmokServer.h"
#include "Session.h"
OmokServer::OmokServer() 
	: nTotalSockets { 0 }
	, SocketInfoArray{}
	, timer_thread{INVALID_HANDLE_VALUE}
	, listen_sock{INVALID_SOCKET}
	, server_last_error{}
	, time_tick{}
	, max_connection_count { 2 }
{
	this->omok.reset(new Omok);
}

OmokServer::~OmokServer()
{
	this->omok.reset();
}

bool OmokServer::init()
{
	this->timer_thread = CreateThread(NULL, 0, timer_thread_func, this,  0, NULL);

	this->omok->Init();

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
	serveraddr.sin_port = htons(TCP_SERVERPORT);

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

	//// 넌블로킹 소켓으로 전환
	//u_long on = 1;
	//retval = ioctlsocket(listen_sock, FIONBIO, &on);
	//if (retval == SOCKET_ERROR)
	//{
	//	err_display("ioctlsocket()");
	//	return false;
	//}
////////////////////////////////////////////////
	// udp  socket()
	{
		this->udp_sock = socket(AF_INET, SOCK_DGRAM, 0);		
		// udp 소켓 생성
		if (this->udp_sock == INVALID_SOCKET)
			err_quit(L"socket()");

			// bind()
			SOCKADDR_IN serveraddr;
			ZeroMemory(&serveraddr, sizeof(serveraddr));
			serveraddr.sin_family = AF_INET;
			serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);		// 모든 ip로   부터 수신
			serveraddr.sin_port = htons(UDP_SERVERPORT);
			retval = bind(this->udp_sock, (SOCKADDR*)&serveraddr, sizeof	(serveraddr));	

		if (retval == SOCKET_ERROR)
			err_quit(L"bind()");
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

void OmokServer::update_network()
{
// network update
	int retval;
	FD_SET rset, wset;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen, i;

	// 소켓 셋 초기화
	FD_ZERO(&rset);
	FD_ZERO(&wset);
	FD_SET(this->listen_sock, &rset);

	// 접속한 소켓이 하나라도 있으면 
	//for (i = 0; i < nTotalSockets; i++)
	//{
	//	if (SocketInfoArray[i]->recvbytes > SocketInfoArray[i]->sendbytes)
	//		FD_SET(SocketInfoArray[i]->sock, &wset);
	//	else
	//		FD_SET(SocketInfoArray[i]->sock, &rset);
	//}

	for (i = 0; i < nTotalSockets; i++)
	{
		SOCKETINFO* sockinfo_ptr = SocketInfoArray[i];
		Session* session_ptr = session_map[sockinfo_ptr];
		if (0 < session_ptr->send_buffer->get_use_size())
		{
			FD_SET(sockinfo_ptr->sock, &wset);
		}

		FD_SET(SocketInfoArray[i]->sock, &rset);
	}

	// select()
	timeval time_out;
	time_out.tv_sec = 0;
	time_out.tv_usec = 0;
	retval = select(0, &rset, &wset, NULL, &time_out);
	if (retval == SOCKET_ERROR)
	{
		err_quit("select()");
	}


	// 소켓 셋 검사(1): 클라이언트 접속 수용
	if (FD_ISSET(listen_sock, &rset))
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		//printf("\n\n%d\n\n", GetLastError());
		// 최대 접속 처리 MESSAGE 보내고
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
		}
		else if (this->max_connection_count <= nTotalSockets)
		{
			this->kick_threads[this->kick_thread_index] = CreateThread(NULL, 0, kick_thread_func, (void*)client_sock, 0, NULL);

			kick_thread_index = (this->kick_thread_index+1) % MAX_KICK_THREAD;
		}
		else
		{
			printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
			// 소켓 정보 추가
			AddSocketInfo(client_sock);
			// 세션 정보 추가.
			insert_session(SocketInfoArray[nTotalSockets-1]);
			process_accpet(SocketInfoArray[nTotalSockets - 1]);
		}
	}
	
	// 소켓 셋 검사(2): 데이터 통신
	// read를 할 수 있는지 검사.
	//for (i = 0; i < nTotalSockets; i++)
	for (i = 0; i < nTotalSockets; i++)
	{
		SOCKETINFO* ptr = SocketInfoArray[i];
		Session*	session_ptr = this->session_map[ptr]; // 추가.

		if (FD_ISSET(ptr->sock, &rset))
		{
			// 데이터 받기
			//retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
			retval = recv(ptr->sock, (char*)session_ptr->recv_buffer->get_write_buffer(), session_ptr->recv_buffer->get_buffer_size(), NULL);

			if (retval == SOCKET_ERROR) // 문제
			{
				printf("\n%d\n\n",GetLastError());
				err_display("recv()");
				RemoveSocketInfo(i);
				
				remove_session(ptr);
				
				continue;
			}
			else if (retval == 0) // 끊긴거
			{
				printf("\n%d\n\n", GetLastError());
				err_display("recv()");

				RemoveSocketInfo(i);
				
				remove_session(ptr);

				continue;
			}
			//ptr->recvbytes = retval;
			// 받은 데이터 출력

			//printf("send byte : %d \n", retval);
			addrlen = sizeof(clientaddr);
			getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);
			ptr->buf[retval] = '\0';
			//printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
			//	ntohs(clientaddr.sin_port), ptr->buf);

			session_ptr->recv_buffer->move_write_head(retval);
			session_ptr->parse_packet(); // 패킷 처리.
		}

		if (FD_ISSET(ptr->sock, &wset))
		{
			// 데이터 보내기
			//retval = send(ptr->sock, ptr->buf + ptr->sendbytes, ptr->recvbytes - ptr->sendbytes, 0);
			//printf("use_size %d", session_ptr->send_buffer->get_use_size());
			size_t send_ret = send(ptr->sock, session_ptr->send_buffer->get_buffer(), session_ptr->send_buffer->get_use_size(), NULL);

			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				RemoveSocketInfo(i);
				continue;
			}
			//ptr->sendbytes += retval;
			//printf("send byte : %d \n", send_ret);
			session_ptr->send_buffer->move_read_head(send_ret);
			session_ptr->send_buffer->rewind();

			//if (ptr->recvbytes == ptr->sendbytes)
			//{
			//	ptr->recvbytes = ptr->sendbytes = 0;
			//}

		}
	}
	//}
}

void OmokServer::insert_session(SOCKETINFO* sock_info)
{
	Session* new_session = new Session;
	new_session->sock_info = sock_info;
	new_session->owner = this;
	this->session_map.insert(std::pair<SOCKETINFO*, Session*>(sock_info, new_session));
}

void OmokServer::remove_session(SOCKETINFO* sock_info)
{
	this->session_map.erase(sock_info);
}

void OmokServer::process_accpet(SOCKETINFO* sessioninfo)
{
	assignStoneType(session_map[sessioninfo]);
}

void OmokServer::broadcast(Packet* packet)
{
	for (auto& session : session_map )
		session.second->pre_send(packet);
}

std::unique_ptr<Omok>& OmokServer::get_game()
{
	return this->omok;
}

size_t OmokServer::get_timer_tick()
{
	return this->time_tick;
}

void OmokServer::make_packet_game_result(Packet* packet)
{
	PacketHeader header;

	header.type			= PT_SC_NOTIFY_GAME_RESULT;
	header.length		= sizeof(header) + sizeof(size_t) + sizeof(OmokStatus);

	OmokStatus winner	= this->get_game()->GetWinner();
	size_t time_tick	= this->time_tick;
	
	packet->write(&header, sizeof(header));
	packet->write(&winner, sizeof(winner));
	packet->write(&time_tick, sizeof(time_tick));
}

void OmokServer::make_packet_game_server_time(Packet* packet)
{
	PacketHeader header;

	header.type			= PT_SC_NOTIFY_SERVER_TIME;
	header.length		= sizeof(header) + sizeof(size_t);

	size_t time_tick	= this->time_tick;

	packet->write(&header, sizeof(header));
	packet->write(&time_tick, sizeof(time_tick));
}


// 세션 별로 데이터 가져와서  // 서버가
// 매 루프마다 세션별로 받은 데이터가 있으면 데이터 처리 // 클라이언트가


void OmokServer::update()
{
	for (;;)
	{
		this->update_network();
		// game update()
		
		// 시간 업데이트 
		if ( 0 < this->timer_event )
		{
			Packet* out_packet = new Packet;
			this->make_packet_game_server_time(out_packet);
			this->broadcast(out_packet);
			delete out_packet;

			InterlockedDecrement(&this->timer_event);
		}
		
		// 종료 확인
		if (true == this->get_game()->IsGameover())
		{
			Packet* out_packet = new Packet;
			this->make_packet_game_result(out_packet);
			this->broadcast(out_packet);
			this->omok->Init();
			delete out_packet;
		}
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

void OmokServer::assignStoneType(Session* session)
{
	OmokStatus type = omok->GetTurn();
	omok->SwapTurn();

	printf("[%d] 부여", type);
	Packet* out_packet = new Packet;

	// packet_size를 읽고 그 뒤 사이즈를 읽음.
	PacketHeader header;
	header.type = PT_SC_ASSIGN_STONE; // 종류 추가
	header.length = sizeof(header)  + sizeof(type);
	out_packet->write(&header, sizeof(header));
	out_packet->write(&type, sizeof(type));

	session->pre_send(out_packet);

	delete out_packet;
}

void OmokServer::requestGameStart(Session* session)
{

}


DWORD __stdcall OmokServer::timer_thread_func(LPVOID param)// spsc
{
	OmokServer* server	= reinterpret_cast<OmokServer*>(param);
	Omok*		game	= reinterpret_cast<Omok*>(param);

	size_t start_time_tick = GetTickCount64();
	size_t elapsed_time = 0;

	for (;;)
	{
		if (true == game->IsGameover()) // 게임이 종료 된다면 업데이트 중지.
			break;

		Sleep( 500 );
		
		//*shraed_timer_tick 
		server->time_tick = GetTickCount64() - start_time_tick;
		if ( 1000 >= server->time_tick - elapsed_time  ) // 1초가지난다면 
		{
			InterlockedIncrement(&server->timer_event); // 이벤트 추가.
			elapsed_time = server->time_tick;			// 시간 갱신.
		}

	}

	return 0;
}

// 들어온 유저를 안내사항과 함께 안전하게 끊어준다.
DWORD __stdcall OmokServer::kick_thread_func(LPVOID param)
{
	SOCKET client_sock = reinterpret_cast<SOCKET>(param); 
	
	Packet*		 kick_packet = new Packet;
	PacketHeader header;

	header.type		= PT_SC_NOTIFY_KICK;
	header.length	= sizeof(PacketHeader) ;

	kick_packet->write(&header, sizeof(header));

	// 클라가 접속을 끊게 유도해줌.
	size_t ret = send(client_sock, kick_packet->getBufferPtr(), kick_packet->size(), NULL);
	if (SOCKET_ERROR == client_sock)
	{
		printf("kick thread : %d , %d \n", GetLastError(), ret);
	}
	char buffer[5]{};
	size_t rett = recvn(client_sock, buffer, sizeof(PacketHeader), NULL);
	if (sizeof(PacketHeader) == rett ) // 정상적으로 끊어줌.
	{
		closesocket(client_sock);
		printf("한명의 유저가 접속했지만 자리가 없어서 내 쫒음.\n");
	}
	else
	{
		err_display("KICK failure : response X \n");
	}

	delete kick_packet;
	
	return 0;
}

//DWORD __stdcall OmokServer::chtting_thread_func(LPVOID param)
//{
//	/*size_t* shraed_timer_tick = (size_t*)param;*/
//	OmokServer* server = reinterpret_cast<OmokServer*>(param);
//	SOCKET chatting_sock = server->udp_sock;
//	//unordered_map<SOCKET, SOCKADDk>
//
//	for (;;)
//	{
//		int retval;
//		FD_SET rset, wset;
//		SOCKET client_sock;
//		SOCKADDR_IN clientaddr;
//		int addrlen, i;
//
//		// 소켓 셋 초기화
//		FD_ZERO(&rset);
//		FD_ZERO(&wset);
//		FD_SET(chatting_sock, &rset);
//
//		for (i = 0; i < nTotalSockets; i++)
//		{
//			SOCKETINFO* sockinfo_ptr = SocketInfoArray[i];
//			Session* session_ptr = session_map[sockinfo_ptr];
//			if (0 < session_ptr->send_buffer->get_use_size())
//			{
//				FD_SET(sockinfo_ptr->sock, &wset);
//			}
//
//			FD_SET(SocketInfoArray[i]->sock, &rset);
//		}
//
//		// select()
//		timeval time_out;
//		time_out.tv_sec = 0;
//		time_out.tv_usec = 0;
//		retval = select(0, &rset, &wset, NULL, &time_out);
//		if (retval == SOCKET_ERROR)
//		{
//			err_quit("select()");
//		}
//
//
//		// 소켓 셋 검사(1): 클라이언트 접속 수용
//		if (FD_ISSET(listen_sock, &rset))
//		{
//			addrlen = sizeof(clientaddr);
//			client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
//			printf("\n\n%d\n\n", GetLastError());
//			// if ( 2 <= nTotalSockets )
//			// 최대 접속 처리 MESSAGE 보내고
//			//{
//			//}
//			//else
//			//{
//			if (client_sock == INVALID_SOCKET)
//			{
//				err_display("accept()");
//			}
//			else
//			{
//				printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
//					inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
//				// 소켓 정보 추가
//				AddSocketInfo(client_sock);
//				insertSession(SocketInfoArray[nTotalSockets - 1]);
//				process_accpet(SocketInfoArray[nTotalSockets - 1]);
//				// 세션 정보 추가.
//			}
//			//}
//		}
//
//		// 소켓 셋 검사(2): 데이터 통신
//		// read를 할 수 있는지 검사.
//		//for (i = 0; i < nTotalSockets; i++)
//		for (i = 0; i < nTotalSockets; i++)
//		{
//			SOCKETINFO* ptr = SocketInfoArray[i];
//			Session* session_ptr = this->session_map[ptr]; // 추가.
//
//			if (FD_ISSET(ptr->sock, &rset))
//			{
//				// 데이터 받기
//				//retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
//				retval = recv(ptr->sock, (char*)session_ptr->recv_buffer->get_write_buffer(), session_ptr->recv_buffer->get_buffer_size(), NULL);
//
//				if (retval == SOCKET_ERROR) // 문제
//				{
//					printf("\n%d\n\n", GetLastError());
//					err_display("recv()");
//					RemoveSocketInfo(i);
//
//					removeSession(ptr);
//
//					continue;
//				}
//				else if (retval == 0) // 끊긴거
//				{
//					printf("\n%d\n\n", GetLastError());
//					err_display("recv()");
//
//					RemoveSocketInfo(i);
//
//					removeSession(ptr);
//
//					continue;
//				}
//				//ptr->recvbytes = retval;
//				// 받은 데이터 출력
//
//				printf("send byte : %d \n", retval);
//				addrlen = sizeof(clientaddr);
//				getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);
//				ptr->buf[retval] = '\0';
//				printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
//					ntohs(clientaddr.sin_port), ptr->buf);
//
//				session_ptr->recv_buffer->move_write_head(retval);
//				session_ptr->parse_packet(); // 패킷 처리.
//			}
//
//			if (FD_ISSET(ptr->sock, &wset))
//			{
//				// 데이터 보내기
//				//retval = send(ptr->sock, ptr->buf + ptr->sendbytes, ptr->recvbytes - ptr->sendbytes, 0);
//				printf("use_size %d", session_ptr->send_buffer->get_use_size());
//				size_t send_ret = send(ptr->sock, session_ptr->send_buffer->get_buffer(), session_ptr->send_buffer->get_use_size(), NULL);
//
//				if (retval == SOCKET_ERROR)
//				{
//					err_display("send()");
//					RemoveSocketInfo(i);
//					continue;
//				}
//				//ptr->sendbytes += retval;
//				printf("send byte : %d \n", send_ret);
//				session_ptr->send_buffer->move_read_head(send_ret);
//				session_ptr->send_buffer->rewind();
//
//				//if (ptr->recvbytes == ptr->sendbytes)
//				//{
//				//	ptr->recvbytes = ptr->sendbytes = 0;
//				//}
//
//			}
//		}
//	}
//
//	return 0;
//}

