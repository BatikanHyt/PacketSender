#include "TrafficLoggerWidget.h"

TrafficLoggerWidget* TrafficLoggerWidget::mTraficLoggerWidget;

TrafficLoggerWidget::TrafficLoggerWidget(QWidget *parent)
	: QWidget(parent)
	, mMaxItemCount(1000)
{
	ui.setupUi(this);
	ui.tableWidget->resizeColumnsToContents();
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
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

void TrafficLoggerWidget::updateTrafficLogger(TrafficLogsItems item)
{
	QMetaObject::invokeMethod(this,
		"updateTrafficLoggerInternal",
		Q_ARG(TrafficLogsItems,item));
}

void TrafficLoggerWidget::updateTrafficLoggerInternal(TrafficLogsItems logItem)
{
	if (mMaxItemCount <= ui.tableWidget->rowCount())
	{
		ui.tableWidget->removeRow(ui.tableWidget->rowCount() - 1);
	}
	if(ui.cbLogTraffic->isChecked())
	{
		QTableWidgetItem* timeItem = new QTableWidgetItem(logItem.time);
		QTableWidgetItem* directionItem = new QTableWidgetItem(logItem.direction);
		QTableWidgetItem* methodItem = new QTableWidgetItem(logItem.method);
		QTableWidgetItem* fromIpItem = new QTableWidgetItem(logItem.fromIp);
		QTableWidgetItem* fromPortItem = new QTableWidgetItem(QString::number(logItem.fromPort));
		QTableWidgetItem* toIpItem = new QTableWidgetItem(logItem.toIp);
		QTableWidgetItem* toPortItem = new QTableWidgetItem(QString::number(logItem.toPort));

		ui.tableWidget->insertRow(0);
		ui.tableWidget->setItem(0, 0, timeItem);
		ui.tableWidget->setItem(0, 1, directionItem);
		ui.tableWidget->setItem(0, 2, methodItem);
		ui.tableWidget->setItem(0, 3, fromIpItem);
		ui.tableWidget->setItem(0, 4, fromPortItem);
		ui.tableWidget->setItem(0, 5, toIpItem);
		ui.tableWidget->setItem(0, 6, toPortItem);
	}
	if (1 == ui.tableWidget->rowCount())
	{
		ui.tableWidget->resizeColumnsToContents();
		//ui.tableWidget->resizeRowsToContents();
		ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	}

}

void TrafficLoggerWidget::on_pbClearLog_clicked()
{
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(0);
}
