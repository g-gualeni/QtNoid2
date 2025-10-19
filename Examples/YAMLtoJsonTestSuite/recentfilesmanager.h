#ifndef RECENTFILESMANAGER_H
#define RECENTFILESMANAGER_H

#include <QObject>
#include <QAction>
#include <QWidget>
#include <QStringList>

class recentFilesManager : public QObject
{
    Q_OBJECT

public:
    recentFilesManager(QAction *recentFilesParent, QWidget *parent);
    void updateRecentFilesMenu(const QStringList &recentFiles);

signals:
    void fileSelected(const QString& fileName);
    void listCleared();

private:
    QAction* m_recentFilesParent = nullptr;
    QWidget* m_parentWidget = nullptr;
};

#endif // RECENTFILESMANAGER_H
