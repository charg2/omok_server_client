#pragma once

// pair - request response  
// notify - �ܹ���.
// SC : server to client
// CS : client to server
enum PakcetType
{
	PT_NONE,
	
	PT_SC_ASSIGN_STONE,
	 
	PT_SC_NOTIFY_KICK,			// ���̻� ������ ������������ 	
	PT_CS_NOTIFY_KICK,			// ���̻� ������ ������������ �˷��� �����.

	PT_SC_NOTIFY_SERVER_TIME,	/// �ð� �˸� �Ϲ��뺸

// content logic packet
	PT_CS_DROP_STONE_REQUEST,   // �������� ����. 
	PT_SC_DROP_STONE_RESPONSE,  // ������ ��ο��� ��ε� ĳ���� , �߸��� ��ġ�� ���� ���°��� ������ ������.

////////////////////////////
	PT_SC_NOTIFY_GAME_RESULT,	// �º�
	
	PT_MAX, 
};

// common packetHeader �̰��� ������ ��� �޾Ƽ� ����Ͻÿ�.
struct PacketHeader
{
	unsigned short length;
	unsigned short type;
};

