#include "FileEndMessage.h"

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
	data.append(mData);

	return data;
}

void FileEndMessage::parseMessage(QDataStream & dataStream)
{
	dataStream >> mData;
}

const QByteArray & FileEndMessage::getData() const
{
	return mData;
}