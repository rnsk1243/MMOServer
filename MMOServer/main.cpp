#include"ReadyServer.h"
#include<list>
#include<iostream>
#include"Area.h"
typedef std::list<LinkPtr> LinkList;

void main()
{
	CReadyServer server;
	DWORD receiveBytes = 0;
	DWORD flags = 0;
	CArea area("00", 0);
	while (true)
	{
		LinkPtr linkPtr(server.Accept());
		area.PushClient(linkPtr);
		MyVector3 pos(1.1f, 1.2f, 1.3f);
		MyVector3 rot(2.1f, 2.2f, 2.3f);
		MyVector3 sca(3.1f, 3.2f, 3.3f);
		MyTransform testTr(pos, rot, sca);
		Packet testPacket(ProtocolInfo::Tr, testTr);
		linkPtr.get()->SendnMine(testPacket);
		testPacket.Tr.Position.x = 100.0f;
		linkPtr.get()->SendnMine(testPacket);
		Packet testChatPacket(ProtocolInfo::Chat, "mmabc");
		linkPtr.get()->SendnMine(testChatPacket);
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