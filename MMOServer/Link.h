#pragma once
#include<memory>
#include<WinSock2.h>
#include"ConstValue.h"
#include"EnumInfo.h"
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
	void Sendn(WSABUF wsaBuf, LPVOID packet);
	bool mIsErrorState;
	MyTransform mCurMyTransform;
public:
	CLink(SOCKET* clientSocket, int curAreaNumber, int distinguishcode);
	~CLink();
	SOCKET* GetClientSocket();
	//bool SendnMine(const Packet & packet);
	void SendnMine(const PacketKindEnum PacketKind, LPVOID packet);
	void Recvn(DWORD flags = 0);
	WSABUF GetRecvBuf();
	int GetCurArea();
	const int GetMyDistinguishCode();
	void SetErrorState();
	bool IsErrorClient();
	void SetMyTransform(const MyTransform& tr);
	const MyTransform& GetMyTransform();
	//const std::string& GetMyName();
};

