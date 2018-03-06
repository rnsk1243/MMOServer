#pragma once

enum ProtocolInfo
{
	None,
	Request,
	Tr,
	Chat,
	NewLink, // Area에 들어온 Link(새로 들어온 사람 입장에서는 기존에 있던 사람들을 새로 들어온 사람으로 본다.)
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

