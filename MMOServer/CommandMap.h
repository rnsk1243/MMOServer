#pragma once
#include<map>
#include"Link.h"
#include"AreaManager.h"

class CCommandMap
{
	typedef void(CCommandMap::*pf)(const LinkPtr&, CAreaManager*, PacketMessage*); //CCommandMapのメンバー関数であり、返り値がvoid、買収もvoidの関数ポインタ原型
	typedef std::map<std::string, pf> MAP_FUNC; //mapにstringとメンバー関数ポインタを盛るmap typedef

	void SendMyDistinguishCode(const LinkPtr & targetClient, CAreaManager* areaManager, PacketMessage* packetMessage);
	void MoveArea(const LinkPtr & targetClient, CAreaManager* areaManager, PacketMessage* packetMessage);
	MAP_FUNC mMapFunc;
public:
	CCommandMap();
	~CCommandMap();
	void Call(const std::string& funcName, const LinkPtr & targetClient, CAreaManager* areaManager, PacketMessage* packetMessage);
};

