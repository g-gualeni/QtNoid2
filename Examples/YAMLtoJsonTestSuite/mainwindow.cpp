#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QtNoidJson/QtNoidJson"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QCoreApplication>

/*

:/yaml-test-suite/data-2022-01-17/yaml-test-suite/data-2022-01-17/2AUY/===

:/yaml-test-suite/data-2022-01-17/TestSuite-2022-01-17



*/

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

void MainWindow::on_actionTestSuite_2022_01_17_triggered()
{
    generateTestDataFromResource();
    QMessageBox::information(this, tr("Test Suite"),
        tr("Test data generated successfully!"));
}

void MainWindow::generateTestDataFromResource()
{
    // Read the file list from resources
    QFile fileListResource(":/yaml-test-suite/data-2022-01-17/TestSuite-2022-01-17");
    if (!fileListResource.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"),
            tr("Cannot read resource file list"));
        return;
    }

    QTextStream in(&fileListResource);
    QString baseOutputPath = QCoreApplication::applicationDirPath() + "/TestSuite-2022-01-17";

    int fileCount = 0;
    int errorCount = 0;

    while (!in.atEnd()) {
        QString resourcePath = in.readLine().trimmed();
        if (resourcePath.isEmpty())
            continue;

        // Open the resource file
        QFile resourceFile(resourcePath);
        if (!resourceFile.open(QIODevice::ReadOnly)) {
            errorCount++;
            continue;
        }

        // Read the content
        QByteArray content = resourceFile.readAll();
        resourceFile.close();

        // Extract the relative path (remove the :/yaml-test-suite/data-2022-01-17/yaml-test-suite/data-2022-01-17/ prefix)
        QString relativePath = resourcePath;
        relativePath.remove(":/yaml-test-suite/data-2022-01-17/yaml-test-suite/data-2022-01-17/");

        // Create full output path
        QString outputPath = baseOutputPath + "/" + relativePath;

        // Create directories if needed
        QFileInfo fileInfo(outputPath);
        QDir dir;
        if (!dir.mkpath(fileInfo.absolutePath())) {
            errorCount++;
            continue;
        }

        // Write the file
        QFile outputFile(outputPath);
        if (!outputFile.open(QIODevice::WriteOnly)) {
            errorCount++;
            continue;
        }

        outputFile.write(content);
        outputFile.close();
        fileCount++;
    }

    fileListResource.close();

    // Show summary in status bar
    ui->statusbar->showMessage(
        tr("Generated %1 files (%2 errors) in %3")
        .arg(fileCount)
        .arg(errorCount)
        .arg(baseOutputPath)
    );
}
