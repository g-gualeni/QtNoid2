#include "frmsavedataset.h"
#include "ui_frmsavedataset.h"
#include <QFileDialog>
#include <QtNoidApp/QtNoidApp>

frmSaveDataset::frmSaveDataset(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::frmSaveDataset)
{
    ui->setupUi(this);
}

frmSaveDataset::~frmSaveDataset()
{
    delete ui;
}

void frmSaveDataset::setCollectionFolder(const QString &collectionFolder)
{
    ui->txtCollectionFolder->setText(collectionFolder);
}

QString frmSaveDataset::collectionFolder() const
{
    auto current = ui->txtCollectionFolder->text();
    if(!current.isEmpty()) {
        current = QtNoid::App::Core::appExeOrAppBundleDirPath() +"/" + current;
    }
    return current;
}


void frmSaveDataset::setDatasetFolder(const QString &datasetFolder)
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

    currentFolder = QDir(QtNoid::App::Core::appExeOrAppBundleDirPath()).relativeFilePath(currentFolder);
    ui->txtCollectionFolder->setText(currentFolder);
}


void frmSaveDataset::on_cmdBrowseDestination_clicked()
{
    QString currentFolder = collectionFolder() + "/" + datasetFolder();
    currentFolder = QFileDialog::getExistingDirectory(
        this,
        tr("Select Dataset Folder"),
        currentFolder
    );

    if (currentFolder.isEmpty()) {
        return;
    }
    currentFolder = QDir(collectionFolder()).relativeFilePath(currentFolder);
    ui->txtDatasetFolder->setText(currentFolder);
}


void frmSaveDataset::updateUI_dataSetFolder()
{
    if(m_baseFolderPath.isEmpty())
        return;
    if(m_filePath.isEmpty())
        return;

    QDir dir(m_baseFolderPath);
    QFileInfo FI(m_filePath);
    auto destinationFolder = dir.relativeFilePath(FI.absolutePath());
    ui->txtDatasetFolder->setText(destinationFolder);

}


bool frmSaveDataset::saveDescription() const
{
    return ui->chkSaveDescription->isChecked();
}

bool frmSaveDataset::saveInputYAML() const
{
    return ui->chkSaveInputYAML->isChecked();
}

bool frmSaveDataset::saveExpectedJSON() const
{
    return ui->chkSaveExpectedJSON->isChecked();
}

bool frmSaveDataset::saveExpectedTokens() const
{
    return ui->chkSaveExpectedTokens->isChecked();
}

bool frmSaveDataset::saveErrorEmptyFile() const
{
    return ui->chkSaveErrorEmptyFile->isChecked();
}


