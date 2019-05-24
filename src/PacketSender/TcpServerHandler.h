#pragma once

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QHostAddress>
#include "FileSenderMessageType.h"

class QThread;
class QTcpSocket;
class QTcpServer;
class QAbstractSocket;
class FileSenderHandler;

class TcpServerHandler
	:public QObject
{

	Q_OBJECT

public:

	TcpServerHandler();
	
	~TcpServerHandler();

	void createTcpServer();

	void setHostAddres(QString hostAddress);

	void setListenPort(int port);

	void writeToClient(QByteArray& data);

public slots:

	void handleNewConnection();

	void handleReadyRead();

	void handleDisconnected();

	void handleError(QAbstractSocket::SocketError socketError);

	void createTcpServerInternal();

	void saveToFile(QString fileName);

private:

	QString mHostAddress;

	int mListenPort;

	QThread* mThread;

	QHash<int, QTcpSocket*> mSocketMap;

	QTcpServer* mTcpServer;

	QTcpSocket* mTcpSocket;

	quint32 mSocketId;

	QHash<FileSenderMessageType, FileSenderHandler*> mHandlerHash;

	QHash<QString, QByteArray> mFileDataBuffer;
	
	int testCount;

	int totalPacket;
};


