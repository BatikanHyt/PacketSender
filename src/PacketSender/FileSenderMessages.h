#pragma once

#include <QtCore/QObject>
#include <QtCore/QHash>

#include "FileSenderMessageType.h"

/*
	Message Definition
	< MessageType(1Byte) | ContentLenght(2Byte) | Content(Binary) >
*/

class FileSenderMessages
{
public:

	FileSenderMessages();

	~FileSenderMessages();

	QByteArray generateData();

	FileSenderMessageType getMessageType() const;

	void parseData(const QByteArray& data);

	static int getHeaderSize();

	quint16 getContentSize();

protected:

	virtual void parseMessage(QDataStream& dataStream);

	virtual QByteArray generateContent();

	void setMessageType(FileSenderMessageType messageType);

private:
	
	quint16 mContentSize;

	FileSenderMessageType mMessageType;

};