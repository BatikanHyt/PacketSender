
#include "UdpHandler.h"

#include "FileSenderHandler.h"
#include "FileSenderMessages.h"
#include "FileSenderMessageType.h"
#include "FileSenderHandler.h"
#include "FileDataHandler.h"
#include "FileEndHandler.h"

#include <QtCore/QFile>
#include <QtNetwork/QUdpSocket>

UdpHandler::UdpHandler()
	: mThread(0)
	, mUdpSocket(0)
	, mUdpPort(0)
{
	mThread = new QThread(this);
	mUdpSocket = new QUdpSocket(this);

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

void UdpHandler::initializeUdpSocket()
{
	QMetaObject::invokeMethod(this,
		"createUdpSocketInternal");
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
	if (QAbstractSocket::BoundState == mUdpSocket->state())
	{
		qint64 written = mUdpSocket->writeDatagram(
			data,
			QHostAddress(mUnicastAddress),
			mUdpPort);
	}
}

void UdpHandler::createUdpSocketInternal()
{
	bool bound = mUdpSocket->bind(QHostAddress(mHostAddress), mUdpPort);
	if (bound)
	{
		qInfo() <<
			QString("Udp socket bound successfully on %1:%2")
			.arg(mHostAddress)
			.arg(mUdpPort);
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
			QByteArray readData(size, Qt::Uninitialized);
			mUdpSocket->readDatagram(readData.data(), size);
			
			FileSenderMessages message;
			message.parseData(readData);

			FileSenderMessageType messageType = message.getMessageType();
			FileSenderHandler *handler = mHandlerHash.value(messageType);

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
			else
			{
				qDebug("Unexpected");
			}

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