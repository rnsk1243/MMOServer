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
	HANDLE CPObj = *((HANDLE*)hCPObj);	//完了ポートオブジェクト
	DWORD receiveBytes = 0;				//されるバイトの大きさ
	LinkPtr linkPtr = nullptr;			 //recv完了されたlink
	CAreaManager* areas = nullptr;		 //area情報
	CCommandMap commandMap;				 //命令を盛り込んでいるMap
	int packetKind = WrongValue;		//あるパケットを受け取ったかどうか塀を変数
	PacketTransform packetTr;			//を受けた情報をドプオスィウルTr変数。
	PacketMessage packetM;
	const int recvSizeTr = sizeof(PacketTransform); //Transformパケットの大きさ
	const int recvSizeM = sizeof(PacketMessage);	 //Messageパケットの大きさ

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
			//printf("受けたサイズ:%d\n"、receiveBytes);
			//printf("受けたパケットの種類:%d\n"、packetKind);
			//、ディー・シーリアルする
			switch (packetKind)
			{
			case PacketKindEnum::Transform:
				memcpy_s(&packetTr, recvSizeTr, linkPtr.get()->GetRecvBuf().buf, recvSizeTr);
				//printf("送るTr種類:%d\n"、packetTr.PacketKind);
				//printf("送るTr Protocol:%d\n"、packetTr.InfoProtocol);
				//printf("送るTr区分:%d\n"、packetTr.DistinguishCode);
				linkPtr.get()->SetMyTransform(packetTr.Tr);
				areas->Broadcast(linkPtr, PacketKindEnum::Transform, &packetTr);
				break;
			case PacketKindEnum::Message:
				memcpy_s(&packetM, recvSizeM, linkPtr.get()->GetRecvBuf().buf, recvSizeM);
				//printf("受けたmessage:%s\n"、packetM.Message);
				if (packetM.InfoProtocol == ProtocolInfo::Request)
				{
					//printf("IOCP完了ポート主義linkPtr Count=%d\n"、linkPtr.use_count());
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