#include "Link.h"
#include"ErrorHandler.h"


CLink::CLink(SOCKET* clientSocket, int curAreaNumber, int distinguishcode)
{
	memset((void*)this, 0x00, sizeof(CLink));
	mCurAreaNumber = curAreaNumber;
	mMyDistinguishCode = distinguishcode;
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

bool CLink::SendnMine(const Packet & packet)
{
	DWORD sendBytes;
	WSABUF wsBuf;
	const int sendSize = sizeof(Packet);
	//printf("Packet Size = %d\n", sendSize);
	printf("보내는 구별 번호 = %d\n", packet.RequestVal);
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
			ErrorHandlerPtr->TakeError(ErrorLevel::Low, ErrorCode::ErrorSendnMine, this);
			return false;
		}
	}
	//printf("보낸 byte수 : %d \n", sendBytes);
	return true;
}

bool CLink::Recvn(DWORD flags)
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
			ErrorHandlerPtr->TakeError(ErrorLevel::Low, ErrorCode::ErrorRecvn, this);
			return false;
		}
	}
	return true;
}

WSABUF CLink::GetRecvBuf()
{
	return mRecvWsaBuf;
}

int CLink::GetCurArea()
{
	return mCurAreaNumber;
}

const int CLink::GetMyDistinguishCode()
{
	return mMyDistinguishCode;
}

//const std::string & CLink::GetMyName()
//{
//	return "임시이름";
//}
