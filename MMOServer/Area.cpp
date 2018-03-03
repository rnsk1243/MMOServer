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
			// erase하면 반환값으로 다음 반복자가 반환됨. 그러므로 for문 돌때마다 ++iterator할 필요없음
			// 다만 erase를 하지 않을 경우(else문) 다음 반복자로 넘어가도록 해준다.
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

void CArea::SendNewClientNotice(const LinkPtr & newClientPtr)
{
	int newClientDisCode = newClientPtr.get()->GetMyDistinguishCode();
	PacketTransform newLinkTr(ProtocolInfo::NewLink, newClientDisCode, newClientPtr.get()->GetMyTransform());
	//printf("전입 신고 Position.y = %f\n", newClientPtr.get()->GetMyTransform().Position.y);
	// 원래 있던 사람에게 "전입 신고 하기"
	Broadcast(PacketKindEnum::Transform, &newLinkTr);
	// 원래 있던 사람들이 현재 자신의 위치를 새로 들어온 사람에게 알려주기
	LinkListIt linkIterBegin = mClientInfos.begin();
	for (; linkIterBegin != mClientInfos.end(); ++linkIterBegin)
	{
		CLink* oldLinkPtr = (*linkIterBegin).get();
		if (oldLinkPtr != nullptr)
		{
			PacketTransform oldLinkTr(ProtocolInfo::NewLink, oldLinkPtr->GetMyDistinguishCode(), oldLinkPtr->GetMyTransform());
			newClientPtr.get()->SendnMine(PacketKindEnum::Transform, &oldLinkTr);
		}
	}
}

bool CArea::PushClient(const LinkPtr & shared_client)
{
	mClientInfos.push_back(shared_client);
	//printf("%d 번 Area 들어옴\n", mAreaNumber);
	SendNewClientNotice(shared_client);
	++mAmountPeople;
	return true;
}

void CArea::Broadcast(const PacketKindEnum PacketKind, LPVOID packet)
{
	if (ErrorHandlerPtr->IsErrorLinkRemoveStart(mAreaNumber)) // 에러난 Link를 제거할 개수가 기준을 초과했나?
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

