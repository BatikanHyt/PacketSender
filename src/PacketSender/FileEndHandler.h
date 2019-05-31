#pragma once

#include "PacketSenderHandler.h"

class FileEndHandler 
	: public PacketSenderHandler
{

public:

	FileEndHandler();

	~FileEndHandler();

	virtual void handle(QByteArray& data);

	virtual QString getFileName();

	virtual QByteArray getParsedData();

private:

	QString mFileName;
};

