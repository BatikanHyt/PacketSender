#pragma once

#include <QtCore/QObject>

class FileSender
	: public QObject
{
public:

	FileSender();

	~FileSender();

	static QByteArray sendData(QString fileName,const QByteArray& data);

	static QByteArray sendEnd(const QByteArray& data);

	static QByteArray sendRawData(const QByteArray& data);

	void sendStart();

};

