#pragma once

#include <QWidget>
#include <QObject>
#include "ui_ConnectionWidget.h"

class QHostAddress;
class TcpClientHandler;
class TcpServerHandler;

class ConnectionWidget : public QWidget
{
	Q_OBJECT

public:
	ConnectionWidget(QWidget *parent = Q_NULLPTR);

	~ConnectionWidget();

	void createTcpConnection(QString hostAddress, int port);

	void createTcpServer(QString hostAddress, int port);

signals:

	void clientConnectedEvent(QString info);

public slots:

	void on_pbConnect_clicked();

	void writeToTcp(QByteArray &data);

private:

	Ui::ConnectionWidget ui;

	TcpClientHandler* mTcpClientHandler;

	TcpServerHandler* mTcpServerHandler;
};
