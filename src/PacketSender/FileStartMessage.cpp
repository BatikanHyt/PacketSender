
#include "FileStartMessage.h"

FileStartMessage::FileStartMessage()
	: FileSenderMessages()
	, mMessageSize(0)
	, mMessageDelay(0)
{
	setMessageType(eFileTransferStart);
}

FileStartMessage::FileStartMessage(const QByteArray & data)
	: FileSenderMessages()
	, mMessageDelay(0)
	, mMessageSize(0)
{
	parseData(data);
}

FileStartMessage::~FileStartMessage()
{
}

void FileStartMessage::setData(const QByteArray & data)
{
	mData = data;
}

const QByteArray & FileStartMessage::getData() const
{
	return mData;
}

void FileStartMessage::setMessageSize(quint32 messageSize)
{
	mMessageSize = messageSize;
}

const quint32 FileStartMessage::getMessageSize() const
{
	return mMessageSize;
}

void FileStartMessage::setMessageDelay(quint8 delay)
{
	mMessageDelay = delay;
}

const quint8 FileStartMessage::getMessageDelay() const
{
	return mMessageDelay;
}

QByteArray FileStartMessage::generateContent()
{
	QByteArray data;

	QDataStream dataStream(&data, QIODevice::WriteOnly);
	dataStream << mMessageSize;
	dataStream << mMessageDelay;

	data.append(mData);

	return data;
}

void FileStartMessage::parseMessage(QDataStream & dataStream)
{
	dataStream >> mMessageSize;
	dataStream >> mMessageDelay;
	dataStream >> mData;
}
