#pragma once
#include<WinSock2.h>

const int Port = 9000;
const int RecvBufSize = 1024;
const int ChatBufSize = 128;
const int StartCurArea = 0;
const int AreaAmount = 3;

enum ProtocolInfo
{
	None,
	Tr,
	Chat
};

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

struct Packet // 168byte
{
	int InfoProtocol; // 4byte
	MyTransform Tr; // 36byte
	char ChatMessage[ChatBufSize]; // ChatBufSize byte

	Packet()
	{}

	Packet(int infoProtocol, MyTransform tr, const char* chatMessage = nullptr):
		InfoProtocol(infoProtocol), Tr(tr)
	{
		if (nullptr != chatMessage)
		{
			strcpy_s(ChatMessage, ChatBufSize, chatMessage);
		}
	}

	Packet(int infoProtocol, const char* chatMessage) :
		InfoProtocol(infoProtocol)
	{
		if (nullptr != chatMessage)
		{
			strcpy_s(ChatMessage, ChatBufSize, chatMessage);
		}
	}
};



