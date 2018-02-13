#include "Area.h"
#include"RAII.h"

CArea::CArea(std::string areaName, int areaNum):
	mAreaName(areaName), mAreaNumber(areaNum), mAmountPeople(0)
{
}

CArea::~CArea()
{
}

bool CArea::PushClient(const LinkPtr & shared_client)
{
	mClientInfos.push_back(shared_client);
	printf("%d ¹ø Area µé¾î¿È", mAreaNumber);
	return true;
}

void CArea::Broadcast(const Packet & packet)
{
	LinkListIt linkIterBegin = mClientInfos.begin();
	int i = 0;
	for (; linkIterBegin != mClientInfos.end(); ++linkIterBegin)
	{
		//++i;
		if (false == (*linkIterBegin).get()->SendnMine(packet))
		{
			ScopeLock<CRITICALSECTION> CS(mCS);
			linkIterBegin = mClientInfos.erase(linkIterBegin);
		}
		//printf("º¸³½ È½¼ö : %d", i);
	}
}

void CArea::EraseClient(const int & clientPKnum)
{
	LinkListIt linkIterBegin = mClientInfos.begin();
	for (; linkIterBegin != mClientInfos.end(); ++linkIterBegin)
	{
		if (clientPKnum == (*linkIterBegin).get()->GetMyPKNumber())
		{
			ScopeLock<CRITICALSECTION> CS(mCS);
			linkIterBegin = mClientInfos.erase(linkIterBegin);
			return;
		}
	}
}
