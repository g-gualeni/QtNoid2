#include "foldercombobox.h"
#include <QDesktopServices>
#include <QDir>
#include <QEvent>
#include <QFileInfo>
#include <QMenu>
#include <QMouseEvent>
#include "QtNoidApp/QtNoidApp"

FolderComboBox::FolderComboBox(QWidget* parent)
    : QComboBox(parent)
{
}

bool FolderComboBox::event(QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QRect dropdownRect = rect();
        dropdownRect.setLeft(width() - 20);
        if (dropdownRect.contains(mouseEvent->pos())) {
            // qDebug() << __func__ << event;
            // qDebug() << "Click sul pulsante dropdown!";
            showPopup();
            return true;
        }
    }
    else if (event->type() == QEvent::MouseButtonDblClick) {
        // qDebug() << __func__ << event;
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            emit doubleClicked();
            m_ignoreNextMouseRelease = true;
            return true;
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease && m_ignoreNextMouseRelease) {
        m_ignoreNextMouseRelease = false;
        return true;
    }

    return QComboBox::event(event);
}


void FolderComboBox::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu;
    QAction* openOsAction = menu.addAction(tr("Open folder in File Explorer"));
    QAction* resetAction = menu.addAction(tr("Reset Recent List"));
    QAction* selected = menu.exec(event->globalPos());

    if (selected == resetAction) {
        clear();
        setCurrentIndex(-1);
    }
    else if(selected == openOsAction) {
        // qDebug() << Q_FUNC_INFO << currentText();
        QDir dir(QtNoid::App::Core::appExeOrAppBundleDirPath());
        QDesktopServices::openUrl(QUrl::fromLocalFile(dir.absoluteFilePath(currentText())));
    }
}
