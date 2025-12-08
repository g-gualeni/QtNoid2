#ifndef FRMMANAGEPROJECTRESOURCES_H
#define FRMMANAGEPROJECTRESOURCES_H

#include <QDialog>
#include <QStringList>
#include <QStandardItemModel>

namespace Ui {
class frmManageProjectResources;
}

class frmManageProjectResources : public QDialog
{
    Q_OBJECT

public:
    explicit frmManageProjectResources(QWidget *parent = nullptr);
    ~frmManageProjectResources();

    QString projectFolder() const;
    void setProjectFolder(const QString &newProjectFolder);

    QString localFolder() const;
    void setLocalFolder(const QString &newLocalFolder);

    QString projectResFile() const;
    void setProjectResFile(const QString &newProjectResFile);

    QString projectResPrefix() const;
    void setProjectResPrefix(const QString &newProjectResPrefix);

    // File comparison model methods
    void addFileComparison(const QString &sourceFile, const QString &destinationFile);
    void addDestinationFiles(const QStringList &sourceFiles);

    void clearFileComparisons();
    int fileComparisonCount() const;

private slots:

    void on_txtProjectResPrefix_currentTextChanged(const QString &arg1);

private:
    void updateUi_resourceFileList();
    void updateUi_resourceFilePrefix();
    void updateUi_filesInPrefix();

private:
    Ui::frmManageProjectResources *ui;
    QStandardItemModel *m_fileComparisonModel;
};

#endif // FRMMANAGEPROJECTRESOURCES_H
