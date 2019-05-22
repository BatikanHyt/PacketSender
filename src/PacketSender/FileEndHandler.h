#pragma once
#include "FileSenderHandler.h"
class FileEndHandler :
	public FileSenderHandler
{
public:
	FileEndHandler();
	~FileEndHandler();

	virtual void handle(QByteArray& data);
};

