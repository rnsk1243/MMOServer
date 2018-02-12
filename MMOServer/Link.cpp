#include "Link.h"



CLink::CLink(SOCKET* clientSocket, int curAreaNumber):
	mCurAreaNumber(curAreaNumber)
{
	memset((void*)this, 0x00, sizeof(CLink));
	mClientSocket = clientSocket;
	memset(&mClientAddr, 0, sizeof(SOCKADDR_IN));
	mRecvWsaBuf.buf = mMessageBuf;
	mRecvWsaBuf.len = RecvBufSize;
}

CLink::~CLink()
{
	printf("Link 삭제");
	closesocket(*mClientSocket);
	delete mClientSocket;
}

SOCKET * CLink::GetClientSocket()
{
	return mClientSocket;
}

void CLink::SendnMine(const Packet & packet)
{
	DWORD sendBytes;
	WSABUF wsBuf;
	const int sendSize = sizeof(Packet);
	printf("Packet Size = %d\n", sendSize);
	char sendTemp[sendSize];
	wsBuf.buf = sendTemp; wsBuf.len = sendSize;

	memcpy_s(wsBuf.buf, sendSize, (void*)&packet, sendSize);
	if (WSASend(*mClientSocket, &wsBuf, 1, &sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			printf("SendnMine PENDING...\n");
		}
		else
		{
			printf("Error - Fail WSASend(error_code : %d)\n", WSAGetLastError());
			// 예외처리 필요.
			return;
		}
	}
	printf("보낸 byte수 : %d \n", sendBytes);
}

void CLink::Recvn(DWORD flags)
{
	DWORD receiveBytes;
	const int recvSize = sizeof(Packet);
	if (WSARecv(*mClientSocket, &mRecvWsaBuf, 1, &receiveBytes, &flags, &mOverlapped, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			printf("Recvn PENDING...\n");
			
		}
		else
		{
			printf("Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
			// 예외처리 필요.
		}
	}
	//if (WSAGetLastError() != WSA_IO_PENDING)
	//{
	//	memcpy_s(&packet, recvSize, mRecvWsaBuf.buf, recvSize);
	//	printf("packet protocol %d\n", packet.InfoProtocol);
	//	printf("packet position.x %f\n", packet.Tr.Position.x);
	//	printf("packet message %s\n", packet.ChatMessage);
	//}
	
}

WSABUF CLink::GetRecvBuf()
{
	return mRecvWsaBuf;
}

int CLink::GetCurArea()
{
	return mCurAreaNumber;
}
