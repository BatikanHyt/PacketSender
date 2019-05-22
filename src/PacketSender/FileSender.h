#pragma once

#include <QtCore/QObject>

class FileSender
	: public QObject
{
public:

	FileSender();

	~FileSender();

	QByteArray sendData(QString fileName,const QByteArray& data);

	void sendEnd(const QByteArray& data);

	void sendStart();

};

