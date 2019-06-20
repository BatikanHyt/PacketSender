
#pragma once
#include <QtCore/QObject>

enum FileSenderMessageType: quint8
{
	eFileTransferInvalid,
	eFileTransferStart,
	eFileTransferEnd,
	eFileTransferData,
	eRawDataTransfer
};

inline bool isValidMessageType(FileSenderMessageType type)
{
	if (type == eFileTransferData || type == eFileTransferStart || type == eFileTransferEnd || type == eRawDataTransfer)
	{
		return true;
	}

	return false;
}