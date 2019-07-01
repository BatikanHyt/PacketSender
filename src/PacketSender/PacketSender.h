#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PacketSender.h"

#include "PacketSenderWidget.h"
#include "ConnectionWidget.h"
#include "TrafficLoggerWidget.h"
#include "LogWidget.h"
#include "ClientListWidget.h"
#include <QtWidgets/QTextEdit>
#include "TcpWriterDialog.h"

class QTreeWidget;
class PacketSender : public QMainWindow
{
	Q_OBJECT

public:

	PacketSender(QWidget *parent = Q_NULLPTR);

	static LogWidget* getLogWidget();

public slots:

	void newServerCreated(QString info);

	void serverCloseEvent();

	void on_actionFusion_triggered();

	void on_actionClassic_triggered();

	void on_actionVintage_triggered();

	void on_actionWhite_triggered();

	void on_actionDark_triggered();

signals:

	void shutdownServer(QString info);

private:

	Ui::PacketSenderClass ui;

	PacketSenderWidget* mPackWidget;

	ConnectionWidget* mConWidget;

	static LogWidget* mLoggerWidget;

	ClientListWidget* mClientListWidget;

	QSet<QString> mServerList;

	TrafficLoggerWidget* mTrafficLogger;
};
