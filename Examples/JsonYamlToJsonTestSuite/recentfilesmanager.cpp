#include "recentfilesmanager.h"
#include <QMenu>
#include <QDebug>

recentFilesManager::recentFilesManager(QAction *recentFilesParent, QWidget *parent) :
    QObject(parent), m_parentWidget(parent), m_recentFilesParent(recentFilesParent)
{
}

void recentFilesManager::updateRecentFilesMenu(const QStringList &recentFiles)
{
    if(m_recentFilesParent == nullptr)
        return;

    QMenu *recentFilesMenu = m_recentFilesParent->menu();
    if (!recentFilesMenu) {
        // if the menu is not there, we create it
        recentFilesMenu = new QMenu(m_parentWidget);
        m_recentFilesParent->setMenu(recentFilesMenu);
    }
    recentFilesMenu->clear();

    for (int ii = 0; ii < recentFiles.size() && ii < 10; ++ii) {
        QString fileName = recentFiles[ii];

        auto fileTitle = recentFilesMenu->fontMetrics().elidedText(fileName, Qt::ElideMiddle, 200);
        QString displayName = QString("%1| %2").arg(ii + 1).arg(fileTitle);

        QAction *fileAction = recentFilesMenu->addAction(displayName);
        fileAction->setData(fileName); // save the path without elision

        // Connect the action
        connect(fileAction, &QAction::triggered, this, [this, fileName](){
            emit fileSelected(fileName);
        });
    }
    // add separator and a command to reset the menu content
    if (!recentFiles.isEmpty()) {
        recentFilesMenu->addSeparator();
        QAction *clearAction = recentFilesMenu->addAction(tr("Clear Recent Files"));
        connect(clearAction, &QAction::triggered, this, [this, recentFilesMenu]() {
            recentFilesMenu->clear();
            // qDebug() << "clearRecentFiles()";
            emit listCleared();
        });
    }
}
