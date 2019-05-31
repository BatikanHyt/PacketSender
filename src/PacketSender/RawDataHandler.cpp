#include "RawDataHandler.h"
#include "RawDataMessage.h"


RawDataHandler::RawDataHandler()
{
}


RawDataHandler::~RawDataHandler()
{
}

void RawDataHandler::handle(QByteArray & data)
{
	QSharedPointer<RawDataMessage> message(new RawDataMessage(data));
	mData = message->getData();

	qDebug() << "Received data : " << mData;
}

QByteArray RawDataHandler::getParsedData()
{
	return mData;
}

QString RawDataHandler::getFileName()
{
	return QString();
}
