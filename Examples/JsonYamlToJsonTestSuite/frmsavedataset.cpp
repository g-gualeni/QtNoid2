#include "frmsavedataset.h"
#include "ui_frmsavedataset.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QtNoidApp/QtNoidApp>

void frmSaveDataset::updateUi_cmdSelectAll()
{
    if(chkAllSelected()) {
        ui->cmdSelectAll->setText("Unselect All");
    }
    else {
        ui->cmdSelectAll->setText("Select All");
    }
}

frmSaveDataset::frmSaveDataset(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::frmSaveDataset)
{
    ui->setupUi(this);
    updateUi_cmdSelectAll();
    connect(ui->cmdSelectAll, &QToolButton::clicked, this, &frmSaveDataset::onCmdSelectAllClicked);
    restoreFromAppConfig();
}

frmSaveDataset::~frmSaveDataset()
{
    saveToAppConfig();
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
    QDir collectionFolderDir(QtNoid::App::Core::appExeOrAppBundleDirPath());
    collectionFolderDir.setPath(collectionFolderDir.absoluteFilePath(collectionFolder()));

    QString currentFolder = collectionFolderDir.absoluteFilePath(datasetFolder());
    // qDebug() << Q_FUNC_INFO << currentFolder;

    currentFolder = QFileDialog::getExistingDirectory(
        this,
        tr("Select Dataset Folder"),
        currentFolder
    );

    if (currentFolder.isEmpty()) {
        return;
    }

    // QDir::isAbsolutePath()


    /// CONTROLLA CHE SIA UN FIGLIO

    auto datasetPath = collectionFolderDir.relativeFilePath(currentFolder);
    if(!currentFolder.isEmpty()
        && datasetPath != "."
        && datasetPath != ".."
        && !datasetPath.startsWith("../")
        && QDir::isRelativePath(datasetPath))
    {
        ui->txtDatasetFolder->setText(datasetPath);
        return;
    }

    // Invalid path selected
    QMessageBox::critical(this, tr("Dataset Folder"),
                          tr("%1\nIs not a valid path")
                              .arg(currentFolder));

}

void frmSaveDataset::saveToAppConfig()
{
    const QString pageName = QtNoid::App::Core::groupNameFromObjectOrClass(this);
    appConfig->saveValue("geometry", saveGeometry(), pageName);
}

void frmSaveDataset::restoreFromAppConfig()
{
    const QString pageName = QtNoid::App::Core::groupNameFromObjectOrClass(this);
    restoreGeometry(appConfig->restoreAsByteArray("geometry", saveGeometry(), pageName));
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
    updateUi_cmdSelectAll();
}

bool frmSaveDataset::saveExpectedJSON() const
{
    return ui->chkSaveExpectedJSON->isChecked();
}

void frmSaveDataset::setSaveExpectedJSON(bool value)
{
    ui->chkSaveExpectedJSON->setChecked(value);
    updateUi_cmdSelectAll();
}

bool frmSaveDataset::saveExpectedTokens() const
{
    return ui->chkSaveExpectedTokens->isChecked();    
}

void frmSaveDataset::setSaveExpectedTokens(bool value)
{
    ui->chkSaveExpectedTokens->setChecked(value);
    updateUi_cmdSelectAll();
}

bool frmSaveDataset::saveErrorFile() const
{
    return ui->chkSaveErrorEmptyFile->isChecked();
}

void frmSaveDataset::setSaveErrorFile(bool value)
{
    ui->chkSaveErrorEmptyFile->setChecked(value);
    updateUi_cmdSelectAll();
}

bool frmSaveDataset::chkAllSelected()
{
    bool allSelected = ui->chkSaveDescription->isChecked();
    allSelected &= ui->chkSaveErrorEmptyFile->isChecked();
    allSelected &= ui->chkSaveExpectedJSON->isChecked();
    allSelected &= ui->chkSaveInputYAML->isChecked();
    allSelected &= ui->chkSaveExpectedTokens->isChecked();

    qDebug() << __func__ << "allSelected" << allSelected;
    // qDebug() << __func__ << "chkSaveDescription" << ui->chkSaveDescription->isChecked();;

    return allSelected;
}

void frmSaveDataset::onCmdSelectAllClicked()
{
    bool newStatus = !chkAllSelected();
    ui->chkSaveDescription->setChecked(newStatus);
    ui->chkSaveErrorEmptyFile->setChecked(newStatus);
    ui->chkSaveExpectedJSON->setChecked(newStatus);
    ui->chkSaveInputYAML->setChecked(newStatus);
    ui->chkSaveExpectedTokens->setChecked(newStatus);
    updateUi_cmdSelectAll();
}

