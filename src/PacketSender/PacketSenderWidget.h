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

	void initializeFileData(QString fileName);

signals:

	void writeTcpSocket(QByteArray &data,QString info);

	void writeUdpSocket(QByteArray &data,QString info);

public slots:

	void onConnectionEstablished(QString protocolName, QString information);

	void onClientDisconnected(QString protocolName, QString information);

	void on_pbLoadFile_clicked();

	void on_pbStartTransfer_clicked();

	void on_rbUseTcp_clicked();

	void on_rbUseUdp_clicked();

	void onUdpSocketCreatedEvent(QString protocolName, QString informarion);

protected:

	void timerEvent(QTimerEvent *event);

	void initializeConnectionList();

private:

	Ui::PacketSenderWidget ui;

	QString mFilePath;

	QHash<QString, QByteArray> mFileDataHash;

	QHash<int, QString> mFileHash;

	QHash<int, bool> mTransferFinished;

	QHash<int, int> mFileProcess;

	QHash<int, QString> mSendInfoHash;

	int mDataPartSize;

	int mDelay;

	bool mUseTcp;

	QHash<QString, QString> mConnectionList;
};
