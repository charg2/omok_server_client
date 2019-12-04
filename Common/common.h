#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <Windows.h>
#include <mstcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define TCP_SERVERPORT	23230
#define UDP_SERVERPORT	23231
#define SERVERIP	"127.0.0.1"
#define BUFSIZE		512		// 512바이트 버퍼를 사용해 데이터를 읽음. 실제로 가변길이의 길이가 이보다 작다고 가정함

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

// 소켓 정보 저장을 위한 구조체와 변수
struct SOCKETINFO
{
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
};



// 소켓 함수 오류 출력 후 종료
void err_quit(const wchar_t* msg);
// 소켓 함수 오류 출력
void err_display(const wchar_t* msg);
// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char* buf, int len, int flags);     // 고정길이 데이터를 읽기 위해 사용자 정의함수 recvn
// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg);
// 소켓 함수 오류 출력
void err_display(const char* msg);

