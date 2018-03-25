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
	// すべての人に私のクリアと送信
	//すべての人に日消しと送ったこと
	PacketDeleteObj myDeletePacket(ProtocolInfo::DeleteObj, WrongValue);//すべての人たち、私、消して!
	myDeletePacket.EraseObjDiscodeArray[0] = linkPtr.get()->GetMyDistinguishCode();
	//printf("%dの住民たちよ%d消して\n"、mAreaNumber、myDeletePacket.EraseObjDiscodeArray[0]);
	Broadcast(PacketKindEnum::DeleteObjEnum, &myDeletePacket);
	//私にすべての人を消しと送ったこと
	PacketDeleteObj everyDeletePacket(ProtocolInfo::DeleteObj, WrongValue);//私がすべての人クライアント消して!

	int index = 0;
	LinkListIt linkIterBegin = mClientInfos.begin();
	for (; linkIterBegin != mClientInfos.end(); ++linkIterBegin)
	{
		CLink*oldLinkPtr = (*linkIterBegin).get();
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
	linkPtr.get()->SendnMine(PacketKindEnum::DeleteObjEnum, &everyDeletePacket);//残りの最後まで送ったこと。
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
	//printf("私を起こしなんて掃除を始めて見るか...AreaNumber = %d\n", mAreaNumber);
	ScopeLock<CRITICALSECTION> CS(mCS);
	mIsRemoveErrorLink = true;
	LinkListIt linkListIterBegin = mClientInfos.begin();

	PacketDeleteObj myDeletePacket(ProtocolInfo::DeleteObj, WrongValue); //すべての人たち、私、消して!
	int index = 0;
	for (; linkListIterBegin != mClientInfos.end();)
	{
		// ソケットエラーまたはArea移動してここAreaの番号と一致しなければ削除
		if ((*linkListIterBegin).get()->IsErrorClient() == true || (*linkListIterBegin).get()->GetCurArea() != mAreaNumber)
		{
			if ((*linkListIterBegin).get()->IsErrorClient() == true)
			{
				myDeletePacket.EraseObjDiscodeArray[index] = (*linkListIterBegin).get()->GetMyDistinguishCode();
				++index;
			}

			//mClientInfos.remove(*linkListIterBegin));
			//eraseすると、返り値で次の反復者が返還されたこと。。。だから、for文回るたびに、++iteratorは無用
			//ただ、eraseをしない場合(else文)次のレプリケーターに渡るようにしている。
			//printf("SearchEndRemoveErrorLink count=%d\n"、(*linkListIterBegin)。use_count());
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
	
	//エラーのないすべての人に日消しと送ったこと
	mIsRemoveErrorLink = false;
	Broadcast(PacketKindEnum::DeleteObjEnum, &myDeletePacket);

}

void CArea::SendNewClientNotice(const LinkPtr & newClientPtr)
{
	int newClientDisCode = newClientPtr.get()->GetMyDistinguishCode();
	PacketTransform newLinkTr(ProtocolInfo::NewLink, newClientDisCode, newClientPtr.get()->GetMyTransform());
	//printf("転入申告Position.y=%f\n"、newClientPtr.get()->GetMyTransform()。Position.y);
	//元々あった人に"転入申告する"
	Broadcast(PacketKindEnum::Transform, &newLinkTr);
	//元々あった人たちが現在、自分の位置を新たに入ってきた人に教え
	LinkListIt linkIterBegin = mClientInfos.begin();
	for (; linkIterBegin != mClientInfos.end(); ++linkIterBegin)
	{
		CLink* oldLinkPtr = (*linkIterBegin).get();
		if (oldLinkPtr != nullptr)
		{
			//printf("生成の位置:%d"、oldLinkPtr->GetMyTransform()。Position.x);
			PacketTransform oldLinkTr(ProtocolInfo::NewLink, oldLinkPtr->GetMyDistinguishCode(), oldLinkPtr->GetMyTransform());
			newClientPtr.get()->SendnMine(PacketKindEnum::Transform, &oldLinkTr);
		}
	}
}


bool CArea::PushClient(const LinkPtr & shared_client)
{
	if (ErrorHandlerPtr->IsErrorLinkRemoveStart(mAreaNumber))//エラーしたLinkを除去する改修が基準を超過したのか?
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
	if (ErrorHandlerPtr->IsErrorLinkRemoveStart(mAreaNumber)) //エラーしたLinkを除去する改修が基準を超過したのか?
	{
		SearchEndRemoveErrorLink();
	}
	if (!mIsRemoveErrorLink) //IsErrorLinkRemoveStart関数が呼び出し中でなければ。
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

