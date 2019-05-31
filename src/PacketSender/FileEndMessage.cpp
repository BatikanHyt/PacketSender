
#include "FileEndMessage.h"

#include <QtCore/QVariant>
#include <QtCore/QDataStream>

FileEndMessage::FileEndMessage()
	: PacketSenderMessage()
{
	setMessageType(eFileTransferEnd);
}

FileEndMessage::FileEndMessage(const QByteArray & data)
{
	parseData(data);
}


FileEndMessage::~FileEndMessage()
{
}

void FileEndMessage::setData(const QByteArray & data)
{
	mData = data;
}

QByteArray FileEndMessage::generateContent()
{
	//QByteArray data;
	//QDataStream dataStream(&data, QIODevice::WriteOnly);
	//dataStream << QVariant::fromValue(mData);
	//dataStream.append(mData);

	//return data;

	return mData;
}

void FileEndMessage::parseMessage(QDataStream & dataStream)
{
	//QVariant data;
	qint32 pos = dataStream.device()->pos();
	qint32 size = dataStream.device()->size();
	qint32 length = size - pos;
	mData = QByteArray(length, Qt::Uninitialized);
	dataStream.readRawData(mData.data(),size);

	//mData = data.toByteArray();
}

const QByteArray & FileEndMessage::getData() const
{
	return mData;
}