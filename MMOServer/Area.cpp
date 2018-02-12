#include "Area.h"

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
		(*linkIterBegin).get()->SendnMine(packet);
		//printf("º¸³½ È½¼ö : %d", i);
	}
}
