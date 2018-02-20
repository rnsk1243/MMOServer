#include "CommandMap.h"
#include"EnumInfo.h"


void CCommandMap::SendMyDistinguishCode(CLink * targetClient)
{
	Packet p(ProtocolInfo::Request, targetClient->GetMyDistinguishCode(), RequestCollection::GetInstance()->SendDistinguishCode.c_str());
	printf("sendMyDistinguishcode = %d", p.RequestVal);
	targetClient->SendnMine(p);
}

CCommandMap::CCommandMap()
{
	mMapFunc.insert(std::make_pair(RequestCollection::GetInstance()->SendDistinguishCode, &CCommandMap::SendMyDistinguishCode));
}


CCommandMap::~CCommandMap()
{
}

void CCommandMap::Call(const std::string & funcName, CLink* targetClient)
{
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
