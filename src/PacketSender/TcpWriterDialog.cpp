#include "TcpWriterDialog.h"
TcpWriterDialog::TcpWriterDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.widget->setOverwriteMode(false);
}

TcpWriterDialog::~TcpWriterDialog()
{
}

void TcpWriterDialog::on_pbOk_clicked()
{
	QByteArray data = ui.widget->data();
	emit messageFinished(data);
	accept();
}


void TcpWriterDialog::on_pbClose_clicked()
{
	reject();
}
