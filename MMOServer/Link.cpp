#include "Link.h"
#include"ErrorHandler.h"


void CLink::Sendn(WSABUF wsaBuf, LPVOID packet)
{
	DWORD sendBytes;
	memcpy_s(wsaBuf.buf, wsaBuf.len, packet, wsaBuf.len);
	if (WSASend(*mClientSocket, &wsaBuf, 1, &sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			//printf("SendnMine PENDING...\n");
		}
		else
		{
			printf("Error - Fail WSASend(error_code : %d)\n", WSAGetLastError());
			//例外処理が必要。
			ErrorHandlerPtr->TakeError(ErrorLevel::Low, ErrorCode::ErrorSendnMine, this);
		}
	}
	//printf("送ったbyte数:%d\n"、sendBytes);
}

CLink::CLink(SOCKET* clientSocket, int curAreaNumber, int distinguishcode)
{
	memset((void*)this, 0x00, sizeof(CLink));
	mIsErrorState = false;
	mCurAreaNumber = curAreaNumber;
	mMyDistinguishCode = distinguishcode;
	mClientSocket = clientSocket;
	memset(&mClientAddr, 0, sizeof(SOCKADDR_IN));
	mRecvWsaBuf.buf = mMessageBuf;
	mRecvWsaBuf.len = RecvBufSize;
	mCurMyTransform.Position.y = 1.0f;
	mCurMyTransform.Scale.x = 1.0f; mCurMyTransform.Scale.y = 1.0f; mCurMyTransform.Scale.z = 1.0f;
}

CLink::~CLink()
{
	printf("Link削除\n");
	closesocket(*mClientSocket);
	delete mClientSocket;
}

SOCKET * CLink::GetClientSocket()
{
	return mClientSocket;
}
//
//bool CLink::SendnMine(const Packet & packet)
//{
//	DWORD sendBytes;
//	WSABUF wsBuf;
//	const int sendSize = sizeof(Packet);
//	//printf("Packet Size = %d\n", sendSize);
// printf("送った区別番号=%d\n"、packet.RequestVal);
//	char sendTemp[sendSize];
//	wsBuf.buf = sendTemp; wsBuf.len = sendSize;
//
//	memcpy_s(wsBuf.buf, sendSize, (void*)&packet, sendSize);
//	if (WSASend(*mClientSocket, &wsBuf, 1, &sendBytes, 0, NULL, NULL) == SOCKET_ERROR)
//	{
//		if (WSAGetLastError() == WSA_IO_PENDING)
//		{
//			printf("SendnMine PENDING...\n");
//		}
//		else
//		{
//			printf("Error - Fail WSASend(error_code : %d)\n", WSAGetLastError());
//			  //例外処理が必要。
//			ErrorHandlerPtr->TakeError(ErrorLevel::Low, ErrorCode::ErrorSendnMine, this);
//			return false;
//		}
//	}
//	//printf("送ったbyte数:%d\n"、sendBytes);
//	return true;
//}

void CLink::SendnMine(const PacketKindEnum PacketKind, LPVOID packet)
{
	if (mIsErrorState == true)
		return;
//	PacketTransform* lpTr = nullptr; PacketMessage* lpM = nullptr;
	//DWORD sendBytes;
	WSABUF wsBuf;
	switch (PacketKind)
	{//char動的割り振ること嫌いで各caseの中に重複されるコードがある。
	case PacketKindEnum::Transform:
		//printf("Transformを送る\n");
//		lpTr = (PacketTransform*)packet;
		char sendTempTr[SendSizeTransform];
		wsBuf.buf = sendTempTr; wsBuf.len = SendSizeTransform;
		Sendn(wsBuf, packet);
		break;
	case PacketKindEnum::Message:
//		lpM = (PacketMessage*)packet;
		char sendTempM[SendSizeMessage];
		wsBuf.buf = sendTempM; wsBuf.len = SendSizeMessage;
		Sendn(wsBuf, packet);
		break;
	case PacketKindEnum::DeleteObjEnum:
		char sendTempDelete[SendSizeDeleteObj];
		wsBuf.buf = sendTempDelete; wsBuf.len = SendSizeDeleteObj;
		Sendn(wsBuf, packet);
		break;
	default:
		break;
	}
}

void CLink::Recvn(DWORD flags)
{
	if (mIsErrorState == true)
		return;
	DWORD receiveBytes;
	//const int recvSize = sizeof(Packet);
	if (WSARecv(*mClientSocket, &mRecvWsaBuf, 1, &receiveBytes, &flags, &mOverlapped, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			//printf("Recvn PENDING...\n");
			
		}
		else
		{
			printf("Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
			//例外処理が必要。
			ErrorHandlerPtr->TakeError(ErrorLevel::Low, ErrorCode::ErrorRecvn, this);
		}
	}
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

void CLink::SetErrorState()
{
	mIsErrorState = true;
}

bool CLink::IsErrorClient()
{
	return mIsErrorState;
}

void CLink::SetMyTransform(const MyTransform & tr)
{
	mCurMyTransform = tr;
}

const MyTransform & CLink::GetMyTransform()
{
	return mCurMyTransform;
}

void CLink::UpdateMyAreaNumber(int newAreaNumber)
{
	mCurAreaNumber = newAreaNumber;
}

//const std::string & CLink::GetMyName()
//{
// return"臨時の名前";
//}
