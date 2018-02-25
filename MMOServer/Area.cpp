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
	mAreaName(areaName), mAreaNumber(areaNum), mAmountPeople(0), mErrorLinkAmount(0)
{
}

CArea::~CArea()
{
}

void CArea::SearchEndRemoveErrorLink()
{
	printf("날 깨우다니 청소를 시작해 볼까...\n");
	ScopeLock<CRITICALSECTION> CS(mCS);
	LinkListIt linkListIterBegin = mClientInfos.begin();
	for (; linkListIterBegin != mClientInfos.end();)
	{
		if ((*linkListIterBegin).get()->IsErrorClient() == true)
		{
			linkListIterBegin = mClientInfos.erase(linkListIterBegin);
			--mAmountPeople;
			ErrorHandlerPtr->DecreaseErrorLink(mAreaNumber);
		}
		else
		{
			++linkListIterBegin;
		}
	}

}

bool CArea::PushClient(const LinkPtr & shared_client)
{
	mClientInfos.push_back(shared_client);
	printf("%d 번 Area 들어옴\n", mAreaNumber);
	++mAmountPeople;
	return true;
}

void CArea::Broadcast(const PacketKindEnum PacketKind, LPVOID packet)
{
	if (ErrorHandlerPtr->IsErrorLinkRemoveStart(mAreaNumber))
	{
		SearchEndRemoveErrorLink();
	}

	LinkListIt linkIterBegin = mClientInfos.begin();
	for (; linkIterBegin != mClientInfos.end(); ++linkIterBegin)
	{
		//++i;
		if ((*linkIterBegin).get() != nullptr)
		{
			(*linkIterBegin).get()->SendnMine(PacketKind, packet);
		}

		//printf("보낸 횟수 : %d", i);
	}
}

