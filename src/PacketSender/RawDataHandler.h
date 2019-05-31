#pragma once
#include "PacketSenderHandler.h"
class RawDataHandler 
	: public PacketSenderHandler
{
public:

	RawDataHandler();

	~RawDataHandler();

	virtual void handle(QByteArray &data);
	
	QByteArray getParsedData();

	QString getFileName();

private:

	QByteArray mData;
};

