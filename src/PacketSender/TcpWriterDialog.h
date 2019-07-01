#pragma once

#include <QDialog>
#include "ui_TcpWriterDialog.h"

class TcpWriterDialog : public QDialog
{
	Q_OBJECT

public:

	TcpWriterDialog(QWidget *parent = Q_NULLPTR);

	~TcpWriterDialog();

public slots:

	void on_pbOk_clicked();

	void on_pbClose_clicked();

signals:

	void messageFinished(QByteArray data);

private:
	Ui::TcpWriterDialog ui;
};
