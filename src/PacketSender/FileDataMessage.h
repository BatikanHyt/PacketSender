#pragma once

#include "FileSenderMessages.h"
#include <QtCore/QDataStream>

/*
	Data Message Content Definition
	< FileNameSize(1Byte) | FileName(Binary) | FileData(Binary) >
*/

class FileDataMessage
	: public FileSenderMessages
{
public:

	FileDataMessage();

	FileDataMessage(const QByteArray& data);
	
	~FileDataMessage();

	void setData(const QByteArray &data);
	const QByteArray& getData() const;

	void setFileName(QString fileName);
	const QString getFileName() const;

	const quint8 getFileNameSize() const;

protected:

	virtual QByteArray generateContent();

	virtual void parseMessage(QDataStream& dataStream);

private:

	QByteArray mData;

	quint8 mFileNameSize;

	QString mFileName;
};