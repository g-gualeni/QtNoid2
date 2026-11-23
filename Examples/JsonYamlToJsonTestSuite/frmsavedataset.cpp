#include "frmsavedataset.h"
#include "ui_frmsavedataset.h"
#include <QFileDialog>
#include <QtNoidApp/QtNoidApp>

frmSaveDataset::frmSaveDataset(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::frmSaveDataset)
{
    ui->setupUi(this);
}

frmSaveDataset::~frmSaveDataset()
{
    delete ui;
}

void frmSaveDataset::setCollectionFolder(const QString& collectionFolder)
{
    ui->txtCollectionFolder->setText(collectionFolder);
}

QString frmSaveDataset::collectionFolder() const
{
    return ui->txtCollectionFolder->text();
}


void frmSaveDataset::setDatasetFolder(const QString& datasetFolder)
{
    ui->txtDatasetFolder->setText(datasetFolder);
}

QString frmSaveDataset::datasetFolder() const
{
    return ui->txtDatasetFolder->text();
}

void frmSaveDataset::on_cmdBrowseCollection_clicked()
{
    QString currentFolder = collectionFolder();
    currentFolder = QFileDialog::getExistingDirectory(
        this,
        tr("Select Collection Folder"),
        currentFolder
        );

    if (currentFolder.isEmpty()) {
        return;
    }

    QDir appPath = QDir(QtNoid::App::Core::appExeOrAppBundleDirPath());
    QString relative = appPath.relativeFilePath(currentFolder);
    bool isDescendant = !QDir::isAbsolutePath(relative) && !relative.startsWith("..");
    if(isDescendant) {
        currentFolder = relative;
    }
    ui->txtCollectionFolder->setText(currentFolder);
}


void frmSaveDataset::on_cmdBrowseDestination_clicked()
{
    static QDir collectionFolderDir(collectionFolder());
    QString currentFolder = collectionFolderDir.filePath(datasetFolder());
    // qDebug() << Q_FUNC_INFO << currentFolder;

    currentFolder = QFileDialog::getExistingDirectory(
        this,
        tr("Select Dataset Folder"),
        currentFolder
    );

    if (currentFolder.isEmpty()) {
        return;
    }
    currentFolder = collectionFolderDir.relativeFilePath(currentFolder);
    ui->txtDatasetFolder->setText(currentFolder);
}


bool frmSaveDataset::saveDescription() const
{
    return ui->chkSaveDescription->isChecked();
}

void frmSaveDataset::setSaveDescription(bool value)
{
    ui->chkSaveDescription->setChecked(value);
}

bool frmSaveDataset::saveInputYAML() const
{
    return ui->chkSaveInputYAML->isChecked();
}

void frmSaveDataset::setSaveInputYAML(bool value)
{
    ui->chkSaveInputYAML->setChecked(value);
}

bool frmSaveDataset::saveExpectedJSON() const
{
    return ui->chkSaveExpectedJSON->isChecked();
}

void frmSaveDataset::setSaveExpectedJSON(bool value)
{
    ui->chkSaveExpectedJSON->setChecked(value);
}

bool frmSaveDataset::saveExpectedTokens() const
{
    return ui->chkSaveExpectedTokens->isChecked();
}

void frmSaveDataset::setSaveExpectedTokens(bool value)
{
    ui->chkSaveExpectedTokens->setChecked(value);
}

bool frmSaveDataset::saveErrorFile() const
{
    return ui->chkSaveErrorEmptyFile->isChecked();
}

void frmSaveDataset::setSaveErrorFile(bool value)
{
    ui->chkSaveErrorEmptyFile->setChecked(value);

}


