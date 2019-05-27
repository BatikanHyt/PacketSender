#include "PacketSender.h"
#include <QtWidgets/QApplication>
#include <QtCore/QDebug>
#include <QtCore/QMutex>
#include <QtCore/QDateTime>
static QMutex staticMutex;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context ,const QString &msg)
{
	QMutexLocker locker(&staticMutex);
	QString currentTime = QDateTime::currentDateTimeUtc().toString("hh:mm:ss");
	QString debugText = QString("<TD><font color = 'green'><b>(%1) Debug : < / b>< / font>< / TD><TD>").arg(currentTime);
	QString infoText = QString("<TD><font color = 'green'><b>(%1) Info : < / b>< / font>< / TD><TD>").arg(currentTime);
	QString warningText = QString("<TD><font color = 'orange'><b>(%1) Warning : < / b>< / font>< / TD><TD>").arg(currentTime);
	QString criticalText = QString("<TD><font color = 'red'><b>(%1) Critical : < / b>< / font>< / TD><TD>").arg(currentTime);
	QString fatalText = QString("<TD><font color = 'red'><b>(%1) Fatal : < / b>< / font>< / TD><TD>(%1)").arg(currentTime);
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
	if (PacketSender::getLogWidget())
	{
		finalString.remove('"');
		PacketSender::getLogWidget()->generateLogSignal(finalString);
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
