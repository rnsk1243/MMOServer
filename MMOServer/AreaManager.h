#pragma once
#include<vector>
#include<memory>
#include"Area.h"

typedef std::shared_ptr<CArea> AreaPtr;
typedef std::vector<AreaPtr> AreaVector;
typedef AreaVector::iterator AreaVectorIt;

class CAreaManager
{
	//HANDLE hRemoveEvent;
	//HANDLE hBroadcastEvent;
	AreaVector mAreas;
//	void PatrolErrorLinkWorkThread();
public:
	CAreaManager(int areaAmount);
	~CAreaManager();
	CAreaManager& operator=(const CAreaManager&) = delete;
	CAreaManager(const CAreaManager&) = delete;
	bool EnterArea(int areaNumber, const LinkPtr & linkPtr);
	bool MoveArea(int areaNumber, const LinkPtr & linkPtr);
	const LinkPtr GetLinkPtr(int areaNumber, int disCode);
	//void EraseClient(LinkPtr linkPtr);
	void Broadcast(LinkPtr link, const PacketKindEnum PacketKind, LPVOID packet);
};

