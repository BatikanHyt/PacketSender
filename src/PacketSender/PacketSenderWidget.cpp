#include "PacketSenderWidget.h"
#include <QtWidgets/QFileDialog>
#include <QtCore/QDir>
#include "TcpClientHandler.h"
#include <QtCore/QByteArray>
#include <QtCore/QDebug>

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
		quint64 size = (i.value() / 1024) + 1;
		item->setText(0, fileName);
		item->setText(1, QString::number(size) + " KB");
		item->setCheckState(0, Qt::Unchecked);

		ui.twFileList->addTopLevelItem(item);
	}
	ui.twFileList->resizeColumnToContents(0);
	ui.twFileList->resizeColumnToContents(1);
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
		QDir::homePath(),
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);

	QDir dir(directoryPath);
	QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::NoDot | QDir::NoDotDot);
	QHash<QString, quint64> fileHash;
	foreach(QFileInfo fileInfo, fileInfoList)
	{
		QString fileName = fileInfo.baseName()+"."+fileInfo.completeSuffix();
		quint64 size = fileInfo.size();
		fileHash.insert(fileName, size);
	}
	ui.twFileList->clear();
	updateFileWidget(fileHash);
}

void PacketSenderWidget::on_pbStartTransfer_clicked()
{
	bool itemSelected = false;
	QTreeWidgetItemIterator item(ui.twFileList);
	if (ui.leData->text().isEmpty())
	{
		itemSelected = true;
	}

	while (*item)
	{
		if ((*item)->checkState(0) == Qt::Checked)
		{
			QString fileName = (*item)->text(0);
			qDebug() << "Selected file names : " << fileName;
			itemSelected = true;
			
		}
		++item;
	}
	if (!itemSelected)
	{
		QByteArray data = ui.leData->text().toUtf8();
		emit writeTcpSocket(data);
	}
}
