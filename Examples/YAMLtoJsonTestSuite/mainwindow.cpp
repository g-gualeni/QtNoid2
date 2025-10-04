#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QtNoidJson/QtNoidJson"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


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

void MainWindow::on_cmdConvertToJson_clicked()
{
    QString yamlText = ui->txtYAML->toPlainText();
    // QJsonObject jsonObj = QtNoid::Json::Yaml2Json::yamlToJson(yamlText);
    // QJsonDocument doc(jsonObj);
    // ui->txtJson->setPlainText(doc.toJson(QJsonDocument::Indented));
}


void MainWindow::on_cmdConvertToYAML_clicked()
{
    QString jsonText = ui->txtJson->toPlainText();
    QJsonDocument doc = QJsonDocument::fromJson(jsonText.toUtf8());
    // QString yamlText = QtNoid::Json::Yaml2Json::jsonToYaml(doc.object());
    // ui->txtYAML->setPlainText(yamlText);
}

void MainWindow::on_actionLoadYAML_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load YAML File"), "",
        tr("YAML Files (*.yml *.yaml);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"),
            tr("Cannot read file %1:\n%2.").arg(fileName, file.errorString()));
        return;
    }

    QTextStream in(&file);
    QString yamlContent = in.readAll();
    file.close();

    ui->txtYAML->setPlainText(yamlContent);

    // Update status bar with file path
    ui->statusbar->showMessage(fileName);

    // Look for corresponding .json file
    QFileInfo FI(fileName);

    QString jsonFileName = FI.absolutePath() + QDir::separator() +
                      FI.baseName() + ".json";
    QString errorFileName = FI.absolutePath() + QDir::separator() +
                            "error";
    QString descriptionFileName = FI.absolutePath() + QDir::separator() +
                                  "===";
    QString tokensFileName = FI.absolutePath() + QDir::separator() +
                             "test.event";

    QFile jsonFile(jsonFileName);
    if (jsonFile.exists() && jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream jsonIn(&jsonFile);
        QString jsonContent = jsonIn.readAll();
        jsonFile.close();
        ui->txtJsonExpected->setPlainText(jsonContent);
    } else {
        // Look for "error" file in the same folder
        QFile errorFile(errorFileName);
        if (errorFile.exists()) {
            ui->txtJsonExpected->setPlainText("ERROR");
        } else {
            ui->txtJsonExpected->clear();
        }
    }

    // Look for "===" description file
    QFile descriptionFile(descriptionFileName);
    if (descriptionFile.exists() && descriptionFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream descIn(&descriptionFile);
        QString descContent = descIn.readAll();
        descriptionFile.close();
        ui->lblDescription->setText(descContent);
    } else {
        ui->lblDescription->clear();
    }

    // Look for "test.event" tokens file
    QFile tokensFile(tokensFileName);
    if (tokensFile.exists() && tokensFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream tokensIn(&tokensFile);
        QString tokensContent = tokensIn.readAll();
        tokensFile.close();
        ui->txtTokensExpected->setPlainText(tokensContent);
    } else {
        ui->txtTokensExpected->clear();
    }
}
