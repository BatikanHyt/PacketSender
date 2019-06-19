
#pragma once
#include "PacketSenderMessage.h"

/*
	End Message Content Definition
	< FileName(Binary) >
*/

class FileEndMessage 
	: public PacketSenderMessage
{

public:

	FileEndMessage();

	FileEndMessage(const QByteArray& data);

	~FileEndMessage();

	void setData(const QByteArray &data);
	const QByteArray& getData() const;

protected:

	virtual QByteArray generateContent() override;

	virtual void parseMessage(QDataStream& dataStream) override;

private:

	QByteArray mData;
};