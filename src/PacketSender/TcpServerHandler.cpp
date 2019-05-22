#include "TcpServerHandler.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QThread>
#include <QtCore/QFile>

#include "FileSenderMessages.h"
#include "FileDataMessage.h"
#include "FileEndMessage.h"
#include "FileStartMessage.h"
#include "FileSenderMessageType.h"
#include "FileSenderHandler.h"
#include "FileDataHandler.h"
#include "FileEndHandler.h"

TcpServerHandler::TcpServerHandler()
	: mTcpServer(0)
	, mThread(0)
	, mSocketId(0)
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

void TcpServerHandler::handleNewConnection()
{
	if (mTcpServer->hasPendingConnections())
	{
		QTcpSocket* tcpSocket = mTcpServer->nextPendingConnection();
		mTcpSocket = tcpSocket;
		tcpSocket->setProperty("socketId", mSocketId);

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
	QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>(sender());
	if (0 != tcpSocket)
	{
		quint32 socketId = tcpSocket->property("socketId").toUInt();

		QByteArray data = tcpSocket->readAll();
		qInfo() << QString("Data read from client: ") << data;
		//FileSenderMessages message;
		FileSenderMessages message;
		message.parseData(data);
		
		FileSenderMessageType messageType = message.getMessageType();
		FileSenderHandler* handler = mHandlerHash.value(messageType);
		if (handler != 0)
		{
			handler->handle(data);
		}
		//if (messageType == eFileTransferData)
		//{
		//	qDebug() << "Data message";
		//}

		//QFile sampleFile("ReceivedMessage");
		//if (!sampleFile.open(QFile::WriteOnly | QFile::Append))
		//{
		//	return;
		//}
		//sampleFile.write(data);
		//sampleFile.flush();
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