#include "TcpServerHandler.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QThread>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>
#include <QtCore/QTime>

#include "PacketSenderMessage.h"
#include "FileDataMessage.h"
#include "FileEndMessage.h"
#include "FileStartMessage.h"
#include "FileSenderMessageType.h"
#include "PacketSenderHandler.h"
#include "FileDataHandler.h"
#include "FileEndHandler.h"
#include "RawDataHandler.h"
#include "TrafficLoggerWidget.h"
#include "ClientListWidget.h"

TcpServerHandler::TcpServerHandler()
	: mTcpServer(0)
	, mThread(0)
	, mSocketId(0)
	, testCount(0)
	, totalPacket(0)
{

	mThread = new QThread(this);
	mTcpServer = new QTcpServer(this);
	mClientWidget = ClientListWidget::getClientListWidget();

	//qRegisterMetaType<QByteArray>("QByteArray");

	connect(mTcpServer,
		&QTcpServer::newConnection,
		this,
		&TcpServerHandler::handleNewConnection);

	connect(mClientWidget,
		&ClientListWidget::disconnectClient,
		this,
		&TcpServerHandler::disconnectClient);

	connect(mClientWidget,
		&ClientListWidget::messageFinishedEvent,
		this,
		&TcpServerHandler::writeToClient);

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
		QString info = "TCP("+mHostAddress + ":" + QString::number(mListenPort)+")";
		emit tcpServerCreated(info);
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

void TcpServerHandler::shutdownTcpServerInternal(QString info)
{
	QStringList splitter = info.split(":");
	QString hostAddress = splitter.at(0);
	int port = splitter.at(1).toInt();
	if (mHostAddress.contains(hostAddress) && mListenPort == port)
	{
		mTcpServer->close();
		qInfo() << tr("Shutting down TCP server on %1:%2")
			.arg(mHostAddress)
			.arg(mListenPort);
		mTcpServer->deleteLater();
	}
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

		connect(tcpSocket, &QTcpSocket::readyRead, this, &TcpServerHandler::handleReadyRead);
		connect(tcpSocket, &QTcpSocket::disconnected, this, &TcpServerHandler::handleDisconnected);
		connect(tcpSocket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), this, &TcpServerHandler::handleError);

		qInfo() << tr("New client connected to the Tcp server on %1:%2.")
			.arg(tcpSocket->localAddress().toString())
			.arg(tcpSocket->localPort());

		ClientInfo info;
		info.hostIp = tcpSocket->localAddress().toString();
		info.clientIp = info.hostIp;
		info.hostPort = tcpSocket->localPort();
		info.clientPort = tcpSocket->peerPort();
		info.socketId = mSocketId;

		mClientWidget->updateClientList(info);

		mSocketId++;
	}
}

void TcpServerHandler::handleReadyRead()
{
	QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>(sender());
	if (0 != tcpSocket)
	{
		quint32 socketId = tcpSocket->property("socketId").toUInt();
		tcpSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
		QByteArray readData = tcpSocket->readAll();
		mBuffer.append(readData);
		QDataStream dataStream(mBuffer);
		QByteArray completePacket;

		while(mBuffer.size()>1)
		{
			quint16 len = 0;
			quint8 mesId = 0;

			dataStream >> mesId;
			dataStream >> len;
			//Logging
			TrafficLogsItems logItems;
			logItems.toIp = "You";
			logItems.toPort = tcpSocket->localPort();
			logItems.fromIp = tcpSocket->peerAddress().toString();
			logItems.fromPort = tcpSocket->peerPort();
			logItems.method = "TCP";
			logItems.direction = "Rx";
			logItems.time = QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz");

			FileSenderMessageType messageType = (FileSenderMessageType)mesId;
			if(isValidMessageType(messageType))
			{
				QByteArray content(len + 3, Qt::Uninitialized);
				if (mBuffer.size() < len + 3)
				{
					return;
				}
				content = mBuffer.mid(0, len + 3);
				logItems.data = content;
				PacketSenderHandler* handler = mHandlerHash.value(messageType);
				if (handler != 0)
				{
					handler->handle(content);
					QString fileName = handler->getFileName();
					QByteArray fileData = handler->getParsedData();
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
				dataStream.skipRawData(len);
				mBuffer.remove(0, len + 3);
			}
			else
			{
				logItems.data = mBuffer;
				mBuffer.remove(0, mBuffer.size());
				dataStream.skipRawData(mBuffer.size());
			}
			TrafficLoggerWidget::getLoggerWidget()->updateTrafficLogger(logItems);
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

		ClientInfo info;
		info.socketId = socketId;
		mClientWidget->clientDisconnected(info);
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

void TcpServerHandler::writeToClient(QByteArray data,int socketId)
{
	QTcpSocket* clientSocket = mSocketMap[socketId];
	if (clientSocket != 0)
	{
		clientSocket->write(data, data.size());
		clientSocket->flush();
		clientSocket->waitForBytesWritten(500);
		
		//Logging
		TrafficLogsItems items;
		items.toIp = clientSocket->peerAddress().toString();
		items.toPort = clientSocket->peerPort();
		items.direction = "Tx";
		items.method = "TCP";
		items.fromIp = "You";
		items.fromPort = clientSocket->localPort();
		items.time = QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz");
		items.data = data;

		TrafficLoggerWidget::getLoggerWidget()->updateTrafficLogger(items);
	}
}

void TcpServerHandler::createTcpServer()
{
	QMetaObject::invokeMethod(this,
		"createTcpServerInternal");
}

void TcpServerHandler::shutdownTcpServer(QString info)
{
	QMetaObject::invokeMethod(this, 
		"shutdownTcpServerInternal", 
		Q_ARG(QString, info));
}

void TcpServerHandler::disconnectClient(int socketId)
{
	QTcpSocket* clientSocket = mSocketMap[socketId];
	if (clientSocket != 0)
	{
		mSocketMap.remove(socketId);
		clientSocket->deleteLater();
		qDebug() << "Socket closed";
	}
}
