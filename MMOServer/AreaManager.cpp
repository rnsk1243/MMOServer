#include "AreaManager.h"



CAreaManager::CAreaManager(int areaAmount)
{
	mAreas.reserve(areaAmount);
	for (int i = 0; i < areaAmount; ++i)
	{
		AreaPtr newArea(new CArea("test", i));
		mAreas.push_back(newArea);
	}
}


CAreaManager::~CAreaManager()
{
}

void CAreaManager::EnterArea(int areaNumber, LinkPtr linkPtr)
{
	mAreas[areaNumber].get()->PushClient(linkPtr);
}

void CAreaManager::EraseClient(LinkPtr linkPtr)
{
	int myArea = linkPtr.get()->GetCurArea();
	mAreas[myArea].get()->EraseClient(linkPtr.get()->GetMyDistinguishCode());
}

void CAreaManager::Broadcast(LinkPtr link, const Packet& packet)
{
	int myArea = link.get()->GetCurArea();
	mAreas[myArea].get()->Broadcast(packet);
}
