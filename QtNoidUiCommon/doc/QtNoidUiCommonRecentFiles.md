# Class: RecentFiles
This class, derived from `QObject`, manages the list of the most recently used files of an application and keeps a **Recent Files** submenu in sync with it. The most recent file is always the first one: adding a file that is already in the list moves it to the front instead of duplicating it, and the list never grows beyond `maxCount()`.

The class does not save the list: to save and restore it use `files()` and `setFiles()`, with any storage you like (see the [Example](#example)).

## The "Recent Files" submenu
The submenu is always created by the class. There are two ways to place it:

1. **`setMenu(parentMenu, title)`, the simplest.** Pass the menu that must contain the submenu, for example the File menu, and its title. The class creates the submenu with `parentMenu` as parent and adds it to `parentMenu`: the visible entry is the submenu's `menuAction()`. Everything the class creates has the File menu as parent, so it is deleted together with it.
2. **`setAction(action)`, the finer one.** Prepare an action in the menu yourself, for example `actionRecent_Files` in Designer, and pass it to the class. The class creates the submenu and attaches it to the action with `QAction::setMenu()`: the visible entry is your action. A `QMenu` cannot have a `QAction` as parent, so the submenu's parent is the widget that owns the action (the window, for actions created by Designer) or, if the action has no widget parent, the first menu that shows it. When the window is closed nothing is left dangling.

You can change your mind at any time: calling `setMenu()` or `setAction()` again moves the submenu. The old submenu is detached at once (its entry leaves the old menu, and the old action is given back without submenu and enabled) and it is deleted later with `deleteLater()`, so it is safe to move it even from a slot connected to `fileSelected()`. The list of files is not touched.

The submenu is created and filled by the class: do not add other actions to it, they would be replaced at the next change of the list.

## Path handling
- Every path is normalized: leading and trailing spaces are removed, the native separators are converted to `/` and the path is cleaned with `QDir::cleanPath()` (for example `C:\data\.\a.yaml` becomes `C:/data/a.yaml`). Paths are **not** made absolute: a relative path stays relative.
- Two paths are the same file if they are equal after normalization. On Windows the comparison is case insensitive, on the other platforms it is case sensitive.
- Empty paths are never added.

## Properties
- `files` (`QStringList`, read/write, notify `filesChanged`): the list of files, the most recent first.
- `maxCount` (`int`, read/write, notify `maxCountChanged`): the maximum number of files in the list. The default value is `RecentFiles::DefaultMaxCount` (10).
- `count` (`int`, read only, notify `filesChanged`): the number of files in the list.

## Static Methods
* None

## Constructors
- `RecentFiles(QObject *parent = nullptr)`: creates an empty list, without a submenu. Use this one when `RecentFiles` is a member of the window, then call `setMenu()` or `setAction()` after `ui->setupUi(this)`: when a member is constructed, the widgets created by `setupUi()` do not exist yet.
- `RecentFiles(QMenu *parentMenu, const QString &title, QObject *parent = nullptr)`: same as the default constructor followed by `setMenu(parentMenu, title)`.
- `RecentFiles(QAction *action, QObject *parent = nullptr)`: same as the default constructor followed by `setAction(action)`.

## Destructors
- `~RecentFiles()`: deletes the submenu and removes its entry from the parent menu. With `setAction()`, the action is given back without submenu and enabled. If the window or the menus were already deleted, there is nothing to do.

## List management methods
- `files()`: returns the list of files, the most recent first.
- `setFiles(const QStringList &files)`: replaces the list. The paths are normalized, empty entries and duplicates are removed (the first occurrence wins) and the list is truncated to `maxCount()`. Use it to restore a saved list.
- `addFile(const QString &filePath)`: adds `filePath` at the front of the list. If the file is already in the list it is moved to the front. If the list grows beyond `maxCount()`, the oldest files are removed. Returns false if `filePath` is empty.
- `removeFile(const QString &filePath)`: removes `filePath` from the list, for example when the file does not exist anymore. Returns false if the file is not in the list.
- `contains(const QString &filePath)`: returns true if `filePath` is in the list.
- `clear()`: removes all the files and emits `cleared()`. Nothing happens if the list is already empty.
- `count()`: returns the number of files in the list.
- `isEmpty()`: returns true if the list is empty.
- `maxCount()`: returns the maximum number of files in the list.
- `setMaxCount(int maxCount)`: sets the maximum number of files. If the list is longer, the oldest files are removed. Values lower than 1 are ignored.

## Menu methods
- `setMenu(QMenu *parentMenu, const QString &title, QAction *before = nullptr)`: creates the submenu `title` and adds it to `parentMenu`, at the end or before the action `before`. If a submenu already exists, it is moved (see [The "Recent Files" submenu](#the-recent-files-submenu)). Pass `nullptr` as `parentMenu` to remove the submenu.
- `setAction(QAction *action)`: creates the submenu and attaches it to `action`. The title of the submenu is the text of the action. If a submenu already exists, it is moved. If the action is deleted, the submenu is deleted too. Pass `nullptr` to remove the submenu.
- `menu()`: returns the submenu created by the class, or `nullptr` if there is none or it was deleted together with its parent. The list keeps working without a submenu.
- `elideWidth()`: returns the maximum width, in pixels, of a file path in the menu. The default value is `RecentFiles::DefaultElideWidth` (300).
- `setElideWidth(int pixels)`: sets the maximum width of a file path in the menu and rebuilds the menu. Longer paths are elided in the middle, so that the file name stays visible; the tooltip and `QAction::data()` always keep the full path. Negative values are ignored.

The menu contains, in order:
- one action for each file, with the text `&1 path`, `&2 path`, ... (the first nine entries can be selected with the keyboard), the full native path as tooltip and status tip, and the path in `QAction::data()`. Triggering the action emits `fileSelected()`. A `&` in a file name is shown as is, it is not a shortcut;
- a separator;
- the action **Clear Recent Files** (object name `actionClearRecentFiles`), connected to `clear()`.

When the list is empty the submenu has no actions and its entry is disabled, so it is shown greyed out. This works in both ways: after `QAction::setMenu()`, Qt uses your action as the submenu's `menuAction()`, so with `setAction()` it is your action that is enabled and disabled.

It is safe to change the list from a slot connected to `fileSelected()` or `cleared()`, for example to call `addFile()` with the selected file: the old actions are removed from the submenu immediately but deleted later, with `deleteLater()`.

## Signals
- `fileSelected(const QString &filePath)`: emitted when the user selects a file in the menu.
- `filesChanged(const QStringList &files)`: emitted every time the list changes, with the new list. It is not emitted if an operation leaves the list unchanged, for example adding the file that is already the first one.
- `maxCountChanged(int maxCount)`: emitted when the maximum number of files changes.
- `cleared()`: emitted by `clear()`, also when it is called by the **Clear Recent Files** action of the menu.

## Examples
The examples save the list with [QtNoid::App::Config](../../QtNoidApp/doc/QtNoidAppConfig.md) (the global object `appConfig`), in the parameter `"RecentFiles"` of the page `"Settings"`. `RecentFiles` itself does not depend on QtNoidApp.

**1. The simplest: the class adds the submenu to the File menu.**
```cpp
#include "QtNoidApp/QtNoidApp"
#include "QtNoidUiCommon/QtNoidUiCommon"

using namespace QtNoid::UiCommon;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // "Recent Files" submenu at the end of the File menu, or before "Exit"
    m_recentFiles = new RecentFiles(ui->menuFile, tr("Recent Files"), this);
    // m_recentFiles->setMenu(ui->menuFile, tr("Recent Files"), ui->actionExit);

    m_recentFiles->setFiles(appConfig->restoreAsStringList("RecentFiles", {}));   // restore
    connect(m_recentFiles, &RecentFiles::fileSelected, this, &MainWindow::openFile);
}

MainWindow::~MainWindow()
{
    appConfig->saveValue("RecentFiles", m_recentFiles->files());   // save
    delete ui;
}

void MainWindow::openFile(const QString &fileName)
{
    // ... load the file ...
    m_recentFiles->addFile(fileName);
}
```

**2. The finer one: the submenu is attached to an action prepared in Designer.**

Here `RecentFiles` is a member of the window, so the submenu is placed after `setupUi()`:
```cpp
// mainwindow.h
QtNoid::UiCommon::RecentFiles m_recentFiles;

// mainwindow.cpp
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_recentFiles.setAction(ui->actionRecent_Files);
    m_recentFiles.setFiles(appConfig->restoreAsStringList("RecentFiles", {}));
    connect(&m_recentFiles, &RecentFiles::fileSelected, this, &MainWindow::openFile);
}
```

[⬆ Back to QtNoidUiCommon](QtNoidUiCommon.md)
[← Back to README](../../README.md)
