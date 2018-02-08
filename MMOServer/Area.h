#pragma once
#include<list>
#include<string>
#include"Link.h"

typedef std::list<LinkPtr> LinkList;
typedef LinkList::iterator LinkListIt;

class CArea
{
	LinkList mClientInfos;
	std::string mAreaName;
	int mAmountPeople;
	const int mAreaNumber;
public:
	CArea(const CArea&) = delete;
	CArea& operator=(const CArea&) = delete;
	CArea(std::string areaName, int areaNum);
	~CArea();
	bool PushClient(const LinkPtr& shared_client);
};

