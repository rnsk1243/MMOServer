#pragma once
#include<WinSock2.h>
#include<iostream>
#include"EnumInfo.h"

const int Port = 9000;
const int RecvBufSize = 1024;
const int MessageBufSize = 128;
const int StartCurArea = 0;
const int AreaAmount = 3;
const int timeKind = 6; //時間種類数(年、月、日、時、分、初)6個
const int WrongValue = -1; //間違った値。あるいは、まだ初期化されていない値。
const int StartDistinguishCode = 0; //区別番号開始値
const int ErrorLinkLimitAmount = 1; //Errorが発生したLinkをいくつまで積んでおいて除去させるかについた値段(クライアントSendEraseObjArraySize値と一致させるつもり。)
const int SendEraseObjArraySize = ErrorLinkLimitAmount; //クライアントに負わせるという区別番号送ることができる配列の大きさindex 0~ErrorLinkLimitAmountまで
const int MoveAreaSuccessValue = 1; //areaの移動成功時、クライアントに送る費用

const std::string ErrorLogTxt = "ErrorLog.txt";
const std::string ErrorLV_Serious = "[深刻]";
const std::string ErrorLV_Normal = "[普通]";
const std::string ErrorLV_Low = "[低い]";
const std::string ErrorLV_UnKnown = "[定義されていないレベル]";



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
		InfoProtocol(WrongValue),DistinguishCode(WrongValue) //初期化されていないのは間違った値を入れて表示する。
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
		InfoProtocol(WrongValue), DistinguishCode(WrongValue), RequestVal(WrongValue) //初期化されていないのは間違った値を入れて表示する。
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
	int EraseObjDiscodeArray[SendEraseObjArraySize]; //削除するオブジェクト区別番号
	
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
	RequestCollection() :
		SendDistinguishCode("RequestMyDisCode"),
		SendMoveArea("MoveArea"),
		SendMoveAreaComplete("MoveAreaComplete")
	{
	}
	~RequestCollection(){}
public:
	static RequestCollection* GetInstance();
	const std::string SendDistinguishCode;
	const std::string SendMoveArea;
	const std::string SendMoveAreaComplete;
};
static RequestCollection* RequestCollectionPtr = RequestCollection::GetInstance();