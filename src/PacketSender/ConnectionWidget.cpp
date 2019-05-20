#include "ConnectionWidget.h"
#include "TcpClientHandler.h"
#include "TcpServerHandler.h"

#include <QtCore/QDebug>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

ConnectionWidget::ConnectionWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	mTcpClientHandler = new TcpClientHandler();
}

ConnectionWidget::~ConnectionWidget()
{
}

void ConnectionWidget::on_pbConnect_clicked()
{
	int portNumber = ui.lePort->text().toInt();
	QString hostIp = ui.leIP->text();

	if (ui.rbTcp->isChecked())
	{
		if (ui.cbMode->currentText().contains("Client"))
		{
			createTcpConnection(hostIp,portNumber);
		}
		else
		{
			createTcpServer(hostIp, portNumber);
		}

	}
}

void ConnectionWidget::createTcpConnection(QString hostAddress, int portNumber)
{
	int result = mTcpClientHandler->createTcpClientConnection(hostAddress, portNumber);
	if (result)
	{
		QString info = hostAddress + ":" + QString::number(portNumber);
		emit clientConnectedEvent(info);
	}
}

void ConnectionWidget::createTcpServer(QString hostAddress, int portNumber)
{
	mTcpServerHandler = new TcpServerHandler();
	mTcpServerHandler->setHostAddres(hostAddress);
	mTcpServerHandler->setListenPort(portNumber);
	mTcpServerHandler->createTcpServer();
}

void ConnectionWidget::writeToTcp(QByteArray &data)
{
	mTcpClientHandler->writeIt(data);
}
