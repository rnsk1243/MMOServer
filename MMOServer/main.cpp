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
	while (true)
	{
		CLink* link = server.Accept(&areaManager);
		link->Recvn();
	}
}