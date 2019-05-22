#include "FileSender.h"
#include "FileDataMessage.h"
#include "FileStartMessage.h"
#include "FileEndMessage.h"
#include "FileSenderMessages.h"

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

void FileSender::sendEnd(const QByteArray& data)
{

}