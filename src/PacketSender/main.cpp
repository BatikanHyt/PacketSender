#include "PacketSender.h"
#include <QtWidgets/QApplication>
#include <QtCore/QDebug>
#include <QtCore/QMutex>

static QMutex staticMutex;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context ,const QString &msg)
{
	QMutexLocker locker(&staticMutex);
	QTextEdit* logWidget = PacketSender::getLogWidget();
	QString debugText = "<TD><font color = 'green'><b>Debug : < / b>< / font>< / TD><TD>";
	QString infoText = "<TD><font color = 'green'><b>Info : < / b>< / font>< / TD><TD>";
	QString warningText = "<TD><font color = 'orange'><b>Warning : < / b>< / font>< / TD><TD>";
	QString criticalText = "<TD><font color = 'red'><b>Critical : < / b>< / font>< / TD><TD>";
	QString fatalText = "<TD><font color = 'red'><b>Fatal : < / b>< / font>< / TD><TD>";
	QString finalString;
	switch (type) {
	case QtDebugMsg:
		//fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
		debugText.append(msg);
		finalString = debugText;
		break;
	case QtInfoMsg:
		//fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
		infoText.append(msg);
		finalString = infoText;
		break;
	case QtWarningMsg:
		//fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
		warningText.append(msg);
		finalString = warningText;
		break;
	case QtCriticalMsg:
		//fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
		criticalText.append(msg);
		finalString = criticalText;
		break;
	case QtFatalMsg:
		//fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
		fatalText.append(msg);
		finalString = fatalText;
		break;
	}
	if (logWidget)
	{
		logWidget->append(finalString);
	}
}

int main(int argc, char *argv[])
{
	qInstallMessageHandler(myMessageOutput);

	QApplication a(argc, argv);
	PacketSender w;
	qDebug() << "Program Started";
	w.show();
	return a.exec();
}
