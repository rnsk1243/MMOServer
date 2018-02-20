#include "WorkThreadInfo.h"
#include"ErrorHandler.h"
#include"CommandMap.h"


WorkThreadInfo::WorkThreadInfo()
{
}


WorkThreadInfo::~WorkThreadInfo()
{
}

DWORD WorkThreadInfo::ThreadWork(LPVOID hCPObj)
{
	HANDLE CPObj = *((HANDLE*)hCPObj);
	DWORD receiveBytes = 0;
	LinkPtr linkPtr = nullptr;
	CAreaManager* areas = nullptr;
	CCommandMap commandMap;

	Packet recvPacket;
	const int recvSize = sizeof(Packet);
	while (true)
	{
		if (GetQueuedCompletionStatus(CPObj, &receiveBytes, (PULONG_PTR)(&areas), (LPOVERLAPPED*)&linkPtr, INFINITE) == 0)
		{
			printf("Error - GetQueuedCompletionStatus(error_code : %d)\n", WSAGetLastError());
			ErrorHandlerPtr->TakeError(ErrorLevel::Serious, ErrorCode::ErrorGetQueuedCompletionStatusZeroReturn);
			return 0;
		}
		if (receiveBytes == 0)
		{
			printf("Error - receiveBytes == 0");
			ErrorHandlerPtr->TakeError(ErrorLevel::Normal, ErrorCode::ErrorGetQueuedCompletionStatusReceiveByteZero, linkPtr.get());
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
			//printf("packet protocol %d\n", recvPacket.InfoProtocol);
			//printf("packet position.x %f\n", recvPacket.Tr.Position.x);
			//printf("packet message %s\n", recvPacket.ChatMessage);
			printf("GetMyDistinguishCode = %d\n", linkPtr.get()->GetMyDistinguishCode());
			if (recvPacket.InfoProtocol == ProtocolInfo::Request)
			{
				commandMap.Call(recvPacket.ChatMessage, linkPtr.get());
			}

			/*recvPacket.InfoProtocol = ProtocolInfo::Chat;
			areas->Broadcast(linkPtr, recvPacket);*/
			if (false == linkPtr.get()->Recvn())
			{
				areas->EraseClient(linkPtr);
			}
		}
	}
}




//
//DWORD WorkThreadInfo::ThreadWork(LPVOID hCPObj)
//{
//HANDLE CPObj = *((HANDLE*)hCPObj);
//DWORD receiveBytes = 0;
////DWORD sendBytes;
//CLink* link = nullptr;
//LinkPtr linkPtr;
//CAreaManager* areas = nullptr;
//
//DWORD flags = 0;
//Packet recvPacket;
//printf("thread Start!");
//const int recvSize = sizeof(Packet);
////	Test* test = nullptr;
//while (true)
//{
//printf("-\n");
//if (GetQueuedCompletionStatus(CPObj, &receiveBytes, (PULONG_PTR)(&areas), (LPOVERLAPPED*)&linkPtr, INFINITE) == 0)
//{
//printf("Error - GetQueuedCompletionStatus(error_code : %d)\n", WSAGetLastError());
//closesocket(*(linkPtr.get()->GetClientSocket()));
//return 1;
//}
//printf("!!!");
//if (receiveBytes == 0)
//{
//printf("Error - receiveBytes == 0");
//continue;
//}
//else
//{
////link = linkPtr.get();
//WSABUF wsabuf;
//char mMessageBuf[RecvBufSize];
//wsabuf.buf = mMessageBuf;
//wsabuf.len = RecvBufSize;
//memcpy_s(&recvPacket, recvSize, linkPtr.get()->GetRecvBuf().buf, recvSize);
//printf("packet protocol %d\n", recvPacket.InfoProtocol);
//printf("packet position.x %f\n", recvPacket.Tr.Position.x);
//printf("packet message %s\n", recvPacket.ChatMessage);
//recvPacket.InfoProtocol = ProtocolInfo::Chat;
//areas->Broadcast(linkPtr, recvPacket);
//if (false == linkPtr.get()->Recvn(flags))
//{
//areas->EraseClient(linkPtr);
//}
//
///*if (WSARecv(*(link->GetClientSocket()), &(link->mDataBuf), 1, &receiveBytes, &flags, &link->mOverlapped, NULL) == SOCKET_ERROR)
//{
//if (WSAGetLastError() == WSA_IO_PENDING)
//{
//printf("thread PENDING...\n");
//}
//else
//{
//printf("Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
//link = nullptr;
//continue;
//}
//}
//printf("TRACE - Receive message : %s (%d bytes)\n", link->mDataBuf.buf, link->mDataBuf.len);*/
//		}
//	}
//}