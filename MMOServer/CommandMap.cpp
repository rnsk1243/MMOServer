#include "CommandMap.h"
#include"EnumInfo.h"

void CCommandMap::SendMyDistinguishCode(CLink * targetClient)
{
	//Packet p(ProtocolInfo::Request, targetClient->GetMyDistinguishCode(), RequestCollection::GetInstance()->SendDistinguishCode.c_str());
	PacketMessage answerPacket(ProtocolInfo::Request, targetClient->GetMyDistinguishCode(), RequestCollection::GetInstance()->SendDistinguishCode.c_str());
	answerPacket.RequestVal = targetClient->GetMyDistinguishCode();
	
	//printf("sendMyDistinguishcode = %d", p.DistinguishCode);
	//printf("p.Message = %s\n", p.Message);

	targetClient->SendnMine(PacketKindEnum::Message,&answerPacket); // 예외처리 안되어있음.. list제거는 Area클래스에서 스레드를 만들어서 해결해야 할 듯.
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
	printf("요청 받은 내용 : %s\n", funcName.c_str());
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
		printf("함수가 없습니다.");
	}
}
