#pragma once

enum ProtocolInfo
{
	None,
	Request,
	Tr,
	Chat,
	NewLink // Area�� ���� Link(���� ���� ��� ���忡���� ������ �ִ� ������� ���� ���� ������� ����.)
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
	Message
};

