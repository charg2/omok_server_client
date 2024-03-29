///**********************************
//예제 10-1 
//
//논블록 예제. 
//ioctlsocket() 함수 사용 간단 예제
//***********************************/
//
////#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
//#pragma comment(lib, "ws2_32")
//#include <winsock2.h>
//#include <stdlib.h>
//#include <stdio.h>
//
////#define SERVERPORT 9000
////#define BUFSIZE    512

#include "stdafx.h"

//#include "../Common/common.h"
//
//int main(int argc, char *argv[])
//{
//    int retval;
//
//    // 윈속 초기화
//    WSADATA wsa;
//    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
//        return 1;
//
//    // socket()
//    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
//    if (listen_sock == INVALID_SOCKET) err_quit("socket()");
//
//    // 넌블로킹 소켓으로 전환
//    u_long on = 1;
//    retval = ioctlsocket(listen_sock, FIONBIO, &on);
//    if (retval == SOCKET_ERROR) err_quit("ioctlsocket()");
//
//    // bind()
//    SOCKADDR_IN serveraddr;
//    ZeroMemory(&serveraddr, sizeof(serveraddr));
//    serveraddr.sin_family = AF_INET;
//    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
//    serveraddr.sin_port = htons(SERVERPORT);
//    retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
//    if (retval == SOCKET_ERROR) err_quit("bind()");
//
//    // listen()
//    retval = listen(listen_sock, SOMAXCONN);
//    if (retval == SOCKET_ERROR) err_quit("listen()");
//
//    // 데이터 통신에 사용할 변수
//    SOCKET client_sock;
//    SOCKADDR_IN clientaddr;
//    int addrlen;
//    char buf[BUFSIZE + 1];
//
//    while (1) {
//        // accept()
//    ACCEPT_AGAIN:
//        addrlen = sizeof(clientaddr);
//        client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
//        if (client_sock == INVALID_SOCKET) {
//            if (WSAGetLastError() == WSAEWOULDBLOCK)
//                goto ACCEPT_AGAIN;
//            err_display("accept()");
//            break;
//        }
//
//        // 접속한 클라이언트 정보 출력
//        printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
//            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
//
//        // 클라이언트와 데이터 통신
//        while (1) {
//            // 데이터 받기
//        RECEIVE_AGAIN:
//            retval = recv(client_sock, buf, BUFSIZE, 0);
//            if (retval == SOCKET_ERROR) {
//                if (WSAGetLastError() == WSAEWOULDBLOCK)
//                    goto RECEIVE_AGAIN;
//                err_display("recv()");
//                break;
//            }
//            else if (retval == 0)
//                break;
//
//            // 받은 데이터 출력
//            buf[retval] = '\0';
//            printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
//                ntohs(clientaddr.sin_port), buf);
//
//            // 데이터 보내기
//        SEND_AGAIN:
//            retval = send(client_sock, buf, retval, 0);
//            if (retval == SOCKET_ERROR) {
//                if (WSAGetLastError() == WSAEWOULDBLOCK)
//                    goto SEND_AGAIN;
//                err_display("send()");
//                break;
//            }
//        }
//
//        // closesocket()
//        closesocket(client_sock);
//        printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
//            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
//    }
//
//    // closesocket()
//    closesocket(listen_sock);
//
//    // 윈속 종료
//    WSACleanup();
//    return 0;
//}
