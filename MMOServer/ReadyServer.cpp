#include "ReadyServer.h"
#include<stdio.h>
#include<iostream>
#include<thread>
#include"ConstValue.h"

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
		// 6. 소켓종료
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
		// 6. 소켓종료
		closesocket(mListenSock);
		// Winsock End
		WSACleanup();
		return false;
	}
	MakeThreadPool();
	return true;
}

DWORD WINAPI thWork(LPVOID hCPObj)
{
	HANDLE CPObj = *((HANDLE*)hCPObj);
	DWORD receiveBytes = 0;
	//DWORD sendBytes;
	CLink* link = nullptr;
	LinkPtr linkPtr;
	CAreaManager* areas = nullptr;
	
	DWORD flags = 0;
	Packet recvPacket;
	printf("thread Start!");
	const int recvSize = sizeof(Packet);
//	Test* test = nullptr;
	while (true)
	{
		printf("-\n");
		if (GetQueuedCompletionStatus(CPObj, &receiveBytes, (PULONG_PTR)(&areas), (LPOVERLAPPED*)&linkPtr, INFINITE) == 0)
		{
			printf("Error - GetQueuedCompletionStatus(error_code : %d)\n", WSAGetLastError());
			closesocket(*(linkPtr.get()->GetClientSocket()));
			return 1;
		}
		printf("!!!");
		if (receiveBytes == 0)
		{
			printf("Error - receiveBytes == 0");
			continue;
		}
		else
		{
			//link = linkPtr.get();
			WSABUF wsabuf;
			char mMessageBuf[RecvBufSize];
			wsabuf.buf = mMessageBuf;
			wsabuf.len = RecvBufSize;
			memcpy_s(&recvPacket, recvSize, linkPtr.get()->GetRecvBuf().buf, recvSize);
			printf("packet protocol %d\n", recvPacket.InfoProtocol);
			printf("packet position.x %f\n", recvPacket.Tr.Position.x);
			printf("packet message %s\n", recvPacket.ChatMessage);
			recvPacket.InfoProtocol = ProtocolInfo::Chat;
			areas->Broadcast(linkPtr, recvPacket);
			//link->Recvn(flags);
			
			/*if (WSARecv(*(link->GetClientSocket()), &(link->mDataBuf), 1, &receiveBytes, &flags, &link->mOverlapped, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() == WSA_IO_PENDING)
				{
					printf("thread PENDING...\n");
				}
				else
				{
					printf("Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
					link = nullptr;
					continue;
				}
			}
			printf("TRACE - Receive message : %s (%d bytes)\n", link->mDataBuf.buf, link->mDataBuf.len);*/
		}
	}
}

void CReadyServer::MakeThreadPool(int actThreadNum)
{
	// 완료결과를 처리하는 객체(CP : Completion Port) 생성
	mCPObj = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, actThreadNum);
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	int threadAmount = systemInfo.dwNumberOfProcessors * 2;
	for (int i = 0; i < threadAmount; ++i)
	{
		std::thread workerThread(thWork, &mCPObj);
		workerThread.detach();
		CloseHandle(workerThread.native_handle());
	}
}

CReadyServer::CReadyServer()
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
	CLink* link = new CLink(clientSocket, StartCurArea);
	LinkPtr linkPtr(link);
	areaManager->EnterArea(StartCurArea, linkPtr);
	//Test* test = new Test(87);
	mCPObj = CreateIoCompletionPort((HANDLE)(*(clientSocket)), mCPObj, (ULONG_PTR)(areaManager), 0); // Client와 CP 연결
	return link;
}
