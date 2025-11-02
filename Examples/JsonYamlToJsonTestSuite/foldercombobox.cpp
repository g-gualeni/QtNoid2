#include "foldercombobox.h"
#include <QEvent>
#include <QMouseEvent>

FolderComboBox::FolderComboBox(QWidget *parent)
    : QComboBox(parent)
{
}

bool FolderComboBox::event(QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        qDebug() << __func__ << event;
        this->showPopup();
        return true;
    }
    else if (event->type() == QEvent::MouseButtonDblClick) {
        qDebug() << __func__ << event;
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
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
