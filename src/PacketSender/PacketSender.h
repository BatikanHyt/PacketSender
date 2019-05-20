#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PacketSender.h"

#include "PacketSenderWidget.h"
#include "ConnectionWidget.h"
#include "LogWidget.h"
#include <QtWidgets/QTextEdit>

class QTreeWidget;
class PacketSender : public QMainWindow
{
	Q_OBJECT

public:

	PacketSender(QWidget *parent = Q_NULLPTR);

	static LogWidget* getLogWidget();

private:

	Ui::PacketSenderClass ui;

	PacketSenderWidget* mPackWidget;

	ConnectionWidget* mConWidget;

	static LogWidget* mLoggerWidget;

};