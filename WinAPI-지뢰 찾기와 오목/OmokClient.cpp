#include "stdafx.h"
#include "../Common/protocol.h"
#include "OmokClient.h"


OmokClient::OmokClient() : state{}, sock_addr{}, sock{ INVALID_SOCKET }
{
	this->recv_buffer = new PacketVector<65536>;
	this->send_buffer = new PacketVector<65536>;
	this->inner_packet = new Packet;
}

OmokClient::~OmokClient()
{
	delete this->recv_buffer;
	delete this->send_buffer;
	delete this->inner_packet;

}

bool OmokClient::init()
{
	WSAData wsa;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		err_quit("OmokClient::init() WSAStartup() failure");
		//this->client_last_error = static_cast<size_t>(ErrorCode::WINSOCK_LIB_INIT_FAIL);
		return false;
	}
	printf("호출모딤?");

	this->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock)
	{
		err_quit("OmokClient::init() socket() failure");
		//this->client_last_error = static_cast<size_t>(ErrorCode::LISTEN_SOCKET_CREATION_FAIL);
		return false;
	}

	this->apply_sockopt();

	// nonblocking로 바꾸고 ㅇㅇ
	return true;
}

// non-blocking socket
void OmokClient::update()
{
	this->recv_packet();
	this->send_packet();
}

void OmokClient::fin()
{
	WSACleanup();
}

size_t OmokClient::recv_packet()
{
	//printf("\n%d\n",(size_t)this->sock);
	size_t recv_byte = recv(this->sock, (char*)this->recv_buffer->get_write_buffer(), this->recv_buffer->get_buffer_size(), NULL);
	if (SOCKET_ERROR == recv_byte || 0 == recv_byte)
	{
		//owner->client_last_error = static_cast<size_t>(ErrorCode::DUPLICATE_SUBCRIPTION);
		size_t err_code = WSAGetLastError();
		//printf("\n%d\n", err_code);
		//err_display("OmokClient::recv_packet() ");

		if(WSAEWOULDBLOCK != err_code )
			disconnect();

		return recv_byte;
	}
	else if (0 < recv_byte) /// 보낸거.
	{
		//printf("recv byte [%d] ",recv_byte);
		this->recv_buffer->move_write_head(recv_byte);

		this->parse_packet(); // 패킷 처리.
	}

	return recv_byte;
}

size_t OmokClient::send_packet()
{
	if (this->send_buffer->get_use_size() <= 0)
		return 0;
	//printf("s_p()\n");

	size_t send_ret = send(this->sock, this->send_buffer->get_buffer(), this->send_buffer->get_use_size(), NULL);

	//printf("sent_bytes %d \n", send_ret);

	this->send_buffer->move_read_head(send_ret);

	this->send_buffer->rewind();

	return send_ret;
}

size_t OmokClient::pre_send(const Packet* out_packet)
{
	size_t remaining_size = this->send_buffer->get_buffer_size(); //  남은 버퍼사이즈
	size_t packet_size = out_packet->size();

	if (remaining_size < packet_size)
		c2::util::crash();

	this->send_buffer->write(out_packet->getReadBufferPtr(), packet_size);

	return packet_size;
}

void OmokClient::disconnect()
{
	closesocket(this->sock);
}

void OmokClient::connect()
{
	int retval;
	
	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr =  inet_addr(SERVERIP);
	serveraddr.sin_port = htons(TCP_SERVERPORT);

	for (;;)
	{
		Sleep(100);
		//printf("\n %d, %d\n ",(size_t)this->sock);
		retval = ::connect(this->sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
		size_t err_code = GetLastError();

		if (retval == SOCKET_ERROR)
		{
			if (WSAEWOULDBLOCK == err_code)
			{
				//printf("아 우드 블락? ㅋㅋ? : %d", err_code);
				err_display("OmokClient::init() connect() failure");
				//this->client_last_error = static_cast<size_t>(ErrorCode::LISTEN_SOCKET_CREATION_FAIL);
				//continue;
				return;
			}
			else if (WSAEISCONN == err_code)
			{
				printf("와 연결 완료!! : %d", err_code);
				return;
			}
			else 
			{
				printf("ERROR_REASON : %d", err_code);
				err_quit("OmokClient::init() connect() failure");
			}
		}
		else
		{
			break;
		}

	}
	printf("연결됨와;;");
}

void OmokClient::requset_to_drop_stone(int x, int y, OmokStatus type)
{
	Packet* out_packet = new Packet;
	
	// packet_size를 읽고 그 뒤 사이즈를 읽음.
	PacketHeader packet_header;

	packet_header.type	= PT_CS_DROP_STONE_REQUEST; // 종류 추가
	packet_header.length= sizeof(packet_header)+ sizeof(OmokStatus) + sizeof(int) + sizeof(int);

	out_packet->write(&packet_header, sizeof(PacketHeader));

	out_packet->write(&type, sizeof(OmokStatus)); // 돌 종류.
	out_packet->write(&x, sizeof(int));
	out_packet->write(&y, sizeof(int));

	pre_send(out_packet);

	out_packet->moveWriteHead(packet_header.length);// 이거 왜 이렇게 해놧더라?

	delete out_packet;
}


void OmokClient::set_game(OmokScene* game)
{
	this->game = game;
}

void OmokClient::parse_packet()
{
	for (;;)
	{
		size_t use_size = recv_buffer->get_use_size();
		// recv_buffer에서 하나씩 꺼냄.
		if (use_size < sizeof(PacketHeader))
			break;

		PacketHeader* header = recv_buffer->get_header();
		if (header->length > recv_buffer->get_use_size())
			break;

		// recv_buffer.
		if (PT_NONE < header->type && header->type > PT_MAX)
			// crash
			break;

		inner_packet->write(header, header->length);

		this->process_packet(header, inner_packet);

		inner_packet->reset();

		this->recv_buffer->move_read_head(header->length);
	}

	this->recv_buffer->rewind();
}


void OmokClient::process_packet(PacketHeader* header)
{
	switch (header->type)
	{
	case PT_SC_ASSIGN_STONE:
		(Packet*)header;
		//game->SetOmokStone();
		break;

	default:
		this->last_error = 0x1234;
		c2::util::crash();
		break;
	}
}

void OmokClient::process_packet(PacketHeader* header, Packet* packet)
{
	switch (header->type)
	{
		case PT_SC_NOTIFY_KICK:
		{
			PacketHeader	header;
			
			packet->read(&header, sizeof(header));

			printf("\n[서버 공지] 접속할 자리가 없다\n");

			Packet* out_packet = new Packet; // 응답은 해야 예의임. 유사 3way hand shaking
			header.type = PT_CS_NOTIFY_KICK;
			out_packet->write(&header, sizeof(header));
			pre_send(out_packet);

			break;
		}

		case PT_SC_ASSIGN_STONE:
		{
			PacketHeader header;
			OmokStatus stone_type;
			packet->read(&header, sizeof(header));
			packet->read(&stone_type, sizeof(stone_type));

			if (stone_type == OMOK_BLACK)
				printf("[공지]::내돌은 블랙\n");
			else if (stone_type == OMOK_WHITE)
				printf("[공지]::내돌은 화이트\n");
			else
				printf("내돌은 머냐;;");


			game->SetMyOmokStone(stone_type);

			break;
		}
		case PT_SC_DROP_STONE_RESPONSE:
		{
			PacketHeader header;
			OmokStatus stone_type;
			int x, y;

			packet->read(&header, sizeof(header));
			packet->read(&stone_type, sizeof(stone_type));
			packet->read(&x, sizeof(x));
			packet->read(&y, sizeof(y));

			game->DropStone(x, y, stone_type);
			game->SwapTurn();
			break;
		}
		case PT_SC_NOTIFY_GAME_RESULT:
		{
			PacketHeader header;
			OmokStatus winner;
			size_t time_tick;

			packet->read(&header, sizeof(header));
			packet->read(&winner, sizeof(winner));
			packet->read(&time_tick, sizeof(size_t));

			this->game->SetTimeTick(time_tick);

			this->game->GameOver(winner);
			//game->dropStone(x, y, stone_type);
			//game->SwapTurn();

			break;
		}

		case PT_SC_NOTIFY_SERVER_TIME:
		{
			PacketHeader	header;
			size_t			time_tick;

			packet->read(&header, sizeof(header));
			packet->read(&time_tick, sizeof(size_t));

			this->game->SetTimeTick(time_tick);

			break;
		}


		default:
			this->last_error = 0x1234;
			c2::util::crash();
			break;
	}
}

void OmokClient::apply_sockopt()
{
	// nagle opt
	BOOL optval = TRUE; // Nagle 알고리즘 중지
	int retval = setsockopt(this->sock, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR)
		err_quit("setsockopt()");

	// linger opt
	linger ling_optval;
	ling_optval.l_onoff = 1;
	ling_optval.l_linger = 0;

	retval = setsockopt(this->sock, SOL_SOCKET, SO_LINGER,
		(char*)&ling_optval, sizeof(ling_optval));

	if (retval == SOCKET_ERROR)
		err_quit("setsockopt()");

	// 넌블로킹 소켓으로 전환
	u_long on = 1;
	retval = ioctlsocket(this->sock, FIONBIO, &on);
	if (retval == SOCKET_ERROR)
		err_quit("ioctlsocket()");

	BOOL bEnable = TRUE;
	retval = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE,
		(char*)&bEnable, sizeof(bEnable));
	if (retval == SOCKET_ERROR) 
	err_quit("setsockopt()");
	
}
