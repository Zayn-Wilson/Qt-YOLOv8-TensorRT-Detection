#include "frmmain.h"
#include "ui_frmmain.h"
#include "QtWidgetsApplication1.h"

frmMain::frmMain(QWidget *parent) : QWidget(parent), ui(new Ui::frmMain)
{
    ui->setupUi(this);
    this->initForm();
    this->initConfig();
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::initForm()
{
    ui->tabWidget->addTab(new QtWidgetsApplication1, "Test Thread 1");
    ui->tabWidget->addTab(new QtWidgetsApplication1, "Test Thread 2");
}

void frmMain::initConfig()
{
}

void frmMain::saveConfig()
{
}
