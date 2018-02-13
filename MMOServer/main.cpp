#include"ReadyServer.h"
#include<list>
#include<iostream>
#include"Area.h"
#include"AreaManager.h"
#include"EnumInfo.h"
typedef std::list<LinkPtr> LinkList;

void main()
{
	CReadyServer server;
	CAreaManager areaManager(AreaAmount);
	DWORD receiveBytes = 0;
	DWORD flags = 0;
	char mMessageBuf[RecvBufSize];
	while (true)
	{
		CLink* link = server.Accept(&areaManager);
		
		MyVector3 pos(1.1f, 1.2f, 1.3f);
		MyVector3 rot(2.1f, 2.2f, 2.3f);
		MyVector3 sca(3.1f, 3.2f, 3.3f);
		MyTransform testTr(pos, rot, sca);
		Packet testPacket(ProtocolInfo::Tr, testTr);
		link->SendnMine(testPacket);
		testPacket.Tr.Position.x = 100.0f;
		link->SendnMine(testPacket);
		Packet testChatPacket(ProtocolInfo::Chat, "mmabc");
		link->SendnMine(testChatPacket);
		WSABUF wsabuf;
		wsabuf.buf = mMessageBuf;
		wsabuf.len = RecvBufSize;
		link->Recvn(0);
		//if (WSARecv(*(linkPtr.get()->GetClientSocket()), &(linkPtr.get()->mDataBuf), 1, &receiveBytes, &flags, &(linkPtr.get()->mOverlapped), NULL))
		//{
		//	if (WSAGetLastError() == WSA_IO_PENDING)
		//	{
		//		//std::cout << "receiveBytes = " << receiveBytes << std::endl;
		//		printf("Main PENDING...\n");
		//		//printf("TRACE - Receive message : %s (%d bytes)\n", linkPtr->mDataBuf.buf, linkPtr->mDataBuf.len);
		//	}
		//	else
		//	{
		//		printf("Main Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
		//		continue;
		//	}
		//}
	}
}