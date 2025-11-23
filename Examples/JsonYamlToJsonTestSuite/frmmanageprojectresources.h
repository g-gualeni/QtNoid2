#ifndef FRMMANAGEPROJECTRESOURCES_H
#define FRMMANAGEPROJECTRESOURCES_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class frmManageProjectResources;
}

class frmManageProjectResources : public QDialog
{
    Q_OBJECT

public:
    explicit frmManageProjectResources(QWidget *parent = nullptr);
    ~frmManageProjectResources();

    void setSourceFolder(const QString &folder);
    QString sourceFolder() const;

    void setResourcePrefix(const QString &prefix);
    QString resourcePrefix() const;

    void setQrcPrefix(const QString &prefix);
    QString qrcPrefix() const;

    void setMapFileName(const QString &fileName);
    QString mapFileName() const;

private slots:
    void onCmdBrowseSourceFolder();
    void onCmdScanFolder();
    void onCmdGenerateMapFile();
    void onCmdGenerateQrcEntries();

private:
    void scanFolderRecursively(const QString &path, QStringList &fileList);
    void updateStatistics();
    QString generateMapFileContent();
    QString generateQrcEntriesContent();
    QString getRelativePath(const QString &fullPath);

private:
    Ui::frmManageProjectResources *ui;
    QStringList m_fileList;
    int m_folderCount;
};

#endif // FRMMANAGEPROJECTRESOURCES_H
