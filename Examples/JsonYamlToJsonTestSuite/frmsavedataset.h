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
    explicit frmSaveDataset(QWidget* parent = nullptr);
    ~frmSaveDataset();
    void setCollectionFolder(const QString& collectionFolder);
    QString collectionFolder() const;
    void setDatasetFolder(const QString& datasetFolder);
    QString datasetFolder() const;


    bool saveDescription() const;
    void setSaveDescription(bool value);
    bool saveInputYAML() const;
    void setSaveInputYAML(bool value);
    bool saveExpectedJSON() const;
    void setSaveExpectedJSON(bool value);
    bool saveExpectedTokens() const;
    void setSaveExpectedTokens(bool value);
    bool saveErrorFile() const;
    void setSaveErrorFile(bool value);

private slots:
    void on_cmdBrowseCollection_clicked();
    void on_cmdBrowseDestination_clicked();
    void onCmdSelectAllClicked();

protected:
    void saveToAppConfig();
    void restoreFromAppConfig();
    bool chkAllSelected();
    void updateUi_cmdSelectAll();



private:
    Ui::frmSaveDataset* ui;
};

#endif // FRMSAVEDATASET_H
