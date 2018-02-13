#pragma once
#include<vector>
#include<time.h>
#include<iostream>
#include"EnumInfo.h"
#include"ConstValue.h"

class CUtil
{
	CUtil();
public:
	~CUtil();
	static CUtil* GetInstance();
	void GetCurtime(std::vector<std::string> & targetStrVec);
	const std::string IntToString(const int& targetInt);
};
static CUtil* UtilPtr = CUtil::GetInstance();

