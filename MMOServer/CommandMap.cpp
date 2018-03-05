#include "CommandMap.h"
#include"EnumInfo.h"

void CCommandMap::SendMyDistinguishCode(CLink * targetClient)
{
	//Packet p(ProtocolInfo::Request, targetClient->GetMyDistinguishCode(), RequestCollection::GetInstance()->SendDistinguishCode.c_str());
	PacketMessage answerPacket(ProtocolInfo::Request, targetClient->GetMyDistinguishCode(), RequestCollection::GetInstance()->SendDistinguishCode.c_str());
	answerPacket.RequestVal = targetClient->GetMyDistinguishCode();
	
	//printf("sendMyDistinguishcode = %d", p.DistinguishCode);
	//printf("p.Message = %s\n", p.Message);

	targetClient->SendnMine(PacketKindEnum::Message,&answerPacket); // ����ó�� �ȵǾ�����.. list���Ŵ� AreaŬ�������� �����带 ���� �ذ��ؾ� �� ��.
}

CCommandMap::CCommandMap()
{
	// command add
	mMapFunc.insert(std::make_pair(RequestCollection::GetInstance()->SendDistinguishCode, &CCommandMap::SendMyDistinguishCode));
}


CCommandMap::~CCommandMap()
{
}

void CCommandMap::Call(const std::string & funcName, CLink* targetClient)
{
	printf("��û ���� ���� : %s\n", funcName.c_str());
	MAP_FUNC::iterator iterBegin = mMapFunc.begin();
	iterBegin = mMapFunc.find(funcName);
	if (iterBegin != mMapFunc.end())
	{
		//cout << &((*iterBegin).second) << endl;
		pf p((*iterBegin).second);
		(this->*p)(targetClient);
	}
	else
	{
		printf("�Լ��� �����ϴ�.");
	}
}
