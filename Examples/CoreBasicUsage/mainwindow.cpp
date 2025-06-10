#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "QtNoidCore/QtNoidCore"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plainTextEdit->setPlainText(QtNoid::Core::buildInfo());
}

MainWindow::~MainWindow()
{
    delete ui;
}
