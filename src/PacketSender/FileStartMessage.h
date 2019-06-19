
#pragma once
#include "PacketSenderMessage.h"
#include <QtCore/QDataStream>

class FileStartMessage 
	: public PacketSenderMessage
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

	virtual QByteArray generateContent() override;

	virtual void parseMessage(QDataStream& dataStream) override;

private:
	
	quint32 mMessageSize;

	quint8 mMessageDelay;

	QByteArray mData;
};