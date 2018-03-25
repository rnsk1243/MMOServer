#include "CommandMap.h"
#include"EnumInfo.h"

void CCommandMap::SendMyDistinguishCode(const LinkPtr & targetClient, CAreaManager* areaManager, PacketMessage* packetMessage)
{
	//Packet p(ProtocolInfo::Request, targetClient->GetMyDistinguishCode(), RequestCollection::GetInstance()->SendDistinguishCode.c_str());
	PacketMessage answerPacket(ProtocolInfo::Request, targetClient.get()->GetCurArea(), RequestCollection::GetInstance()->SendDistinguishCode.c_str());
	answerPacket.RequestVal = targetClient->GetMyDistinguishCode();
	
	//printf("sendMyDistinguishcode = %d", p.DistinguishCode);
	//printf("p.Message = %s\n", p.Message);

	targetClient->SendnMine(PacketKindEnum::Message,&answerPacket); //��O��������Ă��Ȃ�����..list�̏�����Area�N���X�ŃX���b�h������ĉ������Ȃ���΂Ȃ�Ȃ��悤�B
}

void CCommandMap::MoveArea(const LinkPtr & targetClient, CAreaManager * areaManager, PacketMessage* packetMessage)
{
	int curAreaNumber = targetClient.get()->GetCurArea();
	int myDisCode = targetClient.get()->GetMyDistinguishCode();
	int moveArea = packetMessage->RequestVal;
	const LinkPtr & linkPtr = areaManager->GetLinkPtr(curAreaNumber, myDisCode);
	if (linkPtr == nullptr)
		return;
	if (areaManager->MoveArea(moveArea, linkPtr))
	{
		PacketMessage answerPacket(ProtocolInfo::Request, targetClient.get()->GetMyDistinguishCode(), RequestCollection::GetInstance()->SendMoveAreaComplete.c_str());
		answerPacket.RequestVal = targetClient.get()->GetCurArea(); //area�̈ړ������s�ړ����ꂽarea�ԍ����邱�Ƃ�
		targetClient.get()->SendnMine(PacketKindEnum::Message, &answerPacket);
	}
	else
	{
		printf("Move fail\n");
	}
}

CCommandMap::CCommandMap()
{
	RequestCollection* requestCollection = RequestCollection::GetInstance();
	// command add
	mMapFunc.insert(std::make_pair(requestCollection->SendDistinguishCode, &CCommandMap::SendMyDistinguishCode));
	mMapFunc.insert(std::make_pair(requestCollection->SendMoveArea, &CCommandMap::MoveArea));
}


CCommandMap::~CCommandMap()
{
}

void CCommandMap::Call(const std::string & funcName, const LinkPtr & targetClient, CAreaManager* areaManager, PacketMessage* packetMessage)
{
	//printf("�v�����ꂽ���e:%s\n"�AfuncName.c_str());
	MAP_FUNC::iterator iterBegin = mMapFunc.begin();
	iterBegin = mMapFunc.find(funcName);
	if (iterBegin != mMapFunc.end())
	{
		//cout << &((*iterBegin).second) << endl;
		pf p((*iterBegin).second);
		(this->*p)(targetClient, areaManager, packetMessage);
	}
	else
	{
		printf("function������܂���B");
	}
}
