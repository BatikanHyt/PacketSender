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

	static int getHeaderSize();

protected:

	virtual void parseMessage(QDataStream& dataStream);

	virtual QByteArray generateContent();

	void setMessageType(FileSenderMessageType messageType);

private:
	
	quint16 mContentSize;

	FileSenderMessageType mMessageType;

};

