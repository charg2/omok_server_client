#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <Windows.h>
#include <mstcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define TCP_SERVERPORT	23230
#define UDP_SERVERPORT	23231
#define SERVERIP	"127.0.0.1"
#define BUFSIZE		512		// 512����Ʈ ���۸� ����� �����͸� ����. ������ ���������� ���̰� �̺��� �۴ٰ� ������

constexpr size_t	KMaximumPacketSize{ 1460 }; 

enum OmokStatus 
{
	OMOK_EMPTY,
	OMOK_BLACK,
	OMOK_WHITE
};

enum GameStatus
{
	GAME_BEFORE,
	GAME_START,
	GAME_AFTER
};

struct tagGomokuInfo
{
	int x, y;
	RECT rc;
	OmokStatus status;
};

// ���� ���� ������ ���� ����ü�� ����
struct SOCKETINFO
{
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
};



// ���� �Լ� ���� ��� �� ����
void err_quit(const wchar_t* msg);
// ���� �Լ� ���� ���
void err_display(const wchar_t* msg);
// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char* buf, int len, int flags);     // �������� �����͸� �б� ���� ����� �����Լ� recvn
// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg);
// ���� �Լ� ���� ���
void err_display(const char* msg);

