#ifndef DLGFILEDIFF_H
#define DLGFILEDIFF_H

#include <QDialog>
#include <QPlainTextEdit>
#include <QSplitter>

class QLabel;

class DlgFileDiff : public QDialog
{
    Q_OBJECT

public:
    explicit DlgFileDiff(QWidget *parent = nullptr);
    ~DlgFileDiff();

    void setFiles(const QString &sourceFilePath, const QString &destFilePath);
    void setTitles(const QString &sourceTitle, const QString &destTitle);
    void setNavigationInfo(int currentIndex, int totalCount);

signals:
    void previousFileRequested();
    void nextFileRequested();

private slots:
    void onOpenSourceInEditor();
    void onOpenDestInEditor();
    void onRefresh();

private:
    void loadAndCompare();
    void highlightDifferences(const QStringList &sourceLines, const QStringList &destLines);
    void openFileInEditor(const QString &filePath);

private:
    QPlainTextEdit *m_sourceEdit;
    QPlainTextEdit *m_destEdit;
    QString m_sourceFilePath;
    QString m_destFilePath;
    QLabel *m_navLabel;           // Shows "X of Y files"
};

#endif // DLGFILEDIFF_H
