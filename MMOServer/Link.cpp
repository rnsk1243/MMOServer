#include "Link.h"



CLink::CLink(SOCKET* clientSocket)
{
	memset((void*)this, 0x00, sizeof(CLink));
	mClientSocket = clientSocket;
	memset(&mClientAddr, 0, sizeof(SOCKADDR_IN));
	mDataBuf.buf = mMessageBuf;
	mDataBuf.len = MYBUFSIZE;
}

CLink::~CLink()
{
	printf("Link ªË¡¶");
	closesocket(*mClientSocket);
	delete mClientSocket;
}

SOCKET * CLink::GetClientSocket()
{
	return mClientSocket;
}
