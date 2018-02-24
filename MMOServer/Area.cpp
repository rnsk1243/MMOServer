#include "Area.h"
#include"RAII.h"
#include"ErrorHandler.h"

void CArea::EraseClient(const int & clientPKnum)
{
	LinkListIt linkIterBegin = mClientInfos.begin();
	for (; linkIterBegin != mClientInfos.end(); ++linkIterBegin)
	{
		if (clientPKnum == (*linkIterBegin).get()->GetMyDistinguishCode())
		{
			ScopeLock<CRITICALSECTION> CS(mCS);
			linkIterBegin = mClientInfos.erase(linkIterBegin);
			return;
		}
	}
}

CArea::CArea(std::string areaName, int areaNum):
	mAreaName(areaName), mAreaNumber(areaNum), mAmountPeople(0)
{
}

CArea::~CArea()
{
}

void CArea::SearchEndRemoveErrorLink()
{
	LinkListIt linkListIterBegin = mClientInfos.begin();
	for (; linkListIterBegin != mClientInfos.end(); ++linkListIterBegin)
	{
		if ((*linkListIterBegin).get()->IsErrorClient() == true)
		{
			ScopeLock<CRITICALSECTION> CS(mCS);
			linkListIterBegin = mClientInfos.erase(linkListIterBegin);
		}
	}

}

bool CArea::PushClient(const LinkPtr & shared_client)
{
	mClientInfos.push_back(shared_client);
	printf("%d �� Area ����", mAreaNumber);
	return true;
}

void CArea::Broadcast(const PacketKindEnum PacketKind, LPVOID packet)
{
	LinkListIt linkIterBegin = mClientInfos.begin();
	for (; linkIterBegin != mClientInfos.end(); ++linkIterBegin)
	{
		//++i;
		if ((*linkIterBegin).get() != nullptr)
		{
			(*linkIterBegin).get()->SendnMine(PacketKind, packet);
		}

		//printf("���� Ƚ�� : %d", i);
	}
}

