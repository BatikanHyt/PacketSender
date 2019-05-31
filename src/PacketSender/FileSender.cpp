#include "FileSender.h"
#include "FileDataMessage.h"
#include "FileStartMessage.h"
#include "FileEndMessage.h"
#include "PacketSenderMessage.h"
#include "RawDataMessage.h"

#include <QtCore/QSharedPointer>


FileSender::FileSender()
{
}


FileSender::~FileSender()
{
}

QByteArray FileSender::sendData(QString fileName,const QByteArray & data)
{
	QSharedPointer<FileDataMessage> message(new FileDataMessage());
	message->setFileName(fileName);
	message->setData(data);
	QByteArray fileData = message->generateData();

	return fileData;
}

QByteArray FileSender::sendEnd(const QByteArray& data)
{
	QSharedPointer<FileEndMessage> message(new FileEndMessage());
	message->setData(data);
	QByteArray fileData = message->generateData();
	
	return fileData;
}

QByteArray FileSender::sendRawData(const QByteArray & data)
{
	QSharedPointer<RawDataMessage> message(new RawDataMessage());
	message->setData(data);
	QByteArray rawData = message->generateData();

	return rawData;
}
