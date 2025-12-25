#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "recentfilesmanager.h"
#include "frmsavedataset.h"
#include "frmnewdataset.h"
#include "frmmanageprojectresources.h"
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
#include <QStringLiteral>
#include <QTextBrowser>


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
    connect(m_recentFilesManager, &recentFilesManager::fileSelected, this, [this](const QString& absoluteYamlFilePath){
        updateUI_loadYamlFile(absoluteYamlFilePath);
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
        updateUI_scanTestCollectionFolder(collectionFolderAbsolutePath());
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
    updateUI_scanTestCollectionFolder(collectionFolderAbsolutePath());

    // Last selected file
    setYamlTestCollectionListCurrent(appConfig->restoreAsInt("yamlTestCollectionListCurrent", 0));
    updateUI_loadYamlFile(m_yamlTestCollectionList.value(m_yamlTestCollectionListCurrent, {}));
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
    m_yamlTestCollectionListCurrent = 0;
    m_yamlTestCollectionList.clear();
    ui->txtCurrentFile->clear();

    if(folder.isEmpty()) {
        return;
    }

    m_yamlTestCollectionList = QtNoid::Common::File::listSubPathRecursively(folder, {"in.yaml"});
    for(const QString &file : std::as_const(m_yamlTestCollectionList)) {
        ui->txtCurrentFile->addItem(file);
    }
}

void MainWindow::updateUI_progressBar()
{
    // qDebug() << __func__;
    ui->cboCollectionProgress->setVisible(!m_yamlTestCollectionList.isEmpty());
    ui->cboCollectionProgress->setMinimum(0);
    ui->cboCollectionProgress->setMaximum(m_yamlTestCollectionList.count());
    ui->cboCollectionProgress->setValue(m_yamlTestCollectionListCurrent + 1);
    QString msg = QString("%1/%2").arg(m_yamlTestCollectionListCurrent+1).arg(m_yamlTestCollectionList.count());
    ui->cboCollectionProgress->setToolTip(msg);
}

void MainWindow::updateUI_cmdPrevNext()
{
    // qDebug() << __func__;
    // Set cmdPrevious enabled if I am not
    bool enabled = (!m_yamlTestCollectionList.isEmpty()) && (m_yamlTestCollectionListCurrent >0);
    ui->cmdPrevious->setEnabled(enabled);

    enabled = (!m_yamlTestCollectionList.isEmpty()) && (m_yamlTestCollectionListCurrent+1 < m_yamlTestCollectionList.count());
    ui->cmdNext->setEnabled(enabled);
}

void MainWindow::updateUI_txtCurrentFile()
{
    // qDebug() << __func__ << m_yamlTestCollectionListCurrent << m_yamlTestCollectionList.count();
    ui->txtCurrentFile->blockSignals(true);
    ui->txtCurrentFile->setCurrentIndex(m_yamlTestCollectionListCurrent);
    ui->txtCurrentFile->blockSignals(false);
}

void MainWindow::updateUI_statusBar(const QString& msg)
{
    // qDebug() << __func__ << msg;
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
        // qDebug() << __func__ << sbMsg;
        ui->statusbar->showMessage(sbMsg);
        return;
    }    
}

void MainWindow::on_actionLoadYAML_triggered()
{
    QString currentPath = collectionFolderAbsolutePath();
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Load YAML File"), currentPath,
        tr("YAML Files (*.yml *.yaml);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    if(fileName.contains(currentPath)) {
        // Is new file part of current collection?
        auto relPath = QDir(currentPath).relativeFilePath(fileName);
        if(m_yamlTestCollectionList.contains(relPath)) {
            ui->txtCurrentFile->setCurrentText(relPath);
        }
        return;
    }

    // This is a different folder, so disable the collection
    setCollectionFolder({});
    ui->txtCurrentFile->clear();
    m_yamlTestCollectionList.clear();

    updateUI_loadYamlFile(fileName);
    updateUI_convertYamlToJson();
    updateUI_recentFiles(fileName);
}

void MainWindow::on_actionTestSuite_2022_01_17_triggered()
{
    // :/TestData/yaml-test-suite/data-2022-01-17/2AUY/===
    if(generateTestDataFromResource(":/TestData/yaml-test-suite/data-2022-01-17/"))
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
    // :/TestData/Txt2Json-test-suite/QtNoid-2.2.0/Array-001/===
    if(generateTestDataFromResource("://TestData/Txt2Json-test-suite/QtNoid-2.2.0/"))
    {
        QMessageBox::information(this, "Txt2Json Test Suite QtNoid_2_2_0",
                                 tr("Test data generated successfully!"));
    }
    else {
        QMessageBox::information(this, "Txt2Json Test Suite QtNoid_2_2_0",
                                 tr("Error generating test data"));
    }
}

bool MainWindow::generateTestDataFromResource(const QString &dataPrefix)
{
    // Destination path is in CWD + dataPrefix
    QString baseOutputPath = dataPrefix;
    baseOutputPath.remove(":");
    baseOutputPath = QtNoid::App::Settings::appExeOrAppBundleDirPath() + baseOutputPath;

    QDir dir;
    dir.mkpath(baseOutputPath);
    // qDebug() << __func__ << baseOutputPath;

    int fileCount = 0;
    int errorCount = 0;

    QtNoid::Common::File QtNoidFile;
    auto testDataFiles = QtNoidFile.listPathRecursively(dataPrefix);
    int dataDirLength = dataPrefix.length();
    // qDebug() << __func__ << testDataFiles;

    // "://TestData/Txt2Json-test-suite/QtNoid-2.2.0/Array-001/==="
    for (const QString &srcPath : std::as_const(testDataFiles)) {
        // qDebug() << srcPath.sliced(dataDirLength) << dataPrefix;
        QFile srcFile(srcPath);
        QFileInfo FI(baseOutputPath + srcPath.sliced(dataDirLength));
        dir.mkpath(FI.absolutePath());
        auto dstPath = FI.absoluteFilePath();
        auto res = srcFile.copy(dstPath);
        if(!res) {
            errorCount++;
        }
        else {
            QFile::setPermissions(dstPath, QFile::ReadOwner | QFile::WriteOwner);
        }
        fileCount++;
    }

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
    QString appPath = QtNoid::App::Settings::appExeOrAppBundleDirPath() ;

    auto res = QDesktopServices::openUrl(QUrl::fromLocalFile(appPath + "/TestData"));
    if(res) return;

    // If fails, let's open the application folder
    QDesktopServices::openUrl(QUrl::fromLocalFile(appPath));
}

void MainWindow::on_actionManageProjectResources_triggered()
{
    frmManageProjectResources dialog(this);
    dialog.setDestinationFolder(SOURCE_FILES_PATH "/resources");
    dialog.setDestinationResourceFile("JsonYamlToJsonTestSuite.qrc");
    dialog.setSourceBaseFolder(QtNoid::App::Settings::appExeOrAppBundleDirPath());
    dialog.setSourceSubFolder(ui->txtCollectionFolder->currentText());

    // Restore Geometry
    auto pageName = QtNoid::App::Settings::groupNameFromObjectOrClass(dialog);
    dialog.restoreGeometry(appConfig->restoreAsByteArray(
        "Geometry", dialog.saveGeometry(), pageName));
    dialog.setProjectResPrefix(appConfig->restoreAsString(
        "LastPrefix", dialog.projectResPrefix(), pageName));
    dialog.exec();

    appConfig->saveValue("Geometry", dialog.saveGeometry(), pageName);
    appConfig->saveValue("LastPrefix", dialog.projectResPrefix(), pageName);
}

void MainWindow::on_actionConvert_To_Json_triggered()
{
    updateUI_convertYamlToJson();
}

void MainWindow::on_actionSave_Dataset_triggered()
{
    onCmdSaveTestDataset();
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


void MainWindow::updateUI_loadYamlFile(const QString &yamlFilePath)
{
    // qDebug() << __func__ << yamlFilePath;

    QString absoluteYamlFilePath;
    QString yamlContent;
    do{
        if(yamlFilePath.isEmpty()) {
            break;
        }

        absoluteYamlFilePath = yamlFilePath;
        if(QDir::isRelativePath(yamlFilePath)){
            absoluteYamlFilePath = collectionFolderAbsolutePath()
                               + QDir::separator() + yamlFilePath;
        }

        QFile file(absoluteYamlFilePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            break;
        }
        QTextStream in(&file);
        yamlContent = in.readAll();
        file.close();
    } while(0);

    ui->txtYAML->clear();
    ui->txtYAML->setToolTip({});
    ui->txtJsonExpected->clear();
    ui->txtJsonExpected->setToolTip({});
    ui->txtTokensExpected->clear();
    ui->txtTokensExpected->setToolTip({});
    ui->txtErrorExpected->clear();
    ui->txtErrorExpected->setToolTip({});
    ui->txtDescription->clear();
    ui->txtDescription->setToolTip("Example description");

    if(yamlContent.isEmpty()){
        // No YAML no party
        return;
    }

    ui->txtYAML->setPlainText(yamlContent);
    QDir baseDir(collectionFolderAbsolutePath());
    auto tooltipPath = baseDir.relativeFilePath(absoluteYamlFilePath);
    ui->txtYAML->setToolTip(tooltipPath);
    ui->tabWidgetInput->setToolTip(tooltipPath);

    // Look for corresponding files
    QFileInfo FI(absoluteYamlFilePath);
    QString jsonFilePath = FI.absolutePath() + QDir::separator() +
                           FI.baseName() + ".json";
    QString errorFilePath = FI.absolutePath() + QDir::separator() +
                            "error";
    QString descriptionFilePath = FI.absolutePath() + QDir::separator() +
                                  "===";
    QString tokensFilePath = FI.absolutePath() + QDir::separator() +
                             "test.event";

    // Look for "===" description file
    QFile descriptionFile(descriptionFilePath);
    tooltipPath = baseDir.relativeFilePath(descriptionFilePath);
    ui->txtDescription->setToolTip(tooltipPath);
    if (descriptionFile.exists() && descriptionFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream descIn(&descriptionFile);
        QString descContent = descIn.readAll();
        descriptionFile.close();
        ui->txtDescription->setText(descContent);
    }

    // Load Expected JSON file
    QFile jsonFile(jsonFilePath);
    tooltipPath = baseDir.relativeFilePath(jsonFilePath);
    ui->txtJsonExpected->setToolTip(tooltipPath);
    if (jsonFile.exists() && jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream jsonIn(&jsonFile);
        QString jsonContent = jsonIn.readAll();
        jsonFile.close();
        ui->txtJsonExpected->setPlainText(jsonContent);
    }

    // Look for "error" file in the same folder
    QFile errorFile(errorFilePath);
    tooltipPath = baseDir.relativeFilePath(errorFilePath);
    ui->txtErrorExpected->setToolTip(tooltipPath);
    if (errorFile.exists() &&  errorFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream errorIn(&errorFile);
        QString errorContent = errorFile.readAll();
        errorFile.close();
        if(errorContent.isEmpty()){
            errorContent = "Error file is present but it is empty";
        }
        ui->txtErrorExpected->setPlainText(errorContent);
    }

    // Look for "test.event" tokens file
    QFile tokensFile(tokensFilePath);
    tooltipPath = baseDir.relativeFilePath(tokensFilePath);
    ui->txtTokensExpected->setToolTip(tooltipPath);
    if (tokensFile.exists() && tokensFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream tokensIn(&tokensFile);
        QString tokensContent = tokensIn.readAll();
        tokensFile.close();
        ui->txtTokensExpected->setPlainText(tokensContent);
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
    qDebug() << __func__ ;

    QString folderPath = collectionFolderAbsolutePath();
    folderPath = QFileDialog::getExistingDirectory(this, tr("Select the main folder"), folderPath);

    if (folderPath.isEmpty())
        return;

    setCollectionFolder(folderPath);
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
    dialog.setCollectionFolder(ui->txtCollectionFolder->currentText());
    QString yamlFilePath = m_yamlTestCollectionList.value(m_yamlTestCollectionListCurrent, {});
    if(!yamlFilePath.isEmpty()){
        QFileInfo fileInfo(yamlFilePath);
        yamlFilePath = fileInfo.path();
    }
    dialog.setDatasetFolder(yamlFilePath);

    if(dialog.exec() == 0) {
        // Request not accepted
        return;
    }
    auto collectionFolder = dialog.collectionFolder();


    QString outFolderPath(QDir::cleanPath(collectionFolder + QDir::separator() + dialog.datasetFolder()) + "/");
    // qDebug() << __func__ << outFolderPath;

    auto res = QDir().mkpath(outFolderPath);
    if(!res) {
        qDebug() << __func__ << "Error creating subfolder" << outFolderPath;
    }

    if(dialog.saveDescription()) {
        auto description = ui->txtDescription->text();
        QFile descriptionFile(outFolderPath + "===");
        if (descriptionFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&descriptionFile);
            out << description;
            descriptionFile.close();
        }
        else {
            qDebug() << __func__ << "Error saving" << descriptionFile.fileName();
        }
    }

    if(dialog.saveInputYAML()){
        auto inYaml = ui->txtYAML->toPlainText();
        QFile yamlFile(outFolderPath + "in.yaml");
        if (yamlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&yamlFile);
            out << inYaml;
            yamlFile.close();
        }
        else {
            qDebug() << __func__ << "Error saving" << yamlFile.fileName();
        }
    }

    if(dialog.saveExpectedJSON()) {
        auto inJson = ui->txtJsonExpected->toPlainText();
        QFile jsonFile(outFolderPath + "in.json");
        if (jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&jsonFile);
            out << inJson;
            jsonFile.close();
        }
        else {
            qDebug() << __func__ << "Error saving" << jsonFile.fileName();
        }
    }

    if(dialog.saveErrorEmptyFile()) {
        auto inErrors = ui->txtErrorExpected->toPlainText();
        QFile errorFile(outFolderPath + "error");
        if(inErrors.isEmpty()){
            auto res = errorFile.remove();
            // qDebug() << __func__ << res;
        }
        else {
            if (errorFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&errorFile);
                out << inErrors;
                errorFile.close();
            }
            else {
                qDebug() << __func__ << "Error saving" << errorFile.fileName();
            }
        }
    }

    if(dialog.saveExpectedTokens()) {
        auto inTokens = ui->txtTokensExpected->toPlainText();
        QFile tokensFile(outFolderPath + "test.event");
        if (tokensFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&tokensFile);
            out << inTokens;
            tokensFile.close();
        }
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
    QtNoid::App::Development::initConfigFromResources(developmentMenu, ":/");
}



void MainWindow::onCreateNewTest()
{
    qDebug() << __func__ << "DA FINIRE";

    frmNewDataset dialog(this);
    QString root = QtNoid::App::Settings::appExeOrAppBundleDirPath();
    auto rootFolder = collectionFolderAbsolutePath();
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

}



void MainWindow::on_actionOpen_JsonYamlToJsonTestSuite_md_triggered()
{
    QDialog dialog(this);
    QString fileName = qAppName() + ".md";
    dialog.setWindowTitle(this->windowTitle() + ": " +  fileName);
    dialog.resize(800, 600);
    dialog.restoreGeometry(appConfig->restoreAsByteArray("Geometry", dialog.saveGeometry(), "MdViewer"));


    auto *browser = new QTextBrowser(&dialog);
    // open link in the same text browser
    browser->setOpenExternalLinks(true);

    auto *layout = new QVBoxLayout(&dialog);
    layout->addWidget(browser);

    QString path = QStringLiteral(SOURCE_FILES_PATH  "/doc/");
    QFile file(path + fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Unable to open: " + path);
        return;
    }
    QString markdown = QString::fromUtf8(file.readAll());
    browser->setMarkdown(markdown);

    dialog.exec();

    appConfig->saveValue("Geometry", dialog.saveGeometry(), "MdViewer");

    dialog.saveGeometry();

}

void MainWindow::setCollectionFolder(const QString &absolutePath)
{
    if(absolutePath.isEmpty()) {
        int index = ui->txtCollectionFolder->findText({});
        if (index == -1) {
            ui->txtCollectionFolder->addItem({});
        }
        ui->txtCollectionFolder->setCurrentText({});
        return;
    }

    // Remove root
    QString root = QtNoid::App::Settings::appExeOrAppBundleDirPath();
    auto folderPath = QDir(root).relativeFilePath(absolutePath);

    // Add the folder to the comboBox if it's not already there
    int index = ui->txtCollectionFolder->findText(folderPath);

    if (index == -1) {
        ui->txtCollectionFolder->addItem(folderPath);
        ui->txtCollectionFolder->setCurrentIndex(ui->txtCollectionFolder->count() - 1);
    } else {
        ui->txtCollectionFolder->setCurrentIndex(index);
    }
}

QString MainWindow::collectionFolderAbsolutePath() const
{
    auto relPath = ui->txtCollectionFolder->currentText();
    if(relPath.isEmpty()) {
        return {};
    }
    auto resPath = QDir::cleanPath(QtNoid::App::Settings::appExeOrAppBundleDirPath() +
                                   QDir::separator() + relPath);

    return resPath;
}

