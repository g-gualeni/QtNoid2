#ifndef FRMSAVEDATASET_H
#define FRMSAVEDATASET_H

#include <QDialog>

namespace Ui {
class frmSaveDataset;
}

class frmSaveDataset : public QDialog
{
    Q_OBJECT

public:
    explicit frmSaveDataset(QWidget *parent = nullptr);
    ~frmSaveDataset();
    void setCollectionFolder(const QString &collectionFolder);
    QString collectionFolder() const;
    void setDatasetFolder(const QString &datasetFolder);
    QString datasetFolder() const;

    bool saveDescription() const;
    bool saveInputYAML() const;
    bool saveExpectedJSON() const;
    bool saveExpectedTokens() const;
    bool saveErrorEmptyFile() const;

private slots:
    void on_cmdBrowseCollection_clicked();
    void on_cmdBrowseDestination_clicked();

private:
    void updateUI_dataSetFolder();

private:
    Ui::frmSaveDataset *ui;
    QString m_baseFolderPath;
    QString m_filePath;
};

#endif // FRMSAVEDATASET_H
