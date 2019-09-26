#pragma once

#include <QWidget>
#include "ui_TrafficLoggerWidget.h"

#include <QtCore/QThread>

struct TrafficLogsItems
{
	QString toIp;
	QString fromIp;
	int toPort;
	int fromPort;
	QString time;
	QString method;
	QString direction;
	QByteArray data;
};
Q_DECLARE_METATYPE(TrafficLogsItems);

class TrafficLoggerWidget : public QWidget
{
	Q_OBJECT

public:

	~TrafficLoggerWidget();

	static TrafficLoggerWidget* getLoggerWidget();

	void updateTrafficLogger(TrafficLogsItems items);

public slots:

	void updateTrafficLoggerInternal(TrafficLogsItems items);

	void on_pbClearLog_clicked();

	void onCellDoubleClicked(int row, int col);

private:
	
	TrafficLoggerWidget(QWidget *parent = Q_NULLPTR);

private:

	Ui::TrafficLoggerWidget ui;

	static TrafficLoggerWidget* mTraficLoggerWidget;

	int mMaxItemCount;
};
