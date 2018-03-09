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

void CArea::SendDeleteCommand(const LinkPtr & linkPtr)
{
	// 모든 사람에게 날 지우라고 보냄
	PacketDeleteObj myDeletePacket(ProtocolInfo::DeleteObj, WrongValue); // 모든 사람들아 나 지워!
	myDeletePacket.EraseObjDiscodeArray[0] = linkPtr.get()->GetMyDistinguishCode();
	//printf("%d 주민들아 %d 지워 \n",mAreaNumber, myDeletePacket.EraseObjDiscodeArray[0]);
	Broadcast(PacketKindEnum::DeleteObjEnum, &myDeletePacket);
	// 나에게 모든 사람을 지우라고 보냄
	PacketDeleteObj everyDeletePacket(ProtocolInfo::DeleteObj, WrongValue); // 내가 모든 사람 클라이언트 지워!

	int index = 0;
	LinkListIt linkIterBegin = mClientInfos.begin();
	for (; linkIterBegin != mClientInfos.end(); ++linkIterBegin)
	{
		CLink* oldLinkPtr = (*linkIterBegin).get();
		if (oldLinkPtr != nullptr)
		{
			everyDeletePacket.EraseObjDiscodeArray[index] = oldLinkPtr->GetMyDistinguishCode();
			++index;
			if (index >= SendEraseObjArraySize)
			{
				linkPtr.get()->SendnMine(PacketKindEnum::DeleteObjEnum, &everyDeletePacket);
				index = 0;
			}
		}
	}
	linkPtr.get()->SendnMine(PacketKindEnum::DeleteObjEnum, &everyDeletePacket); // 나머지 마지막 까지 보냄.
}


CArea::CArea(std::string areaName, int areaNum):
	mAreaName(areaName), mAreaNumber(areaNum), mAmountPeople(0), mErrorLinkAmount(0), mIsRemoveErrorLink(false)
{
}

CArea::~CArea()
{
}

void CArea::SearchEndRemoveErrorLink()
{
	//printf("날 깨우다니 청소를 시작해 볼까...AreaNumber = %d\n", mAreaNumber);
	ScopeLock<CRITICALSECTION> CS(mCS);
	mIsRemoveErrorLink = true;
	LinkListIt linkListIterBegin = mClientInfos.begin();

	PacketDeleteObj myDeletePacket(ProtocolInfo::DeleteObj, WrongValue); // 모든 사람들아 나 지워!
	int index = 0;
	for (; linkListIterBegin != mClientInfos.end();)
	{
		// 소켓 에러 혹은 Area 이동해서 이곳 Area번호와 일치하지 않으면 삭제
		if ((*linkListIterBegin).get()->IsErrorClient() == true || (*linkListIterBegin).get()->GetCurArea() != mAreaNumber)
		{
			if ((*linkListIterBegin).get()->IsErrorClient() == true)
			{
				myDeletePacket.EraseObjDiscodeArray[index] = (*linkListIterBegin).get()->GetMyDistinguishCode();
				++index;
			}

			//mClientInfos.remove((*linkListIterBegin));
			// erase하면 반환값으로 다음 반복자가 반환됨. 그러므로 for문 돌때마다 ++iterator할 필요없음
			// 다만 erase를 하지 않을 경우(else문) 다음 반복자로 넘어가도록 해준다.
			//printf("SearchEndRemoveErrorLink count = %d\n", (*linkListIterBegin).use_count());
			linkListIterBegin = mClientInfos.erase(linkListIterBegin);
			--mAmountPeople;
			//ErrorHandlerPtr->DecreaseErrorLink(mAreaNumber);
			ErrorHandlerPtr->GetDeletePlayerDisCode(mAreaNumber);
			if (index >= SendEraseObjArraySize)
			{
				break;
			}
		}
		else
		{
			++linkListIterBegin;
		}
	}
	
	// 에러 없는 모든 사람에게 날 지우라고 보냄
	mIsRemoveErrorLink = false;
	Broadcast(PacketKindEnum::DeleteObjEnum, &myDeletePacket);

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
			//printf("생성 위치 : %d", oldLinkPtr->GetMyTransform().Position.x);
			PacketTransform oldLinkTr(ProtocolInfo::NewLink, oldLinkPtr->GetMyDistinguishCode(), oldLinkPtr->GetMyTransform());
			newClientPtr.get()->SendnMine(PacketKindEnum::Transform, &oldLinkTr);
		}
	}
}


bool CArea::PushClient(const LinkPtr & shared_client)
{
	if (ErrorHandlerPtr->IsErrorLinkRemoveStart(mAreaNumber)) // 에러난 Link를 제거할 개수가 기준을 초과했나?
	{
		SearchEndRemoveErrorLink();
	}
	mClientInfos.push_back(shared_client);
	shared_client.get()->UpdateMyAreaNumber(mAreaNumber);
	SendNewClientNotice(shared_client);
	++mAmountPeople;
	return true;
}

const LinkPtr CArea::GetLinkPtr(int disCode)
{
	LinkListIt linkIterBegin = mClientInfos.begin();
	for (; linkIterBegin != mClientInfos.end(); ++linkIterBegin)
	{
		if ((*linkIterBegin) != nullptr)
		{
			if (disCode == (*linkIterBegin).get()->GetMyDistinguishCode())
			{
				return (*linkIterBegin);
			}
		}
	}
	return nullptr;
}

void CArea::Broadcast(const PacketKindEnum PacketKind, LPVOID packet)
{
	if (ErrorHandlerPtr->IsErrorLinkRemoveStart(mAreaNumber)) // 에러난 Link를 제거할 개수가 기준을 초과했나?
	{
		SearchEndRemoveErrorLink();
	}
	if (!mIsRemoveErrorLink) // IsErrorLinkRemoveStart 함수가 호출 중이 아니면.
	{
		LinkListIt linkIterBegin = mClientInfos.begin();
		for (; linkIterBegin != mClientInfos.end(); ++linkIterBegin)
		{
			if ((*linkIterBegin) != nullptr)
			{
				(*linkIterBegin).get()->SendnMine(PacketKind, packet);
			}
		}
	}
}

