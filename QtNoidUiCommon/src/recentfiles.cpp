#include "QtNoidUiCommon/recentfiles.h"

#include <QAction>
#include <QDir>
#include <QFontMetrics>
#include <QMenu>

namespace QtNoid {
namespace UiCommon {


RecentFiles::RecentFiles(QObject *parent)
    : QObject(parent)
{
}


RecentFiles::RecentFiles(QMenu *parentMenu, const QString &title, QObject *parent)
    : QObject(parent)
{
    setMenu(parentMenu, title);
}


RecentFiles::RecentFiles(QAction *action, QObject *parent)
    : QObject(parent)
{
    setAction(action);
}


RecentFiles::~RecentFiles()
{
    detachMenu(false);
}


QStringList RecentFiles::files() const
{
    return m_files;
}


void RecentFiles::setFiles(const QStringList &files)
{
    updateFiles(cleanList(files, m_maxCount));
}


bool RecentFiles::addFile(const QString &filePath)
{
    const QString path = normalizedPath(filePath);
    if (path.isEmpty())
        return false;

    QStringList list = m_files;
    list.removeIf([&path](const QString &item) { return samePath(item, path); });
    list.prepend(path);
    if (list.size() > m_maxCount)
        list.resize(m_maxCount);

    updateFiles(list);
    return true;
}


bool RecentFiles::removeFile(const QString &filePath)
{
    const QString path = normalizedPath(filePath);
    QStringList list = m_files;
    const auto removed = list.removeIf([&path](const QString &item) { return samePath(item, path); });
    if (removed == 0)
        return false;

    updateFiles(list);
    return true;
}


bool RecentFiles::contains(const QString &filePath) const
{
    const QString path = normalizedPath(filePath);
    if (path.isEmpty())
        return false;

    for (const QString &item : m_files) {
        if (samePath(item, path))
            return true;
    }
    return false;
}


void RecentFiles::clear()
{
    if (m_files.isEmpty())
        return;

    updateFiles({});
    emit cleared();
}


int RecentFiles::count() const
{
    return int(m_files.size());
}


bool RecentFiles::isEmpty() const
{
    return m_files.isEmpty();
}


int RecentFiles::maxCount() const
{
    return m_maxCount;
}


void RecentFiles::setMaxCount(int maxCount)
{
    if (maxCount < 1 || maxCount == m_maxCount)
        return;

    m_maxCount = maxCount;
    emit maxCountChanged(m_maxCount);

    if (m_files.size() > m_maxCount)
        updateFiles(m_files.mid(0, m_maxCount));
}


void RecentFiles::setMenu(QMenu *parentMenu, const QString &title, QAction *before)
{
    detachMenu(true);
    if (parentMenu == nullptr)
        return;

    // The submenu is a child of parentMenu: it is deleted together with it.
    // The visible entry is the submenu's menuAction(), owned by the submenu.
    m_parentMenu = parentMenu;
    m_menu = new QMenu(title, parentMenu);
    if (before != nullptr)
        parentMenu->insertMenu(before, m_menu);
    else
        parentMenu->addMenu(m_menu);

    rebuildMenu();
}


void RecentFiles::setAction(QAction *action)
{
    detachMenu(true);
    if (action == nullptr)
        return;

    // A QMenu cannot have a QAction as parent: use the widget that owns the
    // action, so the submenu is deleted together with the window.
    m_action = action;
    m_menu = new QMenu(action->text(), actionWidget(action));
    action->setMenu(m_menu.data());

    // If only the action is deleted, its submenu must not stay orphan
    m_actionConnection = connect(action, &QObject::destroyed, this, [this]() {
        if (m_menu)
            m_menu->deleteLater();
        m_menu = nullptr;
        m_action = nullptr;
    });

    rebuildMenu();
}


QMenu *RecentFiles::menu() const
{
    return m_menu;
}


int RecentFiles::elideWidth() const
{
    return m_elideWidth;
}


void RecentFiles::setElideWidth(int pixels)
{
    if (pixels < 0 || pixels == m_elideWidth)
        return;

    m_elideWidth = pixels;
    rebuildMenu();
}


QString RecentFiles::normalizedPath(const QString &filePath)
{
    const QString trimmed = filePath.trimmed();
    if (trimmed.isEmpty())
        return {};

    return QDir::cleanPath(QDir::fromNativeSeparators(trimmed));
}


QStringList RecentFiles::cleanList(const QStringList &files, int maxCount)
{
    QStringList list;
    for (const QString &file : files) {
        const QString path = normalizedPath(file);
        if (path.isEmpty())
            continue;

        bool duplicate = false;
        for (const QString &item : std::as_const(list)) {
            if (samePath(item, path)) {
                duplicate = true;
                break;
            }
        }
        if (duplicate)
            continue;

        list << path;
        if (list.size() >= maxCount)
            break;
    }
    return list;
}


bool RecentFiles::samePath(const QString &a, const QString &b)
{
#ifdef Q_OS_WIN
    return a.compare(b, Qt::CaseInsensitive) == 0;
#else
    return a == b;
#endif
}


QWidget *RecentFiles::actionWidget(QAction *action)
{
    if (auto *widget = qobject_cast<QWidget*>(action->parent()))
        return widget;

    // Fall back to the first widget that shows the action, for example a menu
    const QList<QObject*> objects = action->associatedObjects();
    for (QObject *object : objects) {
        if (auto *widget = qobject_cast<QWidget*>(object))
            return widget;
    }
    return nullptr;
}


void RecentFiles::detachMenu(bool deleteLater)
{
    if (m_actionConnection)
        disconnect(m_actionConnection);

    QMenu *menu = m_menu.data();

    if (m_action) {
        if (menu != nullptr && m_action->menu() == menu)
            m_action->setMenu(static_cast<QMenu*>(nullptr));
        m_action->setEnabled(true);      // give the action back in its normal state
    }

    if (menu != nullptr) {
        if (m_parentMenu)
            m_parentMenu->removeAction(menu->menuAction());
        // Delete later when the submenu is moved: this can happen inside the
        // triggered() signal of one of its actions.
        if (deleteLater)
            menu->deleteLater();
        else
            delete menu;
    }

    m_menu = nullptr;
    m_parentMenu = nullptr;
    m_action = nullptr;
}


void RecentFiles::updateFiles(const QStringList &files)
{
    if (files == m_files)
        return;

    m_files = files;
    rebuildMenu();
    emit filesChanged(m_files);
}


void RecentFiles::rebuildMenu()
{
    if (m_menu.isNull())
        return;

    // Detach the old actions first and delete them later: this method can be
    // called from inside the triggered() signal of one of those actions.
    const QList<QAction*> oldActions = m_menu->actions();
    for (QAction *action : oldActions) {
        m_menu->removeAction(action);
        if (action->parent() == m_menu)
            action->deleteLater();
    }

    const QFontMetrics metrics = m_menu->fontMetrics();
    for (int ii = 0; ii < m_files.size(); ++ii) {
        const QString filePath = m_files.at(ii);
        const QString nativePath = QDir::toNativeSeparators(filePath);

        QString title = metrics.elidedText(nativePath, Qt::ElideMiddle, m_elideWidth);
        title.replace('&', "&&");   // a '&' in a file name is not a shortcut
        const QString text = (ii < 9) ? QString("&%1 %2").arg(ii + 1).arg(title)
                                      : QString("%1 %2").arg(ii + 1).arg(title);

        QAction *action = m_menu->addAction(text);
        action->setData(filePath);
        action->setToolTip(nativePath);
        action->setStatusTip(nativePath);
        connect(action, &QAction::triggered, this, [this, filePath]() {
            emit fileSelected(filePath);
        });
    }

    if (!m_files.isEmpty()) {
        m_menu->addSeparator();
        QAction *clearAction = m_menu->addAction(tr("Clear Recent Files"));
        clearAction->setObjectName("actionClearRecentFiles");
        connect(clearAction, &QAction::triggered, this, &RecentFiles::clear);
    }

    // The visible entry: the submenu's menuAction(). After action->setMenu()
    // Qt returns the programmer's action as menuAction(), so both modes work.
    m_menu->menuAction()->setEnabled(!m_files.isEmpty());
}

} // namespace UiCommon
} // namespace QtNoid
