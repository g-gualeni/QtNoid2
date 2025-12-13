#include "frmmanageprojectresources.h"
#include "ui_frmmanageprojectresources.h"
#include <QFileDialog>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QSet>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QHeaderView>
#include <QtNoidApp/configglobal.h>
#include <QtNoidApp/settings.h>
#include <QtNoidCommon/QtNoidCommon>

frmManageProjectResources::frmManageProjectResources(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::frmManageProjectResources)
{
    ui->setupUi(this);

    // Initialize file comparison model
    m_fileComparisonModel = new QStandardItemModel(0, 2, this);
    m_fileComparisonModel->setHorizontalHeaderLabels({"Source", "Destination"});
    ui->tableView->setModel(m_fileComparisonModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Geometry
    auto pageName = QtNoid::App::Settings::groupNameFromObjectOrClass(this);
    restoreGeometry(appConfig->restoreAsByteArray("Geometry", saveGeometry(), pageName));

     addFileComparison("C:/source/file1.txt", "C:/dest/file1.txt");
}

frmManageProjectResources::~frmManageProjectResources()
{
    auto pageName = QtNoid::App::Settings::groupNameFromObjectOrClass(this);
    appConfig->saveValue("Geometry", saveGeometry(), pageName);
    delete ui;
}

QString frmManageProjectResources::projectFolder() const
{
    return ui->txtProjectFolder->text();
}

void frmManageProjectResources::setProjectFolder(const QString &newProjectFolder)
{
    auto projectFolder = ui->txtProjectFolder->text();
    if(projectFolder != newProjectFolder) {
        ui->txtProjectFolder->setText(newProjectFolder);
        updateUi_resourceFileList();
        updateUi_resourceFilePrefix();
    }
}

void frmManageProjectResources::updateUi_resourceFileList()
{
    // QDir dir(ui->txtProjectFolder->text());
    // dir.setNameFilters({"*.qrc"});
    QtNoid::Common::File QtNoidFile;
    auto projectFolder = ui->txtProjectFolder->text();
    auto projectFolderLength = projectFolder.length();
    auto relativeFileList = QtNoidFile.listSubPathRecursively(projectFolder, {".qrc"});
    ui->txtProjectRes->clear();
    ui->txtProjectRes->addItems(relativeFileList);
}

void frmManageProjectResources::updateUi_resourceFilePrefix()
{
    ui->txtProjectResPrefix->clear();
    QFile file(projectResFile());
    if(!file.exists()) {
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Impossibile aprire il file:" << projectResFile();
        return;
    }
    QStringList prefixes;
    QXmlStreamReader xml(&file);
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            if (xml.name() == u"qresource") {
                QString prefix = xml.attributes().value(u"prefix").toString();
                if (!prefix.isEmpty()) {
                    prefixes.append(prefix);
                }
            }
        }
    }

    if (xml.hasError()) {
        qWarning() << "Errore parsing XML:" << xml.errorString();
    }
    ui->txtProjectResPrefix->addItems(prefixes);
}

void frmManageProjectResources::updateUi_filesInPrefix()
{
    QFile file(projectResFile());
    if(!file.exists()) {
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Impossibile aprire il file:" << projectResFile();
        return;
    }

    auto prefix = projectResPrefix();
    auto shortedPrexis = prefix.sliced(1); // Remove /
    QXmlStreamReader xml(&file);
    bool inTargetPrefix = false;
    QStringList files;

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartElement) {
            if (xml.name() == u"qresource") {
                QString currentPrefix = xml.attributes().value(u"prefix").toString();
                inTargetPrefix = (currentPrefix == prefix);
            }
            else if (xml.name() == u"file" && inTargetPrefix) {
                QString fileName = xml.readElementText();
                if (!fileName.isEmpty()) {
                    files.append(shortedPrexis + '/' + fileName);
                }
            }
        }
        else if (token == QXmlStreamReader::EndElement) {
            if (xml.name() == u"qresource") {
                inTargetPrefix = false;
            }
        }
    }

    // qDebug() << __func__<< files;
    clearFileComparisons();
    addDestinationFiles(files);

}

QString frmManageProjectResources::projectResPrefix() const
{
    return ui->txtProjectResPrefix->currentText();
}

void frmManageProjectResources::setProjectResPrefix(const QString &newProjectResPrefix)
{
    auto current = ui->txtProjectResPrefix->currentText();
    if(current != newProjectResPrefix){
        ui->txtProjectResPrefix->setCurrentText(newProjectResPrefix);
        updateUi_filesInPrefix();
    }
}

QString frmManageProjectResources::projectResFile() const
{
    auto fileName = ui->txtProjectRes->currentText();
    return projectFolder() + "/" + fileName;
}

void frmManageProjectResources::setProjectResFile(const QString &newProjectResFile)
{
    auto current = ui->txtProjectRes->currentText();
    if(current != newProjectResFile) {
        ui->txtProjectRes->setCurrentText(newProjectResFile);
        updateUi_resourceFilePrefix();
    }

}

QString frmManageProjectResources::localFolder() const
{
    return ui->txtLocalFolder->text();
}

void frmManageProjectResources::setLocalFolder(const QString &newLocalFolder)
{
    auto localFolder = ui->txtLocalFolder->text();
    if(localFolder != newLocalFolder) {
        ui->txtLocalFolder->setText(newLocalFolder);
    }
}

void frmManageProjectResources::on_txtProjectResPrefix_currentTextChanged(const QString &arg1)
{
    updateUi_filesInPrefix();
}

void frmManageProjectResources::addFileComparison(const QString &sourceFile, const QString &destinationFile)
{
    QList<QStandardItem*> row;
    row.append(new QStandardItem(sourceFile));
    row.append(new QStandardItem(destinationFile));
    m_fileComparisonModel->appendRow(row);
}

void frmManageProjectResources::addDestinationFiles(const QStringList &sourceFiles)
{
    for(const QString &sourceFile : sourceFiles) {
        QList<QStandardItem*> row;
        row.append(new QStandardItem(QString())); // Empty destination
        row.append(new QStandardItem(sourceFile));
        m_fileComparisonModel->appendRow(row);
    }
}

void frmManageProjectResources::clearFileComparisons()
{
    m_fileComparisonModel->removeRows(0, m_fileComparisonModel->rowCount());
}

int frmManageProjectResources::fileComparisonCount() const
{
    return m_fileComparisonModel->rowCount();
}

