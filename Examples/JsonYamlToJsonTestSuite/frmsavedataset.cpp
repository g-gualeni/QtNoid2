#include "frmsavedataset.h"
#include "ui_frmsavedataset.h"
#include <QFileDialog>

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

void frmSaveDataset::on_cmdBrowseDestination_clicked()
{

    auto currentFolder = m_baseFolderPath + QDir::separator() + ui->txtDestinationFolder->text();
    QString dataSetFolderPath = QFileDialog::getExistingDirectory(
        this,
        tr("Select Destination Folder"),
        currentFolder
    );


    if (dataSetFolderPath.isEmpty()) {
        return;
    }
    QDir dir(m_baseFolderPath);
    ui->txtDestinationFolder->setText(dir.relativeFilePath(dataSetFolderPath));
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
    ui->txtDestinationFolder->setText(destinationFolder);

}

void frmSaveDataset::setFilePath(const QString &newFilePath)
{
    m_filePath = newFilePath;
    updateUI_dataSetFolder();
}

void frmSaveDataset::setBaseFolderPath(const QString &baseFolderPath)
{
    m_baseFolderPath = baseFolderPath;
    updateUI_dataSetFolder();
}

QString frmSaveDataset::destinationFolder() const
{
    return ui->txtDestinationFolder->text();
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
