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
    // ui->cmdPrevious->setText("\u2B05");
    // ui->cmdNext->setText("\u27a1\ufe0f"); // ➡️
    auto save_icon = style()->standardIcon(QStyle::SP_DialogSaveButton);
    ui->cmdSaveDescription->setIcon(save_icon);
    ui->cmdSaveDescription->setIconSize({16,16});
    auto iconArrowLeft = style()->standardIcon(QStyle::SP_ArrowLeft);
    ui->cmdPrevious->setIcon(iconArrowLeft);
    ui->cmdPrevious->setIconSize({32,32});
    ui->cmdPrevious->setText("");
    auto iconArrowRight = style()->standardIcon(QStyle::SP_ArrowRight);
    ui->cmdNext->setIcon(iconArrowRight);
    ui->cmdNext->setIconSize({32,32});
    ui->cmdNext->setText("");

    ui->txtDescription->clear();
    restoreGeometry(appConfig->restoreAsByteArray("Geometry", saveGeometry()));


    m_screenshotShortcut = QtNoid::App::Settings::initFullDialogGrabShortcut(this);

    // Recent files management
    m_recentFilesManager = new recentFilesManager(ui->actionRecent_Files, this);
    connect(m_recentFilesManager, &recentFilesManager::fileSelected, this, [this](const QString& fileName){
        updateUI_loadYamlFile(fileName);
        updateUI_convertYamlToJson();
    });
    connect(m_recentFilesManager, &recentFilesManager::listCleared, this, [this](){
        appConfig->clearRecentFiles();
    });
    updateUI_recentFiles(QString());

    // Recent test collection folder list
    updateUI_restoreRecentTestCollectionFolders();




    // Connect double-click signal from folder comboBox
    connect(ui->txtCollectionFolder, &FolderComboBox::doubleClicked, this, &MainWindow::onFolderComboBoxDoubleClicked);
    connect(ui->cmdBrowseFolder, &QToolButton::clicked, this, &MainWindow::onFolderComboBoxDoubleClicked);
    connect(ui->cmdNext, &QPushButton::clicked, this, &MainWindow::onNextText);
    connect(ui->cmdPrevious, &QPushButton::clicked, this, &MainWindow::onPreviousText);
}

MainWindow::~MainWindow()
{
    appConfig->saveValue("Geometry", saveGeometry());

    QStringList collectionFolderList;
    for(int ii=0; ii<ui->txtCollectionFolder->count(); ii++) {
        collectionFolderList << ui->txtCollectionFolder->itemText(ii);
    }
    appConfig->saveValue("CollectionFolderList", collectionFolderList);

    delete ui;
}

void MainWindow::on_cmdConvertToJson_clicked()
{
    updateUI_convertYamlToJson();
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

void MainWindow::updateUI_restoreRecentTestCollectionFolders()
{
    auto items = appConfig->restoreAsStringList("CollectionFolderList", {});
    ui->txtCollectionFolder->clear();
    ui->txtCollectionFolder->addItems(items);
}

void MainWindow::updateUI_scanTestCollectionFolder(const QString &folder)
{
    qDebug() << __func__ << folder;
    // Devo cercare tutti i file *.yaml nella cartella o nelle sue sottocartelle

    // QStringList listPathRecursively(const QString &path, const QStringList &nameFilters={});
    QtNoid::Common::File QtNoidFile;
    m_yamlTestCollectionList = QtNoidFile.listPathRecursively(folder, {".yaml"});
    m_yamlTestCollectionListCurrent = 0;
    QString msg = tr("Scanned %1 Yaml files").arg(m_yamlTestCollectionList.count());
    ui->statusbar->showMessage(msg);

    // qDebug() << __func__ << m_yamlTestCollectionList;

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

    updateUI_convertYamlToJson();
    updateUI_recentFiles(fileName);
}

void MainWindow::on_actionTestSuite_2022_01_17_triggered()
{
    if(generateTestDataFromResource("://yaml-test-suite/", "YamlTestSuite-2022-01-17")) {
        QMessageBox::information(this, tr("Test Suite"),
                                 tr("Test data generated successfully!"));
    }
    else {
        QMessageBox::information(this, tr("Test Suite"),
                                 tr("Error generating test data"));
    }
}

void MainWindow::on_actionTxt2JsonTestSuite_QtNoid_2_2_0_triggered()
{
    // :/Txt2JsonTestSuite/Txt2JsonTestSuite-QtNoid-2.2.0
    // ://Txt2JsonTestSuite/Txt2JsonTestSuite-QtNoid-2.2.0
    if(generateTestDataFromResource("://Txt2JsonTestSuite/", "Txt2JsonTestSuite-QtNoid-2.2.0")) {
        QMessageBox::information(this, tr("Test Suite"),
                                 tr("Test data generated successfully!"));
    }
    else {
        QMessageBox::information(this, tr("Test Suite"),
                                 tr("Error generating test data"));
    }
}

bool MainWindow::generateTestDataFromResource(const QString &root, const QString &resPath)
{
    // Read the file list from resources    
    QFile fileListResource(root + resPath);
    // qDebug() << __func__ << fileListResource.fileName();
    if (!fileListResource.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&fileListResource);
    QString baseOutputPath = QtNoid::App::Settings::appExeOrAppBundleDirPath();
    baseOutputPath += QDir::separator() + resPath;
    qDebug() << __func__ << baseOutputPath;

    // C:\GitHub\g-gualeni-public-QtNoid2\build\Desktop_Qt_6_9_3_llvm_mingw_64_bit-Debug\Examples\JsonYamlToJsonTestSuite
    //"C:/GitHub/g-gualeni-public-QtNoid2/build/Desktop_Qt_6_9_3_llvm_mingw_64_bit-Debug/Examples/JsonYamlToJsonTestSuite"
    // C:/GitHub/g-gualeni-public-QtNoid2/build/Desktop_Qt_6_9_3_llvm_mingw_64_bit-Debug/Examples/JsonYamlToJsonTestSuite\\Txt2JsonTestSuite-QtNoid-2.2.0"

    int fileCount = 0;
    int errorCount = 0;

    while (!in.atEnd()) {
        QString resourcePath = in.readLine().trimmed();
        if (resourcePath.isEmpty())
            continue;

        // qDebug() << __func__ << resourcePath;

        // :/Txt2JsonTestSuite/QtNoid-2.2.0/Txt2JsonTestSuite/QtNoid-2.2.0/S1/===
        // :/Txt2JsonTestSuite/QtNoid-2.2.0/Txt2JsonTestSuite/QtNoid-2.2.0/S1/===

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
    auto tooltipPath = QDir(QtNoid::App::Settings::appExeOrAppBundleDirPath()).relativeFilePath(filePath);
    ui->txtYAML->setToolTip(tooltipPath);
    ui->tabWidgetInput->setToolTip(tooltipPath);

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
        ui->txtDescription->setText(descContent);
    } else {
        ui->txtDescription->clear();
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

    // Run the converter

    return true;
}

void MainWindow::updateUI_convertYamlToJson()
{
    QString yamlText = ui->txtYAML->toPlainText();
    auto converter = QtNoid::Json::Yaml2Json(yamlText, this);
    ui->txtJsonOutput->setPlainText(converter.jsonAsString());
    ui->txtTokensOutput->setPlainText(converter.tokens().join("\n"));
    ui->txtErrorOutput->setPlainText(converter.errorString());
}

void MainWindow::onFolderComboBoxDoubleClicked()
{
    QString folderPath = ui->txtCollectionFolder->currentText();
    QString root = QtNoid::App::Settings::appExeOrAppBundleDirPath();
    folderPath = QFileDialog::getExistingDirectory(this, tr("Select the main folder"), folderPath);

    if (folderPath.isEmpty())
        return;

    // Remove root
    QDir dir(folderPath);
    folderPath = QDir(root).relativeFilePath(dir.absolutePath());
    // dir.cdUp();

    // Add the folder to the comboBox if it's not already there
    int index = ui->txtCollectionFolder->findText(folderPath);
    if (index == -1) {
        ui->txtCollectionFolder->addItem(folderPath);
        ui->txtCollectionFolder->setCurrentIndex(ui->txtCollectionFolder->count() - 1);
    } else {
        ui->txtCollectionFolder->setCurrentIndex(index);
    }

    // updateUI_loadYamlFile(dir.absoluteFilePath("in.yaml"));

    // Update status bar
    ui->statusbar->showMessage(tr("Selected folder: %1").arg(folderPath));
}

void MainWindow::onPreviousText()
{
    if(m_yamlTestCollectionListCurrent > 0) {
        m_yamlTestCollectionListCurrent--;
        updateUI_loadYamlFile(m_yamlTestCollectionList.at(m_yamlTestCollectionListCurrent));
    }
}

void MainWindow::onNextText()
{
    if((m_yamlTestCollectionListCurrent + 1) < m_yamlTestCollectionList.count()) {
        m_yamlTestCollectionListCurrent++;
        updateUI_loadYamlFile(m_yamlTestCollectionList.at(m_yamlTestCollectionListCurrent));
    }
}






void MainWindow::on_txtCollectionFolder_currentTextChanged(const QString &arg1)
{
    updateUI_scanTestCollectionFolder(arg1);
}

