
#pragma once
#include <QtCore/QObject>
#include <QtNetwork/QAbstractSocket>
#include <QtCore/QThread>
#include <QtCore/QHash>

#include "FileSenderMessageType.h"

class QUdpSocket;
class FileSenderHandler;

class UdpHandler
	: public QObject
{
	Q_OBJECT

public:

	UdpHandler();

	~UdpHandler();

	void setHostAddress(const QString& hostAddress);

	void setPort(quint16 port);

	void setUnicastAddress(const QString& hostAddress);

	void initializeUdpSocket();

protected:

	virtual void timerEvent(QTimerEvent* timerEvent);

	void saveToFile(QString fileName);

public slots:

	void onReadyRead();

	void onError(QAbstractSocket::SocketError socketError);

	void writeDataToUdpSocket(const QByteArray& data);

	void createUdpSocketInternal();

private:

	QString mHostAddress;

	QString mUnicastAddress;

	quint16 mUdpPort;

	QUdpSocket* mUdpSocket;

	QThread* mThread;

	QHash<FileSenderMessageType, FileSenderHandler*> mHandlerHash;

	QHash<QString, QByteArray> mFileDataBuffer;

};

