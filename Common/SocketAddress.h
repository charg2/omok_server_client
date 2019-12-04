#ifndef SOCKET_ADDRESS_H
#define SOCKET_ADDRESS_H

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string>
#include <cstdint>

#pragma comment(lib, "ws2_32")
// inetNtop () 함수로 문자열로 바꿔줌. 추가 ㄱㄱ.
// 생성자에 default param  true, false로 넣고 
// true 도메인 false string"127.0.0.1" 이런식.

class SocketAddress
{
public:
	SocketAddress(uint32_t address, uint16_t port)
	{
		getAsSockAddrIn()->sin_family = AF_INET;
		getAsSockAddrIn()->sin_port	  = htons(port);

		getAsSockAddrIn()->sin_addr.s_addr = htonl(address);
	}

	SocketAddress(std::string&& address, uint16_t port)
	{
		getAsSockAddrIn()->sin_family	= AF_INET;
		getAsSockAddrIn()->sin_port		= htons(port);

		InetPtonA(AF_INET, address.c_str(), &getAsSockAddrIn()->sin_addr);
	}

	SocketAddress(std::wstring&& address, uint16_t port)
	{
		getAsSockAddrIn()->sin_family	= AF_INET;
		getAsSockAddrIn()->sin_port		= htons(port);

		InetPtonW(AF_INET, address.c_str(), &getAsSockAddrIn()->sin_addr);
	}

	SocketAddress(const char* address , uint16_t port)
	{
		getAsSockAddrIn()->sin_family	= AF_INET;
		getAsSockAddrIn()->sin_port		= htons(port);

		InetPtonA(AF_INET, address, &getAsSockAddrIn()->sin_addr);
	}

	SocketAddress(const wchar_t* address, uint16_t port)
	{
		getAsSockAddrIn()->sin_family = AF_INET;
		getAsSockAddrIn()->sin_port = htons(port);

		InetPtonW(AF_INET, address, &getAsSockAddrIn()->sin_addr);
	}

	//SocketAddress(in_addr address, uint16_t port)
	//{
	//	//getAsSockAddrIn()->sin_family = AF_INET;
	//	//getAsSockAddrIn()->sin_port = htons(port);

	//	//InetPtonA(AF_INET, inet_ntoa(address), &getAsSockAddrIn()->sin_addr);
	//}

	SocketAddress(const sockaddr& inSockAddr)
	{
		std::memmove(&sock_addr, &inSockAddr, sizeof(sockaddr));
	}

	size_t size() const noexcept 
	{ 
		return sizeof(sockaddr); 
	}


	sockaddr* getAsSockAddr()
	{
		return &sock_addr;
	}

	sockaddr_in* getAsSockAddrIn()
	{
		return reinterpret_cast<sockaddr_in*>(&sock_addr);
	}

private:
	sockaddr sock_addr;
};

//using SockAddressPtr = std::shared_ptr<SocketAddress>;


#endif

