#pragma once
#include<WinSock2.h>
#include<iostream>

const int Port = 9000;
const int RecvBufSize = 1024;
const int ChatBufSize = 128;
const int StartCurArea = 0;
const int AreaAmount = 3;
const int timeKind = 6; // 시간 종류 갯수 (년, 월, 일, 시, 분, 초) 6개
const int WrongValue = -1; // 잘 못된 값. 혹은 아직 초기화 되지 않은 값.
const int StartDistinguishCode = 0;

const std::string ErrorLogTxt = "ErrorLog.txt";
const std::string ErrorLV_Serious = "[심각]";
const std::string ErrorLV_Normal = "[보통]";
const std::string ErrorLV_Low = "[낮음]";
const std::string ErrorLV_UnKnown = "[정의되지 않은 레벨]";

struct MyVector3
{
	float x;
	float y;
	float z;

	MyVector3():
		x(0.0f),y(0.0f),z(0.0f)
	{
	}
	MyVector3(float _x, float _y, float _z) :
		x(_x), y(_y), z(_z)
	{
	}
};

struct MyTransform
{
	MyVector3 Position;
	MyVector3 Rotation;
	MyVector3 Scale;

	MyTransform()
	{}

	MyTransform(MyVector3 pos, MyVector3 rot, MyVector3 sca):
		Position(pos), Rotation(rot), Scale(sca)
	{

	}
};

struct Packet // 172byte
{
	int InfoProtocol; // 4byte
	int RequestVal; // 4byte
	MyTransform Tr; // 36byte
	char ChatMessage[ChatBufSize]; // ChatBufSize byte

	Packet()
	{}

	Packet(int infoProtocol, int requestVal, MyTransform tr, const char* chatMessage = nullptr):
		InfoProtocol(infoProtocol), RequestVal(requestVal), Tr(tr)
	{
		if (nullptr != chatMessage)
		{
			strcpy_s(ChatMessage, ChatBufSize, chatMessage);
		}
	}

	Packet(int infoProtocol, int requestVal, const char* chatMessage = nullptr) :
		InfoProtocol(infoProtocol), RequestVal(requestVal)
	{
		if (nullptr != chatMessage)
		{
			strcpy_s(ChatMessage, ChatBufSize, chatMessage);
		}
	}
};

class RequestCollection
{
private:
	RequestCollection():
		SendDistinguishCode("RequestMyDisCode")
	{
	}
	~RequestCollection(){}
public:
	static RequestCollection* GetInstance();
	const std::string SendDistinguishCode;
};
static RequestCollection* RequestCollectionPtr = RequestCollection::GetInstance();

