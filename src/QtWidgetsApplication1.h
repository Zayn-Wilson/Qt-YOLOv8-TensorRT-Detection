#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"
#include "ThreadYOLOV8test1.h"

#define TIMEMS QTime::currentTime().toString("hh:mm:ss zzz")
#pragma execution_character_set("utf-8")

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget *parent = nullptr);
    ~QtWidgetsApplication1();

private:
    Ui::QtWidgetsApplication1Class ui;
	ThreadYOLOV8test1* test1;

private slots:
	void initForm();
	void initConfig();
	void saveConfig();
	void initTimer();
	void append(const QString &data);
	void append(int type, const QString &data, bool clear = false);

private slots:
	void connected();
	void disconnected();
	void error();
	void readData();
	void sendData(const QString &data);

private slots:
	void on_btnConnect_clicked();
	void on_btnSave_clicked();
	void on_btnClear_clicked();
	void on_btnSend_clicked();
};
