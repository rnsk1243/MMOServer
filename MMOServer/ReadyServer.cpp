#include "ReadyServer.h"
#include<stdio.h>
#include<iostream>
#include<thread>
#include"ConstValue.h"
#include"EnumInfo.h"
#include"WorkThreadInfo.h"

const bool CReadyServer::Listen()
{
	if (WSAStartup(MAKEWORD(2, 2), &mWSAData) != 0)
	{
		printf("Error - Can not load 'winsock.dll' file\n");
		return false;
	}
	mListenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (mListenSock == INVALID_SOCKET)
	{
		printf("Error - Invalid socket\n");
		return false;
	}
	memset(&mServerAddr, 0, sizeof(SOCKADDR_IN));
	mServerAddr.sin_family = PF_INET;
	mServerAddr.sin_port = htons(Port);
	mServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if (bind(mListenSock, (struct sockaddr*)&mServerAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		printf("Error - Fail bind\n");
		//6.ソケット終了
		closesocket(mListenSock);
		// Winsock End
		WSACleanup();
		return false;
	}
	int listenResult = listen(mListenSock, 5);
//	std::cout << "Error Code3 = " << WSAGetLastError() << std::endl;
	if (listenResult == SOCKET_ERROR)
	{
		printf("Error - Fail listen\n");
		//cout << "listen(listenSocket, 5) = " << listenResult << endl;
		//cout << "Error Code4 = " << WSAGetLastError() << endl;
		//6.ソケット終了
		closesocket(mListenSock);
		// Winsock End
		WSACleanup();
		return false;
	}
	MakeThreadPool();
	return true;
}

void CReadyServer::IncreaseDisCode()
{
	++mNextDistinguishCode;
}

void CReadyServer::MakeThreadPool(int actThreadNum)
{
	//完了の結果を処理するオブジェクト(CP:Completion Port)生成
	mCPObj = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, actThreadNum);
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	int threadAmount = systemInfo.dwNumberOfProcessors * 2;
	for (int i = 0; i < threadAmount; ++i)
	{
		std::thread workerThread(WorkThreadInfo::ThreadWork, &mCPObj);
		workerThread.detach();
		CloseHandle(workerThread.native_handle());
	}
}

CReadyServer::CReadyServer():
	mNextDistinguishCode(StartDistinguishCode)
{
	if (!Listen())
		exit(1);
}


CReadyServer::~CReadyServer()
{
	closesocket(mListenSock);
	WSACleanup();
}

CLink* CReadyServer::Accept(CAreaManager* areaManager)
{
	int addrLen = sizeof(SOCKADDR_IN);
	SOCKET* clientSocket = new SOCKET();
	*clientSocket = accept(mListenSock, (struct sockaddr *)&(mServerAddr), &addrLen);
	if (*clientSocket == INVALID_SOCKET)
	{
		printf("Error - Accept Failure\n");
		closesocket(*clientSocket);
		return nullptr;
	}
	int disCode = GetDistinguishcode();
	CLink* link = new CLink(clientSocket, StartCurArea, disCode);
	printf("[%d]番 clientが接続しました。\n", link->GetMyDistinguishCode());
	LinkPtr linkPtr(link);
	areaManager->EnterArea(StartCurArea, linkPtr);
	//Test* test = new Test(87);
	mCPObj = CreateIoCompletionPort((HANDLE)(*(clientSocket)), mCPObj, (ULONG_PTR)(areaManager), 0); //ClientとCP接続
	return link;
}

int CReadyServer::GetDistinguishcode()
{
	int returnVal = mNextDistinguishCode;
	IncreaseDisCode();
	//printf("区別番号の返還:%d\n"、returnVal);
	return returnVal;
}
