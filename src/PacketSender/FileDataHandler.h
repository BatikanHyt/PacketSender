#pragma once
#include "FileSenderHandler.h"
#include <QtCore/QString>
class FileDataHandler :
	public FileSenderHandler
{

public:
	FileDataHandler();
	~FileDataHandler();

	virtual void handle(QByteArray& data);

	QByteArray getParsedData();

	QString getFileName();

private:

	QByteArray mData;

	QString mFileName;

	int mCount;

	int mPacketSizeRecv;
};

