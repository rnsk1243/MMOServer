#pragma once
#include<vector>
#include<memory>
#include"Area.h"

typedef std::shared_ptr<CArea> AreaPtr;
typedef std::vector<AreaPtr> AreaVector;
typedef AreaVector::iterator AreaVectorIt;

class CAreaManager
{
	AreaVector mAreas;
public:
	CAreaManager(int areaAmount);
	~CAreaManager();
	CAreaManager& operator=(const CAreaManager&) = delete;
	CAreaManager(const CAreaManager&) = delete;
	void EnterArea(int areaNumber, LinkPtr linkPtr);
	void EraseClient(LinkPtr linkPtr);
	void Broadcast(LinkPtr link, const Packet& packet);
};

