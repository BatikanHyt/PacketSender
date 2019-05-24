#pragma once

#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QSharedPointer>

class FileSenderHandler
{
public:

	FileSenderHandler();

	~FileSenderHandler();

	virtual void handle(QByteArray& data) = 0;

	virtual QString getFileName() = 0;

	virtual QByteArray getParsedData() = 0;
};

