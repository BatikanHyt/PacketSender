#pragma once
#include "FileSenderHandler.h"
class FileDataHandler :
	public FileSenderHandler
{
public:
	FileDataHandler();
	~FileDataHandler();

	virtual void handle(QByteArray& data);
};

