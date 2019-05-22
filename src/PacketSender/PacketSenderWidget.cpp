#include "PacketSenderWidget.h"
#include <QtWidgets/QFileDialog>
#include <QtCore/QDir>
#include "TcpClientHandler.h"
#include "FileSender.h"
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
	mFilePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		QDir::homePath(),
		QFileDialog::ShowDirsOnly
		| QFileDialog::DontResolveSymlinks);

	QDir dir(mFilePath);
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
			initializeFileData(fileName);
			mDelay = ui.sbMessageDelay->value();
			mDataPartSize = ui.cbMessageSize->currentText().toInt();
			int id = startTimer(mDelay);

			mFileHash.insert(id, fileName);		
			mFileProcess.insert(id, 0);
			mTransferFinished.insert(id, false);
		}
		++item;
	}
	if (!itemSelected)
	{
		QByteArray data = ui.leData->text().toUtf8();
		emit writeTcpSocket(data);
	}
}

void PacketSenderWidget::timerEvent(QTimerEvent * event)
{
	int id = event->timerId();
	FileSender fileSender;
	QString fileName = mFileHash[id];
	QByteArray fileData = mFileDataHash[fileName];

	//QByteArray data = fileSender.sendData(fileName, fileData);
	int totalSize = fileData.size();
	int bytesWritten = mFileProcess[id];
	if (mTransferFinished[id])
	{
		killTimer(id);
		mFileDataHash.remove(fileName);
		mFileHash.remove(id);
		mTransferFinished.remove(id);
	}
	else
	{
		if (bytesWritten < totalSize)
		{
			QByteArray arr = fileData.mid(bytesWritten, mDataPartSize);
			bytesWritten += arr.size();
			QByteArray splitedFileData = fileSender.sendData(fileName, arr);
			emit writeTcpSocket(splitedFileData);
			mFileProcess[id] = bytesWritten;
		}
		else
		{
			mTransferFinished[id] = true;
		}
	}
}


void PacketSenderWidget::initializeFileData(QString fileName)
{
	QString absolutePath = mFilePath + "/" + fileName;
	QFile fileToSend(absolutePath);
	
	QByteArray fileData;
	if (!fileToSend.open(QIODevice::ReadOnly))
	{
		return;
	}
	fileData = fileToSend.readAll();
	mFileDataHash.insert(fileName, fileData);
	fileToSend.close();
}