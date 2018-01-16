#include"ReadyServer.h"
#include<list>
#include<iostream>
typedef std::list<LinkPtr> LinkList;

void main()
{
	CReadyServer server;
	DWORD receiveBytes = 0;
	DWORD flags = 0;
	LinkList clients;
	while (true)
	{
		LinkPtr linkPtr(server.Accept());
		clients.push_back(linkPtr);
		if (WSARecv(*(linkPtr.get()->GetClientSocket()), &(linkPtr.get()->mDataBuf), 1, &receiveBytes, &flags, &(linkPtr.get()->mOverlapped), NULL))
		{
			if (WSAGetLastError() == WSA_IO_PENDING)
			{
				//std::cout << "receiveBytes = " << receiveBytes << std::endl;
				printf("Main PENDING...\n");
				//printf("TRACE - Receive message : %s (%d bytes)\n", linkPtr->mDataBuf.buf, linkPtr->mDataBuf.len);
			}
			else
			{
				printf("Main Error - Fail WSARecv(error_code : %d)\n", WSAGetLastError());
				continue;
			}
		}
	}
}