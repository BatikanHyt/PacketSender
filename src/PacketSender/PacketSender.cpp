#include "PacketSender.h"

#include <QDebug>

LogWidget* PacketSender::mLoggerWidget;

PacketSender::PacketSender(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	mPackWidget = new PacketSenderWidget();
	mConWidget = new ConnectionWidget();
	if (mLoggerWidget == 0)
	{
		mLoggerWidget = new LogWidget();
	}

	connect(mConWidget,
		&ConnectionWidget::clientConnectedEvent,
		mPackWidget,
		&PacketSenderWidget::onConnectionEstablished);

	connect(mPackWidget,
		&PacketSenderWidget::writeTcpSocket,
		mConWidget,
		&ConnectionWidget::onWriteToTcpClient);


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
	layout->addWidget(mConWidget,0,1);
	layout->addWidget(mLoggerWidget,1,0,1,0);

	ui.centralWidget->setLayout(layout);
}

LogWidget* PacketSender::getLogWidget()
{
	return mLoggerWidget;
}
