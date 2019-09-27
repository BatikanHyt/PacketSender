
#include "UdpHandler.h"

#include "PacketSenderHandler.h"
#include "PacketSenderMessage.h"
#include "FileSenderMessageType.h"
#include "FileDataHandler.h"
#include "FileEndHandler.h"
#include "RawDataHandler.h"
#include "TrafficLoggerWidget.h"

#include <QtCore/QFile>
#include <QtCore/QTime>
#include <QtNetwork/QUdpSocket>

UdpHandler::UdpHandler()
	: mThread(0)
	, mUdpSocket(0)
	, mUdpPort(0)
{
	mThread = new QThread(this);
	mUdpSocket = new QUdpSocket(this);

	qRegisterMetaType<QAbstractSocket::SocketError>("SocketError");

	connect(mUdpSocket,
		&QUdpSocket::readyRead,
		this,
		&UdpHandler::onReadyRead);

	connect(mUdpSocket,
		QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
		this,
		&UdpHandler::onError);

	moveToThread(mThread);
	mThread->start();

	mHandlerHash.insert(eFileTransferData, new FileDataHandler());
	mHandlerHash.insert(eFileTransferEnd, new FileEndHandler());
	mHandlerHash.insert(eRawDataTransfer, new RawDataHandler());
}


UdpHandler::~UdpHandler()
{
}

void UdpHandler::setHostAddress(const QString & hostAddress)
{
	mHostAddress = hostAddress;
}

void UdpHandler::setPort(quint16 port)
{
	mUdpPort = port;
}

void UdpHandler::setUnicastAddress(const QString & hostAddress)
{
	mUnicastAddress = hostAddress;
}

void UdpHandler::setBindPort(quint16 port)
{
	mBindPort = port;
}

void UdpHandler::initializeUdpSocket()
{
	QMetaObject::invokeMethod(this,
		"createUdpSocketInternal");
}

void UdpHandler::unboundUdpSocket()
{
	QMetaObject::invokeMethod(this,
		"unboundUdpSocketInternal");
}

void UdpHandler::unboundUdpSocketInternal()
{
	mUdpSocket->close();
	qInfo() <<
		QString("Udp socket unbound successfully on %1:%2")
		.arg(mHostAddress)
		.arg(mBindPort);

	QString info = mUnicastAddress + ":" + QString::number(mBindPort);
	emit udpSocketClosed(info);
	mUdpSocket->deleteLater();
	
}

void UdpHandler::timerEvent(QTimerEvent * timerEvent)
{
}

void UdpHandler::onError(QAbstractSocket::SocketError socketError)
{
	qWarning() << "Udp Relay socket error: " << mUdpSocket->errorString();
}

void UdpHandler::writeDataToUdpSocket(const QByteArray & data)
{
	if (mUdpSocket != 0)
	{
		if (QAbstractSocket::BoundState == mUdpSocket->state())
		{
			mUdpSocket->writeDatagram(data,
				QHostAddress(mUnicastAddress),
				mUdpPort);
			//Logging
			TrafficLogsItems items;
			items.toIp = mUnicastAddress;
			items.toPort = mUdpPort;
			items.fromIp = mUdpSocket->localAddress().toString();
			items.fromPort = mUdpSocket->localPort();
			items.direction = "Tx";
			items.method = "UDP";
			items.time = QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz");

			TrafficLoggerWidget::getLoggerWidget()->updateTrafficLogger(items);
		}
	}
}

void UdpHandler::createUdpSocketInternal()
{
	bool bound = mUdpSocket->bind(QHostAddress(mHostAddress), mBindPort);
	if (bound)
	{
		qInfo() <<
			QString("Udp socket bound successfully on %1:%2")
			.arg(mHostAddress)
			.arg(mBindPort);
		QString info = mUnicastAddress + ":" + QString::number(mUdpPort) +":"+mHostAddress + ":" + QString::number(mBindPort);
		emit udpSocketCreated(info);
	}
	else
	{
		qWarning() <<
			QString("Cound not bind udp socket on %1:%2")
			.arg(mHostAddress)
			.arg(mUdpPort);
	}
}


void UdpHandler::onReadyRead()
{
	if (mUdpSocket->hasPendingDatagrams())
	{
		qint32 size = mUdpSocket->pendingDatagramSize();
		if (size > 0)
		{
			QHostAddress sender;
			quint16 senderPort;
			QByteArray readData(size, Qt::Uninitialized);
			mUdpSocket->readDatagram(readData.data(), size, &sender, &senderPort);
			
			PacketSenderMessage message;
			message.parseData(readData);

			FileSenderMessageType messageType = message.getMessageType();
			PacketSenderHandler *handler = mHandlerHash.value(messageType);

			if (handler != 0)
			{
				handler->handle(readData);
				QString fileName = handler->getFileName();
				QByteArray fileData = handler->getParsedData();
				if (messageType == eFileTransferData)
				{
					if (mFileDataBuffer.contains(fileName))
					{
						mFileDataBuffer[fileName].append(fileData);
					}
					else
					{
						mFileDataBuffer.insert(fileName, fileData);
					}
				}
				else if (messageType == eFileTransferEnd)
				{
					saveToFile(fileName);
				}
			}
			TrafficLogsItems items;
			items.toIp = mUdpSocket->localAddress().toString();
			items.toPort = mUdpSocket->localPort();
			items.fromIp = sender.toString();
			items.fromPort = senderPort;
			items.direction = "Rx";
			items.method = "UDP";
			items.time = QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz");
			items.data = readData;

			TrafficLoggerWidget::getLoggerWidget()->updateTrafficLogger(items);
		}
	}
}

void UdpHandler::saveToFile(QString fileName)
{
	QFile sampleFile("ReceivedMessages/" + fileName);
	if (!sampleFile.open(QFile::WriteOnly | QFile::Truncate))
	{
		return;
	}
	QByteArray completeData = mFileDataBuffer[fileName];
	sampleFile.write(completeData);
	sampleFile.flush();
	sampleFile.close();
	mFileDataBuffer.remove(fileName);
}