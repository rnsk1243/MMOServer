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
	// ���ׂĂ̐l�Ɏ��̃N���A�Ƒ��M
	//���ׂĂ̐l�ɓ������Ƒ���������
	PacketDeleteObj myDeletePacket(ProtocolInfo::DeleteObj, WrongValue);//���ׂĂ̐l�����A���A������!
	myDeletePacket.EraseObjDiscodeArray[0] = linkPtr.get()->GetMyDistinguishCode();
	//printf("%d�̏Z��������%d������\n"�AmAreaNumber�AmyDeletePacket.EraseObjDiscodeArray[0]);
	Broadcast(PacketKindEnum::DeleteObjEnum, &myDeletePacket);
	//���ɂ��ׂĂ̐l�������Ƒ���������
	PacketDeleteObj everyDeletePacket(ProtocolInfo::DeleteObj, WrongValue);//�������ׂĂ̐l�N���C�A���g������!

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
	linkPtr.get()->SendnMine(PacketKindEnum::DeleteObjEnum, &everyDeletePacket);//�c��̍Ō�܂ő��������ƁB
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
	//printf("�����N�����Ȃ�đ|�����n�߂Č��邩...AreaNumber = %d\n", mAreaNumber);
	ScopeLock<CRITICALSECTION> CS(mCS);
	mIsRemoveErrorLink = true;
	LinkListIt linkListIterBegin = mClientInfos.begin();

	PacketDeleteObj myDeletePacket(ProtocolInfo::DeleteObj, WrongValue); //���ׂĂ̐l�����A���A������!
	int index = 0;
	for (; linkListIterBegin != mClientInfos.end();)
	{
		// �\�P�b�g�G���[�܂���Area�ړ����Ă���Area�̔ԍ��ƈ�v���Ȃ���΍폜
		if ((*linkListIterBegin).get()->IsErrorClient() == true || (*linkListIterBegin).get()->GetCurArea() != mAreaNumber)
		{
			if ((*linkListIterBegin).get()->IsErrorClient() == true)
			{
				myDeletePacket.EraseObjDiscodeArray[index] = (*linkListIterBegin).get()->GetMyDistinguishCode();
				++index;
			}

			//mClientInfos.remove(*linkListIterBegin));
			//erase����ƁA�Ԃ�l�Ŏ��̔����҂��Ԋ҂��ꂽ���ƁB�B�B������Afor����邽�тɁA++iterator�͖��p
			//�����Aerase�����Ȃ��ꍇ(else��)���̃��v���P�[�^�[�ɓn��悤�ɂ��Ă���B
			//printf("SearchEndRemoveErrorLink count=%d\n"�A(*linkListIterBegin)�Buse_count());
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
	
	//�G���[�̂Ȃ����ׂĂ̐l�ɓ������Ƒ���������
	mIsRemoveErrorLink = false;
	Broadcast(PacketKindEnum::DeleteObjEnum, &myDeletePacket);

}

void CArea::SendNewClientNotice(const LinkPtr & newClientPtr)
{
	int newClientDisCode = newClientPtr.get()->GetMyDistinguishCode();
	PacketTransform newLinkTr(ProtocolInfo::NewLink, newClientDisCode, newClientPtr.get()->GetMyTransform());
	//printf("�]���\��Position.y=%f\n"�AnewClientPtr.get()->GetMyTransform()�BPosition.y);
	//���X�������l��"�]���\������"
	Broadcast(PacketKindEnum::Transform, &newLinkTr);
	//���X�������l���������݁A�����̈ʒu��V���ɓ����Ă����l�ɋ���
	LinkListIt linkIterBegin = mClientInfos.begin();
	for (; linkIterBegin != mClientInfos.end(); ++linkIterBegin)
	{
		CLink* oldLinkPtr = (*linkIterBegin).get();
		if (oldLinkPtr != nullptr)
		{
			//printf("�����̈ʒu:%d"�AoldLinkPtr->GetMyTransform()�BPosition.x);
			PacketTransform oldLinkTr(ProtocolInfo::NewLink, oldLinkPtr->GetMyDistinguishCode(), oldLinkPtr->GetMyTransform());
			newClientPtr.get()->SendnMine(PacketKindEnum::Transform, &oldLinkTr);
		}
	}
}


bool CArea::PushClient(const LinkPtr & shared_client)
{
	if (ErrorHandlerPtr->IsErrorLinkRemoveStart(mAreaNumber))//�G���[����Link������������C����𒴉߂����̂�?
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
	if (ErrorHandlerPtr->IsErrorLinkRemoveStart(mAreaNumber)) //�G���[����Link������������C����𒴉߂����̂�?
	{
		SearchEndRemoveErrorLink();
	}
	if (!mIsRemoveErrorLink) //IsErrorLinkRemoveStart�֐����Ăяo�����łȂ���΁B
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

