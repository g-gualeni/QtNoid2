#ifndef FRMNEWDATASET_H
#define FRMNEWDATASET_H

#include <QDialog>

namespace Ui {
class frmNewDataset;
}

class frmNewDataset : public QDialog
{
    Q_OBJECT

public:
    explicit frmNewDataset(QWidget *parent = nullptr);
    ~frmNewDataset();

    void setRootFolder(const QString &rootFolder);
    QString rootFolder() const;

    QString datasetFolder() const;
    void setDatasetFolder(const QString &datasetFolder);

    QString description() const;
    void setDescription(const QString &description);

    QString yamlInput() const;
    void setYamlInput(const QString& yaml);


private slots:
    void onCmdBrowseRootFolder();
    void onCmdBrowseDatasetFolder();
    void onTxtDatasetTextChanged(const QString &text);

private:
    void loadDescription(const QString &folder);
    void loadYamlInput(const QString &folder);

private:
    Ui::frmNewDataset *ui;
};

#endif // FRMNEWDATASET_H
