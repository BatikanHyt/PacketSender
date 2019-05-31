#pragma once

#include <QtCore/QObject>
#include <QtCore/QHash>

#include "FileSenderMessageType.h"

/*
	Message Definition
	< MessageType(1Byte) | ContentLenght(2Byte) | Content(Binary) >
*/

class PacketSenderMessage
{
public:

	PacketSenderMessage();

	~PacketSenderMessage();

	QByteArray generateData();

	FileSenderMessageType getMessageType() const;

	void parseData(const QByteArray& data);

	static int getHeaderSize();

	quint16 getContentSize();

	void setMessageType(FileSenderMessageType messageType);

	void setContentSize(quint16 size);

protected:

	virtual void parseMessage(QDataStream& dataStream);

	virtual QByteArray generateContent();

private:
	
	quint16 mContentSize;

	FileSenderMessageType mMessageType;

};