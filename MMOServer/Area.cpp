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
	// ��� ������� �� ������ ����
	PacketDeleteObj myDeletePacket(ProtocolInfo::DeleteObj, WrongValue); // ��� ������ �� ����!
	myDeletePacket.EraseObjDiscodeArray[0] = linkPtr.get()->GetMyDistinguishCode();
	//printf("%d �ֹε�� %d ���� \n",mAreaNumber, myDeletePacket.EraseObjDiscodeArray[0]);
	Broadcast(PacketKindEnum::DeleteObjEnum, &myDeletePacket);
	// ������ ��� ����� ������ ����
	PacketDeleteObj everyDeletePacket(ProtocolInfo::DeleteObj, WrongValue); // ���� ��� ��� Ŭ���̾�Ʈ ����!

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
	linkPtr.get()->SendnMine(PacketKindEnum::DeleteObjEnum, &everyDeletePacket); // ������ ������ ���� ����.
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
	//printf("�� ����ٴ� û�Ҹ� ������ ����...AreaNumber = %d\n", mAreaNumber);
	ScopeLock<CRITICALSECTION> CS(mCS);
	mIsRemoveErrorLink = true;
	LinkListIt linkListIterBegin = mClientInfos.begin();

	PacketDeleteObj myDeletePacket(ProtocolInfo::DeleteObj, WrongValue); // ��� ������ �� ����!
	int index = 0;
	for (; linkListIterBegin != mClientInfos.end();)
	{
		// ���� ���� Ȥ�� Area �̵��ؼ� �̰� Area��ȣ�� ��ġ���� ������ ����
		if ((*linkListIterBegin).get()->IsErrorClient() == true || (*linkListIterBegin).get()->GetCurArea() != mAreaNumber)
		{
			if ((*linkListIterBegin).get()->IsErrorClient() == true)
			{
				myDeletePacket.EraseObjDiscodeArray[index] = (*linkListIterBegin).get()->GetMyDistinguishCode();
				++index;
			}

			//mClientInfos.remove((*linkListIterBegin));
			// erase�ϸ� ��ȯ������ ���� �ݺ��ڰ� ��ȯ��. �׷��Ƿ� for�� �������� ++iterator�� �ʿ����
			// �ٸ� erase�� ���� ���� ���(else��) ���� �ݺ��ڷ� �Ѿ���� ���ش�.
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
	
	// ���� ���� ��� ������� �� ������ ����
	mIsRemoveErrorLink = false;
	Broadcast(PacketKindEnum::DeleteObjEnum, &myDeletePacket);

}

void CArea::SendNewClientNotice(const LinkPtr & newClientPtr)
{
	int newClientDisCode = newClientPtr.get()->GetMyDistinguishCode();
	PacketTransform newLinkTr(ProtocolInfo::NewLink, newClientDisCode, newClientPtr.get()->GetMyTransform());
	//printf("���� �Ű� Position.y = %f\n", newClientPtr.get()->GetMyTransform().Position.y);
	// ���� �ִ� ������� "���� �Ű� �ϱ�"
	Broadcast(PacketKindEnum::Transform, &newLinkTr);
	// ���� �ִ� ������� ���� �ڽ��� ��ġ�� ���� ���� ������� �˷��ֱ�
	LinkListIt linkIterBegin = mClientInfos.begin();
	for (; linkIterBegin != mClientInfos.end(); ++linkIterBegin)
	{
		CLink* oldLinkPtr = (*linkIterBegin).get();
		if (oldLinkPtr != nullptr)
		{
			//printf("���� ��ġ : %d", oldLinkPtr->GetMyTransform().Position.x);
			PacketTransform oldLinkTr(ProtocolInfo::NewLink, oldLinkPtr->GetMyDistinguishCode(), oldLinkPtr->GetMyTransform());
			newClientPtr.get()->SendnMine(PacketKindEnum::Transform, &oldLinkTr);
		}
	}
}


bool CArea::PushClient(const LinkPtr & shared_client)
{
	if (ErrorHandlerPtr->IsErrorLinkRemoveStart(mAreaNumber)) // ������ Link�� ������ ������ ������ �ʰ��߳�?
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
	if (ErrorHandlerPtr->IsErrorLinkRemoveStart(mAreaNumber)) // ������ Link�� ������ ������ ������ �ʰ��߳�?
	{
		SearchEndRemoveErrorLink();
	}
	if (!mIsRemoveErrorLink) // IsErrorLinkRemoveStart �Լ��� ȣ�� ���� �ƴϸ�.
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

