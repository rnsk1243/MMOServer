#pragma once
#include<WinSock2.h>
#include<iostream>
#include"EnumInfo.h"

const int Port = 9000;
const int RecvBufSize = 1024;
const int MessageBufSize = 128;
const int StartCurArea = 0;
const int AreaAmount = 3;
const int timeKind = 6; // 시간 종류 갯수 (년, 월, 일, 시, 분, 초) 6개
const int WrongValue = -1; // 잘 못된 값. 혹은 아직 초기화 되지 않은 값.
const int StartDistinguishCode = 0; // 구별번호 시작 값
const int ErrorLinkLimitAmount = 3; // Error가 발생한 Link를 몇개까지 쌓아 두었다가 제거시킬 것인지에 대한 값.
const int SendEraseObjArraySize = ErrorLinkLimitAmount; // 클라이언트에게 지우고자 하는 구별번호 보낼 수 있는 배열 크기 index 0~ErrorLinkLimitAmount 까지

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
	{
		Position.y = 1.0f;
		Scale.x, Scale.y, Scale.z = 1.0f;
	}

	MyTransform(MyVector3 pos, MyVector3 rot, MyVector3 sca):
		Position(pos), Rotation(rot), Scale(sca)
	{

	}
};

//struct Packet // 172byte
//{
//	int InfoProtocol; // 4byte
//	int RequestVal; // 4byte
//	MyTransform Tr; // 36byte
//	char ChatMessage[ChatBufSize]; // ChatBufSize byte
//
//	Packet()
//	{}
//
//	Packet(int infoProtocol, int requestVal, MyTransform tr, const char* chatMessage = nullptr):
//		InfoProtocol(infoProtocol), RequestVal(requestVal), Tr(tr)
//	{
//		if (nullptr != chatMessage)
//		{
//			strcpy_s(ChatMessage, ChatBufSize, chatMessage);
//		}
//	}
//
//	Packet(int infoProtocol, int requestVal, const char* chatMessage = nullptr) :
//		InfoProtocol(infoProtocol), RequestVal(requestVal)
//	{
//		if (nullptr != chatMessage)
//		{
//			strcpy_s(ChatMessage, ChatBufSize, chatMessage);
//		}
//	}
//};

struct PacketTransform // 48byte
{
	int PacketKind;
	int InfoProtocol;
	int DistinguishCode;
	MyTransform Tr; // 36byte

	PacketTransform():PacketKind(PacketKindEnum::Transform),
		InfoProtocol(WrongValue),DistinguishCode(WrongValue) // 초기화 되지 않은 것에는 잘 못된 값을 넣어 표시함.
	{}
	PacketTransform(int infoProtocol, int distinguishCode, MyTransform tr):PacketKind(PacketKindEnum::Transform),
		InfoProtocol(infoProtocol), DistinguishCode(distinguishCode), Tr(tr)
	{}
};

struct PacketMessage // (16 + ChatBufSize) byte 
{
	int PacketKind;
	int InfoProtocol;
	int DistinguishCode;
	int RequestVal;
	char Message[MessageBufSize]; // ChatBufSize byte

	PacketMessage():PacketKind(PacketKindEnum::Message),
		InfoProtocol(WrongValue), DistinguishCode(WrongValue), RequestVal(WrongValue) // 초기화 되지 않은 것에는 잘 못된 값을 넣어 표시함.
	{}

	PacketMessage(int infoProtocol, int distinguishCode, const char* message):PacketKind(PacketKindEnum::Message),
		InfoProtocol(infoProtocol), DistinguishCode(distinguishCode), RequestVal(WrongValue)
	{
		if (message != nullptr)
		{
			strcpy_s(Message, MessageBufSize, message);
		}
	}
};

struct PacketDeleteObj // ((4*3) + (4 * SendEraseObjArraySize)) byte // 52
{
	int PacketKind;
	int InfoProtocol;
	int DistinguishCode;
	int EraseObjDiscodeArray[SendEraseObjArraySize]; // 삭제할 오브젝트 구별번호
	
	PacketDeleteObj():PacketKind(PacketKindEnum::DeleteObjEnum),
		InfoProtocol(WrongValue), DistinguishCode(WrongValue)
	{
		for (int i = 0; i < SendEraseObjArraySize; i++)
		{
			EraseObjDiscodeArray[i] = WrongValue;
		}
	}

	PacketDeleteObj(int infoProtocol, int distinguishCode):PacketKind(PacketKindEnum::DeleteObjEnum),
		InfoProtocol(infoProtocol), DistinguishCode(distinguishCode)
	{
		for (int i = 0; i < SendEraseObjArraySize; i++)
		{
			EraseObjDiscodeArray[i] = WrongValue;
		}
	}
};

const int SendSizeTransform = sizeof(PacketTransform);
const int SendSizeMessage = sizeof(PacketMessage);
const int SendSizeDeleteObj = sizeof(PacketDeleteObj);

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