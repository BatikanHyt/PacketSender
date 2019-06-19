#pragma once

#include "PacketSenderHandler.h"

class FileEndHandler 
	: public PacketSenderHandler
{

public:

	FileEndHandler();

	~FileEndHandler();

	virtual void handle(QByteArray& data) override;

	virtual QString getFileName() override;

	virtual QByteArray getParsedData() override;

private:

	QString mFileName;
};

