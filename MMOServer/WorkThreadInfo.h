#pragma once
#include<stdio.h>
#include<iostream>
#include<thread>
#include "ReadyServer.h"
#include"ConstValue.h"
#include"EnumInfo.h"

class WorkThreadInfo
{
public:
	WorkThreadInfo();
	~WorkThreadInfo();
	static DWORD WINAPI ThreadWork(LPVOID hCPObj);
};

