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

    QString sourceBaseFolder() const;
    void setSourceBaseFolder(const QString &newSourceBaseFolder);

    QString sourceSubFolder() const;
    void setSourceSubFolder(const QString &newSourceSubFolder);

    QString sourceFolder() const;

    QString projectResFile() const;
    void setProjectResFile(const QString &newProjectResFile);

    QString projectResPrefix() const;
    void setProjectResPrefix(const QString &newProjectResPrefix);

    // File comparison model methods
    void addFileComparison(const QString &sourceFile, const QString &status, const QString &destinationFile);

    void updateFileCompare();
    void clearFileComparisons();
    int fileComparisonCount() const;



private slots:
    void on_txtProjectResPrefix_currentTextChanged(const QString &arg1);
    void on_cmdUpdate_clicked();
    void onCopyButtonClicked(const QModelIndex &index);

private:
    void updateUi_resourceFileList();
    void updateUi_resourceFilePrefix();
    void updateUi_filesInPrefix();

    void loadSourceFileList();
    void loadDestinationFileList();

private:
    Ui::frmManageProjectResources *ui;
    QStandardItemModel *m_fileComparisonModel;
    QSet<QString> m_sourceFileList;
    QSet<QString> m_destinationFileList;

};

#endif // FRMMANAGEPROJECTRESOURCES_H
