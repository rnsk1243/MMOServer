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
	bool mIsRemoveErrorLink;
	void EraseClient(const int& clientPKnum);
	int mErrorLinkAmount; //ÉGÉâÅ[ÇµÇΩlinkêîÅB
	void SearchEndRemoveErrorLink();
	void SendNewClientNotice(const LinkPtr & newClientPtr);
	//void Removeclient(LinkListIt)
public:
	CArea(const CArea&) = delete;
	CArea& operator=(const CArea&) = delete;
	CArea(std::string areaName, int areaNum);
	~CArea();
	void SendDeleteCommand(const LinkPtr & linkPtr);
	bool PushClient(const LinkPtr& shared_client);
	const LinkPtr GetLinkPtr(int disCode);
	void Broadcast(const PacketKindEnum PacketKind, LPVOID packet);
};

