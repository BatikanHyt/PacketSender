#include "FileSenderMessages.h"
#include <QDataStream>


FileSenderMessages::FileSenderMessages()
	: mMessageType(eFileTransferInvalid)
	, mContentSize(0)
{
}


FileSenderMessages::~FileSenderMessages()
{
}

QByteArray FileSenderMessages::generateData()
{
	QByteArray data;
	QByteArray content;
	
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << (quint8)getMessageType();
	content = generateContent();
	mContentSize = content.size();
	stream << mContentSize;

	data.append(content);

	return data;
	
}

FileSenderMessageType FileSenderMessages::getMessageType() const
{
	return mMessageType;
}

void FileSenderMessages::parseData(const QByteArray & data)
{
	QDataStream dataStream(data);
	dataStream >> ((quint8&)mMessageType);
	dataStream >> mContentSize;
	parseMessage(dataStream);
}

int FileSenderMessages::getHeaderSize()
{
	return 3;
}

void FileSenderMessages::parseMessage(QDataStream & dataStream)
{
	dataStream.skipRawData(mContentSize);
}

QByteArray FileSenderMessages::generateContent()
{
	return QByteArray();
}

void FileSenderMessages::setMessageType(FileSenderMessageType messageType)
{
	mMessageType = messageType;
}
