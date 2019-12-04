#include "Session.h"


Session::Session() : sock_info{ nullptr }, owner {nullptr},
last_error{} ,state{}
{
	this->recv_buffer = new PacketVector<65536>;
	this->send_buffer = new PacketVector<65536>;
	this->inner_packet = new Packet;
}

Session::~Session()
{
	delete this->recv_buffer;
	delete this->send_buffer;
	delete this->inner_packet;
}

size_t Session::pre_send(const Packet* out_packet)
{
	size_t remaining_size = this->send_buffer->get_buffer_size(); //  남은 버퍼사이즈
	size_t packet_size = out_packet->size();

	if (remaining_size < packet_size)
		c2::util::crash();

	this->send_buffer->write(out_packet->getReadBufferPtr(), packet_size);

	//this->send_buffer->rewind();
	return packet_size;
}

void Session::parse_packet()
{
	//printf("void Session::parse_packet()");
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
		// 예시
		//switch ( header->type)
		//{
		//	case PT_CS_HEARTBEAT_RESPONSE:
		//		// 시간내에 오지 않는다면 cut -

		//	case PT_CS_GAME_RESULT:
		//		//
		//		break;
		//	case PT_CS_GAME_START:
		//		//
		//	case PT_CS_LOGIN_REQUEST:
		//	case PT_CS_READY_REQUEST:
		//		break;

		//	default :
		//		//crash
		//		;
		//}
	}

	this->recv_buffer->rewind();
}

void Session::process_packet(PacketHeader* header)
{
	header->length;

	switch (header->type)
	{
	//case :
	}
	printf("void Session::process_packet(PacketHeader* header)");

	//printf("%d\n", header);
}



void Session::process_packet(PacketHeader* header, Packet* packet)
{
	switch (header->type)
	{
		case PT_CS_DROP_STONE_REQUEST:
		{
			//printf("DROP_STONE_REQUEST");
			PacketHeader header;
			OmokStatus stone_type;
			int x, y;
			packet->read(&header, sizeof(header));
			packet->read(&stone_type, sizeof(stone_type));
			// 유효성 검사.
			if (stone_type != owner->get_game()->GetTurn())
			{
				printf("invalid turn");
				return;
			}

			packet->read(&x, sizeof(x));
			packet->read(&y, sizeof(y));

			// 실제 오목판에 올려 놓기.
		
			if (false == owner->get_game()->DropStone(x, y, stone_type))
				return;
			owner->get_game()->SwapTurn();


			// 애들한테 보내기.
			// broad_casting
			PacketHeader out_packet_header;
			out_packet_header.type	 = PT_SC_DROP_STONE_RESPONSE;
			out_packet_header.length = sizeof(out_packet_header)+sizeof(OmokStatus)+sizeof(y)+sizeof(x);
		
			inner_packet->write(&out_packet_header, sizeof(PacketHeader));
			inner_packet->write(&stone_type, sizeof(OmokStatus)); // 돌 종류.
			inner_packet->write(&x, sizeof(int));
			inner_packet->write(&y, sizeof(int));

			owner->broadcast(inner_packet);
			inner_packet->reset();

			// 게임 승부 체크. 순서때문에 여기다가 놉니다. 일단.
			owner->get_game()->CheckGomoku(x, y, stone_type);

			break;
		}
		default:
			this->last_error = 0x1234;
			c2::util::crash();
			break;
	}
}

void Session::makePacketGameResult(OmokStatus winner, Packet* packet)
{
	PacketHeader header;

	header.type = PT_SC_NOTIFY_GAME_RESULT;
	header.length = sizeof(header);
	size_t time_tick = owner->get_timer_tick();

	inner_packet->write(&header, sizeof(header));
	inner_packet->write(&winner, sizeof(winner));
	inner_packet->write(&time_tick, sizeof(time_tick));
}
