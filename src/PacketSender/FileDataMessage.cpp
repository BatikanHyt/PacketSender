
#include "FileDataMessage.h"

#include <QtCore/QVariant>

FileDataMessage::FileDataMessage()
	: FileSenderMessages()
	, mFileNameSize(0)
{
	setMessageType(eFileTransferData);
}

FileDataMessage::FileDataMessage(const QByteArray & data)
	: FileSenderMessages()
	, mFileNameSize(0)
{
	parseData(data);
}

FileDataMessage::~FileDataMessage()
{
}

void FileDataMessage::setData(const QByteArray & data)
{
	mData = data;
}

const QByteArray & FileDataMessage::getData() const
{
	return mData;
}

void FileDataMessage::setFileName(QString fileName)
{
	mFileName = fileName;
	mFileNameSize = fileName.size();
}

const QString FileDataMessage::getFileName() const
{
	return mFileName;
}

const quint8 FileDataMessage::getFileNameSize() const
{
	return mFileNameSize;
}

QByteArray FileDataMessage::generateContent()
{
	QByteArray data;
	
	QDataStream dataStream(&data, QIODevice::WriteOnly);
	dataStream << QVariant::fromValue(mFileName);
	dataStream << QVariant::fromValue(mData);

	return data;
}

void FileDataMessage::parseMessage(QDataStream & dataStream)
{
	QVariant fileName;
	dataStream >> fileName;
	mFileName = fileName.toString();
	
	QVariant data;
	dataStream >> data;
	mData = data.toByteArray();
}
