#include "mainwindow.h"
#include "./ui_mainwindow.h"
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


void MainWindow::addRecentFileList(QAction *recentFilesParent, QStringList &recentFiles,
                                   std::function<void(const QString&)> onFileSelected)
{
    if(recentFilesParent == nullptr)
        return;

    QMenu *recentFilesMenu = recentFilesParent->menu();
    if (!recentFilesMenu) {
        // Se non esiste ancora, crealo
        recentFilesMenu = new QMenu(this);
        recentFilesParent->setMenu(recentFilesMenu);
    }

    for (int ii = 0; ii < recentFiles.size() && ii < 10; ++ii) {
        QString fileName = recentFiles[ii];
        QString displayName = QString("%1| %2").arg(ii + 1).arg(QFileInfo(fileName).fileName());

        QAction *fileAction = recentFilesMenu->addAction(displayName);
        fileAction->setData(fileName); // salva il path completo

        // Connetti all'apertura del file
        connect(fileAction, &QAction::triggered, this, [fileName, onFileSelected](){
                onFileSelected(fileName);
        });
    }

    // Aggiungi separatore e opzione per pulire
    if (!recentFiles.isEmpty()) {
        recentFilesMenu->addSeparator();
        QAction *clearAction = recentFilesMenu->addAction(tr("Clear Recent Files"));
        connect(clearAction, &QAction::triggered, this, [this, recentFilesMenu]() {
            recentFilesMenu->clear();
            qDebug() << "clearRecentFiles()";
        });
    }
}

void MainWindow::loadYamlFile(const QString &filePath)
{
    qDebug() << __func__ << filePath;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lblDescription->clear();
    restoreGeometry(appConfig->restoreAsByteArray("Geometry", saveGeometry()));
    QStringList recentFiles({"AA", "BB", "CC", "DD", "EE"});
    addRecentFileList(ui->actionRecent_Files, recentFiles, [this](const QString &file) {
        loadYamlFile(file);});
}

MainWindow::~MainWindow()
{
    appConfig->saveValue("Geometry", saveGeometry());
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
