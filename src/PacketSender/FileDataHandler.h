#pragma once
#include "PacketSenderHandler.h"
#include <QtCore/QString>
class FileDataHandler :
	public PacketSenderHandler
{

public:
	FileDataHandler();
	~FileDataHandler();

	virtual void handle(QByteArray& data) override; 

	virtual QByteArray getParsedData() override;

	virtual QString getFileName() override;

private:

	QByteArray mData;

	QString mFileName;

	int mCount;

	int mPacketSizeRecv;
};

