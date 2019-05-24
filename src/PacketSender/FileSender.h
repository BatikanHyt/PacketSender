#pragma once

#include <QtCore/QObject>

class FileSender
	: public QObject
{
public:

	FileSender();

	~FileSender();

	QByteArray sendData(QString fileName,const QByteArray& data);

	QByteArray sendEnd(const QByteArray& data);

	void sendStart();

};

