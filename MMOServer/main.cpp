#include"ReadyServer.h"
#include<list>
#include<iostream>
#include"Area.h"
#include"AreaManager.h"
#include"EnumInfo.h"
typedef std::list<LinkPtr> LinkList;

void main()
{
	printf("IOCPを利用したゲームサーバ\n");
	CReadyServer server;
	CAreaManager areaManager(AreaAmount);
	while (true)
	{
		CLink* link = server.Accept(&areaManager);
		link->Recvn();
	}
}