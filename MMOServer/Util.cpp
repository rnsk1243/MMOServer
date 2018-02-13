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
	const time_t curTime(time(NULL));	// localtime�Լ��� ���� ���� ���� // 1970��1��1�Ϻ��� ���ʰ� �������� ���
	tm timeStruct;
	localtime_s(&timeStruct, &curTime); // ���� �ð� �������� ��ȯ
	std::vector<std::string> localStrVector;
	localStrVector.reserve(timeKind); // ���� �Ҵ�
	localStrVector.push_back(IntToString(timeStruct.tm_year - 100) + "��");
	localStrVector.push_back(IntToString(timeStruct.tm_mon + 1) + "��");
	localStrVector.push_back(IntToString(timeStruct.tm_mday) + "��");
	localStrVector.push_back(IntToString(timeStruct.tm_hour) + "��");
	localStrVector.push_back(IntToString(timeStruct.tm_min) + "��");
	localStrVector.push_back(IntToString(timeStruct.tm_sec) + "��");
	targetStrVec.insert(targetStrVec.end(), localStrVector.begin(), localStrVector.end()); // ���� ����
}

const std::string CUtil::IntToString(const int & targetInt)
{
	std::string result;
	char tempChar[10];
	_itoa_s(targetInt, tempChar, 10);
	result = tempChar;
	return result;
}
