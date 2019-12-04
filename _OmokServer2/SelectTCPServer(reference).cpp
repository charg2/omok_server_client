
//#include <unordered_map>

//#include "WinAPI-���� ã��� ����/Omok.h"
//
//
//
//int nTotalSockets = 0;
//SOCKETINFO *SocketInfoArray[FD_SETSIZE];
//
//// ���� ���� �Լ�
//BOOL AddSocketInfo(SOCKET sock);
//void RemoveSocketInfo(int nIndex);
//
//std::unique_ptr<Omok> omok;
//
////////////////////////
//bool init_server();
//bool apply_sock_opt(SOCKET sock);
//void update_select();
//void finalize_server();
//
//class Session;
//
//SOCKET listen_sock;
//size_t server_last_error;
//std::unordered_map<SOCKETINFO*, Session*> session_map;
//int main(int argc, char *argv[])
//{
//	// �� �ʱ�ȭ
//	omok.reset(new Omok);
//	omok->init();
//
//	// ���� �ʱ�ȭ
//	init_server(); // ���� ���� ��� �� �� ����.
//	apply_sock_opt(listen_sock); // linger opt on, nagle opt off, keep alive opt on
//
//////////////////////////////// update server 
//	//select part
//    // ������ ��ſ� ����� ����
//    int retval;
//    FD_SET rset, wset;
//    SOCKET client_sock;
//    SOCKADDR_IN clientaddr;
//    int addrlen, i;
//
//    while ( false == omok->is_gameover() ) // ������ ���� ���� �ʾ����� �پ� ó����.
//	{
//        // ���� �� �ʱ�ȭ
//        FD_ZERO(&rset);
//        FD_ZERO(&wset);
//        FD_SET(listen_sock, &rset);
//
//		// �̰� ���ϴ°ž�??
//		// ���۰� �ϳ������׷���?
//        for (i = 0; i < nTotalSockets; i++) 
//		{
//            if (SocketInfoArray[i]->recvbytes > SocketInfoArray[i]->sendbytes)
//                FD_SET(SocketInfoArray[i]->sock, &wset);
//            else
//                FD_SET(SocketInfoArray[i]->sock, &rset);
//        }
//
//        // select()
//        retval = select(0, &rset, &wset, NULL, NULL);
//		if (retval == SOCKET_ERROR)
//		{
//			err_quit("select()");
//		}
//
//		// ���� �� �˻�(1): Ŭ���̾�Ʈ ���� ����
//        if (FD_ISSET(listen_sock, &rset)) 
//		{
//            addrlen = sizeof(clientaddr);
//            client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
//            if (client_sock == INVALID_SOCKET) 
//			{
//                err_display("accept()");
//            }
//            else 
//			{
//                printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
//                    inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
//                // ���� ���� �߰�
//                AddSocketInfo(client_sock);
//				// ���� ���� �߰�.
//				// ���
//            }
//        }
//
//        // ���� �� �˻�(2): ������ ���
//		// read�� �� �� �ִ��� �˻�.
//        for (i = 0; i < nTotalSockets; i++) 
//		{
//            SOCKETINFO *ptr = SocketInfoArray[i];
//            if (FD_ISSET(ptr->sock, &rset)) 
//			{
//                // ������ �ޱ�
//                retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
//
//                if (retval == SOCKET_ERROR) 
//				{
//                    err_display("recv()");
//                    RemoveSocketInfo(i);
//                    continue;
//                }
//                else if (retval == 0) 
//				{
//                    RemoveSocketInfo(i);
//                    continue;
//                }
//                ptr->recvbytes = retval;
//                // ���� ������ ���
//                addrlen = sizeof(clientaddr);
//                getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
//                ptr->buf[retval] = '\0';
//                printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
//                    ntohs(clientaddr.sin_port), ptr->buf);
//            }
//
//            if (FD_ISSET(ptr->sock, &wset)) 
//			{
//                // ������ ������
//                retval = send(ptr->sock, ptr->buf + ptr->sendbytes,
//                    ptr->recvbytes - ptr->sendbytes, 0);
//                if (retval == SOCKET_ERROR) 
//				{
//                    err_display("send()");
//                    RemoveSocketInfo(i);
//                    continue;
//                }
//                ptr->sendbytes += retval;
//                if (ptr->recvbytes == ptr->sendbytes) 
//				{
//                    ptr->recvbytes = ptr->sendbytes = 0;
//                }
//            }
//        }
//    }
//
//	// ���� ���� ó��.
////////////////////////////////
//	finalize_server();
//    return 0;
//}
//
//// ���� ���� �߰�
//BOOL AddSocketInfo(SOCKET sock)
//{
//    if (nTotalSockets >= FD_SETSIZE) {
//        printf("[����] ���� ������ �߰��� �� �����ϴ�!\n");
//        return FALSE;
//    }
//
//    SOCKETINFO *ptr = new SOCKETINFO;
//    if (ptr == NULL) {
//        printf("[����] �޸𸮰� �����մϴ�!\n");
//        return FALSE;
//    }
//
//    ptr->sock = sock;
//    ptr->recvbytes = 0;
//    ptr->sendbytes = 0;
//    SocketInfoArray[nTotalSockets++] = ptr;
//
//    return TRUE;
//}
//
//// ���� ���� ����
//void RemoveSocketInfo(int nIndex)
//{
//    SOCKETINFO *ptr = SocketInfoArray[nIndex];
//
//    // Ŭ���̾�Ʈ ���� ���
//    SOCKADDR_IN clientaddr;
//    int addrlen = sizeof(clientaddr);
//    getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
//    printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
//        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
//
//    closesocket(ptr->sock);
//    delete ptr;
//
//    if (nIndex != (nTotalSockets - 1))
//        SocketInfoArray[nIndex] = SocketInfoArray[nTotalSockets - 1];
//
//    --nTotalSockets;
//}
//
//bool init_server()
//{
//	int retval;
//
//	// ���� �ʱ�ȭ
//	WSADATA wsa;
//	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
//	{	
//		return false;
//	}
//
//	// socket()
//	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
//	if (listen_sock == INVALID_SOCKET)
//	{
//		err_quit("socket()");
//		return false;
//	}
//
//	// bind()
//	SOCKADDR_IN serveraddr;
//	ZeroMemory(&serveraddr, sizeof(serveraddr));
//	serveraddr.sin_family = AF_INET;
//	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
//	serveraddr.sin_port = htons(SERVERPORT);
//
//	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
//	if (retval == SOCKET_ERROR)
//	{
//		err_quit("bind()");
//		return false;
//	}
//
//	// listen()
//	retval = listen(listen_sock, SOMAXCONN);
//	if (retval == SOCKET_ERROR)
//	{
//		err_quit("listen()");
//		return false;
//	}
//
//	// �ͺ��ŷ �������� ��ȯ
//	u_long on = 1;
//	retval = ioctlsocket(listen_sock, FIONBIO, &on);
//	if (retval == SOCKET_ERROR)
//	{
//		err_display("ioctlsocket()");
//		return false;
//	}
//
//	return true;
//}
//
//
//bool apply_sock_opt(SOCKET sock) // socket handle�� ����Ű���ִ� ���� ��ü�� �ɼ��� ��������.
//{
//	// linger opt
//	linger linger_opt{ 1, 0 };
//	if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&linger_opt, sizeof(LINGER)))
//	{
//		//server_last_error = static_cast<size_t>(ErrorCode::SOCKET_OPT_LINGER_OPT_FAIL);
//		return false;
//	}
//
//	int option = TRUE;
//	if (SOCKET_ERROR == setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option)))
//	{
//		//this->server_last_error = static_cast<size_t>(ErrorCode::SOCKET_OPT_NODELAY_OPT_FAIL);
//		return false;
//	}
//
//	BOOL bEnable = TRUE;
//	int retval = setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE,
//		(char*)&bEnable, sizeof(bEnable));
//	if (retval == SOCKET_ERROR)
//		err_quit("setsockopt()");
//
//	return true;
//}
//
//void update_select()
//{
//
//}
//
//// ���� ����
//void finalize_server()
//{
//	WSACleanup();
//}

#include "stdafx.h"
#include <memory>
#include "OmokServer.h"

void main()
{
	std::unique_ptr<OmokServer> omok_server(std::make_unique<OmokServer>());

	if (false == omok_server->init())
		return;

	omok_server->update();

	omok_server->fin();

}