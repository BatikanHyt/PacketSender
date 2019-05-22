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

	void writeTcpSocket(QByteArray &data);

public slots:

	void onConnectionEstablished(QString information);

	void onClientDisconnected(QString information);

	void on_pbLoadFile_clicked();

	void on_pbStartTransfer_clicked();

protected:

	void timerEvent(QTimerEvent *event);
private:

	Ui::PacketSenderWidget ui;

	QString mFilePath;

	QHash<QString, QByteArray> mFileDataHash;

	QHash<int, QString> mFileHash;

	QHash<int, bool> mTransferFinished;

	QHash<int, int> mFileProcess;

	int mDataPartSize;

	int mDelay;
};
