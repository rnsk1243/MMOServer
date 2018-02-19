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
	int mMyDistinguishCode;
public:
	CLink(SOCKET* clientSocket, int curAreaNumber, int distinguishcode);
	~CLink();
	SOCKET* GetClientSocket();
	bool SendnMine(const Packet & packet);
	bool Recvn(DWORD flags = 0);
	WSABUF GetRecvBuf();
	int GetCurArea();
	const int GetMyDistinguishCode();
	//const std::string& GetMyName();
};

