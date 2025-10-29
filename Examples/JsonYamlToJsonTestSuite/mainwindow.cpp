#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "recentfilesmanager.h"
#include "QtNoidJson/QtNoidJson"
#include "QtNoidCommon/QtNoidCommon"
#include "QtNoidApp/QtNoidApp"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QUrl>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lblDescription->clear();
    restoreGeometry(appConfig->restoreAsByteArray("Geometry", saveGeometry()));

    m_screenshotShortcut = QtNoid::App::Settings::initFullDialogGrabShortcut(this);

    // Recent files management
    m_recentFilesManager = new recentFilesManager(ui->actionRecent_Files, this);
    connect(m_recentFilesManager, &recentFilesManager::fileSelected, this, [this](const QString& fileName){
        updateUI_loadYamlFile(fileName);
    });
    connect(m_recentFilesManager, &recentFilesManager::listCleared, this, [this](){
        appConfig->clearRecentFiles();
    });
    updateUI_recentFiles(QString());
}

MainWindow::~MainWindow()
{
    appConfig->saveValue("Geometry", saveGeometry());    
    delete ui;
}

void MainWindow::on_cmdConvertToJson_clicked()
{
    QString yamlText = ui->txtYAML->toPlainText();
    auto converter = QtNoid::Json::Yaml2Json(yamlText, this);
    ui->txtJsonOutput->setPlainText(converter.jsonAsString());
    ui->txtTokensOutput->setPlainText(converter.tokens().join("\n"));
}


void MainWindow::on_cmdConvertToYAML_clicked()
{
    QString jsonText = ui->txtJsonOutput->toPlainText();
    QJsonDocument doc = QJsonDocument::fromJson(jsonText.toUtf8());
    // QString yamlText = QtNoid::Json::Yaml2Json::jsonToYaml(doc.object());
    // ui->txtYAML->setPlainText(yamlText);
}

void MainWindow::updateUI_recentFiles(QString fileName)
{
    appConfig->addRecentFile(fileName);
    m_recentFilesManager->updateRecentFilesMenu(appConfig->restoreRecentFiles());
}

void MainWindow::on_actionLoadYAML_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load YAML File"), "",
        tr("YAML Files (*.yml *.yaml);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    if(!updateUI_loadYamlFile(fileName)) {
        QMessageBox::warning(this, tr("Error"),
                             tr("Cannot read the file %1:").arg(fileName));
        return;
    };

    updateUI_recentFiles(fileName);
}

void MainWindow::on_actionTestSuite_2022_01_17_triggered()
{
    if(generateTestDataFromResource("TestSuite-2022-01-17")) {
        QMessageBox::information(this, tr("Test Suite"),
                                 tr("Test data generated successfully!"));
    }
    else {
        QMessageBox::information(this, tr("Test Suite"),
                                 tr("Error generating test data"));
    }
}

bool MainWindow::generateTestDataFromResource(const QString &resPath)
{
    // Read the file list from resources
    QString root = "://yaml-test-suite/";
    QFile fileListResource(root + resPath);
    if (!fileListResource.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&fileListResource);
    QString baseOutputPath = QtNoid::App::Settings::appExeOrAppBundleDirPath();
    qDebug() << baseOutputPath;
    baseOutputPath += QDir::separator() + resPath;

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

    if(errorCount) {
        return false;
    }
    return true;
}

void MainWindow::on_actionTestDataFolder_triggered()
{
    QString appPath = QtNoid::App::Settings::appExeOrAppBundleDirPath();

    QDesktopServices::openUrl(QUrl::fromLocalFile(appPath));
}


bool MainWindow::updateUI_loadYamlFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    QString yamlContent = in.readAll();
    file.close();

    ui->txtYAML->setPlainText(yamlContent);

    // Update status bar with file path
    ui->statusbar->showMessage(filePath);

    // Look for corresponding .json file
    QFileInfo FI(filePath);

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
            ui->txtJsonExpected->setPlainText("NO JSON EXPECTED");;
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

    return true;
}



