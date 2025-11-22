#include "frmnewdataset.h"
#include "ui_frmnewdataset.h"
#include "QtNoidApp/QtNoidApp"
#include <QFileDialog>
#include <QDir>

frmNewDataset::frmNewDataset(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::frmNewDataset)
{
    ui->setupUi(this);

    connect(ui->cmdBrowseRootFolder, &QToolButton::clicked, this, &frmNewDataset::onCmdBrowseRootFolder);
    connect(ui->cmdBrowseDatasetFolder, &QToolButton::clicked, this, &frmNewDataset::onCmdBrowseDatasetFolder);
    connect(ui->txtDatasetFolder, &QLineEdit::textChanged, this, &frmNewDataset::onTxtDatasetTextChanged);

}

frmNewDataset::~frmNewDataset()
{
    delete ui;
}


void frmNewDataset::setRootFolder(const QString &rootFolder)
{
    QDir dir(QtNoid::App::Settings::appExeOrAppBundleDirPath());
    auto relativeRootFolder = dir.relativeFilePath(rootFolder);
    ui->txtRootFolder->setText(relativeRootFolder);
}
QString frmNewDataset::rootFolder() const
{
    auto res =  QtNoid::App::Settings::appExeOrAppBundleDirPath() + QDir::separator() + ui->txtRootFolder->text();
    return QDir::cleanPath(res);
}


QString frmNewDataset::description() const
{
    return ui->txtDescription->text();
}
void frmNewDataset::setDescription(const QString &description)
{
    ui->txtDescription->setText(description);
}


QString frmNewDataset::yamlInput() const
{
    return ui->txtYamlInput->toPlainText();
}
void frmNewDataset::setYamlInput(const QString &yaml)
{
    ui->txtYamlInput->setPlainText(yaml);
}


QString frmNewDataset::datasetFolder() const
{
    return ui->txtDatasetFolder->text();
}
void frmNewDataset::setDatasetFolder(const QString &datasetFolder)
{
    ui->txtDatasetFolder->setText(datasetFolder);
}


void frmNewDataset::onCmdBrowseRootFolder()
{
    // qDebug() << __func__;
    QString rootFolderPath = QFileDialog::getExistingDirectory(
        this,
        tr("Select Destination Folder"),
        rootFolder()
    );

    if (rootFolderPath.isEmpty()) {
        return;
    }

    setRootFolder(rootFolderPath);
}

void frmNewDataset::onCmdBrowseDatasetFolder()
{
    // qDebug() << __func__;
    QString currentFolder = rootFolder() + QDir::separator() + datasetFolder();
    // qDebug() << __func__ << currentFolder;

    currentFolder = QFileDialog::getExistingDirectory(
        this,
        tr("Select Destination Folder"),
        currentFolder
        );

    if (currentFolder.isEmpty()) {
        return;
    }

    // Make it relative
    QDir dir(rootFolder());
    setDatasetFolder(dir.relativeFilePath(currentFolder));
}

void frmNewDataset::onTxtDatasetTextChanged(const QString &text)
{
    auto datasetFolderPath = rootFolder() + QDir::separator() + text;
    QDir dir(datasetFolderPath);
    if(!dir.exists()){
        ui->txtYamlInput->clear();
        ui->txtDescription->clear();
        return;
    }
    // qDebug() << __func__ << text << "EXISTS";
    loadDescription(datasetFolderPath);
    loadYamlInput(datasetFolderPath);

}


void frmNewDataset::loadDescription(const QString &folder)
{
    auto filePath = folder + "/===";
    QFile file(filePath);
    QString descriptionContent;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        descriptionContent = in.readAll();
        file.close();
        ui->txtDescription->setText(descriptionContent);
    }
    // qDebug() << __func__ ;
}


void frmNewDataset::loadYamlInput(const QString &folder)
{
    auto filePath = folder + "/in.yaml";
    QFile file(filePath);
    QString yamlContent;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        yamlContent = in.readAll();
        file.close();
        ui->txtYamlInput->setPlainText(yamlContent);
    }
    // qDebug() << __func__ ;
}




