#pragma once
#include "PacketSenderMessage.h"

class RawDataMessage 
	: public PacketSenderMessage
{
public:

	RawDataMessage();

	RawDataMessage(const QByteArray& data);

	~RawDataMessage();

	void setData(const QByteArray &data);
	const QByteArray& getData() const;

protected:

	virtual QByteArray generateContent() override;

	virtual void parseMessage(QDataStream &dataStream) override;

private:

	QByteArray mData;
};

