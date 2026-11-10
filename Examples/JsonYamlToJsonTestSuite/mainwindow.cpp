#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "frmsavedataset.h"
#include "frmnewdataset.h"
#include "frmmanageprojectresources.h"
#include "QtNoidJson/QtNoidJson"
#include "QtNoidCommon/QtNoidCommon"
#include "QtNoidApp/QtNoidApp"
#include "QtNoidUiCommon/QtNoidUiCommon"
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
#include <QSaveFile>
#include <QStringLiteral>
#include <QTextBrowser>






MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_screenshotShortcut = QtNoid::App::Development::initFullDialogGrabShortcut(this);
    setWindowTitle("QtNoid::Json::YamlToJson Test Suite");

    customizeUI();
    customizeUI_initDevelopment();

    initAppConfigFile();
    restoreFromAppConfig();

    m_recentFiles.setAction(ui->actionRecent_Files);
    connect(&m_recentFiles, &QtNoid::UiCommon::RecentFiles::fileSelected, this, [&](const QString& filePath){
        if(!confirmDiscardYaml()) {
            return;
        }
        setSingleFilePath(filePath);
        setCurrentSourceWidget(ui->tabSingleFile);
    });

    connect(ui->txtYAML->document(), &QTextDocument::modificationChanged, [&](){
        //qDebug() << "QTextDocument::modificationChanged: " << changed;
        updateUI_tabInputTitle();
    });

    connect(ui->tabWidgetSource, &QTabWidget::currentChanged, [&](){
        tabWidgetSourceUiUpdate();
    });

    updateUI_recentFiles(QString());
    updateUI_txtCurrentFile();
    updateUI_cmdPrevNext();

    // Connect double-click signal from folder comboBox
    connect(ui->txtCollectionFolder, &FolderComboBox::doubleClicked, this, &MainWindow::onFolderComboBoxDoubleClicked);
    connect(ui->txtCollectionFolder, &FolderComboBox::currentTextChanged, this, [&](const QString& folder){
        if(!confirmDiscardYaml()) {
            return;
        }
        updateUI_scanTestCollectionFolder(collectionFolderPath());
        updateUI_progressBar();
        updateUI_txtCurrentFile();
        updateUI_cmdPrevNext();
        const QString yamlFilePath = yamlFullPathFromCollectionList();
        updateUI_loadYamlFile(yamlFilePath);
        updateUI_loadExpectedFiles(yamlFilePath);
        updateUI_convertYamlToJson();
        updateUI_statusBar(yamlStatusBarMsgFromCollectionList());
    });

    connect(ui->txtCurrentFile, &QComboBox::currentIndexChanged, this, [&](int index){
        // QString folder = ui->txtCollectionFolder->currentText();
        // QString root = QtNoid::App::Core::appExeOrAppBundleDirPath();
        if(!confirmDiscardYaml()) {
            return;
        }
        m_yamlTestCollectionListCurrent = index;
        updateUI_progressBar();
        updateUI_txtCurrentFile();
        updateUI_cmdPrevNext();
        const QString yamlFilePath = yamlFullPathFromCollectionList();
        updateUI_loadYamlFile(yamlFilePath);
        updateUI_loadExpectedFiles(yamlFilePath);
        updateUI_convertYamlToJson();
        updateUI_statusBar(yamlStatusBarMsgFromCollectionList());
    });

    connect(ui->cmdBrowseFolder, &QToolButton::clicked, this, &MainWindow::onFolderComboBoxDoubleClicked);
    connect(ui->cmdNext, &QPushButton::clicked, this, &MainWindow::onNextTest);
    connect(ui->cmdPrevious, &QPushButton::clicked, this, &MainWindow::onPreviousTest);
    connect(ui->cmdSaveTestDataset, &QPushButton::clicked, this, &MainWindow::onCmdSaveTestDataset);
    connect(ui->cmdNewTestDataset, &QPushButton::clicked, this, &MainWindow::onCreateNewTest);
    connect(ui->actionCreate_New_Test, &QAction::triggered, this, &MainWindow::onCreateNewTest);

}

MainWindow::~MainWindow()
{
    const QString pageName = QtNoid::App::Core::groupNameFromObjectOrClass(this);
    appConfig->setName(windowTitle());
    appConfig->saveValue("geometry", saveGeometry(), pageName);
    appConfig->saveValue("splitter", ui->splitter->saveState(), pageName);
    auto comboJ = QtNoid::UiCommon::Core::saveComboBoxTextItems(ui->txtCollectionFolder);
    appConfig->saveValue("collectionFolderList", comboJ, pageName);
    appConfig->saveValue("yamlTestCollectionListCurrent", m_yamlTestCollectionListCurrent, pageName);
    appConfig->saveValue("sourceMode", ui->tabWidgetSource->currentWidget()->objectName(), pageName);
    appConfig->saveValue("singleFilePath", ui->txtSingleFile->text(), pageName);
    appConfig->saveValue("recentFiles", m_recentFiles.files(), pageName);
    delete ui;
}

void MainWindow::restoreFromAppConfig()
{
    // qDebug() << Q_FUNC_INFO;
    const QString pageName = QtNoid::App::Core::groupNameFromObjectOrClass(this);
    restoreGeometry(appConfig->restoreAsByteArray("geometry", saveGeometry(), pageName));
    ui->splitter->restoreState(appConfig->restoreAsByteArray("splitter", ui->splitter->saveState(), pageName));

    // Load single file collection
    ui->txtSingleFile->setText(appConfig->restoreAsString("singleFilePath", {}, pageName));
    m_recentFiles.setFiles(appConfig->restoreAsStringList("recentFiles", {}, pageName));

    // Load last collection configuration
    auto comboJ = appConfig->restoreAsJson("collectionFolderList", {}, pageName);
    QtNoid::UiCommon::Core::restoreComboBoxTextItems(ui->txtCollectionFolder, comboJ);
    updateUI_scanTestCollectionFolder(collectionFolderPath());
    setYamlTestCollectionListCurrent(appConfig->restoreAsQSizeType("yamlTestCollectionListCurrent", 0, pageName));

    // Restore tabWidgetSource configuration
    const QString mode = appConfig->restoreAsString("sourceMode", "tabSingleFile", pageName);
    if (auto page = ui->tabWidgetSource->findChild<QWidget*>(mode)) {
        ui->tabWidgetSource->setCurrentWidget(page);
    }
    tabWidgetSourceUiUpdate();
}



void MainWindow::customizeUI()
{
    // ui->cmdPrevious->setText("\u2B05");
    // ui->cmdNext->setText("\u27a1\ufe0f"); // ➡️
    auto save_icon = style()->standardIcon(QStyle::SP_DialogSaveButton);
    ui->cmdSaveTestDataset->setIcon(save_icon);
    ui->cmdSaveTestDataset->setIconSize({16,16});
    ui->cmdSaveSingleFile->setIcon(save_icon);
    ui->cmdSaveSingleFile->setIconSize({16,16});

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

    ui->tabWidgetSource->installEventFilter(this);
    ui->tabWidgetSource->tabBar()->installEventFilter(this);


}

void MainWindow::customizeUI_initDevelopment()
{
    // qDebug() << __func__;
#ifdef QT_NO_DEBUG
    // Hide the menù Development when in release
    QMenu* dev = ui->menuDevelopment;
    dev->menuAction()->setVisible(false);
    return;
#else
    auto developmentMenu = ui->menuDevelopment;
    developmentMenu->addSeparator();
    QtNoid::App::Development::saveConfigToProject(developmentMenu, SOURCE_FILES_PATH);
    QtNoid::App::Development::initConfigFromResources(developmentMenu, ":/");

    // Update the configuration if the config file is reloaded.
    connect(appConfig, &QtNoid::App::ConfigFile::fileLoaded, this, [&](const QString& fName){
        qDebug() << "File Loaded" << Q_FUNC_INFO;
        restoreFromAppConfig();
    });

#endif


}


void MainWindow::tabWidgetSourceUiUpdate()
{
    if(ui->tabWidgetSource->currentWidget() == ui->tabSingleFile) {
        ui->verticalLayoutWidget->setVisible(false);
        QString singlePath = singleFilePath();
        updateUI_loadYamlFile(singlePath);
        updateUI_convertYamlToJson();
        updateUI_statusBar(singlePath);
        qDebug() << __func__ << "isModified:" << ui->txtYAML->document()->isModified();
    }
    else {
        ui->verticalLayoutWidget->setVisible(true);
        QString fullPath = yamlFullPathFromCollectionList();
        updateUI_loadYamlFile(fullPath);
        updateUI_convertYamlToJson();
        updateUI_loadExpectedFiles(fullPath);
        updateUI_progressBar();
        updateUI_statusBar(yamlStatusBarMsgFromCollectionList());
    }
}


void MainWindow::fileOpenSingleFile()
{
    QString currentPath = singleFilePath();
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load YAML File"), currentPath,
                                                    tr("YAML Files (*.yml *.yaml);;All Files (*)"));

    if (fileName.isEmpty())
        return;

    setSingleFilePath(fileName);
    setCurrentSourceWidget(ui->tabSingleFile);

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
    m_recentFiles.addFile(fileName);
}


void MainWindow::updateUI_scanTestCollectionFolder(const QString& folder)
{
    m_yamlTestCollectionListCurrent = 0;
    m_yamlTestCollectionList.clear();
    ui->txtCurrentFile->clear();

    if(folder.isEmpty()) {
        return;
    }
    m_yamlTestCollectionList = QtNoid::Common::File::listSubPathRecursively(folder, {"in.yaml"});
    ui->txtCurrentFile->addItems(m_yamlTestCollectionList);
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
}

void MainWindow::updateUI_tabInputTitle()
{
    bool changed = ui->txtYAML->document()->isModified();
    QString current = ui->tabWidgetInput->tabText(0);
    current = current.replace("[*]", "").simplified();
    if(changed) {
        // tabInputYAML->windowTitle();
        current += " [*]";
        ui->tabWidgetInput->setTabText(0, current);
    }
    else {
        ui->tabWidgetInput->setTabText(0, current.simplified());
    }
}

void MainWindow::on_actionLoadYAML_triggered()
{
    // qDebug()<< __func__;
    if(!confirmDiscardYaml()) {
        return;
    }
    fileOpenSingleFile();
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

bool MainWindow::generateTestDataFromResource(const QString& dataPrefix)
{
    // Destination path is in CWD + dataPrefix
    QString baseOutputPath = dataPrefix;
    baseOutputPath.remove(":");
    baseOutputPath = QtNoid::App::Core::appExeOrAppBundleDirPath() + baseOutputPath;

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
    for (const QString& srcPath : std::as_const(testDataFiles)) {
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
    QString appPath = QtNoid::App::Core::appExeOrAppBundleDirPath() ;

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
    dialog.setSourceBaseFolder(QtNoid::App::Core::appExeOrAppBundleDirPath());
    dialog.setSourceSubFolder(ui->txtCollectionFolder->currentText());

    // Restore Geometry
    auto pageName = QtNoid::App::Core::groupNameFromObjectOrClass(dialog);
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

void MainWindow::setYamlTestCollectionListCurrent(qsizetype newYamlTestCollectionListCurrent)
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


void MainWindow::updateUI_loadYamlFile(const QString& yamlFilePath)
{
    qDebug() << __func__ << yamlFilePath;

    const QSignalBlocker blocker(ui->txtYAML->document());   // si sblocca a fine blocco

    ui->txtYAML->setToolTip({});
    if(!yamlFilePath.isEmpty()) {
        QString yamlContent = QtNoid::Common::File::readAsString(yamlFilePath);
        ui->txtYAML->setPlainText(yamlContent);
        auto tooltipPath = yamlFilePath;
        ui->txtYAML->setToolTip(tooltipPath);
        ui->tabWidgetInput->setToolTip(tooltipPath);
    }
    else {
        ui->txtYAML->clear();
    }
    ui->txtYAML->document()->setModified(false);
    updateUI_tabInputTitle();      // signals blocked - title need manual update

    return;
}

void MainWindow::updateUI_loadExpectedFiles(const QString& yamlFilePath)
{
    // qDebug() << __func__ << yamlFilePath;

    if(yamlFilePath.isEmpty())
        return;

    ui->txtJsonExpected->clear();
    ui->txtJsonExpected->setToolTip({});
    ui->txtTokensExpected->clear();
    ui->txtTokensExpected->setToolTip({});
    ui->txtErrorExpected->clear();
    ui->txtErrorExpected->setToolTip({});
    ui->txtDescription->clear();
    ui->txtDescription->setToolTip("Example description...");

    QDir baseDir(collectionFolderPath());
    QString tooltipPath;

    // Look for corresponding files
    QFileInfo FI(yamlFilePath);
    QString jsonFilePath = FI.absolutePath() + QDir::separator() +
                           FI.baseName() + ".json";
    QString errorFilePath = FI.absolutePath() + QDir::separator() +
                            "error";
    QString descriptionFilePath = FI.absolutePath() + QDir::separator() +
                                  "===";
    QString tokensFilePath = FI.absolutePath() + QDir::separator() +
                             "test.event";

    // Look for "===" description file
    tooltipPath = baseDir.relativeFilePath(descriptionFilePath);
    ui->txtDescription->setToolTip(tooltipPath);
    ui->txtDescription->setText(
        QtNoid::Common::File::readAsString(descriptionFilePath));

    // Load Expected JSON file
    tooltipPath = baseDir.relativeFilePath(jsonFilePath);
    ui->txtJsonExpected->setPlainText(
        QtNoid::Common::File::readAsString(jsonFilePath));
    ui->tabWidgetExpected->setCurrentWidget(ui->tabJsonExpected);

    // Look for "error" file in the same folder
    tooltipPath = baseDir.relativeFilePath(errorFilePath);
    QFile errorFile(errorFilePath);
    ui->txtErrorExpected->setToolTip(tooltipPath);
    if (errorFile.exists() &&  errorFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream errorIn(&errorFile);
        QString errorContent = errorFile.readAll();
        errorFile.close();
        if(errorContent.isEmpty()){
            errorContent = "Error file is present but it is empty";
        }
        ui->txtErrorExpected->setPlainText(errorContent);
        ui->tabWidgetExpected->setCurrentWidget(ui->tabErrorExpected);
    }

    // Look for "test.event" tokens file
    tooltipPath = baseDir.relativeFilePath(tokensFilePath);
    ui->txtTokensExpected->setPlainText(
        QtNoid::Common::File::readAsString(tokensFilePath));

    return;
}


void MainWindow::updateUI_convertYamlToJson()
{
    QString yamlText = ui->txtYAML->toPlainText();
    auto converter = QtNoid::Json::Yaml2Json(yamlText, this);
    ui->txtJsonOutput->setPlainText(converter.jsonAsString());
    ui->txtErrorOutput->setPlainText(converter.errorString());

    if(converter.errorString().isEmpty()) {
        ui->tabWidgetOutput->setCurrentWidget(ui->tabOutputJson);
    }
    else {
        ui->tabWidgetOutput->setCurrentWidget(ui->tabOutputErrors);
    }
}

void MainWindow::onFolderComboBoxDoubleClicked()
{
    // qDebug() << Q_FUNC_INFO;
    if(!confirmDiscardYaml()) {
        return;
    }
    // Prevent additional request
    ui->txtYAML->document()->setModified(false);

    QString folderPath = collectionFolderPath();
    folderPath = QFileDialog::getExistingDirectory(this, tr("Select the main folder"), folderPath);

    if (folderPath.isEmpty())
        return;

    setCollectionFolderPath(folderPath);
}


void MainWindow::onPreviousTest()
{
    if(!confirmDiscardYaml()) {
        return;
    }

    auto current = m_yamlTestCollectionListCurrent;
    setYamlTestCollectionListCurrent(--current);

    const QString yamlFilePath = yamlFullPathFromCollectionList();
    updateUI_loadYamlFile(yamlFilePath);
    updateUI_loadExpectedFiles(yamlFilePath);
    updateUI_progressBar();
    updateUI_txtCurrentFile();
    updateUI_cmdPrevNext();
    updateUI_statusBar(yamlStatusBarMsgFromCollectionList());
    updateUI_convertYamlToJson();
}


void MainWindow::onNextTest()
{
    if(!confirmDiscardYaml()) {
        return;
    }

    auto current = m_yamlTestCollectionListCurrent;
    setYamlTestCollectionListCurrent(++current);

    const QString yamlFilePath = yamlFullPathFromCollectionList();
    updateUI_loadYamlFile(yamlFilePath);
    updateUI_loadExpectedFiles(yamlFilePath);
    updateUI_progressBar();
    updateUI_txtCurrentFile();
    updateUI_cmdPrevNext();
    updateUI_statusBar(yamlStatusBarMsgFromCollectionList());
    updateUI_convertYamlToJson();
}

void MainWindow::onCmdSaveTestDataset()
{
    frmSaveDataset dialog(this);
    dialog.setCollectionFolder(ui->txtCollectionFolder->currentText());
    QString yamlFilePath = yamlFullPathFromCollectionList();
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
    auto res = appConfig->load();
    qDebug() << Q_FUNC_INFO << "appConfigFile: " << appConfigFile << "res: " << res;
}





void MainWindow::onCreateNewTest()
{
    qDebug() << __func__ << "DA FINIRE";

    frmNewDataset dialog(this);
    QString root = QtNoid::App::Core::appExeOrAppBundleDirPath();
    auto rootFolder = collectionFolderPath();
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
    QString fileName = QtNoid::Common::Text::convertToCamelCase(windowTitle()) + ".md";;
    dialog.setWindowTitle(this->windowTitle() + ": " +  fileName);
    dialog.resize(800, 600);
    dialog.restoreGeometry(appConfig->restoreAsByteArray("geometry", dialog.saveGeometry(), "MdViewer"));

    auto* browser = new QTextBrowser(&dialog);
    auto* layout = new QVBoxLayout(&dialog);
    layout->addWidget(browser);

    QString path = QStringLiteral(SOURCE_FILES_PATH  "/doc/");
    QFile file(path + fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Unable to open: " + path);
        return;
    }
    QString markdown = QString::fromUtf8(file.readAll());
    browser->setMarkdown(markdown);
    browser->setOpenExternalLinks(false);
    browser->setOpenLinks(false);

    dialog.exec();

    appConfig->saveValue("geometry", dialog.saveGeometry(), "MdViewer");

    dialog.saveGeometry();

}

void MainWindow::setCollectionFolderPath(const QString& path)
{
    if(path.isEmpty()) {  // Set an empty path
        int index = ui->txtCollectionFolder->findText({});
        if (index == -1) {
            ui->txtCollectionFolder->addItem({});
        }
        ui->txtCollectionFolder->setCurrentText({});
        return;
    }

    // Remove root
    QString root = QtNoid::App::Core::appExeOrAppBundleDirPath();
    auto folderPath = QDir(root).relativeFilePath(path);



    // Add the folder to the comboBox if it's not already there
    int index = ui->txtCollectionFolder->findText(folderPath);

    if (index == -1) {
        ui->txtCollectionFolder->addItem(folderPath);
        ui->txtCollectionFolder->setCurrentIndex(ui->txtCollectionFolder->count() - 1);
    } else {
        ui->txtCollectionFolder->setCurrentIndex(index);
    }
}

QString MainWindow::collectionFolderPath() const
{
    auto relPath = ui->txtCollectionFolder->currentText();
    if(relPath.isEmpty()) {
        return {};
    }
    auto resPath = QDir::cleanPath(QtNoid::App::Core::appExeOrAppBundleDirPath() +
                                   QDir::separator() + relPath);

    return resPath;
}

void MainWindow::setSingleFilePath(const QString& path)
{
    QDir base(QtNoid::App::Core::appExeOrAppBundleDirPath());
    QString rel = base.relativeFilePath(path);

    bool inside = QDir::isRelativePath(rel) && !rel.startsWith("../") && rel != "..";
    if(inside) {
        ui->txtSingleFile->setText(rel);
    }
    else {
        ui->txtSingleFile->setText(path);
    }
}

QString MainWindow::singleFilePath() const
{
    return ui->txtSingleFile->text();
}

QString MainWindow::yamlFullPathFromCollectionList()
{
    const QString yamlFilePath = m_yamlTestCollectionList.value(m_yamlTestCollectionListCurrent, {});
    if(yamlFilePath.isEmpty()) {
        return {};
    }

    const QString absoluteYamlFilePath = collectionFolderPath()
                                         + QDir::separator() + yamlFilePath;

    return absoluteYamlFilePath;
}

QString MainWindow::yamlStatusBarMsgFromCollectionList()
{
    const QString yamlFilePath = m_yamlTestCollectionList.value(m_yamlTestCollectionListCurrent, {});
    if(yamlFilePath.isEmpty()) {
        return {};
    }
    auto relPath = ui->txtCollectionFolder->currentText();

    const QString path = relPath + QDir::separator() + yamlFilePath;

    const QString msg = QString(" - %1/%2")
                            .arg(m_yamlTestCollectionListCurrent+1)
                            .arg(m_yamlTestCollectionList.count());


    return path + msg;
}

void MainWindow::setCurrentSourceWidget(QWidget* widget)
{
    if(ui->tabWidgetSource->currentWidget() == ui->tabSingleFile){
        const QString fileName = singleFilePath();
        updateUI_loadYamlFile(fileName);
        updateUI_convertYamlToJson();
        updateUI_recentFiles(fileName);
        updateUI_statusBar(fileName);
    }
    else {
        ui->tabWidgetSource->setCurrentWidget(ui->tabSingleFile);
    }


}

bool MainWindow::isSourceChangeRequest(QObject *watched, QEvent *event) const
{
    if (watched == ui->tabWidgetSource && event->type() == QEvent::KeyPress) {
        auto *key = static_cast<QKeyEvent*>(event);
        const bool ctrl = key->modifiers() & Qt::ControlModifier;
        return ctrl && (key->key() == Qt::Key_Tab    || key->key() == Qt::Key_Backtab ||
                        key->key() == Qt::Key_PageUp || key->key() == Qt::Key_PageDown);
    }

    QTabBar *bar = ui->tabWidgetSource->tabBar();
    if (watched == bar) {
        switch (event->type()) {
        case QEvent::MouseButtonPress: {
            auto *mouse = static_cast<QMouseEvent*>(event);
            const int index = bar->tabAt(mouse->position().toPoint());
            return mouse->button() == Qt::LeftButton && index >= 0 && index != bar->currentIndex();
        }
        case QEvent::Wheel:
        case QEvent::KeyPress:
            return true;
        default:
            break;
        }
    }
    return false;
}

bool MainWindow::confirmDiscardYaml()
{
    if (!ui->txtYAML->document()->isModified())
        return true;

    const auto answer = QMessageBox::warning(this, tr("YAML modified"),
                                             tr("The YAML has been modified.\nChanging source will discard the changes.\n\nContinue?"),
                                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    return answer == QMessageBox::Yes;
}


void MainWindow::on_cmdBrowseSingleFile_clicked()
{
    if(!confirmDiscardYaml()) {
        return;
    }

    fileOpenSingleFile();
}


void MainWindow::on_cmdSaveSingleFile_clicked()
{
    const QString yaml = ui->txtYAML->toPlainText();
    qDebug() << __func__<< yaml.size();

    const QString fileName = singleFilePath();

    if(QFileInfo::exists(fileName)) {
        const auto answer = QMessageBox::warning(this, tr("Save YAML File"),
                                                 tr("%1 already exists.\nDo you want to overwrite it?")
                                                     .arg(QDir::toNativeSeparators(fileName)),
                                                 QMessageBox::Yes | QMessageBox::No,
                                                 QMessageBox::No);
        if (answer != QMessageBox::Yes)
            return;
    }

    const QString savedFile = QtNoid::Common::File::saveAsTextFile(yaml, fileName);
    if(savedFile.isEmpty()){
        QMessageBox::warning(this, tr("Save YAML File"),
                             tr("Cannot open %1:\n%2").arg(QDir::toNativeSeparators(fileName)));
        return;
    }

    ui->txtYAML->document()->setModified(false);

    QFileInfo FI(savedFile);
    const QString bytes = QtNoid::Common::Scale::autoUpToExaByte(FI.size());
    updateUI_statusBar(tr("Saved %1 - %2").arg(QDir::toNativeSeparators(fileName), bytes));

}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (isSourceChangeRequest(watched, event) && !confirmDiscardYaml())
        return true;                              // evento scartato: il tab non cambia
    return QMainWindow::eventFilter(watched, event);
}

