#pragma once
#include<list>
#include<string>
#include"Link.h"
#include"RAII.h"

typedef std::list<LinkPtr> LinkList;
typedef LinkList::iterator LinkListIt;

class CArea
{
	CRITICALSECTION mCS;
	LinkList mClientInfos;
	std::string mAreaName;
	int mAmountPeople;
	const int mAreaNumber;
	//void Removeclient(LinkListIt)
public:
	CArea(const CArea&) = delete;
	CArea& operator=(const CArea&) = delete;
	CArea(std::string areaName, int areaNum);
	~CArea();
	bool PushClient(const LinkPtr& shared_client);
	void Broadcast(const Packet & packet);
	void EraseClient(const int& clientPKnum);
};

