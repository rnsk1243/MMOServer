#pragma once

enum ProtocolInfo
{
	None,
	Request,
	Tr,
	Chat,
	NewLink, // Areaに入ってきたLink(新しく入ってきた人の立場では、従来にいた人々を新たに入ってきた人と見ている。)
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

