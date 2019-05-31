#include "TcpServerHandler.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QThread>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>

#include "PacketSenderMessage.h"
#include "FileDataMessage.h"
#include "FileEndMessage.h"
#include "FileStartMessage.h"
#include "FileSenderMessageType.h"
#include "PacketSenderHandler.h"
#include "FileDataHandler.h"
#include "FileEndHandler.h"
#include "RawDataHandler.h"

TcpServerHandler::TcpServerHandler()
	: mTcpServer(0)
	, mThread(0)
	, mSocketId(0)
	, testCount(0)
	, totalPacket(0)
{

	mThread = new QThread(this);
	mTcpServer = new QTcpServer(this);

	connect(mTcpServer,
		&QTcpServer::newConnection,
		this,
		&TcpServerHandler::handleNewConnection);

	moveToThread(mThread);
	mThread->start();

	mHandlerHash.insert(eFileTransferData, new FileDataHandler());
	mHandlerHash.insert(eFileTransferEnd, new FileEndHandler());
	mHandlerHash.insert(eRawDataTransfer, new RawDataHandler());

}


TcpServerHandler::~TcpServerHandler()
{
}

void TcpServerHandler::createTcpServerInternal()
{
	bool result = mTcpServer->listen(QHostAddress(mHostAddress), mListenPort);
	if (result)
	{
		qInfo() << tr("Tcp server is listening on %1:%2")
			.arg(mHostAddress)
			.arg(mListenPort);
	}
	else
	{
		qWarning() << tr("Tcp server could not listen on %1:%2")
			.arg(mHostAddress)
			.arg(mListenPort);
	}
}

void TcpServerHandler::saveToFile(QString fileName)
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

void TcpServerHandler::handleNewConnection()
{
	if (mTcpServer->hasPendingConnections())
	{
		QTcpSocket* tcpSocket = mTcpServer->nextPendingConnection();
		mTcpSocket = tcpSocket;
		tcpSocket->setProperty("socketId", mSocketId);
		tcpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 64 * 1024 * 1024);
		mSocketMap.insert(mSocketId, tcpSocket);
		mSocketId++;

		connect(tcpSocket, &QTcpSocket::readyRead, this, &TcpServerHandler::handleReadyRead);
		connect(tcpSocket, &QTcpSocket::disconnected, this, &TcpServerHandler::handleDisconnected);
		connect(tcpSocket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), this, &TcpServerHandler::handleError);

		qInfo() << tr("New client connected to the Tcp server on %1:%2.")
			.arg(tcpSocket->localAddress().toString())
			.arg(tcpSocket->localPort());
		int port = tcpSocket->localPort();

	}
}

void TcpServerHandler::handleReadyRead()
{
	static QByteArray readBefore;
	QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>(sender());
	if (0 != tcpSocket)
	{
		quint32 socketId = tcpSocket->property("socketId").toUInt();
		tcpSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
		//QByteArray data = tcpSocket->readAll();
		QByteArray readData = tcpSocket->readAll();
		readData.insert(0, readBefore);
		readBefore.clear();
		QDataStream dataStream(readData);

		while(true)
		{
			//QByteArray messageId = tcpSocket->read(1);
			//QByteArray contentLenght = tcpSocket->read(2);
			quint16 len = 0;
			quint8 mesId = 0;

			dataStream >> mesId;
			dataStream >> len;

			QByteArray content(len, Qt::Uninitialized);
			int readLength = dataStream.readRawData(content.data(), len);
			if (readData.size() > len + 3)
			{
				readBefore.append(readData.mid(len + 3, readData.size()));
				qWarning() << "Fazla geldi. Data size " << readData.size() << " len " << readLength;
			}
			else if (len > readLength)
			{
				readBefore.append(readData);
				qWarning() << "Az geldi";
				return;
			}

			//		QDataStream stream1(messageId);
			//		QDataStream stream2(contentLenght);
					//stream1 >> mesId;
					//stream2 >> len;

					/*
					while (tcpSocket->bytesAvailable() < len)
					{
						tcpSocket->waitForReadyRead(500);
					}
					*/
					//QByteArray rawData = tcpSocket->read(len);
					//QByteArray data;
					//data.append(messageId);
					//data.append(contentLenght);
					//data.append(rawData);
					//PacketSenderMessage message;
					//message.parseData(readData);

			FileSenderMessageType messageType = (FileSenderMessageType)mesId;
			PacketSenderHandler* handler = mHandlerHash.value(messageType);
			if (handler != 0)
			{
				handler->handle(readData);
				QString fileName = handler->getFileName();
				QByteArray fileData = handler->getParsedData();
				//qDebug() << "Received packet data size : " << fileData.size();
				testCount += fileData.size();
				if (messageType == eFileTransferData)
				{
					totalPacket++;
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
					qDebug() << "Total bytes received for the " << fileName << " is " << QString::number(mFileDataBuffer[fileName].size());
					qDebug() << "Number of packet recv: " << totalPacket;
					saveToFile(fileName);
					totalPacket = 0;
					testCount = 0;
				}
			}
			else
			{
				qDebug("Unexpected message id.");
			}
		}

		
	}
}

void TcpServerHandler::handleDisconnected()
{
	QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>(sender());
	if (0 != tcpSocket)
	{
		qInfo() << QString("Client disconnected from TCP server on %1:%2.")
			.arg(tcpSocket->localAddress().toString())
			.arg(tcpSocket->localPort());
		quint32 socketId = tcpSocket->property("socketId").toUInt();
		mSocketMap.remove(socketId);
		tcpSocket->disconnect();
		tcpSocket->close();
		tcpSocket->deleteLater();
		qDebug() << "Disconnected socket id " << socketId;

	}
}

void TcpServerHandler::handleError(QAbstractSocket::SocketError socketError)
{
	QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>(sender());
	if (0 != tcpSocket)
	{
		qWarning() << tcpSocket->errorString();
	}
}

void TcpServerHandler::setHostAddres(QString address)
{
	mHostAddress = address;
}

void TcpServerHandler::setListenPort(int port)
{
	mListenPort = port;
}

void TcpServerHandler::writeToClient(QByteArray & data)
{
	QTcpSocket* clientSocket = mSocketMap.begin().value();
	
}

void TcpServerHandler::createTcpServer()
{
	QMetaObject::invokeMethod(this,
		"createTcpServerInternal");
}