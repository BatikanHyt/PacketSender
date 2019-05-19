#include "PacketSenderWidget.h"
#include <QtWidgets/QFileDialog>
#include <QtCore/QDir>
#include "TcpClientHandler.h"

PacketSenderWidget::PacketSenderWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

PacketSenderWidget::~PacketSenderWidget()
{
}

void PacketSenderWidget::updateFileWidget(QHash<QString,quint64> fileHash)
{
	QHash<QString, quint64>::iterator i;
	for (i = fileHash.begin(); i != fileHash.end(); ++i)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem();
		QString fileName = i.key();
		quint64 size = i.value() % 1024;
		item->setText(0, fileName);
		item->setText(1, QString::number(size) + " KB");
		item->setCheckState(0, Qt::Unchecked);

		ui.twFileList->addTopLevelItem(item);
	}
}

void PacketSenderWidget::onConnectionEstablished(QString info)
{
	ui.cbConnectionList->addItem(info);
}

void PacketSenderWidget::onClientDisconnected(QString info)
{
	int index = ui.cbConnectionList->findText(info);
	ui.cbConnectionList->removeItem(index);
}

void PacketSenderWidget::on_pbLoadFile_clicked()
{
	QString directoryPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"/home",
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);

	QDir dir(directoryPath);
	QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::NoDot | QDir::NoDotDot);
	QHash<QString, quint64> fileHash;
	foreach(QFileInfo fileInfo, fileInfoList)
	{
		QString fileName = fileInfo.completeBaseName();
		quint64 size = fileInfo.size();
		fileHash.insert(fileName, size);
	}
	updateFileWidget(fileHash);
}

void PacketSenderWidget::on_pbStartTransfer_clicked()
{
	emit writeTcpSocket();
}
