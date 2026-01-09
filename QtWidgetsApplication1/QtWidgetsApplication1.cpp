#include "QtWidgetsApplication1.h"

QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	test1 = new ThreadYOLOV8test1(this);
	connect(test1, SIGNAL(receiveResult(const QString)), this, SLOT(append(const QString)));
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}

void QtWidgetsApplication1::initForm()
{
}

void QtWidgetsApplication1::initConfig()
{
}

void QtWidgetsApplication1::saveConfig()
{
}

void QtWidgetsApplication1::initTimer()
{
}

void QtWidgetsApplication1::append(const QString &data)
{
	append(0, data);
}

void QtWidgetsApplication1::append(int type, const QString &data, bool clear)
{
	static int currentCount = 0;
	static int maxCount = 100000;

	if (clear) {
		ui.txtMain->clear();
		currentCount = 0;
		return;
	}

	if (currentCount >= maxCount) {
		ui.txtMain->clear();
		currentCount = 0;
	}

	if (ui.ckShow->isChecked()) {
		return;
	}

	QString strData = data;
	strData = strData.replace("\r", "");
	strData = strData.replace("\n", "");

	QString strType;
	if (type == 0) {
		strType = "Recv";
		ui.txtMain->setTextColor(QColor("#22A3A9"));
	}
	else if (type == 1) {
		strType = "Send";
		ui.txtMain->setTextColor(QColor("#753775"));
	}
	else {
		strType = "Error";
		ui.txtMain->setTextColor(QColor("#D64D54"));
	}

	strData = QString("Time[%1] %2: %3").arg(TIMEMS).arg(strType).arg(strData);
	ui.txtMain->append(strData);
	currentCount++;
}

void QtWidgetsApplication1::connected()
{
}

void QtWidgetsApplication1::disconnected()
{
}

void QtWidgetsApplication1::error()
{
}

void QtWidgetsApplication1::readData()
{
}

void QtWidgetsApplication1::sendData(const QString &data)
{
}

void QtWidgetsApplication1::on_btnConnect_clicked()
{
	if (ui.btnConnect->text() == "Start") 
	{
		ui.btnConnect->setText("Stop");
		test1->setDelayTime(ui.spinBox_DelayTime->value());
		test1->slots_start();
	}
	else 
	{
		ui.btnConnect->setText("Start");
		test1->stop();
	}
}

void QtWidgetsApplication1::on_btnSave_clicked()
{
}

void QtWidgetsApplication1::on_btnClear_clicked()
{
	append(0, "", true);
}

void QtWidgetsApplication1::on_btnSend_clicked()
{
}
