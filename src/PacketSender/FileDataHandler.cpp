
#include "FileDataHandler.h"
#include "FileDataMessage.h"

FileDataHandler::FileDataHandler()
	:mCount(0)
	,mPacketSizeRecv(0)
{
}


FileDataHandler::~FileDataHandler()
{
}

void FileDataHandler::handle(QByteArray & data)
{
	QSharedPointer<FileDataMessage> message(new FileDataMessage(data));

	mFileName = message->getFileName();
	mData = message->getData();
	mCount++;
	mPacketSizeRecv += mData.size();
}

QByteArray FileDataHandler::getParsedData()
{
	return mData;
}

QString FileDataHandler::getFileName()
{
	return mFileName;
}
