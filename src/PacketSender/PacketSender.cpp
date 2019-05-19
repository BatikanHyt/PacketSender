#include "PacketSender.h"

#include <QDebug>

QTextEdit* PacketSender::mLogWidget;

PacketSender::PacketSender(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	mPackWidget = new PacketSenderWidget();
	mConWidget = new ConnectionWidget();
	if (mLogWidget == 0)
	{
		mLogWidget = new QTextEdit();
	}
	mLogWidget->setReadOnly(true);

	connect(mConWidget,
		&ConnectionWidget::clientConnectedEvent,
		mPackWidget,
		&PacketSenderWidget::onConnectionEstablished);

	connect(mPackWidget,
		&PacketSenderWidget::writeTcpSocket,
		mConWidget,
		&ConnectionWidget::writeToTcp);

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(mPackWidget,0,0);
	layout->addWidget(mConWidget,0,1);
	layout->addWidget(mLogWidget,1,0,1,0);

	ui.centralWidget->setLayout(layout);
}


QTextEdit* PacketSender::getLogWidget()
{
	return mLogWidget;
}