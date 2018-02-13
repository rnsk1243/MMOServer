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
	char mMessageBuf[RecvBufSize];
	WSABUF mRecvWsaBuf;
	int mCurAreaNumber;
public:
	CLink(SOCKET* clientSocket, int curAreaNumber);
	~CLink();
	SOCKET* GetClientSocket();
	bool SendnMine(const Packet & packet);
	bool Recvn(DWORD flags);
	WSABUF GetRecvBuf();
	int GetCurArea();
	const int GetMyPKNumber();
	const std::string& GetMyName();
};

