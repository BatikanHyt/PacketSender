
#include "RawDataMessage.h"
#include <QtCore/QDataStream>

RawDataMessage::RawDataMessage()
	: PacketSenderMessage()
{
	setMessageType(eRawDataTransfer);
}

RawDataMessage::RawDataMessage(const QByteArray & data)
	: PacketSenderMessage()
{
	parseData(data);
}

RawDataMessage::~RawDataMessage()
{
}

void RawDataMessage::setData(const QByteArray & data)
{
	mData = data;
}

const QByteArray & RawDataMessage::getData() const
{
	return mData;
}

QByteArray RawDataMessage::generateContent()
{
	QByteArray data;
	data.append(mData);

	return data;
}

void RawDataMessage::parseMessage(QDataStream &dataStream)
{
	qint32 pos = dataStream.device()->pos();
	qint32 size = dataStream.device()->size();
	qint32 lenght = size - pos;

	mData = QByteArray(lenght, Qt::Uninitialized);
	dataStream.readRawData(mData.data(), lenght);
}
