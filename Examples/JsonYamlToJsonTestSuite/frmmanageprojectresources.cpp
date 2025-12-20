#include "frmmanageprojectresources.h"
#include "ui_frmmanageprojectresources.h"
#include "dlgfilediff.h"
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
#include <QToolButton>
#include <QScrollBar>
#include <QElapsedTimer>
#include <QCryptographicHash>
#include <QMouseEvent>
#include <QtNoidApp/configglobal.h>
#include <QtNoidApp/settings.h>
#include <QtNoidCommon/QtNoidCommon>

frmManageProjectResources::frmManageProjectResources(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::frmManageProjectResources)
{
    ui->setupUi(this);

    // Initialize file comparison model
    m_fileComparisonModel = new QStandardItemModel(0, ColCount, this);
    m_fileComparisonModel->setHorizontalHeaderLabels({"Source", "<-", "Status", "->", "Destination"});
    ui->tableView->setModel(m_fileComparisonModel);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(ColUpdateSource, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(ColStatus, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(ColAction, QHeaderView::ResizeToContents);

    // Lazy loading: create buttons only for visible rows
    connect(ui->tableView->verticalScrollBar(), &QScrollBar::valueChanged,
            this, &frmManageProjectResources::createVisibleButtons);

    // Double click to open diff dialog - use event filter on viewport
    ui->tableView->viewport()->installEventFilter(this);

}

frmManageProjectResources::~frmManageProjectResources()
{
    delete ui;
}

bool frmManageProjectResources::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->tableView->viewport() && event->type() == QEvent::MouseButtonDblClick) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QModelIndex index = ui->tableView->indexAt(mouseEvent->pos());
        if (index.isValid()) {
            onTableDoubleClicked(index);
            return true;
        }
    }
    return QDialog::eventFilter(watched, event);
}

QString frmManageProjectResources::destinationFolder() const
{
    return ui->txtProjectFolder->text();
}

void frmManageProjectResources::setDestinationFolder(const QString &newDestinationFolder)
{
    auto destinationFolder = ui->txtProjectFolder->text();
    if(destinationFolder != newDestinationFolder) {
        ui->txtProjectFolder->setText(newDestinationFolder);
    }
}

QString frmManageProjectResources::destinationResourceFile() const
{
    return ui->txtProjectRes->currentText();
}

QString frmManageProjectResources::destinationResourceAbsoluteFile() const
{
    return QDir::cleanPath(destinationFolder() + "/" + ui->txtProjectRes->currentText());
}

void frmManageProjectResources::setDestinationResourceFile(const QString &newDestinationResourceFile)
{
    auto destinatinoResourceFile = ui->txtProjectRes->currentText();
    if(destinatinoResourceFile != newDestinationResourceFile) {
        ui->txtProjectRes->clear();
        ui->txtProjectRes->addItem(newDestinationResourceFile);
        updateUi_resourceFilePrefix();
    }
}

QString frmManageProjectResources::destinationAbsoluteFile(const QString &relFile) const
{
    auto prefixLen = projectResPrefix().length();
    return QDir::cleanPath(destinationFolder() + "/" + relFile.sliced(prefixLen));
}

QString frmManageProjectResources::sourceBaseFolder() const
{
    return ui->txtSourceBaseFolder->text();
}

void frmManageProjectResources::setSourceBaseFolder(const QString &newSourceBaseFolder)
{
    auto sourceBaseFolder = ui->txtSourceBaseFolder->text();
    if(sourceBaseFolder != newSourceBaseFolder) {
        ui->txtSourceBaseFolder->setText(newSourceBaseFolder);
    }
}

void frmManageProjectResources::updateUi_resourceFilePrefix()
{
    ui->txtProjectResPrefix->clear();
    QFile file(destinationResourceAbsoluteFile());
    if(!file.exists()) {
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Unable to open file:" << destinationResourceAbsoluteFile();
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


void frmManageProjectResources::loadSourceFileList()
{
    // qDebug() << __func__ << sourceFolder();
    m_sourceFileList.clear();
    const QStringList fullPath = QtNoid::Common::File::listSubPathRecursively(sourceFolder());

    auto subFolder = sourceSubFolder() + "/";
    for(const QString& path : fullPath) {
        m_sourceFileList.insert(subFolder + path);
    }
}

void frmManageProjectResources::loadDestinationFileList()
{
    QFile file(destinationResourceAbsoluteFile());
    if(!file.exists()) {
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // qWarning() << "Impossibile aprire il file:" << projectResFile();
        return;
    }

    auto prefix = projectResPrefix();
    auto shortedPrexis = prefix.sliced(1); // Remove /
    QXmlStreamReader xml(&file);
    bool inTargetPrefix = false;

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
                    m_destinationFileList.insert(shortedPrexis + '/' + fileName);
                }
            }
        }
        else if (token == QXmlStreamReader::EndElement) {
            if (xml.name() == u"qresource") {
                inTargetPrefix = false;
            }
        }
    }
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
    }
}



QString frmManageProjectResources::sourceSubFolder() const
{
    return QDir::cleanPath(ui->txtSourceSubFolder->text());
}

void frmManageProjectResources::setSourceSubFolder(const QString &newSourceSubFolder)
{
    auto localFolder = ui->txtSourceSubFolder->text();
    if(localFolder != newSourceSubFolder) {
        ui->txtSourceSubFolder->setText(newSourceSubFolder);
    }
}

QString frmManageProjectResources::sourceFolder() const
{
    return QDir::cleanPath(sourceBaseFolder() + "/" + sourceSubFolder());
}

QString frmManageProjectResources::sourceAbsoluteFile(const QString& relFile) const
{
    return QDir::cleanPath(sourceBaseFolder() + "/" + relFile);
}

void frmManageProjectResources::addFileComparison(const QString &sourceFile, const QString &status, const QString &destinationFile)
{
    QList<QStandardItem*> items(ColCount, nullptr);
    items[ColSource] = new QStandardItem(sourceFile);
    items[ColUpdateSource] = new QStandardItem();
    items[ColStatus] = new QStandardItem(status);
    items[ColStatus]->setTextAlignment(Qt::AlignCenter);
    items[ColAction] = new QStandardItem();
    items[ColDestination] = new QStandardItem(destinationFile);
    m_fileComparisonModel->appendRow(items);
}

void frmManageProjectResources::createButtonForRow(int row)
{
    QString sourceFile = m_fileComparisonModel->item(row, ColSource)->text();
    QString destFile = m_fileComparisonModel->item(row, ColDestination)->text();
    QString status = m_fileComparisonModel->item(row, ColStatus)->text();

    // Only show buttons if files are different
    // "==" means identical - no buttons needed
    // "!=" means both exist but differ - show both buttons
    // "=>" means source only - show "->" button
    // "<=" means dest only - show "<-" button

    // Create "copy to destination" button (->)
    // Show when: source exists AND (files differ OR source only)
    QModelIndex actionIndex = m_fileComparisonModel->index(row, ColAction);
    bool showCopyToDest = !sourceFile.isEmpty() && (status == "!=" || status == "=>");
    if (!ui->tableView->indexWidget(actionIndex) && showCopyToDest) {
        QToolButton *btn = new QToolButton();
        btn->setText("->");
        btn->setProperty("row", row);
        connect(btn, &QToolButton::clicked, this, [this, btn]() {
            int r = btn->property("row").toInt();
            onCopyButtonClicked(r);
        });
        ui->tableView->setIndexWidget(actionIndex, btn);
    }

    // Create "update source" button (<-)
    // Show when: dest exists AND (files differ OR dest only)
    QModelIndex updateSourceIndex = m_fileComparisonModel->index(row, ColUpdateSource);
    bool showUpdateSource = !destFile.isEmpty() && (status == "!=" || status == "<=");
    if (!ui->tableView->indexWidget(updateSourceIndex) && showUpdateSource) {
        QToolButton *btn = new QToolButton();
        btn->setText("<-");
        btn->setProperty("row", row);
        connect(btn, &QToolButton::clicked, this, [this, btn]() {
            int r = btn->property("row").toInt();
            onUpdateSourceButtonClicked(r);
        });
        ui->tableView->setIndexWidget(updateSourceIndex, btn);
    }
}

void frmManageProjectResources::createVisibleButtons()
{
    updateVisibleRows();
}

void frmManageProjectResources::updateVisibleRows()
{
    QRect viewportRect = ui->tableView->viewport()->rect();
    int firstVisibleRow = ui->tableView->rowAt(viewportRect.top());
    int lastVisibleRow = ui->tableView->rowAt(viewportRect.bottom());

    if (firstVisibleRow < 0)
        firstVisibleRow = 0;
    if (lastVisibleRow < 0)
        lastVisibleRow = m_fileComparisonModel->rowCount() - 1;

    for (int row = firstVisibleRow; row <= lastVisibleRow; ++row) {
        createButtonForRow(row);
        compareFileForRow(row);
    }
}

void frmManageProjectResources::compareFileForRow(int row)
{
    // Skip if already compared
    if (m_comparedRows.contains(row))
        return;

    QString sourceFile = m_fileComparisonModel->item(row, ColSource)->text();
    QString destFile = m_fileComparisonModel->item(row, ColDestination)->text();

    // Only compare if both files exist (status == "==")
    if (sourceFile.isEmpty() || destFile.isEmpty())
        return;

    QString fullSourcePath = sourceAbsoluteFile(sourceFile);
    QString fullDestPath = destinationAbsoluteFile(destFile);

    QFile srcFile(fullSourcePath);
    QFile dstFile(fullDestPath);

    bool filesMatch = false;

    if (srcFile.open(QIODevice::ReadOnly) && dstFile.open(QIODevice::ReadOnly)) {
        QCryptographicHash srcHash(QCryptographicHash::Md5);
        QCryptographicHash dstHash(QCryptographicHash::Md5);

        srcHash.addData(&srcFile);
        dstHash.addData(&dstFile);

        filesMatch = (srcHash.result() == dstHash.result());
    }

    // Mark as compared
    m_comparedRows.insert(row);

    // If files differ, highlight source in red
    if (!filesMatch) {
        QStandardItem *sourceItem = m_fileComparisonModel->item(row, ColSource);
        sourceItem->setForeground(QBrush(Qt::red));

        // Update status to show difference
        QStandardItem *statusItem = m_fileComparisonModel->item(row, ColStatus);
        statusItem->setText("!=");
        statusItem->setForeground(QBrush(Qt::red));

        // Create buttons now that status changed to "!="
        createButtonForRow(row);

        // Update footer to reflect the new difference count
        updateTableFooter();
    }

}

void frmManageProjectResources::updateFileCompare()
{
    QElapsedTimer ET;
    ET.start();

    clearFileComparisons();
    loadSourceFileList();
    loadDestinationFileList();

    QSet<QString> srcSet(m_sourceFileList.begin(), m_sourceFileList.end());
    QSet<QString> dstSet(m_destinationFileList.begin(), m_destinationFileList.end());

    const QSet<QString> allFilesSet = srcSet | dstSet;
    QList<QString> allFilesList(allFilesSet.begin(), allFilesSet.end());
    std::ranges::sort(allFilesList);

    for (const QString &file : std::as_const(allFilesList)) {
        bool src = srcSet.contains(file);
        bool dst = dstSet.contains(file);

        if(src && dst) {
            addFileComparison(file, "==", file);
        }
        else if(src) {
            addFileComparison(file, "=>", {});
        }
        else {
            addFileComparison({}, "<=", file);
        }
    }

    // Create buttons only for initially visible rows
    createVisibleButtons();

    // Update the footer with statistics
    updateTableFooter();

    qDebug() << __func__ << "Elapsed time: " << QtNoid::Common::Scale::nanoSecsUpToDays(ET.nsecsElapsed());
}

void frmManageProjectResources::clearFileComparisons()
{
    m_fileComparisonModel->removeRows(0, m_fileComparisonModel->rowCount());
    m_comparedRows.clear();
    ui->lblTableFooter->clear();
}

void frmManageProjectResources::updateTableFooter()
{
    int total = m_fileComparisonModel->rowCount();
    int matching = 0;      // ==
    int sourceOnly = 0;    // =>
    int destOnly = 0;      // <=
    int different = 0;     // !=

    for (int row = 0; row < total; ++row) {
        QString status = m_fileComparisonModel->item(row, ColStatus)->text();
        if (status == "==")
            ++matching;
        else if (status == "=>")
            ++sourceOnly;
        else if (status == "<=")
            ++destOnly;
        else if (status == "!=")
            ++different;
    }

    QString footerText = tr("Total: %1  |  Matching: %2  |  Source only: %3  |  Dest only: %4  |  Different: %5")
                             .arg(total)
                             .arg(matching)
                             .arg(sourceOnly)
                             .arg(destOnly)
                             .arg(different);

    ui->lblTableFooter->setText(footerText);
}

int frmManageProjectResources::fileComparisonCount() const
{
    return m_fileComparisonModel->rowCount();
}


void frmManageProjectResources::on_cmdUpdate_clicked()
{
    updateFileCompare();
}

void frmManageProjectResources::onCopyButtonClicked(int row)
{
    QString sourceFile = m_fileComparisonModel->item(row, ColSource)->text();
    QString status = m_fileComparisonModel->item(row, ColStatus)->text();

    if (sourceFile.isEmpty()) {
        QMessageBox::warning(this, tr("Copy Error"), tr("No source file to copy."));
        return;
    }

    // Build full source path
    QString fullSourcePath = sourceAbsoluteFile(sourceFile);

    // Build destination path in project folder
    QString fullDestPath = destinationAbsoluteFile(sourceFile);

    // Check if source exists
    if (!QFile::exists(fullSourcePath)) {
        QMessageBox::warning(this, tr("Copy Error"),
                             tr("Source file does not exist:\n%1").arg(fullSourcePath));
        return;
    }

    // Create destination directory if needed
    QFileInfo destInfo(fullDestPath);
    QDir destDir = destInfo.absoluteDir();
    if (!destDir.exists()) {
        destDir.mkpath(".");
    }

    // Remove existing destination file if it exists
    if (QFile::exists(fullDestPath)) {
        QFile::remove(fullDestPath);
    }

    // Copy the file
    if (QFile::copy(fullSourcePath, fullDestPath)) {
        // Update status in the model
        m_fileComparisonModel->item(row, ColStatus)->setText("==");
        m_fileComparisonModel->item(row, ColDestination)->setText(sourceFile);

        // Reset highlighting (remove red color if present)
        QStandardItem *sourceItem = m_fileComparisonModel->item(row, ColSource);
        sourceItem->setForeground(QBrush(Qt::black));
        QStandardItem *statusItem = m_fileComparisonModel->item(row, ColStatus);
        statusItem->setForeground(QBrush(Qt::black));

        updateTableFooter();

        QMessageBox::information(this, tr("Copy Success"),
                                 tr("File copied successfully:\n%1").arg(fullDestPath));
    } else {
        QMessageBox::critical(this, tr("Copy Error"),
                              tr("Failed to copy file:\n%1\nto:\n%2").arg(fullSourcePath, fullDestPath));
    }
}

void frmManageProjectResources::onUpdateSourceButtonClicked(int row)
{
    QString destFile = m_fileComparisonModel->item(row, ColDestination)->text();

    if (destFile.isEmpty()) {
        QMessageBox::warning(this, tr("Update Source Error"), tr("No destination file to copy from."));
        return;
    }

    // Build full destination path (this is the source for copy)
    QString fullDestPath = destinationAbsoluteFile(destFile);

    // Build source path (this is the target for copy)
    QString fullSourcePath = sourceAbsoluteFile(destFile);

    // Check if destination file exists
    if (!QFile::exists(fullDestPath)) {
        QMessageBox::warning(this, tr("Update Source Error"),
                             tr("Destination file does not exist:\n%1").arg(fullDestPath));
        return;
    }

    // Create source directory if needed
    QFileInfo srcInfo(fullSourcePath);
    QDir srcDir = srcInfo.absoluteDir();
    if (!srcDir.exists()) {
        srcDir.mkpath(".");
    }

    // Remove existing source file if it exists
    if (QFile::exists(fullSourcePath)) {
        QFile::remove(fullSourcePath);
    }

    // Copy the file from destination to source
    if (QFile::copy(fullDestPath, fullSourcePath)) {
        // Update status in the model
        m_fileComparisonModel->item(row, ColStatus)->setText("==");
        m_fileComparisonModel->item(row, ColSource)->setText(destFile);

        // Reset highlighting (remove red color if present)
        QStandardItem *sourceItem = m_fileComparisonModel->item(row, ColSource);
        sourceItem->setForeground(QBrush(Qt::black));
        QStandardItem *statusItem = m_fileComparisonModel->item(row, ColStatus);
        statusItem->setForeground(QBrush(Qt::black));

        updateTableFooter();

        QMessageBox::information(this, tr("Update Source Success"),
                                 tr("Source file updated successfully:\n%1").arg(fullSourcePath));
    } else {
        QMessageBox::critical(this, tr("Update Source Error"),
                              tr("Failed to copy file:\n%1\nto:\n%2").arg(fullDestPath, fullSourcePath));
    }
}

void frmManageProjectResources::onTableDoubleClicked(const QModelIndex &index)
{
    showDiffForRow(index.row());
}

void frmManageProjectResources::showDiffForRow(int row)
{
    if (row < 0 || row >= m_fileComparisonModel->rowCount())
        return;

    QString sourceFile = m_fileComparisonModel->item(row, ColSource)->text();
    QString destFile = m_fileComparisonModel->item(row, ColDestination)->text();

    // Need both files to compare
    if (sourceFile.isEmpty() || destFile.isEmpty())
        return;

    QString fullSourcePath = sourceAbsoluteFile(sourceFile);
    QString fullDestPath = destinationAbsoluteFile(destFile);

    // Check both files exist
    if (!QFile::exists(fullSourcePath) || !QFile::exists(fullDestPath))
        return;

    m_currentDiffRow = row;

    // Create dialog if needed
    if (!m_diffDialog) {
        m_diffDialog = new DlgFileDiff(this);
        m_diffDialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(m_diffDialog, &DlgFileDiff::previousFileRequested, this, &frmManageProjectResources::onPreviousFile);
        connect(m_diffDialog, &DlgFileDiff::nextFileRequested, this, &frmManageProjectResources::onNextFile);
        connect(m_diffDialog, &QDialog::destroyed, this, [this]() {
            m_diffDialog = nullptr;

        });
    }

    // Count comparable rows for navigation info
    int comparableCount = 0;
    int currentIndex = 0;
    for (int i = 0; i < m_fileComparisonModel->rowCount(); ++i) {
        QString src = m_fileComparisonModel->item(i, ColSource)->text();
        QString dst = m_fileComparisonModel->item(i, ColDestination)->text();
        if (!src.isEmpty() && !dst.isEmpty()) {
            if (i < row) currentIndex++;
            comparableCount++;
        }
    }

    m_diffDialog->setTitles(tr("Source: %1").arg(sourceFile), tr("Destination: %1").arg(destFile));
    m_diffDialog->setFiles(fullSourcePath, fullDestPath);
    m_diffDialog->setNavigationInfo(currentIndex, comparableCount);

    if (!m_diffDialog->isVisible())
        m_diffDialog->show();
}

int frmManageProjectResources::findNextDiffRow(int fromRow, int direction)
{
    int rowCount = m_fileComparisonModel->rowCount();
    int row = fromRow + direction;

    // Wrap around
    for (int i = 0; i < rowCount; ++i) {
        if (row < 0) row = rowCount - 1;
        if (row >= rowCount) row = 0;

        QString src = m_fileComparisonModel->item(row, ColSource)->text();
        QString dst = m_fileComparisonModel->item(row, ColDestination)->text();

        // Found a comparable row
        if (!src.isEmpty() && !dst.isEmpty())
            return row;

        row += direction;
    }

    return -1;  // No comparable rows found
}

void frmManageProjectResources::onPreviousFile()
{
    int prevRow = findNextDiffRow(m_currentDiffRow, -1);
    if (prevRow >= 0)
        showDiffForRow(prevRow);
}

void frmManageProjectResources::onNextFile()
{
    int nextRow = findNextDiffRow(m_currentDiffRow, +1);
    if (nextRow >= 0)
        showDiffForRow(nextRow);
}

