#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "recentfilesmanager.h"
#include "frmsavedataset.h"
#include "frmnewdataset.h"
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

#include <QColorSpace>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // ui->cmdPrevious->setText("\u2B05");
    // ui->cmdNext->setText("\u27a1\ufe0f"); // ➡️
    auto save_icon = style()->standardIcon(QStyle::SP_DialogSaveButton);
    ui->cmdSaveTestDataset->setIcon(save_icon);
    ui->cmdSaveTestDataset->setIconSize({16,16});

    auto new_icon = style()->standardIcon(QStyle::SP_FileIcon);
    ui->cmdNewTestDataset->setIcon(new_icon);
    ui->cmdNewTestDataset->setIconSize({16,16});

    auto iconArrowLeft = style()->standardIcon(QStyle::SP_ArrowLeft);
    ui->cmdPrevious->setIcon(iconArrowLeft);
    ui->cmdPrevious->setIconSize({32,32});
    ui->cmdPrevious->setText("");
    auto iconArrowRight = style()->standardIcon(QStyle::SP_ArrowRight);
    ui->cmdNext->setIcon(iconArrowRight);
    ui->cmdNext->setIconSize({32,32});
    ui->cmdNext->setText("");
    ui->txtDescription->clear();

    updateUI_initDevelopment();

    initAppConfigFile();
    connect(appConfig, &QtNoid::App::ConfigFile::fileLoaded, this, [&](const QString& fName){
        initFromAppConfig();
    });
    appConfig->load();

    m_screenshotShortcut = QtNoid::App::Development::initFullDialogGrabShortcut(this);

    // Recent files management
    m_recentFilesManager = new recentFilesManager(ui->actionRecent_Files, this);
    connect(m_recentFilesManager, &recentFilesManager::fileSelected, this, [this](const QString& fileName){
        updateUI_loadYamlFile(fileName);
        updateUI_statusBar();
        updateUI_convertYamlToJson();
    });
    connect(m_recentFilesManager, &recentFilesManager::listCleared, this, [this](){
        appConfig->clearRecentFiles();
    });
    updateUI_recentFiles(QString());

    updateUI_progressBar();
    updateUI_txtCurrentFile();
    updateUI_cmdPrevNext();
    updateUI_statusBar();

    // Connect double-click signal from folder comboBox
    connect(ui->txtCollectionFolder, &FolderComboBox::doubleClicked, this, &MainWindow::onFolderComboBoxDoubleClicked);
    connect(ui->txtCollectionFolder, &FolderComboBox::currentTextChanged, this, [&](const QString &folder){
        updateUI_scanTestCollectionFolder(folder);
        updateUI_progressBar();
        updateUI_txtCurrentFile();
        updateUI_cmdPrevNext();
        updateUI_loadYamlFile(m_yamlTestCollectionList.value(0, {}));
        updateUI_statusBar();
    });

    connect(ui->txtCurrentFile, &QComboBox::currentIndexChanged, this, [&](int index){
        // QString folder = ui->txtCollectionFolder->currentText();
        // QString root = QtNoid::App::Settings::appExeOrAppBundleDirPath();
        m_yamlTestCollectionListCurrent = index;
        updateUI_progressBar();
        updateUI_txtCurrentFile();
        updateUI_cmdPrevNext();
        updateUI_loadYamlFile(m_yamlTestCollectionList.value(index, {}));
        updateUI_statusBar();
    });

    connect(ui->cmdBrowseFolder, &QToolButton::clicked, this, &MainWindow::onFolderComboBoxDoubleClicked);
    connect(ui->cmdNext, &QPushButton::clicked, this, &MainWindow::onNextTest);
    connect(ui->cmdPrevious, &QPushButton::clicked, this, &MainWindow::onPreviousTest);
    connect(ui->cmdSaveTestDataset, &QPushButton::clicked, this, &MainWindow::onCmdSaveTestDataset);
    connect(ui->cmdNewTestDataset, &QPushButton::clicked, this, &MainWindow::onCreateNewTest);
    connect(ui->actionCreate_New_Test, &QAction::triggered, this, &MainWindow::onCreateNewTest);

}


void MainWindow::initFromAppConfig()
{
    // qDebug() << __func__ ;

    // Geometry
    restoreGeometry(appConfig->restoreAsByteArray("Geometry", saveGeometry()));

    // Recent test collection folder list
    appConfig->restoreComboBoxTextItems(ui->txtCollectionFolder, "CollectionFolderList", {});
    updateUI_scanTestCollectionFolder(ui->txtCollectionFolder->currentText());
    updateUI_loadYamlFile(m_yamlTestCollectionList.value(0, {}));
    setYamlTestCollectionListCurrent(appConfig->restoreAsInt("yamlTestCollectionListCurrent", 0));
}


MainWindow::~MainWindow()
{
    appConfig->saveValue("Geometry", saveGeometry());
    appConfig->saveComboBoxTextItems(ui->txtCollectionFolder, "CollectionFolderList");
    appConfig->saveValue("yamlTestCollectionListCurrent", m_yamlTestCollectionListCurrent);
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


void MainWindow::updateUI_scanTestCollectionFolder(const QString &folder)
{
    qDebug() << __func__ << folder;
    m_yamlTestCollectionListCurrent = 0;
    m_yamlTestCollectionList.clear();

    if(folder.isEmpty()) {
        return;
    }

    QtNoid::Common::File QtNoidFile;
    QString root = QtNoid::App::Settings::appExeOrAppBundleDirPath() + QDir::separator() + folder;
    m_yamlTestCollectionList = QtNoidFile.listPathRecursively(root, {"in.yaml"});
    qDebug() << __func__ << root << "count:" << m_yamlTestCollectionList.count();
    ui->txtCurrentFile->clear();
    QDir dir(root);
    for(auto it = m_yamlTestCollectionList.constBegin(); it < m_yamlTestCollectionList.constEnd(); ++it) {
        auto file = *it;
        auto relFile = dir.relativeFilePath(file);
        ui->txtCurrentFile->addItem(dir.relativeFilePath(*it));
    }
}

void MainWindow::updateUI_progressBar()
{
    qDebug() << __func__;
    ui->cboCollectionProgress->setVisible(!m_yamlTestCollectionList.isEmpty());
    ui->cboCollectionProgress->setMinimum(0);
    ui->cboCollectionProgress->setMaximum(m_yamlTestCollectionList.count());
    ui->cboCollectionProgress->setValue(m_yamlTestCollectionListCurrent + 1);
    QString msg = QString("%1/%2").arg(m_yamlTestCollectionListCurrent+1).arg(m_yamlTestCollectionList.count());
    ui->cboCollectionProgress->setToolTip(msg);
}

void MainWindow::updateUI_cmdPrevNext()
{
    qDebug() << __func__;
    // Set cmdPrevious enabled if I am not
    bool enabled = (!m_yamlTestCollectionList.isEmpty()) && (m_yamlTestCollectionListCurrent >0);
    ui->cmdPrevious->setEnabled(enabled);

    enabled = (!m_yamlTestCollectionList.isEmpty()) && (m_yamlTestCollectionListCurrent+1 < m_yamlTestCollectionList.count());
    ui->cmdNext->setEnabled(enabled);
}

void MainWindow::updateUI_txtCurrentFile()
{
    qDebug() << __func__ << m_yamlTestCollectionListCurrent << m_yamlTestCollectionList.count();
    ui->txtCurrentFile->blockSignals(true);
    ui->txtCurrentFile->setCurrentIndex(m_yamlTestCollectionListCurrent);
    ui->txtCurrentFile->blockSignals(false);
}

void MainWindow::updateUI_statusBar(const QString& msg)
{
    qDebug() << __func__ << msg;
    if(!msg.isEmpty()) {
        ui->statusbar->showMessage(msg);
        return;
    }

    ui->statusbar->clearMessage();

    QString sbMsg;
    if(!m_yamlTestCollectionList.isEmpty()) {
        auto folderPath = ui->txtCollectionFolder->currentText();
        QDir dir(folderPath);
        auto fileName = dir.relativeFilePath(m_yamlTestCollectionList.value(m_yamlTestCollectionListCurrent, {}));
        sbMsg = tr("%1 Item %2/%3 - %4")
                    .arg(folderPath)
                    .arg(m_yamlTestCollectionListCurrent+1)
                    .arg(m_yamlTestCollectionList.count())
                    .arg(fileName) ;
        qDebug() << __func__ << sbMsg;
        ui->statusbar->showMessage(sbMsg);
        return;
    }    
}

void MainWindow::on_actionLoadYAML_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load YAML File"), "",
        tr("YAML Files (*.yml *.yaml);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    updateUI_loadYamlFile(fileName);
    updateUI_convertYamlToJson();
    updateUI_recentFiles(fileName);
}

void MainWindow::on_actionTestSuite_2022_01_17_triggered()
{
    // :/Test/resources/yaml-test-suite/data-2022-01-17.txt
    // :/Data/resources/yaml-test-suite/data-2022-01-17/2AUY/===
    if(generateTestDataFromResource("://Data/resources/yaml-test-suite/data-2022-01-17/",
                                     ":/Test/resources/yaml-test-suite/data-2022-01-17.txt"))
    {
        QMessageBox::information(this, "YamlTestSuite 2022-01-17",
                                 tr("Test data generated successfully!"));
    }
    else {
        QMessageBox::information(this, "YamlTestSuite 2022-01-17",
                                 tr("Error generating test data"));
    }
}

void MainWindow::on_actionTxt2JsonTestSuite_QtNoid_2_2_0_triggered()
{
    // :/Test/resources/Txt2JsonTestSuite/QtNoid-2.2.0.txt
    // :/Data/resources/Txt2JsonTestSuite/QtNoid-2.2.0/Array-001/===
    if(generateTestDataFromResource("://Data/resources/Txt2JsonTestSuite/QtNoid-2.2.0/",
                                     ":/Test/resources/Txt2JsonTestSuite/QtNoid-2.2.0.txt"))
    {
        QMessageBox::information(this, "Test Suite QtNoid_2_2_0",
                                 tr("Test data generated successfully!"));
    }
    else {
        QMessageBox::information(this, "Test Suite QtNoid_2_2_0",
                                 tr("Error generating test data"));
    }
}

bool MainWindow::generateTestDataFromResource(const QString &dataPrefix, const QString &mapFilePath)
{
    // Read the file map
    QFile fileListResource(mapFilePath);
    if (!fileListResource.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream in(&fileListResource);

    // Destination path is in CWD + dataPrefix
    QString baseOutputPath = dataPrefix;
    baseOutputPath.remove(":");
    baseOutputPath = QtNoid::App::Settings::appExeOrAppBundleDirPath() + baseOutputPath;

    QDir dir;
    dir.mkpath(baseOutputPath);
    // qDebug() << __func__ << baseOutputPath;

    int fileCount = 0;
    int errorCount = 0;

    while (!in.atEnd()) {
        QString resourceFilePath = in.readLine().trimmed();
        if (resourceFilePath.isEmpty())
            continue;

        // qDebug() << __func__ << dataPrefix + resourceFilePath;

        // Copy the resource file to baseOutputPath
        // :/Data/resources/yaml-test-suite/data-2022-01-17/2AUY/===
        QFile resourceFile(dataPrefix + resourceFilePath);
        QFileInfo FI(baseOutputPath + resourceFilePath);
        dir.mkpath(FI.absolutePath());
        auto outFilePath = FI.absoluteFilePath();
        auto res = resourceFile.copy(outFilePath);
        if(!res) {
            errorCount++;
        }
        else {
            QFile::setPermissions(outFilePath, QFile::ReadOwner | QFile::WriteOwner);
        }

        // qDebug() << __func__ << "res:" << res << outFilePath;

        fileCount++;
    }

    fileListResource.close();

    // Show summary in status bar
    QString msg(
        tr("Generated %1 files (%2 errors) in %3")
        .arg(fileCount)
        .arg(errorCount)
        .arg(baseOutputPath)
    );
    updateUI_statusBar(msg);

    if(errorCount) {
        return false;
    }
    return true;
}

void MainWindow::on_actionTestDataFolder_triggered()
{
    QString appPath = QtNoid::App::Settings::appExeOrAppBundleDirPath() + "/Data/resources";

    QDesktopServices::openUrl(QUrl::fromLocalFile(appPath));
}

void MainWindow::setYamlTestCollectionListCurrent(int newYamlTestCollectionListCurrent)
{
    // qDebug() << __func__ <<"NewIndex:" << newYamlTestCollectionListCurrent << m_yamlTestCollectionList.count()
    //          << m_yamlTestCollectionListCurrent;

    if(newYamlTestCollectionListCurrent + 1 > m_yamlTestCollectionList.count()) {
        m_yamlTestCollectionListCurrent = m_yamlTestCollectionList.count() - 1;
    }
    else if(newYamlTestCollectionListCurrent < 0 ) {
        m_yamlTestCollectionListCurrent = 0;
    }
    else {
        m_yamlTestCollectionListCurrent = newYamlTestCollectionListCurrent;
    }
}


void MainWindow::updateUI_loadYamlFile(const QString &filePath)
{
    // qDebug() << __func__ << filePath;

    QString yamlContent;
    do{
        if(filePath.isEmpty()) {
            break;
        }
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            break;
        }
        QTextStream in(&file);
        yamlContent = in.readAll();
        file.close();
    } while(0);

    ui->txtYAML->clear();
    ui->txtJsonExpected->clear();
    ui->txtTokensExpected->clear();
    ui->txtErrorExpected->clear();
    ui->txtDescription->clear();

    if(yamlContent.isEmpty()){
        return;
    }
    ui->txtYAML->setPlainText(yamlContent);
    auto tooltipPath = QDir(QtNoid::App::Settings::appExeOrAppBundleDirPath()).relativeFilePath(filePath);
    ui->txtYAML->setToolTip(tooltipPath);
    ui->tabWidgetInput->setToolTip(tooltipPath);

    // Look for corresponding filed
    QFileInfo FI(filePath);
    QString jsonFileName = FI.absolutePath() + QDir::separator() +
                           FI.baseName() + ".json";
    QString errorFileName = FI.absolutePath() + QDir::separator() +
                            "error";
    QString descriptionFileName = FI.absolutePath() + QDir::separator() +
                                  "===";
    QString tokensFileName = FI.absolutePath() + QDir::separator() +
                             "test.event";

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

    // Load Expected JSON file
    QFile jsonFile(jsonFileName);
    if (jsonFile.exists() && jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream jsonIn(&jsonFile);
        QString jsonContent = jsonIn.readAll();
        jsonFile.close();
        ui->txtJsonExpected->setPlainText(jsonContent);
    } else {
        ui->txtJsonExpected->setPlainText("NO JSON EXPECTED");;
    }

    // Look for "error" file in the same folder
    QFile errorFile(errorFileName);
    if (errorFile.exists()) {
        ui->txtJsonExpected->setPlainText("ERROR");
        QTextStream errorIn(&errorFile);
        QString errorContent = errorFile.readAll();
        errorFile.close();
        if(errorContent.isEmpty()){
            errorContent = "Error file is present but it is empty";
        }
        ui->txtErrorExpected->setPlainText(errorContent);
    }
    else {
        ui->txtErrorExpected->clear();
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

    return;
}


void MainWindow::updateUI_convertYamlToJson()
{

    QString yamlText = ui->txtYAML->toPlainText();
    // qDebug() << __func__ << "LastYaml:" << yamlText;

    auto converter = QtNoid::Json::Yaml2Json(yamlText, this);
    ui->txtJsonOutput->setPlainText(converter.jsonAsString());
    ui->txtTokensOutput->setPlainText(converter.tokens().join("\n"));
    ui->txtErrorOutput->setPlainText(converter.errorString());
}

void MainWindow::onFolderComboBoxDoubleClicked()
{
    QString folderPath = ui->txtCollectionFolder->currentText();
    QString root = QtNoid::App::Settings::appExeOrAppBundleDirPath();
    folderPath = QFileDialog::getExistingDirectory(this, tr("Select the main folder"), root + QDir::separator() + folderPath);

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
}


void MainWindow::onPreviousTest()
{
    auto current = m_yamlTestCollectionListCurrent;
    setYamlTestCollectionListCurrent(--current);
    updateUI_loadYamlFile(m_yamlTestCollectionList.value(m_yamlTestCollectionListCurrent, {}));
    updateUI_progressBar();
    updateUI_txtCurrentFile();
    updateUI_cmdPrevNext();
    updateUI_statusBar();
    updateUI_convertYamlToJson();
}


void MainWindow::onNextTest()
{
    auto current = m_yamlTestCollectionListCurrent;
    setYamlTestCollectionListCurrent(++current);
    updateUI_loadYamlFile(m_yamlTestCollectionList.value(m_yamlTestCollectionListCurrent, {}));
    updateUI_progressBar();
    updateUI_txtCurrentFile();
    updateUI_cmdPrevNext();
    updateUI_statusBar();
    updateUI_convertYamlToJson();
}

void MainWindow::onCmdSaveTestDataset()
{
    frmSaveDataset dialog(this);
    QString root = QtNoid::App::Settings::appExeOrAppBundleDirPath();
    QString folderPath = root + QDir::separator() + ui->txtCollectionFolder->currentText();
    QString filePath = m_yamlTestCollectionList.value(m_yamlTestCollectionListCurrent, {});
    dialog.setBaseFolderPath(folderPath);
    dialog.setFilePath(filePath);
    if(dialog.exec() == 0) {
        // Request not accepted
        return;
    }


    QString outFolderPath = folderPath + QDir::separator() + dialog.destinationFolder() + QDir::separator();
    auto res = QDir().mkpath(outFolderPath);
    if(!res) {
        qDebug() << __func__ << "Error creating subfolder";
    }

    auto description = ui->txtDescription->text();
    QFile descriptionFile(outFolderPath + "===");
    if (descriptionFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&descriptionFile);
        out << description;
        descriptionFile.close();
    }

    auto inYaml = ui->txtYAML->toPlainText();
    QFile yamlFile(outFolderPath + "in.yaml");
    if (yamlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&yamlFile);
        out << inYaml;
        yamlFile.close();
    }

    auto inJson = ui->txtJsonExpected->toPlainText();
    QFile jsonFile(outFolderPath + "in.json");
    if (jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&jsonFile);
        out << inJson;
        jsonFile.close();
    }


    auto inErrors = ui->txtErrorExpected->toPlainText();
    QFile errorFile(outFolderPath + "error");
    if(inErrors.isEmpty()){
        errorFile.remove();
    }
    else {
        if (errorFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&errorFile);
            out << inErrors;
            errorFile.close();
        }
    }



    auto inTokens = ui->txtTokensExpected->toPlainText();
    QFile tokensFile(outFolderPath + "test.event");
    if (tokensFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&tokensFile);
        out << inTokens;
        tokensFile.close();
    }

}

void MainWindow::initAppConfigFile()
{
    QString appConfigFile = appConfig->fileName();
    if(QFile::exists(appConfigFile)){
        return;
    }
    QFile newFile("://resources/JsonYamlToJsonTestSuite.json");
    newFile.copy(appConfigFile);
    QFile::setPermissions(appConfigFile, QFile::ReadOwner | QFile::WriteOwner);
}


void MainWindow::updateUI_initDevelopment()
{
    // qDebug() << __func__;
#ifdef QT_NO_DEBUG
    // Hide the menù Development when in release
    QMenu* dev = ui->menuDevelopment;
    dev->menuAction()->setVisible(false);
    return;
#endif
    // Creare i 2 menu
    auto developmentMenu = ui->menuDevelopment;
    developmentMenu->addSeparator();

    QtNoid::App::Development::saveConfigToProject(developmentMenu, SOURCE_FILES_PATH);
    QtNoid::App::Development::initConfigFromResources(developmentMenu, ":/resources");
}



void MainWindow::onCreateNewTest()
{
    // qDebug() << __func__;

    frmNewDataset dialog(this);
    QString root = QtNoid::App::Settings::appExeOrAppBundleDirPath();
    auto rootFolder = QDir::cleanPath(root + QDir::separator() + ui->txtCollectionFolder->currentText());
    dialog.setRootFolder(rootFolder);
    if(dialog.exec() == 0) {
        // Request not accepted
        return;
    }

    // Update the main folder if it is new
    rootFolder = dialog.rootFolder();
    QDir dir(root);
    rootFolder = dir.relativeFilePath(rootFolder);

    // Add the folder to the comboBox if it's not already there
    // If this is new, this will trigger the update
    int index = ui->txtCollectionFolder->findText(rootFolder);
    if (index == -1) {
        ui->txtCollectionFolder->addItem(rootFolder);
        ui->txtCollectionFolder->setCurrentIndex(ui->txtCollectionFolder->count() - 1);
    } else {
        ui->txtCollectionFolder->setCurrentIndex(index);
    }

    // Create the new in.yaml file
    auto datasetFolder = dialog.rootFolder() + QDir::separator() + dialog.datasetFolder();
    QDir().mkdir(datasetFolder);
    // qDebug() << __func__ << datasetFolder <<  "accepted";

    QFile yamlFile(datasetFolder + "/in.yaml");
    if (yamlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // qDebug() << __func__ << "yamlFile file opened";
        QTextStream out(&yamlFile);
        out << dialog.yamlInput();
        yamlFile.close();
    }

    // Create an empty JSON file
    QFile jsonFile(datasetFolder + "/in.json");
    if (jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // qDebug() << __func__ << "jsonFile file opened";
        QTextStream out(&jsonFile);
        out << "";
        jsonFile.close();
    }


    // Create description file
    QFile descriptionFile(datasetFolder + "/===");
    if (descriptionFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // qDebug() << __func__ << "descriptionFile file opened";
        QTextStream out(&descriptionFile);
        out << dialog.description();
        descriptionFile.close();
    }

    // update the yaml file list
    ui->txtCurrentFile->blockSignals(true);
    updateUI_scanTestCollectionFolder(rootFolder);
    ui->txtCurrentFile->blockSignals(false);

    // select the new file
    // This automatically triggers the load of the file.
    dir.setPath(dialog.rootFolder());
    auto yamlFilePath = dir.relativeFilePath(yamlFile.fileName());
    ui->txtCurrentFile->setCurrentText(yamlFilePath);
    // qDebug() << __func__ << yamlFilePath;



    // QString outFolderPath = folderPath + QDir::separator() + dialog.destinationFolder() + QDir::separator();
    // auto res = QDir().mkpath(outFolderPath);
    // if(!res) {
    //     qDebug() << __func__ << "Error creating subfolder";
    // }

    // auto description = ui->txtDescription->text();
    // QFile descriptionFile(outFolderPath + "===");
    // if (descriptionFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    //     QTextStream out(&descriptionFile);
    //     out << description;
    //     descriptionFile.close();
    // }

    // auto inYaml = ui->txtYAML->toPlainText();
    // QFile yamlFile(outFolderPath + "in.yaml");
    // if (yamlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    //     QTextStream out(&yamlFile);
    //     out << inYaml;
    //     yamlFile.close();
    // }

    // auto inJson = ui->txtJsonExpected->toPlainText();
    // QFile jsonFile(outFolderPath + "in.json");
    // if (jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    //     QTextStream out(&jsonFile);
    //     out << inJson;
    //     jsonFile.close();
    // }


    // auto inErrors = ui->txtErrorExpected->toPlainText();
    // QFile errorFile(outFolderPath + "error");
    // if(inErrors.isEmpty()){
    //     errorFile.remove();
    // }
    // else {
    //     if (errorFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    //         QTextStream out(&errorFile);
    //         out << inErrors;
    //         errorFile.close();
    //     }
    // }



    // auto inTokens = ui->txtTokensExpected->toPlainText();
    // QFile tokensFile(outFolderPath + "test.event");
    // if (tokensFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    //     QTextStream out(&tokensFile);
    //     out << inTokens;
    //     tokensFile.close();
    // }


}

