#include "Link.h"



CLink::CLink(SOCKET* clientSocket)
{
	memset((void*)this, 0x00, sizeof(CLink));
	mClientSocket = clientSocket;
	memset(&mClientAddr, 0, sizeof(SOCKADDR_IN));
	mDataBuf.buf = mMessageBuf;
	mDataBuf.len = MYBUFSIZE;
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
	const int sendSize = sizeof(packet);
	printf("Packet Size = %zd\n", sendSize);
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
			printf("Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
			// 예외처리 필요.
		}
	}
	printf("보낸 byte수 : %d \n", sendBytes);
}
