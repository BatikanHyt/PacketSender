#include "PacketSender.h"
#include "QStyleFactory"
#include <QDebug>
#include <QtCore/QFile>

LogWidget* PacketSender::mLoggerWidget;

PacketSender::PacketSender(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.statusBar->setStyleSheet("background-color: rgb(76, 76, 76);");
	mPackWidget = new PacketSenderWidget();
	mConWidget = new ConnectionWidget();
	mTrafficLogger = TrafficLoggerWidget::getLoggerWidget();
	if (mLoggerWidget == 0)
	{
		mLoggerWidget = new LogWidget();
	}


	connect(mConWidget,
		&ConnectionWidget::clientConnectedEvent,
		mPackWidget,
		&PacketSenderWidget::onConnectionEstablished);

	connect(mConWidget,
		&ConnectionWidget::clientDisconnectedEvent,
		mPackWidget,
		&PacketSenderWidget::onClientDisconnected);

	connect(mPackWidget,
		&PacketSenderWidget::writeTcpSocket,
		mConWidget,
		&ConnectionWidget::onWriteToTcpClient);

	connect(mPackWidget,
		&PacketSenderWidget::writeUdpSocket,
		mConWidget,
		&ConnectionWidget::onWriteToUdpSocket);

	connect(mConWidget,
		&ConnectionWidget::udpSocketCreatedEvent,
		mPackWidget,
		&PacketSenderWidget::onUdpSocketCreatedEvent);

	connect(mConWidget,
		&ConnectionWidget::serverCreatedEvent,
		this,
		&PacketSender::newServerCreated);

	connect(this,
		&PacketSender::shutdownServer,
		mConWidget,
		&ConnectionWidget::shutdownServerEvent);

	/*connect(mPackWidget,
		&PacketSenderWidget::writeTcpSocket,
		mConWidget,
		&ConnectionWidget::onWriteToTcpServer);*/

	connect(mLoggerWidget,
		&LogWidget::emitLogEvent,
		mLoggerWidget,
		&LogWidget::logInternal);

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(mPackWidget,0,0);
	layout->addWidget(mConWidget,0,1,Qt::AlignRight);
	layout->addWidget(mTrafficLogger, 1, 0, 1, 0);
	layout->addWidget(mLoggerWidget,2,0,2,0);

	ui.centralWidget->setLayout(layout);
	ui.centralWidget->setContentsMargins(0,0,0,0);
}

LogWidget* PacketSender::getLogWidget()
{
	return mLoggerWidget;
}

void PacketSender::serverCloseEvent()
{
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	QString info = button->text();
	button->deleteLater();
	emit shutdownServer(info);
	
}

void PacketSender::on_actionFusion_triggered()
{
	QApplication::setStyle(QStyleFactory::create("Fusion"));
}

void PacketSender::on_actionClassic_triggered()
{
	QApplication::setStyle("WindowsVista");
}

void PacketSender::on_actionVintage_triggered()
{
	QApplication::setStyle("windows");
}

void PacketSender::on_actionWhite_triggered()
{
	setStyleSheet("");
}

void PacketSender::on_actionDark_triggered()
{
	QFile file("StyleSheet/darkorange.stylesheet");
	file.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(file.readAll());
	setStyleSheet(styleSheet);
}

void PacketSender::newServerCreated(QString info)
{
	mServerList.insert(info);
	QPushButton* button = new QPushButton(info);
	button->setStyleSheet("background-color: rgb(255, 255, 255);");
	connect(button, 
		SIGNAL(clicked()), 
		this, 
		SLOT(serverCloseEvent()));
	ui.statusBar->addWidget(button);
}
