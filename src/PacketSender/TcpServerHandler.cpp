#include "TcpServerHandler.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QThread>

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

void TcpServerHandler::createTcpServer()
{
	QMetaObject::invokeMethod(this,
		"createTcpServerInternal");
}