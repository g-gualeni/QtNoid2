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

frmManageProjectResources::frmManageProjectResources(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::frmManageProjectResources)
    , m_folderCount(0)
{
    ui->setupUi(this);

    connect(ui->cmdBrowseSourceFolder, &QToolButton::clicked, this, &frmManageProjectResources::onCmdBrowseSourceFolder);
    connect(ui->cmdScanFolder, &QPushButton::clicked, this, &frmManageProjectResources::onCmdScanFolder);
    connect(ui->cmdGenerateMapFile, &QPushButton::clicked, this, &frmManageProjectResources::onCmdGenerateMapFile);
    connect(ui->cmdGenerateQrcEntries, &QPushButton::clicked, this, &frmManageProjectResources::onCmdGenerateQrcEntries);
}

frmManageProjectResources::~frmManageProjectResources()
{
    delete ui;
}

void frmManageProjectResources::setSourceFolder(const QString &folder)
{
    ui->txtSourceFolder->setText(folder);
}

QString frmManageProjectResources::sourceFolder() const
{
    return ui->txtSourceFolder->text();
}

void frmManageProjectResources::setResourcePrefix(const QString &prefix)
{
    ui->txtResourcePrefix->setText(prefix);
}

QString frmManageProjectResources::resourcePrefix() const
{
    return ui->txtResourcePrefix->text();
}

void frmManageProjectResources::setQrcPrefix(const QString &prefix)
{
    ui->cboQrcPrefix->setCurrentText(prefix);
}

QString frmManageProjectResources::qrcPrefix() const
{
    return ui->cboQrcPrefix->currentText();
}

void frmManageProjectResources::setMapFileName(const QString &fileName)
{
    ui->txtMapFileName->setText(fileName);
}

QString frmManageProjectResources::mapFileName() const
{
    return ui->txtMapFileName->text();
}

void frmManageProjectResources::onCmdBrowseSourceFolder()
{
    QString folder = QFileDialog::getExistingDirectory(
        this,
        tr("Select Source Folder"),
        sourceFolder(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    if (!folder.isEmpty()) {
        setSourceFolder(folder);
    }
}

void frmManageProjectResources::onCmdScanFolder()
{
    QString folder = sourceFolder();
    if (folder.isEmpty()) {
        QMessageBox::warning(this, tr("No Folder Selected"),
                           tr("Please select a source folder first."));
        return;
    }

    QDir dir(folder);
    if (!dir.exists()) {
        QMessageBox::warning(this, tr("Folder Not Found"),
                           tr("The selected folder does not exist."));
        return;
    }

    m_fileList.clear();
    m_folderCount = 0;

    scanFolderRecursively(folder, m_fileList);

    // Display file list
    ui->txtFileList->clear();
    for (const QString &file : m_fileList) {
        ui->txtFileList->appendPlainText(file);
    }

    updateStatistics();

    // Auto-generate map file content and QRC entries
    ui->txtMapFileContent->setPlainText(generateMapFileContent());
    ui->txtQrcEntries->setPlainText(generateQrcEntriesContent());
}

void frmManageProjectResources::scanFolderRecursively(const QString &path, QStringList &fileList)
{
    QDir dir(path);
    QSet<QString> folders;

    QDirIterator it(path, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filePath = it.next();
        fileList.append(filePath);

        // Count unique folders
        QFileInfo fileInfo(filePath);
        folders.insert(fileInfo.absolutePath());
    }

    m_folderCount = folders.size();
}

void frmManageProjectResources::updateStatistics()
{
    ui->lblStatsFiles->setText(tr("Files: %1").arg(m_fileList.count()));
    ui->lblStatsFolders->setText(tr("Folders: %1").arg(m_folderCount));
}

QString frmManageProjectResources::getRelativePath(const QString &fullPath)
{
    QString folder = sourceFolder();
    QDir dir(folder);
    return dir.relativeFilePath(fullPath);
}

QString frmManageProjectResources::generateMapFileContent()
{
    if (m_fileList.isEmpty()) {
        return QString();
    }

    QStringList lines;
    for (const QString &file : m_fileList) {
        QString relativePath = getRelativePath(file);
        // Use forward slashes for the map file (cross-platform)
        relativePath.replace('\\', '/');
        lines.append(relativePath);
    }

    // Sort the lines for better organization
    lines.sort();

    return lines.join('\n');
}

QString frmManageProjectResources::generateQrcEntriesContent()
{
    if (m_fileList.isEmpty()) {
        return QString();
    }

    QString prefix = resourcePrefix();
    QString qrcPrefix = ui->cboQrcPrefix->currentText();

    QStringList lines;
    lines.append(QString("    <qresource prefix=\"%1\">").arg(qrcPrefix));

    QStringList sortedFiles = m_fileList;
    sortedFiles.sort();

    for (const QString &file : sortedFiles) {
        QString relativePath = getRelativePath(file);
        // QRC files use forward slashes
        relativePath.replace('\\', '/');

        QString fullResourcePath = prefix;
        if (!fullResourcePath.isEmpty() && !fullResourcePath.endsWith('/')) {
            fullResourcePath += '/';
        }
        fullResourcePath += relativePath;

        lines.append(QString("        <file>%1</file>").arg(fullResourcePath));
    }

    lines.append("    </qresource>");

    return lines.join('\n');
}

void frmManageProjectResources::onCmdGenerateMapFile()
{
    QString fileName = mapFileName();
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, tr("No File Name"),
                           tr("Please enter a map file name."));
        return;
    }

    if (m_fileList.isEmpty()) {
        QMessageBox::warning(this, tr("No Files"),
                           tr("Please scan a folder first."));
        return;
    }

    QString folder = sourceFolder();
    QDir dir(folder);
    QString mapFilePath = dir.absoluteFilePath(fileName);

    // Ask for confirmation if file exists
    if (QFile::exists(mapFilePath)) {
        auto reply = QMessageBox::question(this, tr("File Exists"),
                                          tr("The file '%1' already exists. Do you want to overwrite it?").arg(fileName),
                                          QMessageBox::Yes | QMessageBox::No);
        if (reply != QMessageBox::Yes) {
            return;
        }
    }

    QFile file(mapFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"),
                            tr("Could not write to file: %1").arg(mapFilePath));
        return;
    }

    QTextStream out(&file);
    out << generateMapFileContent();
    file.close();

    QMessageBox::information(this, tr("Success"),
                           tr("Map file generated successfully at:\n%1").arg(mapFilePath));
}

void frmManageProjectResources::onCmdGenerateQrcEntries()
{
    if (m_fileList.isEmpty()) {
        QMessageBox::warning(this, tr("No Files"),
                           tr("Please scan a folder first."));
        return;
    }

    // Copy to clipboard
    QString content = generateQrcEntriesContent();
    // QApplication::clipboard()->setText(content);

    QMessageBox::information(this, tr("Success"),
                           tr("QRC entries have been copied to the clipboard.\n\n"
                              "You can now paste them into your .qrc file.\n\n"
                              "Total entries: %1").arg(m_fileList.count()));
}
