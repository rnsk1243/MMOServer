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
	HANDLE CPObj = *((HANDLE*)hCPObj);	// 완료 포트 오브젝트
	DWORD receiveBytes = 0;				// 받는 바이트 크기
	LinkPtr linkPtr = nullptr;			// recv완료된 link
	CAreaManager* areas = nullptr;		// area 정보
	CCommandMap commandMap;				// 명령을 담고있는 Map
	int packetKind = WrongValue;		// 어떤 패킷을 받았는지 담을 변수
	PacketTransform packetTr;			// 받은 정보를 덮어씌을 Tr변수.
	PacketMessage packetM;
	const int recvSizeTr = sizeof(PacketTransform); // Transform 패킷 크기
	const int recvSizeM = sizeof(PacketMessage);	// Message 패킷 크기

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
			printf("Error - receiveBytes == 0\n");
			ErrorHandlerPtr->TakeError(ErrorLevel::Normal, ErrorCode::ErrorGetQueuedCompletionStatusReceiveByteZero, linkPtr.get());
			continue;
		}
		else
		{
			memcpy_s(&packetKind, 4, linkPtr.get()->GetRecvBuf().buf, 4);
			//printf("받은 사이즈 : %d\n", receiveBytes);
			//printf("받은 패킷 종류 : %d\n", packetKind);
			// 디시리얼 하기
			switch (packetKind)
			{
			case PacketKindEnum::Transform:
				memcpy_s(&packetTr, recvSizeTr, linkPtr.get()->GetRecvBuf().buf, recvSizeTr);
				//printf("보낼 Tr 종류 : %d\n", packetTr.PacketKind);
				//printf("보낼 Tr Protocol : %d\n", packetTr.InfoProtocol);
				//printf("보낼 Tr 구분 : %d\n", packetTr.DistinguishCode);
				linkPtr.get()->SetMyTransform(packetTr.Tr);
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
			linkPtr.get()->Recvn();
		}
	}
}