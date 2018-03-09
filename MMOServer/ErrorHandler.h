#pragma once
#include"ErrorCode.h"
#include"Link.h"
#include<vector>
#include<iostream>
#include<fstream>
#include"EnumInfo.h"
#include<queue>

typedef std::queue<int> IntQueue;

class CErrorHandler
{
	CErrorHandler();
	void Write(const char * textFileName, const std::vector<std::string> & strVec);
	void GetErrorLevel(ErrorLevel errorLV, std::vector<std::string> & result);
	void GetErrorCode(ErrorCode code, std::vector<std::string>& result);
	void GetErrorClientInfo(CLink* client, std::vector<std::string>& result);
	void MakeErrorFrame(ErrorLevel errorLV, ErrorCode code, std::vector<std::string> & result);
	//int mErrorLinkAmount[AreaAmount]; // �� area���� ErrorLink���� (index�� area��ȣ)
	//void IncreaseErrorLink(int areaNumber);
	IntQueue mDeletePlayerDisCodeQueue[AreaAmount]; // �� area���� �����ؾ��� ������ȣ ���� (index�� area��ȣ)
public:
	static CErrorHandler* GetInstance();
	~CErrorHandler();
	void TakeError(ErrorLevel errorLV, ErrorCode code, CLink* targetClient = nullptr);
	//void DecreaseErrorLink(int areaNumber);
	bool IsErrorLinkRemoveStart(int areaNumber);
	//void AddDeletePlayer(int disCode);
	int GetDeletePlayerDisCode(int areaNumber);
	void PushDeletePlayerDisCode(int areaNumber, int targetDisCode);
};
static CErrorHandler* ErrorHandlerPtr = CErrorHandler::GetInstance();
