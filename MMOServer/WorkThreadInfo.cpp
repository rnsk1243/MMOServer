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
	HANDLE CPObj = *((HANDLE*)hCPObj);	// �Ϸ� ��Ʈ ������Ʈ
	DWORD receiveBytes = 0;				// �޴� ����Ʈ ũ��
	LinkPtr linkPtr = nullptr;			// recv�Ϸ�� link
	CAreaManager* areas = nullptr;		// area ����
	CCommandMap commandMap;				// ����� ����ִ� Map
	int packetKind = WrongValue;		// � ��Ŷ�� �޾Ҵ��� ���� ����
	PacketTransform packetTr;			// ���� ������ ����� Tr����.
	PacketMessage packetM;
	const int recvSizeTr = sizeof(PacketTransform); // Transform ��Ŷ ũ��
	const int recvSizeM = sizeof(PacketMessage);	// Message ��Ŷ ũ��

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
			//printf("���� ������ : %d\n", receiveBytes);
			//printf("���� ��Ŷ ���� : %d\n", packetKind);
			// ��ø��� �ϱ�
			switch (packetKind)
			{
			case PacketKindEnum::Transform:
				memcpy_s(&packetTr, recvSizeTr, linkPtr.get()->GetRecvBuf().buf, recvSizeTr);
				//printf("���� Tr ���� : %d\n", packetTr.PacketKind);
				//printf("���� Tr Protocol : %d\n", packetTr.InfoProtocol);
				//printf("���� Tr ���� : %d\n", packetTr.DistinguishCode);
				linkPtr.get()->SetMyTransform(packetTr.Tr);
				areas->Broadcast(linkPtr, PacketKindEnum::Transform, &packetTr);
				break;
			case PacketKindEnum::Message:
				memcpy_s(&packetM, recvSizeM, linkPtr.get()->GetRecvBuf().buf, recvSizeM);
				//printf("���� message : %s\n", packetM.Message);
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