#pragma once
#include<map>
#include"Link.h"

class CCommandMap
{
	typedef void(CCommandMap::*pf)(CLink*); // CCommandMap�� ��� �Լ��̰� ��ȯ���� void, �μ��� void�� �Լ� ������ ����
	typedef std::map<std::string, pf> MAP_FUNC; // map�� string�� ����Լ������͸� ��� map typedef

	void SendMyDistinguishCode(CLink* targetClient);
	MAP_FUNC mMapFunc;
public:
	CCommandMap();
	~CCommandMap();
	void Call(const std::string& funcName, CLink* targetClient);
};

