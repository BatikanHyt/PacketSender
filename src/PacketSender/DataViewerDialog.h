#pragma once

#include <QDialog>
#include "ui_DataViewerDialog.h"

class DataViewerDialog : public QDialog
{
	Q_OBJECT

public:

	DataViewerDialog(QWidget *parent = Q_NULLPTR);

	~DataViewerDialog();

	QByteArray getData() const;

	void setData(const QByteArray& data);

public slots:

	void on_pbClose_clicked();

private:
	Ui::DataViewerDialog ui;
};
