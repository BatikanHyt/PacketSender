#include "LogWidget.h"
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QScrollBar>
LogWidget::LogWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
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

QIcon LogWidget::getLogIcon(QString logLevel)
{
	if (logLevel.contains("Debug"))
	{
		QIcon icon("icons/debug.png");
		return icon;
	}
	else if (logLevel.contains("Info"))
	{
		QIcon icon("icons/info.png");
		return icon;
	}
	else if(logLevel.contains("Warning"))
	{
		QIcon icon("icons/warning.png");
		return icon;
	}
	else
	{
		QIcon icon("icons/error.png");
		return icon;
	}
}

void LogWidget::on_pbClearLog_clicked()
{
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(0);
}

void LogWidget::logInternal(QString msg)
{
	QIcon logIcon = getLogIcon(mLogLevel);
	QTableWidgetItem *logLevelItem = new QTableWidgetItem(mLogLevel);
	logLevelItem->setIcon(logIcon);
	logLevelItem->setTextAlignment(Qt::AlignCenter);
	QTableWidgetItem *logMessage = new QTableWidgetItem(msg);
	QTableWidgetItem *logTime = new QTableWidgetItem(mLogTime);
	ui.tableWidget->insertRow(0);
	ui.tableWidget->setItem(0, 0, logTime);
	ui.tableWidget->setItem(0, 1, logLevelItem);
	ui.tableWidget->setItem(0, 2, logMessage);

	if (1 == ui.tableWidget->rowCount())
	{
		ui.tableWidget->resizeColumnsToContents();
		//ui.tableWidget->resizeRowsToContents();
		ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	}

}

