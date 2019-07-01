#pragma once

#include <QWidget>
#include "ui_ClientListWidget.h"
#include <QtWidgets/QMenu>

struct ClientInfo
{
	QString hostIp;
	int hostPort;
	QString clientIp;
	int clientPort;
	int socketId;
};

Q_DECLARE_METATYPE(ClientInfo);

class ClientListWidget : public QWidget
{
	Q_OBJECT

public:

	~ClientListWidget();

	static ClientListWidget* getClientListWidget();

	void updateClientList(ClientInfo info);

public slots:

	void updateClientListInternal(ClientInfo info);

	void menuRequested(QPoint point);

	void createTcpWriterDialog();

	void disconnectClientEvent();

	void messageEvent(QByteArray data);
signals:

	void disconnectClient(int socketId);

	void messageFinishedEvent(QByteArray data, int socketId);

private:
	
	ClientListWidget(QWidget *parent = Q_NULLPTR);

	Ui::ClientListWidget ui;

	static ClientListWidget* mClientListWidget;

	QMenu* mContext;

	QTableWidgetItem* mSelectedItem;
};
