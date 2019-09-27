#include "TcpClientHandler.h"

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtCore/QMutexLocker>

#include <QtCore/QThread>
#include <QtCore/QTime>
#include "TrafficLoggerWidget.h"


TcpClientHandler::TcpClientHandler()
	: mSocketId(0)
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
	tcpSocket->setProperty("PeerHost", hostAddr);
	tcpSocket->setProperty("PeerPort", port);
	tcpSocket->connectToHost(hostAddr, port);

	tcpSocket->waitForConnected(500);

	if (tcpSocket->state() == QAbstractSocket::ConnectedState)
	{
		qInfo() << tr("Connected to remote server on %1:%2.")
			.arg(tcpSocket->peerAddress().toString())
			.arg(tcpSocket->peerPort());
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

void TcpClientHandler::writeToSocket(QByteArray &data,QString info)
{
	QMutexLocker locker(&mMutex);
	//Parsing The Info
	info.replace(":", " ");
	QStringList infoList = info.split(" ");
	QString hostAddress = infoList.at(0);
	int portNumber = infoList.at(1).toInt();
	int socketId = -1;
	QHashIterator<int, QTcpSocket*>itor(mSocketMap);
	while (itor.hasNext())
	{
		itor.next();
		QString address = itor.value()->property("PeerHost").toString();
		int port = itor.value()->property("PeerPort").toInt();
		if (portNumber == port && hostAddress.contains(address))
		{
			socketId = itor.key();
			break;
		}
	}
	if (mSocketMap.contains(socketId))
	{
		QTcpSocket* tcpSocket = mSocketMap.value(socketId, 0);
		if (0 != tcpSocket)
		{
			if (tcpSocket->state() == QAbstractSocket::ConnectedState)
			{
				int written = tcpSocket->write(data, data.size());
				if (written != data.size())
				{
					qWarning() << "Could not write data.";
				}
				tcpSocket->flush();
				tcpSocket->waitForBytesWritten(3000);
				//Logging
				TrafficLogsItems items;
				items.toIp = tcpSocket->peerAddress().toString();
				items.toPort = tcpSocket->peerPort();
				items.direction = "Tx";
				items.method = "TCP";
				items.fromIp = "You";
				items.fromPort = tcpSocket->localPort();
				items.time = QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz");
				items.data = data;

				TrafficLoggerWidget::getLoggerWidget()->updateTrafficLogger(items);
			}
		}
	}
}


void TcpClientHandler::handleReadyRead()
{
	QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>(sender());
	if (tcpSocket != 0)
	{
		QByteArray data = tcpSocket->readAll();
		TrafficLogsItems logItems;
		logItems.toIp = "You";
		logItems.toPort = tcpSocket->localPort();
		logItems.fromIp = tcpSocket->peerAddress().toString();
		logItems.fromPort = tcpSocket->peerPort();
		logItems.method = "TCP";
		logItems.direction = "Rx";
		logItems.time = QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz");
		logItems.data = data;

		TrafficLoggerWidget::getLoggerWidget()->updateTrafficLogger(logItems);
		qDebug() << "Client receive message" << data;
	}
}

void TcpClientHandler::handleDisconnected()
{
	QMutexLocker locker(&mMutex);
	QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>(sender());
	if (0 != tcpSocket)
	{

		int socketId = tcpSocket->property("socketId").toInt();
		QString peerHostAddress = mSocketMap[socketId]->property("PeerHost").toString();
		int peerHostPort = mSocketMap[socketId]->property("PeerPort").toInt();

		qInfo() << QString("Disconnected to TCP server on %1:%2.")
			.arg(peerHostAddress)
			.arg(peerHostPort);

		QString protocol = "TCP";
		QString info = peerHostAddress + ":" + QString::number(peerHostPort);

		qDebug() << tr("Deleting the socket with id %1").arg(socketId);

		tcpSocket->disconnect();
		tcpSocket->close();
		tcpSocket->deleteLater();
		
		mSocketMap[socketId]->deleteLater();
		mSocketMap.remove(socketId);
		
		emit clientSocketDisconnectedEvent(protocol, info);
	}
}
