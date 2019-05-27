#include "LogWidget.h"
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QScrollBar>
LogWidget::LogWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.teLogWindow->setReadOnly(true);
}

LogWidget::~LogWidget()
{
}

void LogWidget::generateLogSignal(const QString & msg)
{
	emit emitLogEvent(msg);
}

void LogWidget::on_pbClearLog_clicked()
{
	ui.teLogWindow->clear();
}

void LogWidget::logInternal(QString msg)
{
	ui.teLogWindow->append(msg);
	ui.teLogWindow->verticalScrollBar()->setValue(ui.teLogWindow->verticalScrollBar()->maximum());
}
