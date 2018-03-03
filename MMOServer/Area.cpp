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
	printf("�� ����ٴ� û�Ҹ� ������ ����...\n");
	ScopeLock<CRITICALSECTION> CS(mCS);
	LinkListIt linkListIterBegin = mClientInfos.begin();
	for (; linkListIterBegin != mClientInfos.end();)
	{
		if ((*linkListIterBegin).get()->IsErrorClient() == true)
		{
			// erase�ϸ� ��ȯ������ ���� �ݺ��ڰ� ��ȯ��. �׷��Ƿ� for�� �������� ++iterator�� �ʿ����
			// �ٸ� erase�� ���� ���� ���(else��) ���� �ݺ��ڷ� �Ѿ���� ���ش�.
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
			PacketTransform oldLinkTr(ProtocolInfo::NewLink, oldLinkPtr->GetMyDistinguishCode(), oldLinkPtr->GetMyTransform());
			newClientPtr.get()->SendnMine(PacketKindEnum::Transform, &oldLinkTr);
		}
	}
}

bool CArea::PushClient(const LinkPtr & shared_client)
{
	mClientInfos.push_back(shared_client);
	//printf("%d �� Area ����\n", mAreaNumber);
	SendNewClientNotice(shared_client);
	++mAmountPeople;
	return true;
}

void CArea::Broadcast(const PacketKindEnum PacketKind, LPVOID packet)
{
	if (ErrorHandlerPtr->IsErrorLinkRemoveStart(mAreaNumber)) // ������ Link�� ������ ������ ������ �ʰ��߳�?
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

		//printf("���� Ƚ�� : %d", i);
	}
}

