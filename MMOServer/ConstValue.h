#pragma once
#include<WinSock2.h>
#include<iostream>

const int Port = 9000;
const int RecvBufSize = 1024;
const int ChatBufSize = 128;
const int StartCurArea = 0;
const int AreaAmount = 3;
const int timeKind = 6; // �ð� ���� ���� (��, ��, ��, ��, ��, ��) 6��
const int WrongValue = -1; // �� ���� ��. Ȥ�� ���� �ʱ�ȭ ���� ���� ��.
const int StartDistinguishCode = 0;

const std::string ErrorLogTxt = "ErrorLog.txt";
const std::string ErrorLV_Serious = "[�ɰ�]";
const std::string ErrorLV_Normal = "[����]";
const std::string ErrorLV_Low = "[����]";
const std::string ErrorLV_UnKnown = "[���ǵ��� ���� ����]";

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

