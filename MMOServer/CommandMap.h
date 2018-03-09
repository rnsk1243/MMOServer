#pragma once
#include<map>
#include"Link.h"
#include"AreaManager.h"

class CCommandMap
{
	typedef void(CCommandMap::*pf)(const LinkPtr&, CAreaManager*, PacketMessage*); // CCommandMap의 멤버 함수이고 반환값이 void, 인수도 void인 함수 포인터 원형
	typedef std::map<std::string, pf> MAP_FUNC; // map에 string과 멤버함수포인터를 담는 map typedef

	void SendMyDistinguishCode(const LinkPtr & targetClient, CAreaManager* areaManager, PacketMessage* packetMessage);
	void MoveArea(const LinkPtr & targetClient, CAreaManager* areaManager, PacketMessage* packetMessage);
	MAP_FUNC mMapFunc;
public:
	CCommandMap();
	~CCommandMap();
	void Call(const std::string& funcName, const LinkPtr & targetClient, CAreaManager* areaManager, PacketMessage* packetMessage);
};

