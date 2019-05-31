
#include "PacketSenderMessage.h"
#include <QDataStream>

PacketSenderMessage::PacketSenderMessage()
	: mMessageType(eFileTransferInvalid)
	, mContentSize(0)
{
}


PacketSenderMessage::~PacketSenderMessage()
{
}

QByteArray PacketSenderMessage::generateData()
{
	QByteArray data;
	QByteArray content;
	
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << (quint8)getMessageType();
	content = generateContent();
	mContentSize = content.size();
	stream << mContentSize;

	data.append(content);

	return data;
	
}

FileSenderMessageType PacketSenderMessage::getMessageType() const
{
	return mMessageType;
}

void PacketSenderMessage::parseData(const QByteArray & data)
{
	QDataStream dataStream(data);
	dataStream.operator>>((quint8&)mMessageType);
	dataStream >> mContentSize;
	parseMessage(dataStream);
}

int PacketSenderMessage::getHeaderSize()
{
	return 3;
}

quint16 PacketSenderMessage::getContentSize()
{
	return mContentSize;
}

void PacketSenderMessage::parseMessage(QDataStream & dataStream)
{
	dataStream.skipRawData(mContentSize);
}

QByteArray PacketSenderMessage::generateContent()
{
	return QByteArray();
}

void PacketSenderMessage::setMessageType(FileSenderMessageType messageType)
{
	mMessageType = messageType;
}

void PacketSenderMessage::setContentSize(quint16 size)
{
	mContentSize = size;
}
