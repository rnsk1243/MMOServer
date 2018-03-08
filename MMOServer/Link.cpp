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
			// 예외처리 필요.
			ErrorHandlerPtr->TakeError(ErrorLevel::Low, ErrorCode::ErrorSendnMine, this);
		}
	}
	//printf("보낸 byte수 : %d \n", sendBytes);
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
	printf("Link 삭제\n");
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
//	printf("보내는 구별 번호 = %d\n", packet.RequestVal);
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
//			// 예외처리 필요.
//			ErrorHandlerPtr->TakeError(ErrorLevel::Low, ErrorCode::ErrorSendnMine, this);
//			return false;
//		}
//	}
//	//printf("보낸 byte수 : %d \n", sendBytes);
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
	{// char 동적할당 하기 싫어서 각case안에 중복되는 코드가 있다..
	case PacketKindEnum::Transform:
		//printf("Transform 보내기\n");
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
			// 예외처리 필요.
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

//const std::string & CLink::GetMyName()
//{
//	return "임시이름";
//}
