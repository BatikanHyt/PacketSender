
#include "FileDataMessage.h"

#include <QtCore/QVariant>

FileDataMessage::FileDataMessage()
	: PacketSenderMessage()
	, mFileNameSize(0)
{
	setMessageType(eFileTransferData);
}

FileDataMessage::FileDataMessage(const QByteArray & data)
	: PacketSenderMessage()
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
	//dataStream << QVariant::fromValue(mFileName);
	//dataStream << QVariant::fromValue(mData);
	data.append(mFileName);
	data.append(mData);

	return data;
}

void FileDataMessage::parseMessage(QDataStream & dataStream)
{
	dataStream >> mFileNameSize;
	QByteArray fileNameData(mFileNameSize,Qt::Uninitialized);
	dataStream.readRawData(fileNameData.data(), mFileNameSize);
	mFileName = fileNameData;
	qint32 pos = dataStream.device()->pos();
	qint32 size = dataStream.device()->size();
	qint32 lenght = size - pos;
	mData = QByteArray(lenght, Qt::Uninitialized);
	dataStream.readRawData(mData.data(), lenght);
	//QVariant fileName;
	//dataStream >> fileName;
	//mFileName = fileName.toString();
	//
	//QVariant data;
	//dataStream >> data;
	//mData = data.toByteArray();
}
