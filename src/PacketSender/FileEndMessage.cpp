
#include "FileEndMessage.h"

#include <QtCore/QVariant>
#include <QtCore/QDataStream>

FileEndMessage::FileEndMessage()
	: FileSenderMessages()
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
	QByteArray data;
	QDataStream dataStream(&data, QIODevice::WriteOnly);
	dataStream << QVariant::fromValue(mData);

	return data;
}

void FileEndMessage::parseMessage(QDataStream & dataStream)
{
	QVariant data;
	dataStream >> data;

	mData = data.toByteArray();
}

const QByteArray & FileEndMessage::getData() const
{
	return mData;
}