#pragma once
#include<WinSock2.h>
#include"Link.h"
#include"AreaManager.h"

class CReadyServer
{
	WSADATA mWSAData;
	SOCKET mListenSock;
	SOCKADDR_IN mServerAddr;
	HANDLE mCPObj;
	void MakeThreadPool(int actThreadNum = 0);
	const bool Listen();
	int mNextDistinguishCode;
	void IncreaseDisCode();
	int GetDistinguishcode();
public:
	CReadyServer();
	~CReadyServer();
	CLink* Accept(CAreaManager* areaManager);
};