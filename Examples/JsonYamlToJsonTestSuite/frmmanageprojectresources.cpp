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
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QtNoidApp/configglobal.h>
#include <QtNoidApp/settings.h>
#include <QtNoidCommon/QtNoidCommon>

class ButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ButtonDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyleOptionButton buttonOption;
        buttonOption.rect = option.rect.adjusted(4, 4, -4, -4);
        buttonOption.text = tr("Copy");
        buttonOption.state = QStyle::State_Enabled;

        QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter);
    }

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override
    {
        if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (option.rect.adjusted(4, 4, -4, -4).contains(mouseEvent->pos())) {
                emit buttonClicked(index);
                return true;
            }
        }
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }

signals:
    void buttonClicked(const QModelIndex &index);
};

#include "frmmanageprojectresources.moc"

frmManageProjectResources::frmManageProjectResources(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::frmManageProjectResources)
{
    ui->setupUi(this);

    // Initialize file comparison model
    m_fileComparisonModel = new QStandardItemModel(0, 4, this);
    m_fileComparisonModel->setHorizontalHeaderLabels({"Source", "Status", "Destination", "Action"});
    ui->tableView->setModel(m_fileComparisonModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Set up button delegate for the Action column
    ButtonDelegate *buttonDelegate = new ButtonDelegate(this);
    ui->tableView->setItemDelegateForColumn(3, buttonDelegate);
    connect(buttonDelegate, &ButtonDelegate::buttonClicked, this, &frmManageProjectResources::onCopyButtonClicked);

    ui->tableView->setSortingEnabled(true);

    // Geometry
    auto pageName = QtNoid::App::Settings::groupNameFromObjectOrClass(this);
    restoreGeometry(appConfig->restoreAsByteArray("Geometry", saveGeometry(), pageName));

    addFileComparison("C:/source/file1.txt", "==", "C:/dest/file1.txt");
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

void frmManageProjectResources::updateUi_resourceFileList()
{
    // QDir dir(ui->txtProjectFolder->text());
    // dir.setNameFilters({"*.qrc"});
    QtNoid::Common::File QtNoidFile;
    auto projectFolder = ui->txtProjectFolder->text();
    // auto projectFolderLength = projectFolder.length();
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
}

void frmManageProjectResources::loadSourceFileList()
{
    m_sourceFileList.clear();
    const QStringList fullPath = QtNoid::Common::File::listSubPathRecursively(sourceFolder());

    auto subFolder = sourceSubFolder() + "/";
    for(const QString& path : fullPath) {
        m_sourceFileList.insert(subFolder + path);
    }
}

void frmManageProjectResources::loadDestinationFileList()
{
    QFile file(projectResFile());
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

QString frmManageProjectResources::sourceSubFolder() const
{
    return ui->txtSourceSubFolder->text();
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

void frmManageProjectResources::on_txtProjectResPrefix_currentTextChanged(const QString &arg1)
{
    updateUi_filesInPrefix();
}

void frmManageProjectResources::addFileComparison(const QString &sourceFile, const QString &status, const QString &destinationFile)
{
    QList<QStandardItem*> row;
    row.append(new QStandardItem(sourceFile));
    row.append(new QStandardItem(status));
    row.append(new QStandardItem(destinationFile));
    row.append(new QStandardItem()); // Action column (button rendered by delegate)
    m_fileComparisonModel->appendRow(row);
}

void frmManageProjectResources::updateFileCompare()
{
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
}

void frmManageProjectResources::clearFileComparisons()
{
    m_fileComparisonModel->removeRows(0, m_fileComparisonModel->rowCount());
}

int frmManageProjectResources::fileComparisonCount() const
{
    return m_fileComparisonModel->rowCount();
}


void frmManageProjectResources::on_cmdUpdate_clicked()
{
    updateFileCompare();
}

void frmManageProjectResources::onCopyButtonClicked(const QModelIndex &index)
{
    int row = index.row();
    QString sourceFile = m_fileComparisonModel->item(row, 0)->text();
    QString status = m_fileComparisonModel->item(row, 1)->text();

    if (sourceFile.isEmpty()) {
        QMessageBox::warning(this, tr("Copy Error"), tr("No source file to copy."));
        return;
    }

    // Build full source path
    QString fullSourcePath = QDir::cleanPath(sourceBaseFolder() + "/" + sourceFile);

    // Build destination path in project folder
    QString fullDestPath = QDir::cleanPath(projectFolder() + "/" + sourceFile);

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
        m_fileComparisonModel->item(row, 1)->setText("==");
        m_fileComparisonModel->item(row, 2)->setText(sourceFile);
        QMessageBox::information(this, tr("Copy Success"),
                                 tr("File copied successfully:\n%1").arg(fullDestPath));
    } else {
        QMessageBox::critical(this, tr("Copy Error"),
                              tr("Failed to copy file:\n%1\nto:\n%2").arg(fullSourcePath, fullDestPath));
    }
}

