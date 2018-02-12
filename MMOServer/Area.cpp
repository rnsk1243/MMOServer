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
	printf("%d �� Area ����", mAreaNumber);
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
		//printf("���� Ƚ�� : %d", i);
	}
}
