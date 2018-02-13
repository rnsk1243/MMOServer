#pragma once
#include"ErrorCode.h"
#include"Link.h"
#include<vector>
#include<iostream>
#include<fstream>
#include"EnumInfo.h"

class CErrorHandler
{
	CErrorHandler();
	void Write(const char * textFileName, const std::vector<std::string> & strVec);
	void GetErrorLevel(ErrorLevel errorLV, std::vector<std::string> & result);
	void GetErrorCode(ErrorCode code, std::vector<std::string>& result);
	void GetErrorClientInfo(CLink* client, std::vector<std::string>& result);
	void MakeErrorFrame(ErrorLevel errorLV, ErrorCode code, std::vector<std::string> & result);
public:
	static CErrorHandler* GetInstance();
	~CErrorHandler();
	void TakeError(ErrorLevel errorLV, ErrorCode code, CLink* targetClient = nullptr);
};
static CErrorHandler* ErrorHandlerPtr = CErrorHandler::GetInstance();
