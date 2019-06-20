#include "LogWidget.h"
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QScrollBar>
LogWidget::LogWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	//ui.teLogWindow->setReadOnly(true);
}

LogWidget::~LogWidget()
{
}

void LogWidget::generateLogSignal(const QString & msg)
{
	emit emitLogEvent(msg);
}

void LogWidget::setLogLevel(QString level)
{
	mLogLevel = level;
}

void LogWidget::setLogTime(QString time)
{
	mLogTime = time;
}

void LogWidget::on_pbClearLog_clicked()
{
	ui.tableWidget->setRowCount(0);
	//ui.teLogWindow->clear();
}

void LogWidget::logInternal(QString msg)
{
	QTableWidgetItem *logLevelItem = new QTableWidgetItem(mLogLevel);
	QTableWidgetItem *logMessage = new QTableWidgetItem(msg);
	QTableWidgetItem *logTime = new QTableWidgetItem(mLogTime);
	ui.tableWidget->insertRow(ui.tableWidget->rowCount());
	ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 0, logTime);
	ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 1, logLevelItem);
	ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, 2, logMessage);
	//ui.teLogWindow->append(msg);
	//ui.teLogWindow->verticalScrollBar()->setValue(ui.teLogWindow->verticalScrollBar()->maximum());
}

