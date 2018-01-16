#pragma once
#include<WinSock2.h>

struct SOCKETINFO
{
	WSAOVERLAPPED mOverlapped;
	SOCKADDR_IN mClientAddr;
	SOCKET mClientSocket;
	const SOCKETINFO()
	{
		memset(&mClientAddr, 0, sizeof(SOCKADDR_IN));
	}
	const SOCKETINFO(const SOCKET& clientSock, const SOCKADDR_IN clientAddr)
		:mClientSocket(clientSock), mClientAddr(clientAddr)
	{
		memset(&mClientAddr, 0, sizeof(SOCKADDR_IN));
	}

};

struct Test
{
	int data;
};

const int Port = 9000;
const int MYBUFSIZE = 1024;