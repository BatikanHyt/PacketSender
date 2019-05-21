#include "FileDataMessage.h"



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
	dataStream << mFileNameSize;
	dataStream << mFileName;

	data.append(mData);

	return data;
}

void FileDataMessage::parseMessage(QDataStream & dataStream)
{
	dataStream >> mFileNameSize;
	QByteArray fileName = mFileName.toUtf8();
	dataStream.readRawData(fileName.data(), mFileNameSize);
	dataStream >> mData;
}
