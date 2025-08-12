#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QtNoidApp/QtNoidApp"

using namespace QtNoid::App;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString appBundle = Settings::appExeOrAppBundlePath();
    ui->txtAppBundle->setText(appBundle);
    qApp->setApplicationDisplayName("AA");
    QString config = Settings::filePathAsAppSibling();
    ui->txtConfigPath->setText(config);
}

MainWindow::~MainWindow()
{
    delete ui;
}
