#pragma once

enum ProtocolInfo
{
	None,
	Request,
	Tr,
	Chat,
	NewLink, // Area�� ���� Link(���� ���� ��� ���忡���� ������ �ִ� ������� ���� ���� ������� ����.)
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

