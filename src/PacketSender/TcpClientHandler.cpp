#include "TcpClientHandler.h"

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

#include <QtCore/QThread>


TcpClientHandler::TcpClientHandler()
	: mSocketId(0)
	, sendCount(0)
{
	mThread = new QThread(this);
	moveToThread(mThread);
	mThread->start();
}


TcpClientHandler::~TcpClientHandler()
{
}


bool TcpClientHandler::createTcpClientConnection(QString hostAddr, int port)
{
	QTcpSocket* tcpSocket = new QTcpSocket();
	connect(tcpSocket,
		&QTcpSocket::readyRead,
		this,
		&TcpClientHandler::handleReadyRead);

	connect(tcpSocket,
		&QTcpSocket::disconnected,
		this,
		&TcpClientHandler::handleDisconnected);

	tcpSocket->setProperty("socketId", mSocketId);
	tcpSocket->connectToHost(hostAddr, port);

	qInfo() << tr("Trying to connect remote server on %1:%2.")
		.arg(hostAddr)
		.arg(port);

	tcpSocket->waitForConnected(500);

	if (tcpSocket->state() == QAbstractSocket::ConnectedState)
	{
		qInfo() << tr("Connected to remote server on %1:%2.")
			.arg(hostAddr)
			.arg(port);
		mSocketMap.insert(mSocketId, tcpSocket);
		mSocketId++;
		return true;
	}
	else
	{
		qInfo() << tr("Unable to connect remote server on %1:%2.")
			.arg(hostAddr)
			.arg(port);

		tcpSocket->disconnect();
		tcpSocket->close();
		tcpSocket->deleteLater();
	}
	return false;
}

void TcpClientHandler::writeToSocket(QByteArray &data)
{
	//qDebug() << "Test";
	QString dataStr = data.constData();
	//qDebug() << "Data is: " << dataStr;
	QTcpSocket* tcpSocket = mSocketMap.begin().value();
	if (0 != tcpSocket)
	{
		//qDebug() << "interface id: "<<mInterfaceId << " socket id: "<<socketId<<" data read in tcprelayclient: " << data;
		int written = tcpSocket->write(data,data.size());
		if (written != data.size())
		{
			qWarning() << "Could not write data.";
		}
		tcpSocket->flush();
		tcpSocket->waitForBytesWritten(3000);
		sendCount++;
	}
}


void TcpClientHandler::handleReadyRead()
{

}

void TcpClientHandler::handleDisconnected()
{
	QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>(sender());
	if (0 != tcpSocket)
	{
		int socketId = tcpSocket->property("socketId").toInt();

		qInfo() << QString("Unable to connect TCP server on %1:%2.")
			.arg(tcpSocket->localAddress().toString())
			.arg(tcpSocket->localPort());
		qDebug() << tr("Deleting the socket with id %1").arg(socketId);
		tcpSocket->disconnect();
		tcpSocket->close();
		tcpSocket->deleteLater();
	}
}
