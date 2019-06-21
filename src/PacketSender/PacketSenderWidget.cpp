#include "PacketSenderWidget.h"
#include <QtWidgets/QFileDialog>
#include <QtCore/QDir>
#include "TcpClientHandler.h"
#include "FileSender.h"
#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QHashIterator>

PacketSenderWidget::PacketSenderWidget(QWidget *parent)
	: QWidget(parent)
	, mUseTcp(false)
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

void PacketSenderWidget::onConnectionEstablished(QString protocol ,QString info)
{
	mConnectionList.insert(info, protocol);
	initializeConnectionList();
}

void PacketSenderWidget::onClientDisconnected(QString protocol, QString info)
{
	mConnectionList.remove(info);
	initializeConnectionList();
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
	QString selectedConnectionInfo = ui.cbConnectionList->currentText();
	if (ui.rbUseTcp->isChecked())
	{
		mUseTcp = true;
	}
	else if (ui.rbUseUdp->isChecked())
	{
		mUseTcp = false;
	}
	
	bool itemSelected = false;
	QTreeWidgetItemIterator item(ui.twFileList);
	if (ui.leData->text().isEmpty())
	{
		itemSelected = true;
	}

	if(!selectedConnectionInfo.isEmpty())
	{
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
				mDataPartSize -= fileName.size();
				int id = startTimer(mDelay);

				mSendInfoHash.insert(id, selectedConnectionInfo);
				mFileHash.insert(id, fileName);
				mFileProcess.insert(id, 0);
				mTransferFinished.insert(id, false);
			}
			++item;
		}
		if (!itemSelected)
		{
			QByteArray data = ui.leData->text().toUtf8();
			//QByteArray rawData = FileSender::sendRawData(data);
			if (mUseTcp)
			{
				emit writeTcpSocket(data, selectedConnectionInfo);
			}
			else
			{
				emit writeUdpSocket(data, selectedConnectionInfo);
			}
		}
	}
}

void PacketSenderWidget::on_rbUseTcp_clicked()
{
	initializeConnectionList();
}

void PacketSenderWidget::on_rbUseUdp_clicked()
{
	initializeConnectionList();
}

void PacketSenderWidget::onUdpSocketCreatedEvent(QString protocolName, QString information)
{
	mConnectionList.insert(information, protocolName);
	initializeConnectionList();
}

void PacketSenderWidget::timerEvent(QTimerEvent * event)
{
	int id = event->timerId();
	FileSender fileSender;
	QString fileName = mFileHash[id];
	QByteArray fileData = mFileDataHash[fileName];
	QString info = mSendInfoHash[id];

	int totalSize = fileData.size();
	int bytesWritten = mFileProcess[id];
	if (mTransferFinished[id])
	{
		killTimer(id);
		mFileDataHash.remove(fileName);
		mFileHash.remove(id);
		mTransferFinished.remove(id);
		QByteArray endMessage = fileSender.sendEnd(fileName.toUtf8());
		if (mUseTcp)
		{
			emit writeTcpSocket(endMessage, info);
		}
		else
		{
			emit writeUdpSocket(endMessage, info);
		}
	}
	else
	{
		if (bytesWritten < totalSize)
		{
			QByteArray arr = fileData.mid(bytesWritten, mDataPartSize);
			bytesWritten += arr.size();
			QByteArray splitedFileData = fileSender.sendData(fileName, arr);
			if (mUseTcp)
			{
				emit writeTcpSocket(splitedFileData, info);
			}
			else
			{
				emit writeUdpSocket(splitedFileData, info);
			}
			mFileProcess[id] = bytesWritten;
		}
		else
		{
			qDebug() << "Total Byte written :" << QString::number(bytesWritten);
			mTransferFinished[id] = true;
		}
	}
}

void PacketSenderWidget::initializeConnectionList()
{
	ui.cbConnectionList->clear();
	QHash<QString, QString>::iterator i;
	for (i = mConnectionList.begin(); i != mConnectionList.end(); ++i)
	{
		if (ui.rbUseTcp->isChecked())
		{
			if (i.value().contains("TCP"))
			{
				ui.cbConnectionList->addItem(i.key());
			}
		}
		else if (ui.rbUseUdp->isChecked())
		{
			if (ui.rbUseUdp->isChecked())
			{
				if (i.value().contains("UDP"))
				{
					ui.cbConnectionList->addItem(i.key());
				}
			}
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
