#pragma once

enum ProtocolInfo
{
	None,
	Request,
	Tr,
	Chat,
	NewLink, // Area�ɓ����Ă���Link(�V���������Ă����l�̗���ł́A�]���ɂ����l�X��V���ɓ����Ă����l�ƌ��Ă���B)
	DeleteObj
};

enum ErrorLevel
{
	Serious,
	Normal,
	Low
};

enum PacketKindEnum
{
	Transform,
	Message,
	DeleteObjEnum
};

