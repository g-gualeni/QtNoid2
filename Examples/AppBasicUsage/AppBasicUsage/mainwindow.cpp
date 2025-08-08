#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString appBundle = "AA";
    ui->txtAppBundle->setText(appBundle);
    QString config = "QQ";
    ui->txtConfigPath->setText(config);
}

MainWindow::~MainWindow()
{
    delete ui;
}
