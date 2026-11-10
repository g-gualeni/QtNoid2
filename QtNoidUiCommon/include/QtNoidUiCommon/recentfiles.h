#ifndef QTNOID_UICOMMON_RECENTFILES_H
#define QTNOID_UICOMMON_RECENTFILES_H

#include "global.h"
#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>

class QAction;
class QMenu;

namespace QtNoid {
namespace UiCommon {

/**
 * @brief Most recently used file list, with a "Recent Files" submenu kept in sync.
 *
 * The most recent file is always the first one. Adding a file that is already
 * in the list moves it to the front, and the list never grows beyond maxCount().
 * To save and restore the list use files() and setFiles().
 *
 * The submenu is always created by the class, in one of two ways:
 * - setMenu(parentMenu, title): the submenu is added to parentMenu (for example
 *   the File menu) and parentMenu is its parent;
 * - setAction(action): the submenu is attached to an action prepared by the
 *   programmer, and the widget that owns the action is its parent.
 * Calling setMenu() or setAction() again moves the submenu: the old one is
 * detached at once and deleted later.
 */
class QTNOID_UICOMMON_EXPORT RecentFiles : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList files READ files WRITE setFiles NOTIFY filesChanged)
    Q_PROPERTY(int maxCount READ maxCount WRITE setMaxCount NOTIFY maxCountChanged)
    Q_PROPERTY(int count READ count NOTIFY filesChanged)

public:
    static constexpr int DefaultMaxCount = 10;
    static constexpr int DefaultElideWidth = 300;   // pixels

    explicit RecentFiles(QObject *parent = nullptr);
    RecentFiles(QMenu *parentMenu, const QString &title, QObject *parent = nullptr);
    explicit RecentFiles(QAction *action, QObject *parent = nullptr);
    ~RecentFiles() override;

    // List management
    QStringList files() const;
    void setFiles(const QStringList &files);
    bool addFile(const QString &filePath);
    bool removeFile(const QString &filePath);
    bool contains(const QString &filePath) const;
    void clear();
    int count() const;
    bool isEmpty() const;

    int maxCount() const;
    void setMaxCount(int maxCount);

    // Menu
    void setMenu(QMenu *parentMenu, const QString &title, QAction *before = nullptr);
    void setAction(QAction *action);
    QMenu *menu() const;
    int elideWidth() const;
    void setElideWidth(int pixels);

signals:
    void fileSelected(const QString &filePath);
    void filesChanged(const QStringList &files);
    void maxCountChanged(int maxCount);
    void cleared();

private:
    QStringList m_files;
    int m_maxCount = DefaultMaxCount;
    int m_elideWidth = DefaultElideWidth;
    QPointer<QMenu> m_menu;          // the submenu created by the class
    QPointer<QMenu> m_parentMenu;    // setMenu(): the menu that contains the submenu
    QPointer<QAction> m_action;      // setAction(): the action prepared by the programmer
    QMetaObject::Connection m_actionConnection;

    static QString normalizedPath(const QString &filePath);
    static QStringList cleanList(const QStringList &files, int maxCount);
    static bool samePath(const QString &a, const QString &b);
    static QWidget *actionWidget(QAction *action);
    void updateFiles(const QStringList &files);
    void detachMenu(bool deleteLater);
    void rebuildMenu();
};

} // namespace UiCommon
} // namespace QtNoid

#endif // QTNOID_UICOMMON_RECENTFILES_H
