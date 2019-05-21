#pragma once

#include <QtCore/QObject>

class FileSender
	: public QObject
{
public:

	FileSender();

	~FileSender();

	void sendData(const QByteArray& data);

	void sendEnd(const QByteArray& data);

	void sendStart();

};

