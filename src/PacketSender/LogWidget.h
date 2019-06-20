#pragma once

#include <QWidget>
#include "ui_LogWidget.h"

class LogWidget : public QWidget
{
	Q_OBJECT

public:
	LogWidget(QWidget *parent = Q_NULLPTR);

	~LogWidget();

	void generateLogSignal(const QString& msg);

	void setLogLevel(QString level);

	void setLogTime(QString time);

signals:

	void emitLogEvent(QString mgg);

public slots:

	void logInternal(QString msg);

	void on_pbClearLog_clicked();

private:
	Ui::LogWidget ui;

	QString mLogLevel;

	QString mLogTime;
};
