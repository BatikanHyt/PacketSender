#include "ClientListWidget.h"
#include "TcpWriterDialog.h"
ClientListWidget* ClientListWidget::mClientListWidget;

ClientListWidget::ClientListWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//qRegisterMetaType<QByteArray>("QByteArray");

	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	mContext = new QMenu(ui.tableWidget);
	QAction* writeAction = new QAction("Write", 0);
	QAction* disconnectAction = new QAction("Disconnect", 0);
	mContext->addAction(writeAction);
	mContext->addAction(disconnectAction);
	connect(ui.tableWidget,
		SIGNAL(customContextMenuRequested(QPoint)),
			SLOT(menuRequested(QPoint)));
	connect(writeAction,
		&QAction::triggered,
		this,
		&ClientListWidget::createTcpWriterDialog);
	connect(disconnectAction,
		&QAction::triggered,
		this,
		&ClientListWidget::disconnectClientEvent);

}

ClientListWidget::~ClientListWidget()
{
}

void ClientListWidget::updateClientList(ClientInfo info)
{
	QMetaObject::invokeMethod(this,
		"updateClientListInternal",
		Q_ARG(ClientInfo, info));
}

void ClientListWidget::updateClientListInternal(ClientInfo info)
{
	QTableWidgetItem* hostIpItem = new QTableWidgetItem(info.hostIp);
	QTableWidgetItem* hostPortItem = new QTableWidgetItem(QString::number(info.hostPort));
	QTableWidgetItem* clientIpItem = new QTableWidgetItem(info.clientIp);
	QTableWidgetItem* clientPortItem = new QTableWidgetItem(QString::number(info.clientPort));

	hostIpItem->setData(Qt::UserRole, info.socketId);
	hostPortItem->setData(Qt::UserRole, info.socketId);
	clientIpItem->setData(Qt::UserRole, info.socketId);
	clientPortItem->setData(Qt::UserRole, info.socketId);

	ui.tableWidget->insertRow(0);
	ui.tableWidget->setItem(0, 0, hostIpItem);
	ui.tableWidget->setItem(0, 1, hostPortItem);
	ui.tableWidget->setItem(0, 2, clientIpItem);
	ui.tableWidget->setItem(0, 3, clientPortItem);

	if (1 == ui.tableWidget->rowCount())
	{
		ui.tableWidget->resizeColumnsToContents();
		ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	}

}

void ClientListWidget::menuRequested(QPoint point)
{
	QModelIndex index = ui.tableWidget->indexAt(point);
	mContext->popup(ui.tableWidget->viewport()->mapToGlobal(point));
	mSelectedItem = ui.tableWidget->itemAt(point);
}

void ClientListWidget::createTcpWriterDialog()
{
	if (mSelectedItem != 0)
	{
		QVariant val = mSelectedItem->data(Qt::UserRole);
		if (val.isValid())
		{
			TcpWriterDialog dialog;
			connect(&dialog,
				&TcpWriterDialog::messageFinished,
				this,
				&ClientListWidget::messageEvent);
			dialog.exec();
		}
	}
}

void ClientListWidget::disconnectClientEvent()
{
	if (mSelectedItem != 0)
	{
		QVariant val = mSelectedItem->data(Qt::UserRole);
		if (val.isValid())
		{
			int socketId = val.toInt();
			int row = ui.tableWidget->row(mSelectedItem);
			ui.tableWidget->removeRow(row);
			emit disconnectClient(socketId);
		}
	}
}

void ClientListWidget::messageEvent(QByteArray data)
{
	QVariant val = mSelectedItem->data(Qt::UserRole);
	if (val.isValid())
	{
		int socketId = val.toInt();
		emit messageFinishedEvent(data, socketId);
	}
}

ClientListWidget* ClientListWidget::ClientListWidget::getClientListWidget()
{
	if (mClientListWidget == 0)
	{
		mClientListWidget = new ClientListWidget();
	}
	return mClientListWidget;
}

