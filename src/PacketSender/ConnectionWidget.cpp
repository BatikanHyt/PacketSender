#include "ConnectionWidget.h"
#include "TcpClientHandler.h"
#include "TcpServerHandler.h"
#include "UdpHandler.h"

#include <QtCore/QDebug>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtCore/QDir>

ConnectionWidget::ConnectionWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	mTcpClientHandler = new TcpClientHandler();
	mUdpHandler = new UdpHandler();
	connect(mTcpClientHandler,
		&TcpClientHandler::clientSocketDisconnectedEvent,
		this,
		&ConnectionWidget::onClientDisconnected);

	ui.lHost->setVisible(false);
	ui.leHost->setVisible(false);

	QDir dir;
	dir.mkpath("ReceivedMessages");
}

ConnectionWidget::~ConnectionWidget()
{
}

void ConnectionWidget::on_pbConnect_clicked()
{
	int portNumber = ui.lePort->text().toInt();
	QString serverIp = ui.leIP->text();
	QString hostAddress = ui.leHost->text();
	if (ui.rbTcp->isChecked())
	{
		if (ui.cbMode->currentText().contains("Client"))
		{
			createTcpConnection(serverIp,portNumber);
		}
		else
		{
			createTcpServer(serverIp, portNumber);
		}

	}
	else if (ui.rbUdp->isChecked())
	{
		createUdpSocket(hostAddress,serverIp,portNumber);
	}
}

void ConnectionWidget::createTcpConnection(QString hostAddress, int portNumber)
{
	int result = mTcpClientHandler->createTcpClientConnection(hostAddress, portNumber);
	if (result)
	{
		QString info = hostAddress + ":" + QString::number(portNumber);
		QString protocol = "TCP";
		emit clientConnectedEvent(protocol,info);
	}
}

void ConnectionWidget::createTcpServer(QString hostAddress, int portNumber)
{
	mTcpServerHandler = new TcpServerHandler();
	mTcpServerHandler->setHostAddres(hostAddress);
	mTcpServerHandler->setListenPort(portNumber);
	mTcpServerHandler->createTcpServer();

	connect(mTcpServerHandler,
		&TcpServerHandler::tcpServerCreated,
		this,
		&ConnectionWidget::onTcpServerCreated);
}

void ConnectionWidget::onTcpServerCreated(QString info)
{
	QStringList splitter = info.split("(");
	QString protocol = splitter.at(0);
	QString serverInfo = splitter.at(1);
	serverInfo.replace(")", "");
	mTcpServerHash.insert(serverInfo, mTcpServerHandler);

	emit serverCreatedEvent(info);
}

void ConnectionWidget::shutdownServerEvent(QString info)
{
	QStringList splitter = info.split("(");
	QString protocol = splitter.at(0);
	QString serverInfo = splitter.at(1);
	serverInfo.replace(")", "");
	if (protocol.contains("TCP"))
	{
		mTcpServerHash[serverInfo]->shutdownTcpServer(serverInfo);
		mTcpServerHash.remove(serverInfo);
	}
	else if (protocol.contains("UDP"))
	{
		mUdpHash[serverInfo]->unboundUdpSocket();
		mUdpHash.remove(serverInfo);
	}
}

void ConnectionWidget::createUdpSocket(QString hostAddress, QString unicastAddress, int port)
{
	mUdpHandler = new UdpHandler();
	mUdpHandler->setHostAddress(hostAddress);
	mUdpHandler->setPort(port);
	mUdpHandler->setUnicastAddress(unicastAddress);
	mUdpHandler->initializeUdpSocket();

	connect(mUdpHandler,
		&UdpHandler::udpSocketCreated,
		this,
		&ConnectionWidget::onUdpSocketCreate);

	connect(mUdpHandler,
		&UdpHandler::udpSocketClosed,
		this,
		&ConnectionWidget::onUdpSocketClosed);
}

void ConnectionWidget::onWriteToTcpClient(QByteArray &data,QString info)
{
	mTcpClientHandler->writeToSocket(data,info);
}

void ConnectionWidget::onWriteToTcpServer(QByteArray & data,QString info)
{
	mTcpServerHandler->writeToClient(data,info);
}

void ConnectionWidget::onClientDisconnected(QString protocol, QString info)
{
	emit clientDisconnectedEvent(protocol,info);
}

void ConnectionWidget::onUdpSocketClosed(QString info)
{
	emit clientDisconnectedEvent("UDP", info);
}

void ConnectionWidget::on_rbUdp_clicked()
{
	ui.label_2->setText("UnicastAddress:");
	ui.leHost->setVisible(true);
	ui.lHost->setVisible(true);
	ui.cbMode->setDisabled(true);
}

void ConnectionWidget::on_rbTcp_clicked()
{
	ui.label_2->setText("Server IP:");
	ui.leHost->setVisible(false);
	ui.lHost->setVisible(false);
	ui.cbMode->setDisabled(false);
}

void ConnectionWidget::onWriteToUdpSocket(QByteArray & data)
{
	mUdpHandler->writeDataToUdpSocket(data);
}

void ConnectionWidget::onUdpSocketCreate(QString info)
{
	QStringList splitter = info.split(":");
	QString uniHostInfo = splitter.at(0) + ":" + splitter.at(1);
	QString hostInfo = splitter.at(2) + ":" + splitter.at(1);
	mUdpHash.insert(hostInfo, mUdpHandler);
	QString serverInfo = "UDP(" + hostInfo+ ")";

	emit serverCreatedEvent(serverInfo);
	emit udpSocketCreatedEvent("UDP", uniHostInfo);
}