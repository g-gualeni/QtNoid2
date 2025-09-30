#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QtNoidJson/QtNoidJson"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmdUpdateJson_clicked()
{
    QStringList currentText = ui->txtText->toPlainText().split("\n");
    QJsonObject res = QtNoid::Json::Txt2Json::plainTextToJson(currentText);
    QJsonDocument doc(res);
    ui->txtJson->setPlainText(doc.toJson());
}


void MainWindow::on_cmdUpdateText_clicked()
{
    QString currentJson = ui->txtJson->toPlainText();
    QJsonDocument doc = QJsonDocument::fromJson(currentJson.toUtf8());
    QStringList txtList = QtNoid::Json::Txt2Json::plainTextFromJson(doc.object());
    ui->txtText->setPlainText(txtList.join("\n"));
}

