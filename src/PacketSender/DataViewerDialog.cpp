#include "DataViewerDialog.h"

DataViewerDialog::DataViewerDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.widget->setOverwriteMode(false);
	setMinimumSize(600, 600);
	ui.widget->setReadOnly(true);
}

DataViewerDialog::~DataViewerDialog()
{
}

QByteArray DataViewerDialog::getData() const
{
	QByteArray data = ui.widget->data();
	return data;
}


void DataViewerDialog::setData(const QByteArray& data)
{
	ui.widget->setData(data);
}

void DataViewerDialog::on_pbClose_clicked()
{
	reject();
}