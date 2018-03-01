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
	void EraseClient(const int& clientPKnum);
	int mErrorLinkAmount; // 에러난 link 개수.
	void SearchEndRemoveErrorLink();
	void SendNewClientNotice(const LinkPtr & newClientPtr);
	//void Removeclient(LinkListIt)
public:
	CArea(const CArea&) = delete;
	CArea& operator=(const CArea&) = delete;
	CArea(std::string areaName, int areaNum);
	~CArea();
	bool PushClient(const LinkPtr& shared_client);
	void Broadcast(const PacketKindEnum PacketKind, LPVOID packet);
};

