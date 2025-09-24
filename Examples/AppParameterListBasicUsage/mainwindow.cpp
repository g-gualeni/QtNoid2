#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createList()
{
    m_list.setName("Setup");
    // m_list.emplace(10, "Light", "Current Light Intensity");

}





void MainWindow::on_cmdQDebug_clicked()
{
    qDebug() << m_list;
}

