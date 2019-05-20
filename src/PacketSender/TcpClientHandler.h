#pragma once

#include <QObject>
#include <QtCore/QHash>

class QHostAddress;
class QThread;
class QTcpSocket;

class TcpClientHandler
	: public QObject
{

	Q_OBJECT

public:
	TcpClientHandler();

	~TcpClientHandler();

	bool createTcpClientConnection(QString hostAddr, int port);

signals:

	void clientConnectedEvent();

public slots:

	void handleReadyRead();

	void handleDisconnected();

	void writeIt(QByteArray &data);

private:

	QThread *mThread;

	int mSocketId;

	QHash<int, QTcpSocket*> mSocketMap;
};
