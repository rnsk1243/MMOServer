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
	HANDLE CPObj = *((HANDLE*)hCPObj);	//�����|�[�g�I�u�W�F�N�g
	DWORD receiveBytes = 0;				//�����o�C�g�̑傫��
	LinkPtr linkPtr = nullptr;			 //recv�������ꂽlink
	CAreaManager* areas = nullptr;		 //area���
	CCommandMap commandMap;				 //���߂𐷂荞��ł���Map
	int packetKind = WrongValue;		//����p�P�b�g���󂯎�������ǂ�������ϐ�
	PacketTransform packetTr;			//���󂯂������h�v�I�X�B�E��Tr�ϐ��B
	PacketMessage packetM;
	const int recvSizeTr = sizeof(PacketTransform); //Transform�p�P�b�g�̑傫��
	const int recvSizeM = sizeof(PacketMessage);	 //Message�p�P�b�g�̑傫��

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
			//printf("Error - receiveBytes == 0\n");
			ErrorHandlerPtr->TakeError(ErrorLevel::Normal, ErrorCode::ErrorGetQueuedCompletionStatusReceiveByteZero, linkPtr.get());
			continue;
		}
		else
		{
			memcpy_s(&packetKind, 4, linkPtr.get()->GetRecvBuf().buf, 4);
			//printf("�󂯂��T�C�Y:%d\n"�AreceiveBytes);
			//printf("�󂯂��p�P�b�g�̎��:%d\n"�ApacketKind);
			//�A�f�B�[�E�V�[���A������
			switch (packetKind)
			{
			case PacketKindEnum::Transform:
				memcpy_s(&packetTr, recvSizeTr, linkPtr.get()->GetRecvBuf().buf, recvSizeTr);
				//printf("����Tr���:%d\n"�ApacketTr.PacketKind);
				//printf("����Tr Protocol:%d\n"�ApacketTr.InfoProtocol);
				//printf("����Tr�敪:%d\n"�ApacketTr.DistinguishCode);
				linkPtr.get()->SetMyTransform(packetTr.Tr);
				areas->Broadcast(linkPtr, PacketKindEnum::Transform, &packetTr);
				break;
			case PacketKindEnum::Message:
				memcpy_s(&packetM, recvSizeM, linkPtr.get()->GetRecvBuf().buf, recvSizeM);
				//printf("�󂯂�message:%s\n"�ApacketM.Message);
				if (packetM.InfoProtocol == ProtocolInfo::Request)
				{
					//printf("IOCP�����|�[�g��`linkPtr Count=%d\n"�AlinkPtr.use_count());
					commandMap.Call(packetM.Message, linkPtr, areas, &packetM);
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