#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QtNoidCommon/QtNoidCommon"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QtNoid::Common::File QtNoidFile;
    auto res = QtNoidFile.listPathRecursively(qApp->applicationFilePath());
    ui->plainTextEdit->setPlainText(res.join("\n"));

}

MainWindow::~MainWindow()
{
    delete ui;
}
