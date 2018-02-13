#include "Util.h"



CUtil::CUtil()
{
}


CUtil::~CUtil()
{
}

CUtil* CUtil::GetInstance()
{
	if (UtilPtr == nullptr)
	{
		UtilPtr = new CUtil();
	}
	return UtilPtr;
}

void CUtil::GetCurtime(std::vector<std::string>& targetStrVec)
{
	const time_t curTime(time(NULL));	// localtime함수에 넣을 인자 선언 // 1970년1월1일부터 몇초가 지났는지 계산
	tm timeStruct;
	localtime_s(&timeStruct, &curTime); // 지역 시간 기준으로 변환
	std::vector<std::string> localStrVector;
	localStrVector.reserve(timeKind); // 공간 할당
	localStrVector.push_back(IntToString(timeStruct.tm_year - 100) + "년");
	localStrVector.push_back(IntToString(timeStruct.tm_mon + 1) + "월");
	localStrVector.push_back(IntToString(timeStruct.tm_mday) + "일");
	localStrVector.push_back(IntToString(timeStruct.tm_hour) + "시");
	localStrVector.push_back(IntToString(timeStruct.tm_min) + "분");
	localStrVector.push_back(IntToString(timeStruct.tm_sec) + "초");
	targetStrVec.insert(targetStrVec.end(), localStrVector.begin(), localStrVector.end()); // 범위 대입
}

const std::string CUtil::IntToString(const int & targetInt)
{
	std::string result;
	char tempChar[10];
	_itoa_s(targetInt, tempChar, 10);
	result = tempChar;
	return result;
}
