#include "PacketSender.h"
#include <QtWidgets/QApplication>
#include <QtCore/QDebug>
#include <QtCore/QMutex>
#include <QtCore/QDateTime>
#include <QtCore/QFile>
#include <QtWidgets/QDesktopWidget>

static QMutex staticMutex;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context ,const QString &msg)
{
	QMutexLocker locker(&staticMutex);
	QString currentTime = QDateTime::currentDateTimeUtc().toString("hh:mm:ss.zzz");
	PacketSender::getLogWidget()->setLogTime(currentTime);
	/*QString debugText = QString("<TD><font color = 'green'><b>(%1) Debug : < / b>< / font>< / TD><TD>").arg(currentTime);
	QString infoText = QString("<TD><font color = 'green'><b>(%1) Info : < / b>< / font>< / TD><TD>").arg(currentTime);
	QString warningText = QString("<TD><font color = 'orange'><b>(%1) Warning : < / b>< / font>< / TD><TD>").arg(currentTime);
	QString criticalText = QString("<TD><font color = 'red'><b>(%1) Critical : < / b>< / font>< / TD><TD>").arg(currentTime);
	QString fatalText = QString("<TD><font color = 'red'><b>(%1) Fatal : < / b>< / font>< / TD><TD>(%1)").arg(currentTime);
	*/
	QString finalString;
	switch (type) {
	case QtDebugMsg:
		finalString = msg;
		PacketSender::getLogWidget()->setLogLevel("Debug");
		/*debugText.append(msg);
		finalString = debugText;*/
		break;
	case QtInfoMsg:
		finalString = msg;
		PacketSender::getLogWidget()->setLogLevel("Info");
		/*infoText.append(msg);
		finalString = infoText;*/
		break;
	case QtWarningMsg:
		finalString = msg;
		PacketSender::getLogWidget()->setLogLevel("Warning");
	/*	warningText.append(msg);
		finalString = warningText;*/
		break;
	case QtCriticalMsg:
		finalString = msg;
		PacketSender::getLogWidget()->setLogLevel("Critical");
		/*criticalText.append(msg);
		finalString = criticalText;*/
		break;
	case QtFatalMsg:
		finalString = msg;
		PacketSender::getLogWidget()->setLogLevel("Fatal");
		/*fatalText.append(msg);
		finalString = fatalText;*/
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
	w.adjustSize();
	w.move(QApplication::desktop()->screen()->rect().center() - w.rect().center());
	//w.showMaximized();
	w.show();
	return a.exec();
}
