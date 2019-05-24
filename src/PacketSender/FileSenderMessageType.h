
#pragma once
#include <QtCore/QObject>

enum FileSenderMessageType: quint8
{
	eFileTransferInvalid,
	eFileTransferStart,
	eFileTransferEnd,
	eFileTransferData
};