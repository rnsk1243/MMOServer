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
	const time_t curTime(time(NULL));	 //localtime�֐��ɓ������q�錾//1970�N1��1�����琔�b���߂������ǂ����v�Z
	tm timeStruct;
	localtime_s(&timeStruct, &curTime); //�n�掞�Ԋ�ɕϊ�
	std::vector<std::string> localStrVector;
	localStrVector.reserve(timeKind); //��Ԋ��蓖��
	localStrVector.push_back(IntToString(timeStruct.tm_year - 100) + "�N");
	localStrVector.push_back(IntToString(timeStruct.tm_mon + 1) + "��");
	localStrVector.push_back(IntToString(timeStruct.tm_mday) + "��");
	localStrVector.push_back(IntToString(timeStruct.tm_hour) + "��");
	localStrVector.push_back(IntToString(timeStruct.tm_min) + "��");
	localStrVector.push_back(IntToString(timeStruct.tm_sec) + "�b");
	targetStrVec.insert(targetStrVec.end(), localStrVector.begin(), localStrVector.end()); //�͈͑�w����
}

const std::string CUtil::IntToString(const int & targetInt)
{
	std::string result;
	char tempChar[10];
	_itoa_s(targetInt, tempChar, 10);
	result = tempChar;
	return result;
}
