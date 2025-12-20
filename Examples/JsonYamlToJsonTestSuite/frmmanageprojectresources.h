#ifndef FRMMANAGEPROJECTRESOURCES_H
#define FRMMANAGEPROJECTRESOURCES_H

#include <QDialog>
#include <QStringList>
#include <QStandardItemModel>

class DlgFileDiff;

namespace Ui {
class frmManageProjectResources;
}

class frmManageProjectResources : public QDialog
{
    Q_OBJECT

public:
    enum Column {
        ColSource = 0,
        ColUpdateSource,
        ColStatus,
        ColAction,
        ColDestination,
        ColCount
    };

    explicit frmManageProjectResources(QWidget *parent = nullptr);
    ~frmManageProjectResources();

    QString sourceBaseFolder() const;
    void setSourceBaseFolder(const QString &newSourceBaseFolder);
    QString sourceSubFolder() const;
    void setSourceSubFolder(const QString &newSourceSubFolder);
    QString sourceFolder() const;
    QString sourceAbsoluteFile(const QString &relFile) const;

    QString destinationFolder() const;
    void setDestinationFolder(const QString &newDestinationFolder);

    QString destinationResourceFile() const;
    QString destinationResourceAbsoluteFile() const;
    void setDestinationResourceFile(const QString& newDestinationResourceFile);

    QString destinationAbsoluteFile(const QString &relFile) const;

    QString projectResPrefix() const;
    void setProjectResPrefix(const QString &newProjectResPrefix);

    // File comparison model methods
    void addFileComparison(const QString &sourceFile, const QString &status, const QString &destinationFile);

    void updateFileCompare();
    void clearFileComparisons();
    int fileComparisonCount() const;


protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_cmdUpdate_clicked();
    void onCopyButtonClicked(int row);
    void onUpdateSourceButtonClicked(int row);
    void createVisibleButtons();
    void onTableDoubleClicked(const QModelIndex &index);
    void onPreviousFile();
    void onNextFile();

private:
    void updateUi_resourceFilePrefix();
    void updateTableFooter();

    void loadSourceFileList();
    void loadDestinationFileList();
    void createButtonForRow(int row);
    void compareFileForRow(int row);
    void updateVisibleRows();
    void showDiffForRow(int row);
    int findNextDiffRow(int fromRow, int direction);

private:
    Ui::frmManageProjectResources *ui;
    QStandardItemModel *m_fileComparisonModel;
    QSet<QString> m_sourceFileList;
    QSet<QString> m_destinationFileList;
    QSet<int> m_comparedRows;
    DlgFileDiff *m_diffDialog = nullptr;
    int m_currentDiffRow = -1;

};

#endif // FRMMANAGEPROJECTRESOURCES_H
