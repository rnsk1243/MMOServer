#pragma once
#include<memory>
#include<WinSock2.h>
#include"ConstValue.h"
class CLink;
typedef std::shared_ptr<CLink> LinkPtr;

class CLink
{
public:
	WSAOVERLAPPED mOverlapped;
private:
	SOCKADDR_IN mClientAddr;
	SOCKET* mClientSocket;
	char mMessageBuf[MYBUFSIZE];
public:
	CLink(SOCKET* clientSocket);
	~CLink();
	SOCKET* GetClientSocket();
	WSABUF mDataBuf;
};

