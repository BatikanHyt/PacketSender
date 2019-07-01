#pragma once

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QHostAddress>
#include "FileSenderMessageType.h"
#include <QtCore/QMutex>

class QThread;
class QTcpSocket;
class QTcpServer;
class QAbstractSocket;
class PacketSenderHandler;
class ClientListWidget;

class TcpServerHandler
	:public QObject
{

	Q_OBJECT

public:

	TcpServerHandler();
	
	~TcpServerHandler();

	void createTcpServer();

	void shutdownTcpServer(QString info);

	void setHostAddres(QString hostAddress);

	void setListenPort(int port);

public slots:

	void handleNewConnection();

	void handleReadyRead();

	void handleDisconnected();

	void handleError(QAbstractSocket::SocketError socketError);

	void createTcpServerInternal();

	void saveToFile(QString fileName);

	void shutdownTcpServerInternal(QString info);

	void disconnectClient(int sockedId);

	void writeToClient(QByteArray data, int socketId);

signals:

	void tcpServerCreated(QString info);

private:

	QString mHostAddress;

	int mListenPort;

	QThread* mThread;

	QHash<int, QTcpSocket*> mSocketMap;

	QTcpServer* mTcpServer;

	QTcpSocket* mTcpSocket;

	quint32 mSocketId;

	QHash<FileSenderMessageType, PacketSenderHandler*> mHandlerHash;

	QHash<QString, QByteArray> mFileDataBuffer;
	
	int testCount;

	int totalPacket;

	QByteArray mBuffer;

	QMutex mMutex;

	ClientListWidget* mClientWidget;
};


