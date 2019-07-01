#pragma once

#include <QWidget>
#include <QObject>
#include "ui_ConnectionWidget.h"

class QHostAddress;
class TcpClientHandler;
class TcpServerHandler;
class UdpHandler;

class ConnectionWidget : public QWidget
{
	Q_OBJECT

public:
	ConnectionWidget(QWidget *parent = Q_NULLPTR);

	~ConnectionWidget();

	void createTcpConnection(QString hostAddress, int port);

	void createTcpServer(QString hostAddress, int port);

	void createUdpSocket(QString hostAddress, QString unicastAddress , int bindPort ,int port);

signals:

	void clientConnectedEvent(QString protocol, QString info);

	void clientDisconnectedEvent(QString protocol, QString info);

	void udpSocketCreatedEvent(QString protocol,QString info);

	void serverCreatedEvent(QString info);

public slots:

	void on_pbConnect_clicked();

	void onWriteToTcpClient(QByteArray &data,QString info);

	void onWriteToTcpServer(QByteArray &data,int socketId);

	void onClientDisconnected(QString protocol, QString info);

	void on_rbUdp_clicked();

	void on_rbTcp_clicked();

	void onWriteToUdpSocket(QByteArray& data,QString info);

	void onUdpSocketCreate(QString info);

	void onTcpServerCreated(QString info);

	void shutdownServerEvent(QString info);

	void onUdpSocketClosed(QString info);

private:

	Ui::ConnectionWidget ui;

	TcpClientHandler* mTcpClientHandler;

	TcpServerHandler* mTcpServerHandler;

	UdpHandler* mUdpHandler;

	QHash<QString, TcpServerHandler*> mTcpServerHash;

	QHash<QString, UdpHandler*> mUdpHash;

	QHash<QString, QString> mUdpConnectionHash;
};
