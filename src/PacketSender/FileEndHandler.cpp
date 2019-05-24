
#include "FileEndHandler.h"
#include "FileEndMessage.h"

FileEndHandler::FileEndHandler()
{
}

FileEndHandler::~FileEndHandler()
{
}

void FileEndHandler::handle(QByteArray & data)
{
	QSharedPointer<FileEndMessage> message(new FileEndMessage(data));
	
	mFileName = message->getData();

	qDebug() << "Transfer finished for the file " << mFileName;
}

QString FileEndHandler::getFileName()
{
	return mFileName;
}

QByteArray FileEndHandler::getParsedData()
{
	return QByteArray();
}
