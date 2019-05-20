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

signals:

	void emitLogEvent(QString mgg);
public slots:

	void logInternal(QString msg);

private:
	Ui::LogWidget ui;
};
