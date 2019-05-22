#pragma once

#include <QtCore/QByteArray>

class FileSenderHandler
{
public:
	FileSenderHandler();
	~FileSenderHandler();

	virtual void handle(QByteArray& data) = 0;
};

