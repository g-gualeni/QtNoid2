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
    qApp->setApplicationDisplayName("This Is Application Display Name");
    QString config = Settings::filePathAsAppSibling();
    ui->txtConfigPath->setText(config);

    ui->txtGroupName->setText(Settings::groupNameFromObjectOrClass(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmdGroupName_clicked()
{
    this->setObjectName("AppBasicUsage");
    ui->txtGroupName->setText(Settings::groupNameFromObjectOrClass(this));
}


void MainWindow::on_optUpdateWindowTitle_clicked(bool checked)
{
    // I use the central widget as the simplest way to get the main window.
    Settings::updateMainWindowTitle(checked, ui->centralwidget);
}


void MainWindow::on_optUpdateWindowTitle_clicked()
{

}

