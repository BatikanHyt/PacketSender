#include "FileDataHandler.h"
#include <QtCore/QSharedPointer>
#include "FileDataMessage.h"
#include <QtCore/QDebug>

FileDataHandler::FileDataHandler()
{
}


FileDataHandler::~FileDataHandler()
{
}

void FileDataHandler::handle(QByteArray & data)
{
	QSharedPointer<FileDataMessage> message(new FileDataMessage(data));

	qDebug() << "File Name : " << message->getFileName() << "FileSize : " << message->getData();

}
