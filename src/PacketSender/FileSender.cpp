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

void FileSender::sendData(const QByteArray & data)
{
	QSharedPointer<FileDataMessage> message(new FileDataMessage());
	
}
