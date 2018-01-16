#pragma once
#include<WinSock2.h>
#include"Link.h"
class CReadyServer
{
	WSADATA mWSAData;
	SOCKET mListenSock;
	SOCKADDR_IN mServerAddr;
	HANDLE mCPObj;
	const bool Listen();
	void MakeThreadPool(int actThreadNum = 0);
public:
	CReadyServer();
	~CReadyServer();
	CLink* Accept();
};