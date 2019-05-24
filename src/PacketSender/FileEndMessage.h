
#pragma once
#include "FileSenderMessages.h"

/*
	End Message Content Definition
	< FileName(Binary) >
*/

class FileEndMessage 
	: public FileSenderMessages
{

public:

	FileEndMessage();

	FileEndMessage(const QByteArray& data);

	~FileEndMessage();

	void setData(const QByteArray &data);
	const QByteArray& getData() const;

protected:

	virtual QByteArray generateContent();

	virtual void parseMessage(QDataStream& dataStream);

private:

	QByteArray mData;
};