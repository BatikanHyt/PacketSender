#pragma once

#include <QtCore/QObject>
#include <QtCore/QHash>

#include "FileSenderMessageType.h"

class FileSenderMessages
{
public:

	FileSenderMessages();

	~FileSenderMessages();

	QByteArray generateData();

	FileSenderMessageType getMessageType() const;

	void parseData(const QByteArray& data);

protected:

	virtual void parseMessage(QDataStream& dataStream) = 0;

	virtual QByteArray generateContent() = 0;

	void setMessageType(FileSenderMessageType messageType);

private:
	
	quint16 mContentSize;

	FileSenderMessageType mMessageType;
};

