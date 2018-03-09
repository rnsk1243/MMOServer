#pragma once
#include<map>
#include"Link.h"
#include"AreaManager.h"

class CCommandMap
{
	typedef void(CCommandMap::*pf)(const LinkPtr&, CAreaManager*, PacketMessage*); // CCommandMap�� ��� �Լ��̰� ��ȯ���� void, �μ��� void�� �Լ� ������ ����
	typedef std::map<std::string, pf> MAP_FUNC; // map�� string�� ����Լ������͸� ��� map typedef

	void SendMyDistinguishCode(const LinkPtr & targetClient, CAreaManager* areaManager, PacketMessage* packetMessage);
	void MoveArea(const LinkPtr & targetClient, CAreaManager* areaManager, PacketMessage* packetMessage);
	MAP_FUNC mMapFunc;
public:
	CCommandMap();
	~CCommandMap();
	void Call(const std::string& funcName, const LinkPtr & targetClient, CAreaManager* areaManager, PacketMessage* packetMessage);
};

