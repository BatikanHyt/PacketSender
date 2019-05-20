#pragma once

#include <QWidget>
#include "ui_PacketSenderWidget.h"

class PacketSenderWidget : public QWidget
{
	Q_OBJECT

public:

	PacketSenderWidget(QWidget *parent = Q_NULLPTR);

	~PacketSenderWidget();

	void updateFileWidget(QHash<QString, quint64> fileHash);

signals:

	void writeTcpSocket(QByteArray &data);

public slots:

	void onConnectionEstablished(QString information);

	void onClientDisconnected(QString information);

	void on_pbLoadFile_clicked();

	void on_pbStartTransfer_clicked();

private:

	Ui::PacketSenderWidget ui;
};
