#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("App Parameter Basic Usage");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmdUpdate_clicked()
{
    m_parameter.setName(ui->txtName->text());
    m_parameter.setDescription(ui->txtDescription->text());
    m_parameter.setReadOnly(ui->optReadOnly->checkState());
    QStringList range = ui->txtRange->text().split(" ");
    auto min = range.first().toFloat();
    auto max = range.last().toFloat();
    m_parameter.setRange(min, max);
    m_parameter.setValue(ui->txtValue->value());
    m_parameter.setUnit(ui->txtUnit->text());
    const QStringList presetTxtList = ui->txtPresets->toPlainText().split('\n', Qt::SkipEmptyParts);
    qDebug() << __func__ <<presetTxtList;
    for(const QString& preset : presetTxtList) {
        QStringList nameValue = preset.split(" ");
        double val = nameValue.last().toDouble();
        m_parameter.setPreset(nameValue.first(), val);
    }
}


void MainWindow::on_cmdToJson_clicked()
{
    QJsonObject schemaObj = m_parameter.toJsonSchema();
    QJsonObject valueObj = m_parameter.toJsonValue();

    QJsonDocument schemaDoc(schemaObj);
    QString schemaString = schemaDoc.toJson();  //
    ui->txtJsonSchema->setPlainText(schemaString);

    QJsonDocument valueDoc(valueObj);
    QString valueString = valueDoc.toJson();  //
    ui->txtJsonValue->setPlainText(valueString);
}


void MainWindow::on_cmdFromJson_clicked()
{
    ui->txtErrors->setText("ERROR");
}

