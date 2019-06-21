#include "TrafficLoggerWidget.h"

TrafficLoggerWidget* TrafficLoggerWidget::mTraficLoggerWidget;

TrafficLoggerWidget::TrafficLoggerWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.tableWidget->resizeColumnsToContents();
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);

}

TrafficLoggerWidget::~TrafficLoggerWidget()
{
}

TrafficLoggerWidget* TrafficLoggerWidget::TrafficLoggerWidget::getLoggerWidget()
{
	if (mTraficLoggerWidget == 0)
	{
		mTraficLoggerWidget = new TrafficLoggerWidget();
	}
	return mTraficLoggerWidget;
}

void TrafficLoggerWidget::setToIp(QString ip)
{
	mToIp = ip;
}

void TrafficLoggerWidget::setFromIp(QString ip)
{
	mFromIp = ip;
}

void TrafficLoggerWidget::setTime(QString time)
{
	mTime = time;
}

void TrafficLoggerWidget::setToPort(int port)
{
	mToPort = port;
}

void TrafficLoggerWidget::setFromPort(int port)
{
	mFromPort = port;
}

void TrafficLoggerWidget::setDirection(QString direction)
{
	mDirection = direction;
}

void TrafficLoggerWidget::setMethod(QString method)
{
	mMethod = method;
}

void TrafficLoggerWidget::updateTrafficLogger()
{
	QMetaObject::invokeMethod(this,
		"updateTrafficLoggerInternal");
}

void TrafficLoggerWidget::updateTrafficLoggerInternal()
{
	setUpdatesEnabled(false);
	QTableWidgetItem* timeItem = new QTableWidgetItem(mTime);
	QTableWidgetItem* directionItem = new QTableWidgetItem(mDirection);
	QTableWidgetItem* methodItem = new QTableWidgetItem(mMethod);
	QTableWidgetItem* fromIpItem = new QTableWidgetItem(mFromIp);
	QTableWidgetItem* fromPortItem = new QTableWidgetItem(QString::number(mFromPort));
	QTableWidgetItem* toIpItem = new QTableWidgetItem(mToIp);
	QTableWidgetItem* toPortItem = new QTableWidgetItem(QString::number(mToPort));

	ui.tableWidget->insertRow(0);
	ui.tableWidget->setItem(0, 0, timeItem);
	ui.tableWidget->setItem(0, 1, directionItem);
	ui.tableWidget->setItem(0, 2, methodItem);
	ui.tableWidget->setItem(0, 3, fromIpItem);
	ui.tableWidget->setItem(0, 4, fromPortItem);
	ui.tableWidget->setItem(0, 5, toIpItem);
	ui.tableWidget->setItem(0, 6, toPortItem);

	ui.tableWidget->setVisible(false);
	ui.tableWidget->resizeColumnsToContents();
	ui.tableWidget->resizeRowsToContents();
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget->setVisible(true);
	setUpdatesEnabled(true);
}
