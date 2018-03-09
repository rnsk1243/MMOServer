#include "ErrorHandler.h"
#include"Util.h"
#include"ConstValue.h"


CErrorHandler::CErrorHandler()
	//mErrorLinkAmount(0)
{
	//for (int i = 0; i < AreaAmount; i++)
	//{
	//	mErrorLinkAmount[i] = 0;
	//}
}

using namespace std;
void CErrorHandler::Write(const char * textFileName, const std::vector<std::string>& strVec)
{
	ofstream outFile(textFileName, ios::app);
	if (!outFile)
	{
		cout << "파일이 없습니다." << endl;
		return;
	}
	vector<string>::const_iterator iterBeginWrite = strVec.begin();
	for (; iterBeginWrite != strVec.end(); ++iterBeginWrite)
	{
		outFile << (*iterBeginWrite).c_str() << '|';
	}
	outFile << '\n';
	outFile.close();
}


void CErrorHandler::GetErrorLevel(ErrorLevel errorLV, std::vector<std::string>& result)
{
	string strLevel;
	switch (errorLV)
	{
	case ErrorLevel::Serious:
		strLevel = ErrorLV_Serious;
		break;
	case ErrorLevel::Normal:
		strLevel = ErrorLV_Normal;
		break;
	case ErrorLevel::Low:
		strLevel = ErrorLV_Low;
		break;
	default:
		strLevel = ErrorLV_UnKnown;
		break;
	}
	vector<string> localStrVector;
	localStrVector.reserve(1);
	localStrVector.push_back(strLevel);
	result.insert(result.end(), localStrVector.begin(), localStrVector.end()); // 범위 대입
}

void CErrorHandler::GetErrorCode(ErrorCode code, std::vector<std::string>& result)
{
	vector<string> localStrVector;
	localStrVector.reserve(1); // 공간 할당
	localStrVector.push_back(UtilPtr->IntToString((int)code));
	result.insert(result.end(), localStrVector.begin(), localStrVector.end()); // 범위 대입
}

void CErrorHandler::GetErrorClientInfo(CLink* client, std::vector<std::string>& result)
{
	vector<string> localStrVector;
	if (nullptr == client)
	{
		localStrVector.reserve(1);
		localStrVector.push_back("CLink==null");
	}
	else
	{
		localStrVector.reserve(2);
		localStrVector.push_back(UtilPtr->IntToString(client->GetMyDistinguishCode()));
		//localStrVector.push_back(string(client->GetMyName()));
		localStrVector.push_back("임시이름");
	}
	result.insert(result.end(), localStrVector.begin(), localStrVector.end()); // 범위 대입
}

void CErrorHandler::MakeErrorFrame(ErrorLevel errorLV, ErrorCode code, std::vector<std::string>& result)
{
	UtilPtr->GetCurtime(result);
	GetErrorLevel(errorLV, result);
	GetErrorCode(code, result);
}

//void CErrorHandler::IncreaseErrorLink(int areaNumber)
//{
//	if (AreaAmount <= areaNumber)
//		return;
//
//	if (mErrorLinkAmount[areaNumber] >= 0)
//	{
//		++mErrorLinkAmount[areaNumber];
//	}
//	else
//	{
//		mErrorLinkAmount[areaNumber] = 0;
//	}
//}

CErrorHandler* CErrorHandler::GetInstance()
{
	if (ErrorHandlerPtr == nullptr)
	{
		ErrorHandlerPtr = new CErrorHandler();
	}
	return ErrorHandlerPtr;
}

CErrorHandler::~CErrorHandler()
{
}

void CErrorHandler::TakeError(ErrorLevel errorLV, ErrorCode code, CLink* targetClient)
{
	vector<string> errorLog;
	MakeErrorFrame(errorLV, code, errorLog);
	if (nullptr != targetClient)
	{
		GetErrorClientInfo(targetClient, errorLog);
		switch (code)
		{
		case ErrorCode::ErrorRecvn:
		case ErrorCode::ErrorSendnMine:
			targetClient->SetErrorState();
			mDeletePlayerDisCodeQueue[targetClient->GetCurArea()].push(targetClient->GetMyDistinguishCode());
			//IncreaseErrorLink(targetClient->GetCurArea());
			break;
		default:
			break;
		}
	}
	Write(ErrorLogTxt.c_str(), errorLog);
}


//void CErrorHandler::DecreaseErrorLink(int areaNumber)
//{
//	if (AreaAmount <= areaNumber)
//		return;
//
//	if (mErrorLinkAmount[areaNumber] > 0)
//	{
//		--mErrorLinkAmount[areaNumber];
//	}
//	else
//	{
//		mErrorLinkAmount[areaNumber] = 0;
//	}
//}

bool CErrorHandler::IsErrorLinkRemoveStart(int areaNumber)
{
	//printf("mDeletePlayerDisCodeQueue[ %d ].size() = %d\n", areaNumber, mDeletePlayerDisCodeQueue[areaNumber].size());
	return (mDeletePlayerDisCodeQueue[areaNumber].size() >= ErrorLinkLimitAmount);
}

int CErrorHandler::GetDeletePlayerDisCode(int areaNumber)
{
	if (mDeletePlayerDisCodeQueue[areaNumber].empty() == false)
	{
		int disCode = mDeletePlayerDisCodeQueue[areaNumber].front();
		mDeletePlayerDisCodeQueue[areaNumber].pop();
		return disCode;
	}
	else
	{
		return WrongValue;
	}
}

void CErrorHandler::PushDeletePlayerDisCode(int areaNumber, int targetDisCode)
{
	mDeletePlayerDisCodeQueue[areaNumber].push(targetDisCode);
}
