#pragma once
#include "FileSenderMessages.h"
#include <QtCore/QDataStream>
class FileStartMessage :
	public FileSenderMessages
{
public:
	FileStartMessage();

	FileStartMessage(const QByteArray& data);
	~FileStartMessage();

	void setData(const QByteArray &data);
	const QByteArray& getData() const;

	void setMessageSize(quint32 messageSize);
	const quint32 getMessageSize() const;

	void setMessageDelay(quint8 delay);
	const quint8 getMessageDelay() const;

protected:

	virtual QByteArray generateContent();

	virtual void parseMessage(QDataStream& dataStream);

private:
	
	quint32 mMessageSize;

	quint8 mMessageDelay;

	QByteArray mData;
};

