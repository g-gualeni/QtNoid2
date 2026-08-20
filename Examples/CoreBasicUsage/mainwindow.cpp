#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "QtNoidCore/QtNoidCore"
#include "QtNoidApp/QtNoidApp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("CORE Basic Usage");
    m_screenshotShortcut = QtNoid::App::Development::initFullDialogGrabShortcut(this);

    ui->plainTextEdit->setPlainText(QtNoid::Core::buildInfo());
}

MainWindow::~MainWindow()
{
    delete ui;
}
