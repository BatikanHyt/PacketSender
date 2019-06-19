#pragma once
#include "PacketSenderHandler.h"
class RawDataHandler 
	: public PacketSenderHandler
{
public:

	RawDataHandler();

	~RawDataHandler();

	virtual void handle(QByteArray &data) override;
	
	virtual QByteArray getParsedData() override;

	virtual QString getFileName() override;

private:

	QByteArray mData;
};

