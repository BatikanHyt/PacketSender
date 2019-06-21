#pragma once

#include <QWidget>
#include "ui_TrafficLoggerWidget.h"

#include <QtCore/QThread>

class TrafficLoggerWidget : public QWidget
{
	Q_OBJECT

public:

	~TrafficLoggerWidget();

	static TrafficLoggerWidget* getLoggerWidget();

	void setToIp(QString ip);

	void setFromIp(QString ip);

	void setTime(QString time);

	void setToPort(int port);

	void setFromPort(int port);

	void setDirection(QString direction);

	void setMethod(QString method);

	void updateTrafficLogger();

public slots:

	void updateTrafficLoggerInternal();
private:
	
	TrafficLoggerWidget(QWidget *parent = Q_NULLPTR);

private:

	Ui::TrafficLoggerWidget ui;

	static TrafficLoggerWidget* mTraficLoggerWidget;

	QString mToIp;

	QString mFromIp;

	QString mMethod;

	QString mDirection;

	QString mTime;

	int mFromPort;

	int mToPort;
};
