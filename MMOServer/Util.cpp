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
	const time_t curTime(time(NULL));	 //localtime関数に入れる因子宣言//1970年1月1日から数秒が過ぎたかどうか計算
	tm timeStruct;
	localtime_s(&timeStruct, &curTime); //地域時間基準に変換
	std::vector<std::string> localStrVector;
	localStrVector.reserve(timeKind); //空間割り当て
	localStrVector.push_back(IntToString(timeStruct.tm_year - 100) + "年");
	localStrVector.push_back(IntToString(timeStruct.tm_mon + 1) + "月");
	localStrVector.push_back(IntToString(timeStruct.tm_mday) + "日");
	localStrVector.push_back(IntToString(timeStruct.tm_hour) + "時");
	localStrVector.push_back(IntToString(timeStruct.tm_min) + "分");
	localStrVector.push_back(IntToString(timeStruct.tm_sec) + "秒");
	targetStrVec.insert(targetStrVec.end(), localStrVector.begin(), localStrVector.end()); //範囲大学入試
}

const std::string CUtil::IntToString(const int & targetInt)
{
	std::string result;
	char tempChar[10];
	_itoa_s(targetInt, tempChar, 10);
	result = tempChar;
	return result;
}
