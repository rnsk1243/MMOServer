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
	int packetKind = WrongValue;
	PacketTransform packetTr;
	PacketMessage packetM;
	//Packet recvPacket;
	const int recvSizeTr = sizeof(PacketTransform);
	const int recvSizeM = sizeof(PacketMessage);
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
			//memcpy_s(&recvPacket, recvSize, linkPtr.get()->GetRecvBuf().buf, recvSize);
			memcpy_s(&packetKind, 4, linkPtr.get()->GetRecvBuf().buf, 4);
			printf("받은 사이즈 : %d\n", receiveBytes);
			printf("받은 패킷 종류 : %d\n", packetKind);
			// 디시리얼 하기
			switch (packetKind)
			{
			case PacketKindEnum::Transform:
				memcpy_s(&packetTr, recvSizeTr, linkPtr.get()->GetRecvBuf().buf, recvSizeTr);
				printf("보낼 Tr 종류 : %d\n", packetTr.PacketKind);
				printf("보낼 Tr Protocol : %d\n", packetTr.InfoProtocol);
				printf("보낼 Tr 구분 : %d\n", packetTr.DistinguishCode);
				areas->Broadcast(linkPtr, PacketKindEnum::Transform, &packetTr);
				break;
			case PacketKindEnum::Message:
				memcpy_s(&packetM, recvSizeM, linkPtr.get()->GetRecvBuf().buf, recvSizeM);
				//printf("받은 message : %s\n", packetM.Message);
				if (packetM.InfoProtocol == ProtocolInfo::Request)
				{
					commandMap.Call(packetM.Message, linkPtr.get());
					break;
				}
				if (packetM.InfoProtocol == ProtocolInfo::Chat)
				{
					areas->Broadcast(linkPtr, PacketKindEnum::Message, &packetM);
					break;
				}
				break;
			default:
				break;
			}
			//printf("packet protocol %d\n", packetTr.InfoProtocol);
			printf("packet position.x %f\n", packetTr.Tr.Position.x);
			//printf("packet message %s\n", packetTr.ChatMessage);
			//printf("GetMyDistinguishCode = %d\n", linkPtr.get()->GetMyDistinguishCode());


			/*recvPacket.InfoProtocol = ProtocolInfo::Chat;
			areas->Broadcast(linkPtr, recvPacket);*/
			linkPtr.get()->Recvn();
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