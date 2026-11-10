#include <QAction>
#include <QDir>
#include <QFontMetrics>
#include <QMenu>
#include <QPointer>
#include <QSignalSpy>
#include <QTest>
#include <QtNoidUiCommon/QtNoidUiCommon>


class TestQtNoidUiCommonRecentFiles : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // List management
    void testDefaultConstructor();
    void testAddFile();
    void testAddFileMovesExistingToFront();
    void testAddFileRejectsEmptyPath();
    void testAddFileNormalizesPath();
    void testAddFileRespectsMaxCount();
    void testSetFilesCleansTheList();
    void testRemoveFile();
    void testContains();
    void testClear();

    // Properties and signals
    void testSetMaxCountTruncates();
    void testSetMaxCountIgnoresInvalidValues();
    void testFilesChangedOnlyOnRealChanges();
    void testQtProperties();

    // Menu content
    void testMenuIsBuiltFromTheList();
    void testMenuActionEmitsFileSelected();
    void testMenuClearAction();
    void testFileNameWithAmpersandIsNotAShortcut();
    void testAddFileFromFileSelectedSlot();

    // Elide width
    void testElideWidthDefault();
    void testSetElideWidth();
    void testElideWidthIsAppliedToMenu();
    void testSetElideWidthRebuildsMenu();

    // setMenu(): the class adds a "Recent Files" submenu to a menu
    void testSetMenuCreatesSubmenu();
    void testSetMenuWithConstructor();
    void testSetMenuBefore();
    void testSetMenuEntryIsDisabledWhenEmpty();
    void testSetMenuParentMenuDeleted();
    void testSetMenuRecentFilesDeleted();

    // setAction(): the class attaches the submenu to an existing action
    void testSetActionCreatesSubmenu();
    void testSetActionWithConstructor();
    void testSetActionEntryIsDisabledWhenEmpty();
    void testSetActionWindowDeleted();
    void testSetActionActionDeleted();
    void testSetActionRecentFilesDeleted();
    void testSetActionWithoutWidgetParent();

    // Changing idea: moving the submenu
    void testSwitchFromActionToMenu();
    void testSwitchFromMenuToAction();
    void testMoveMenuToAnotherMenu();
    void testSwitchBackAndForth();
    void testOldActionDeletedAfterSwitch();
    void testSwitchInsideFileSelectedSlot();
    void testDetachWithNull();

private:
    static QStringList menuFileActions(const QMenu *menu);
    static QString menuEntryPath(const QAction *action);
    static void processDeferredDeletes();

    // A window with a File menu ("Open", "Recent Files", "Exit"), like Designer creates
    struct Window {
        QWidget widget;
        QMenu *fileMenu = nullptr;
        QAction *actionOpen = nullptr;
        QAction *actionRecentFiles = nullptr;
        QAction *actionExit = nullptr;
        Window() {
            fileMenu = new QMenu("File", &widget);
            actionOpen = new QAction("Open", &widget);
            actionRecentFiles = new QAction("Recent Files", &widget);
            actionExit = new QAction("Exit", &widget);
            fileMenu->addAction(actionOpen);
            fileMenu->addAction(actionRecentFiles);
            fileMenu->addAction(actionExit);
        }
    };
};

using namespace QtNoid::UiCommon;


void TestQtNoidUiCommonRecentFiles::initTestCase()
{}

void TestQtNoidUiCommonRecentFiles::cleanupTestCase()
{}

void TestQtNoidUiCommonRecentFiles::init()
{}

void TestQtNoidUiCommonRecentFiles::cleanup()
{}


QStringList TestQtNoidUiCommonRecentFiles::menuFileActions(const QMenu *menu)
{
    // The file paths stored in the menu actions, in menu order
    QStringList res;
    if (menu == nullptr)
        return res;
    const QList<QAction*> actions = menu->actions();
    for (QAction *action : actions) {
        if (!action->isSeparator() && action->data().isValid())
            res << action->data().toString();
    }
    return res;
}

QString TestQtNoidUiCommonRecentFiles::menuEntryPath(const QAction *action)
{
    // The path shown in a file entry: the text without the "&1 " prefix
    QString text = action->text();
    text = text.mid(text.indexOf(' ') + 1);
    text.replace("&&", "&");
    return text;
}

void TestQtNoidUiCommonRecentFiles::processDeferredDeletes()
{
    QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
}


// =============================================================================
// List management
// =============================================================================
void TestQtNoidUiCommonRecentFiles::testDefaultConstructor()
{
    RecentFiles recent;

    QVERIFY(recent.isEmpty());
    QCOMPARE(recent.count(), 0);
    QCOMPARE(recent.files(), QStringList());
    QCOMPARE(recent.maxCount(), RecentFiles::DefaultMaxCount);
    QCOMPARE(recent.menu(), nullptr);
}

void TestQtNoidUiCommonRecentFiles::testAddFile()
{
    RecentFiles recent;
    QSignalSpy spy(&recent, &RecentFiles::filesChanged);

    QVERIFY(recent.addFile("/data/a.yaml"));
    QVERIFY(recent.addFile("/data/b.yaml"));

    // The most recent file is the first one
    QCOMPARE(recent.files(), QStringList({"/data/b.yaml", "/data/a.yaml"}));
    QCOMPARE(recent.count(), 2);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.last().at(0).toStringList(), recent.files());
}

void TestQtNoidUiCommonRecentFiles::testAddFileMovesExistingToFront()
{
    RecentFiles recent;
    recent.addFile("/data/a.yaml");
    recent.addFile("/data/b.yaml");
    recent.addFile("/data/c.yaml");

    QVERIFY(recent.addFile("/data/a.yaml"));
    QCOMPARE(recent.files(), QStringList({"/data/a.yaml", "/data/c.yaml", "/data/b.yaml"}));
}

void TestQtNoidUiCommonRecentFiles::testAddFileRejectsEmptyPath()
{
    RecentFiles recent;
    QSignalSpy spy(&recent, &RecentFiles::filesChanged);

    QVERIFY(!recent.addFile({}));
    QVERIFY(!recent.addFile("   "));
    QVERIFY(recent.isEmpty());
    QCOMPARE(spy.count(), 0);
}

void TestQtNoidUiCommonRecentFiles::testAddFileNormalizesPath()
{
    RecentFiles recent;
    recent.addFile("/data/./sub/../a.yaml");
    QCOMPARE(recent.files(), QStringList({"/data/a.yaml"}));

    // The same file written in a different way is not duplicated
    recent.addFile("/data//a.yaml");
    QCOMPARE(recent.count(), 1);

    // Leading and trailing spaces are removed
    recent.addFile(" /data/x/../b.yaml ");
    QCOMPARE(recent.files(), QStringList({"/data/b.yaml", "/data/a.yaml"}));
}

void TestQtNoidUiCommonRecentFiles::testAddFileRespectsMaxCount()
{
    RecentFiles recent;
    recent.setMaxCount(3);
    for (int ii = 1; ii <= 5; ++ii)
        recent.addFile(QString("/data/%1.yaml").arg(ii));

    QCOMPARE(recent.files(), QStringList({"/data/5.yaml", "/data/4.yaml", "/data/3.yaml"}));
}

void TestQtNoidUiCommonRecentFiles::testSetFilesCleansTheList()
{
    RecentFiles recent;
    recent.setMaxCount(3);

    // Empty entries and duplicates are removed, the first occurrence wins
    recent.setFiles({"/a.yaml", "", "/b.yaml", "/./a.yaml", "/c.yaml", "/d.yaml"});
    QCOMPARE(recent.files(), QStringList({"/a.yaml", "/b.yaml", "/c.yaml"}));
}

void TestQtNoidUiCommonRecentFiles::testRemoveFile()
{
    RecentFiles recent;
    recent.setFiles({"/a.yaml", "/b.yaml", "/c.yaml"});
    QSignalSpy spy(&recent, &RecentFiles::filesChanged);

    QVERIFY(recent.removeFile("/b.yaml"));
    QCOMPARE(recent.files(), QStringList({"/a.yaml", "/c.yaml"}));
    QCOMPARE(spy.count(), 1);

    QVERIFY(!recent.removeFile("/not-in-list.yaml"));
    QCOMPARE(spy.count(), 1);
}

void TestQtNoidUiCommonRecentFiles::testContains()
{
    RecentFiles recent;
    recent.addFile("/data/a.yaml");

    QVERIFY(recent.contains("/data/a.yaml"));
    QVERIFY(recent.contains("/data/sub/../a.yaml"));
    QVERIFY(!recent.contains("/data/b.yaml"));
    QVERIFY(!recent.contains({}));
}

void TestQtNoidUiCommonRecentFiles::testClear()
{
    RecentFiles recent;
    recent.setFiles({"/a.yaml", "/b.yaml"});
    QSignalSpy clearedSpy(&recent, &RecentFiles::cleared);
    QSignalSpy changedSpy(&recent, &RecentFiles::filesChanged);

    recent.clear();
    QVERIFY(recent.isEmpty());
    QCOMPARE(clearedSpy.count(), 1);
    QCOMPARE(changedSpy.count(), 1);

    // Clearing an empty list does nothing
    recent.clear();
    QCOMPARE(clearedSpy.count(), 1);
    QCOMPARE(changedSpy.count(), 1);
}


// =============================================================================
// Properties and signals
// =============================================================================
void TestQtNoidUiCommonRecentFiles::testSetMaxCountTruncates()
{
    RecentFiles recent;
    recent.setFiles({"/a.yaml", "/b.yaml", "/c.yaml", "/d.yaml"});
    QSignalSpy maxSpy(&recent, &RecentFiles::maxCountChanged);
    QSignalSpy filesSpy(&recent, &RecentFiles::filesChanged);

    recent.setMaxCount(2);
    QCOMPARE(recent.maxCount(), 2);
    QCOMPARE(recent.files(), QStringList({"/a.yaml", "/b.yaml"}));
    QCOMPARE(maxSpy.count(), 1);
    QCOMPARE(maxSpy.first().at(0).toInt(), 2);
    QCOMPARE(filesSpy.count(), 1);

    // A larger maximum does not change the list
    recent.setMaxCount(5);
    QCOMPARE(recent.count(), 2);
    QCOMPARE(filesSpy.count(), 1);
}

void TestQtNoidUiCommonRecentFiles::testSetMaxCountIgnoresInvalidValues()
{
    RecentFiles recent;
    QSignalSpy spy(&recent, &RecentFiles::maxCountChanged);

    recent.setMaxCount(0);
    recent.setMaxCount(-5);
    recent.setMaxCount(RecentFiles::DefaultMaxCount);   // same value

    QCOMPARE(recent.maxCount(), RecentFiles::DefaultMaxCount);
    QCOMPARE(spy.count(), 0);
}

void TestQtNoidUiCommonRecentFiles::testFilesChangedOnlyOnRealChanges()
{
    RecentFiles recent;
    recent.setFiles({"/a.yaml", "/b.yaml"});
    QSignalSpy spy(&recent, &RecentFiles::filesChanged);

    recent.setFiles({"/a.yaml", "/b.yaml"});   // same list
    recent.addFile("/a.yaml");                 // already the first one
    QCOMPARE(spy.count(), 0);
}

void TestQtNoidUiCommonRecentFiles::testQtProperties()
{
    RecentFiles recent;

    QVERIFY(recent.setProperty("files", QStringList({"/a.yaml", "/b.yaml"})));
    QCOMPARE(recent.property("files").toStringList(), QStringList({"/a.yaml", "/b.yaml"}));
    QCOMPARE(recent.property("count").toInt(), 2);

    QVERIFY(recent.setProperty("maxCount", 1));
    QCOMPARE(recent.property("maxCount").toInt(), 1);
    QCOMPARE(recent.property("count").toInt(), 1);
}


// =============================================================================
// Menu content
// =============================================================================
void TestQtNoidUiCommonRecentFiles::testMenuIsBuiltFromTheList()
{
    Window w;
    RecentFiles recent;
    recent.setMenu(w.fileMenu, "Recent Files");
    recent.addFile("/data/a.yaml");
    recent.addFile("/data/b.yaml");

    // Two files, a separator and the "Clear Recent Files" action
    const QList<QAction*> actions = recent.menu()->actions();
    QCOMPARE(actions.size(), 4);
    QCOMPARE(menuFileActions(recent.menu()), recent.files());
    QVERIFY(actions.at(2)->isSeparator());
    QCOMPARE(actions.at(3)->objectName(), QString("actionClearRecentFiles"));

    // Numbered entries with a keyboard shortcut and the full path as tooltip
    QVERIFY(actions.at(0)->text().startsWith("&1 "));
    QVERIFY(actions.at(1)->text().startsWith("&2 "));
    QCOMPARE(actions.at(0)->toolTip(), QDir::toNativeSeparators("/data/b.yaml"));

    // All the actions are children of the submenu
    for (QAction *action : actions)
        QCOMPARE(action->parent(), recent.menu());
}

void TestQtNoidUiCommonRecentFiles::testMenuActionEmitsFileSelected()
{
    Window w;
    RecentFiles recent(w.actionRecentFiles);
    recent.setFiles({"/data/a.yaml", "/data/b.yaml"});
    QSignalSpy spy(&recent, &RecentFiles::fileSelected);

    recent.menu()->actions().at(1)->trigger();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().at(0).toString(), QString("/data/b.yaml"));
}

void TestQtNoidUiCommonRecentFiles::testMenuClearAction()
{
    Window w;
    RecentFiles recent(w.fileMenu, "Recent Files");
    recent.setFiles({"/data/a.yaml", "/data/b.yaml"});
    QSignalSpy spy(&recent, &RecentFiles::cleared);

    QPointer<QAction> clearAction = recent.menu()->findChild<QAction*>("actionClearRecentFiles");
    QVERIFY(clearAction != nullptr);
    clearAction->trigger();

    QCOMPARE(spy.count(), 1);
    QVERIFY(recent.isEmpty());
    QVERIFY(recent.menu()->actions().isEmpty());

    // The old actions are deleted later, not while they are emitting triggered()
    QVERIFY(!clearAction.isNull());
    processDeferredDeletes();
    QVERIFY(clearAction.isNull());
}

void TestQtNoidUiCommonRecentFiles::testFileNameWithAmpersandIsNotAShortcut()
{
    Window w;
    RecentFiles recent(w.fileMenu, "Recent Files");
    recent.addFile("/data/R&D.yaml");

    const QAction *action = recent.menu()->actions().at(0);
    QVERIFY(action->text().contains("R&&D.yaml"));
    QCOMPARE(action->data().toString(), QString("/data/R&D.yaml"));
}

void TestQtNoidUiCommonRecentFiles::testAddFileFromFileSelectedSlot()
{
    // Typical use: the application opens the file and moves it to the front.
    // The menu is rebuilt while the triggered action is still emitting.
    Window w;
    RecentFiles recent(w.actionRecentFiles);
    recent.setFiles({"/data/a.yaml", "/data/b.yaml", "/data/c.yaml"});
    connect(&recent, &RecentFiles::fileSelected, &recent, &RecentFiles::addFile);

    recent.menu()->actions().at(2)->trigger();
    QCOMPARE(recent.files(), QStringList({"/data/c.yaml", "/data/a.yaml", "/data/b.yaml"}));
    QCOMPARE(menuFileActions(recent.menu()), recent.files());

    processDeferredDeletes();
    QCOMPARE(menuFileActions(recent.menu()), recent.files());
}


// =============================================================================
// Elide width
// =============================================================================
void TestQtNoidUiCommonRecentFiles::testElideWidthDefault()
{
    RecentFiles recent;
    QCOMPARE(recent.elideWidth(), RecentFiles::DefaultElideWidth);
}

void TestQtNoidUiCommonRecentFiles::testSetElideWidth()
{
    RecentFiles recent;

    recent.setElideWidth(120);
    QCOMPARE(recent.elideWidth(), 120);

    // Negative values are ignored
    recent.setElideWidth(-1);
    QCOMPARE(recent.elideWidth(), 120);

    recent.setElideWidth(RecentFiles::DefaultElideWidth);
    QCOMPARE(recent.elideWidth(), RecentFiles::DefaultElideWidth);
}

void TestQtNoidUiCommonRecentFiles::testElideWidthIsAppliedToMenu()
{
    Window w;
    RecentFiles recent(w.fileMenu, "Recent Files");
    const QString shortPath = "/data/a.yaml";
    const QString longPath = "/data/" + QString("very_long_folder_name/").repeated(20) + "file.yaml";
    recent.setFiles({longPath, shortPath});

    const QFontMetrics metrics = recent.menu()->fontMetrics();
    const QList<QAction*> actions = recent.menu()->actions();
    const QString longShown = menuEntryPath(actions.at(0));
    const QString shortShown = menuEntryPath(actions.at(1));

    // The long path is elided within DefaultElideWidth, keeping the file name
    QVERIFY(metrics.horizontalAdvance(QDir::toNativeSeparators(longPath)) > RecentFiles::DefaultElideWidth);
    QVERIFY(longShown != QDir::toNativeSeparators(longPath));
    QVERIFY(metrics.horizontalAdvance(longShown) <= RecentFiles::DefaultElideWidth);
    QVERIFY(longShown.endsWith("file.yaml"));
    QVERIFY(longShown.startsWith(QDir::toNativeSeparators("/data/")));

    // A short path is shown as is
    QCOMPARE(shortShown, QDir::toNativeSeparators(shortPath));

    // Tooltip and data always keep the full path
    QCOMPARE(actions.at(0)->toolTip(), QDir::toNativeSeparators(longPath));
    QCOMPARE(actions.at(0)->data().toString(), longPath);
}

void TestQtNoidUiCommonRecentFiles::testSetElideWidthRebuildsMenu()
{
    Window w;
    RecentFiles recent(w.actionRecentFiles);
    const QString longPath = "/data/" + QString("very_long_folder_name/").repeated(20) + "file.yaml";
    recent.setFiles({longPath});

    const QFontMetrics metrics = recent.menu()->fontMetrics();
    const QString defaultShown = menuEntryPath(recent.menu()->actions().at(0));

    // A smaller width is applied at once to the existing entries
    const int smallWidth = RecentFiles::DefaultElideWidth / 3;
    recent.setElideWidth(smallWidth);
    const QString smallShown = menuEntryPath(recent.menu()->actions().at(0));
    QVERIFY(metrics.horizontalAdvance(smallShown) <= smallWidth);
    QVERIFY(smallShown.length() < defaultShown.length());

    // A width larger than the path shows the full path
    recent.setElideWidth(metrics.horizontalAdvance(QDir::toNativeSeparators(longPath)) + 10);
    QCOMPARE(menuEntryPath(recent.menu()->actions().at(0)), QDir::toNativeSeparators(longPath));

    // Back to the default
    recent.setElideWidth(RecentFiles::DefaultElideWidth);
    QCOMPARE(menuEntryPath(recent.menu()->actions().at(0)), defaultShown);
}


// =============================================================================
// setMenu(): the class adds a "Recent Files" submenu to a menu
// =============================================================================
void TestQtNoidUiCommonRecentFiles::testSetMenuCreatesSubmenu()
{
    Window w;
    RecentFiles recent;
    recent.setFiles({"/data/a.yaml"});
    recent.setMenu(w.fileMenu, "Recent Files");

    QMenu *submenu = recent.menu();
    QVERIFY(submenu != nullptr);
    QCOMPARE(submenu->parent(), w.fileMenu);              // the File menu is the parent
    QCOMPARE(submenu->title(), QString("Recent Files"));
    QCOMPARE(menuFileActions(submenu), recent.files());

    // The submenu entry is added at the end, the other entries are untouched
    const QList<QAction*> entries = w.fileMenu->actions();
    QCOMPARE(entries.size(), 4);
    QCOMPARE(entries.at(0), w.actionOpen);
    QCOMPARE(entries.at(1), w.actionRecentFiles);
    QCOMPARE(entries.at(2), w.actionExit);
    QCOMPARE(entries.at(3), submenu->menuAction());
}

void TestQtNoidUiCommonRecentFiles::testSetMenuWithConstructor()
{
    Window w;
    RecentFiles recent(w.fileMenu, "Recent Files");

    QVERIFY(recent.menu() != nullptr);
    QCOMPARE(recent.menu()->parent(), w.fileMenu);
    QVERIFY(w.fileMenu->actions().contains(recent.menu()->menuAction()));
}

void TestQtNoidUiCommonRecentFiles::testSetMenuBefore()
{
    Window w;
    RecentFiles recent;
    recent.setMenu(w.fileMenu, "Recent Files", w.actionExit);

    const QList<QAction*> entries = w.fileMenu->actions();
    QCOMPARE(entries.size(), 4);
    QCOMPARE(entries.at(2), recent.menu()->menuAction());
    QCOMPARE(entries.at(3), w.actionExit);
}

void TestQtNoidUiCommonRecentFiles::testSetMenuEntryIsDisabledWhenEmpty()
{
    Window w;
    RecentFiles recent(w.fileMenu, "Recent Files");
    QAction *entry = recent.menu()->menuAction();
    QVERIFY(!entry->isEnabled());

    recent.addFile("/data/a.yaml");
    QVERIFY(entry->isEnabled());

    recent.clear();
    QVERIFY(!entry->isEnabled());
}

void TestQtNoidUiCommonRecentFiles::testSetMenuParentMenuDeleted()
{
    RecentFiles recent;
    recent.addFile("/data/a.yaml");
    {
        Window w;
        recent.setMenu(w.fileMenu, "Recent Files");
        QVERIFY(recent.menu() != nullptr);
    }
    // The File menu and its submenu are gone: the list keeps working
    QCOMPARE(recent.menu(), nullptr);
    QVERIFY(recent.addFile("/data/b.yaml"));
    QCOMPARE(recent.count(), 2);
}

void TestQtNoidUiCommonRecentFiles::testSetMenuRecentFilesDeleted()
{
    Window w;
    auto *recent = new RecentFiles(w.fileMenu, "Recent Files");
    QPointer<QMenu> submenu = recent->menu();
    QAction *entry = submenu->menuAction();

    delete recent;

    // The submenu and its entry are removed from the File menu
    QVERIFY(submenu.isNull());
    QVERIFY(!w.fileMenu->actions().contains(entry));
    QCOMPARE(w.fileMenu->actions().size(), 3);
}


// =============================================================================
// setAction(): the class attaches the submenu to an existing action
// =============================================================================
void TestQtNoidUiCommonRecentFiles::testSetActionCreatesSubmenu()
{
    Window w;
    RecentFiles recent;
    recent.setFiles({"/data/a.yaml"});
    recent.setAction(w.actionRecentFiles);

    QMenu *submenu = recent.menu();
    QVERIFY(submenu != nullptr);
    QCOMPARE(w.actionRecentFiles->menu(), submenu);
    QCOMPARE(submenu->parent(), &w.widget);               // the widget that owns the action
    QCOMPARE(submenu->title(), QString("Recent Files"));
    QCOMPARE(menuFileActions(submenu), recent.files());

    // No new entry in the File menu: the entry is the programmer's action
    QCOMPARE(w.fileMenu->actions().size(), 3);
}

void TestQtNoidUiCommonRecentFiles::testSetActionWithConstructor()
{
    Window w;
    RecentFiles recent(w.actionRecentFiles);

    QVERIFY(recent.menu() != nullptr);
    QCOMPARE(w.actionRecentFiles->menu(), recent.menu());
}

void TestQtNoidUiCommonRecentFiles::testSetActionEntryIsDisabledWhenEmpty()
{
    Window w;
    RecentFiles recent(w.actionRecentFiles);
    QVERIFY(!w.actionRecentFiles->isEnabled());

    recent.addFile("/data/a.yaml");
    QVERIFY(w.actionRecentFiles->isEnabled());

    recent.clear();
    QVERIFY(!w.actionRecentFiles->isEnabled());
}

void TestQtNoidUiCommonRecentFiles::testSetActionWindowDeleted()
{
    RecentFiles recent;
    recent.addFile("/data/a.yaml");
    {
        Window w;
        recent.setAction(w.actionRecentFiles);
        QVERIFY(recent.menu() != nullptr);
    }
    // The window, the action and the submenu are gone: no dangling pointers
    processDeferredDeletes();
    QCOMPARE(recent.menu(), nullptr);
    QVERIFY(recent.addFile("/data/b.yaml"));
    QCOMPARE(recent.count(), 2);
}

void TestQtNoidUiCommonRecentFiles::testSetActionActionDeleted()
{
    Window w;
    RecentFiles recent(w.actionRecentFiles);
    QPointer<QMenu> submenu = recent.menu();

    // Only the action is deleted: the submenu must not stay orphan in the window
    delete w.actionRecentFiles;
    w.actionRecentFiles = nullptr;
    QCOMPARE(recent.menu(), nullptr);

    processDeferredDeletes();
    QVERIFY(submenu.isNull());
    QVERIFY(recent.addFile("/data/a.yaml"));
}

void TestQtNoidUiCommonRecentFiles::testSetActionRecentFilesDeleted()
{
    Window w;
    auto *recent = new RecentFiles(w.actionRecentFiles);
    QPointer<QMenu> submenu = recent->menu();
    QVERIFY(!w.actionRecentFiles->isEnabled());           // empty list

    delete recent;

    // The action is given back without submenu and enabled
    QVERIFY(submenu.isNull());
    QCOMPARE(w.actionRecentFiles->menu(), nullptr);
    QVERIFY(w.actionRecentFiles->isEnabled());
}

void TestQtNoidUiCommonRecentFiles::testSetActionWithoutWidgetParent()
{
    // An action without a widget parent, shown in a menu: the menu is the parent
    QMenu fileMenu;
    QAction action("Recent Files");
    fileMenu.addAction(&action);

    RecentFiles recent(&action);
    QCOMPARE(recent.menu()->parent(), &fileMenu);

    // An action without any widget: the submenu has no parent and the class deletes it
    QAction lonelyAction("Recent Files");
    auto *recent2 = new RecentFiles(&lonelyAction);
    QPointer<QMenu> submenu = recent2->menu();
    QCOMPARE(submenu->parent(), nullptr);
    delete recent2;
    QVERIFY(submenu.isNull());
}


// =============================================================================
// Changing idea: moving the submenu
// =============================================================================
void TestQtNoidUiCommonRecentFiles::testSwitchFromActionToMenu()
{
    Window w;
    RecentFiles recent(w.actionRecentFiles);
    recent.setFiles({"/data/a.yaml"});
    QPointer<QMenu> oldSubmenu = recent.menu();

    recent.setMenu(w.fileMenu, "Recent Files");

    // The action is given back at once
    QCOMPARE(w.actionRecentFiles->menu(), nullptr);
    QVERIFY(w.actionRecentFiles->isEnabled());

    // The new submenu is in the File menu, with the same files
    QVERIFY(recent.menu() != oldSubmenu);
    QVERIFY(w.fileMenu->actions().contains(recent.menu()->menuAction()));
    QCOMPARE(menuFileActions(recent.menu()), recent.files());

    // The old submenu is deleted later
    processDeferredDeletes();
    QVERIFY(oldSubmenu.isNull());
}

void TestQtNoidUiCommonRecentFiles::testSwitchFromMenuToAction()
{
    Window w;
    RecentFiles recent(w.fileMenu, "Recent Files");
    recent.setFiles({"/data/a.yaml"});
    QPointer<QMenu> oldSubmenu = recent.menu();
    QAction *oldEntry = oldSubmenu->menuAction();

    recent.setAction(w.actionRecentFiles);

    // The old entry leaves the File menu at once
    QVERIFY(!w.fileMenu->actions().contains(oldEntry));
    QCOMPARE(w.fileMenu->actions().size(), 3);
    QCOMPARE(w.actionRecentFiles->menu(), recent.menu());
    QCOMPARE(menuFileActions(recent.menu()), recent.files());

    processDeferredDeletes();
    QVERIFY(oldSubmenu.isNull());
}

void TestQtNoidUiCommonRecentFiles::testMoveMenuToAnotherMenu()
{
    Window w;
    QMenu *otherMenu = new QMenu("Project", &w.widget);
    RecentFiles recent(w.fileMenu, "Recent Files");
    recent.setFiles({"/data/a.yaml"});

    recent.setMenu(otherMenu, "Recent Projects");

    QCOMPARE(w.fileMenu->actions().size(), 3);
    QCOMPARE(otherMenu->actions().size(), 1);
    QCOMPARE(recent.menu()->parent(), otherMenu);
    QCOMPARE(recent.menu()->title(), QString("Recent Projects"));
    QCOMPARE(menuFileActions(recent.menu()), recent.files());
}

void TestQtNoidUiCommonRecentFiles::testSwitchBackAndForth()
{
    Window w;
    RecentFiles recent;
    recent.setFiles({"/data/a.yaml", "/data/b.yaml"});

    for (int ii = 0; ii < 5; ++ii) {
        recent.setAction(w.actionRecentFiles);
        recent.setMenu(w.fileMenu, "Recent Files");
        recent.setMenu(w.fileMenu, "Recent Files", w.actionExit);
    }
    processDeferredDeletes();

    // Only one submenu exists, in the File menu, before "Exit"
    QCOMPARE(w.fileMenu->actions().size(), 4);
    QCOMPARE(w.fileMenu->actions().at(2), recent.menu()->menuAction());
    QCOMPARE(w.actionRecentFiles->menu(), nullptr);
    QCOMPARE(w.widget.findChildren<QMenu*>().size(), 2);   // File menu + submenu
    QCOMPARE(menuFileActions(recent.menu()), recent.files());
}

void TestQtNoidUiCommonRecentFiles::testOldActionDeletedAfterSwitch()
{
    Window w;
    RecentFiles recent(w.actionRecentFiles);
    recent.setMenu(w.fileMenu, "Recent Files");
    QPointer<QMenu> submenu = recent.menu();

    // The old action is deleted later: it must not delete the new submenu
    delete w.actionRecentFiles;
    w.actionRecentFiles = nullptr;
    processDeferredDeletes();

    QVERIFY(!submenu.isNull());
    QCOMPARE(recent.menu(), submenu.data());
}

void TestQtNoidUiCommonRecentFiles::testSwitchInsideFileSelectedSlot()
{
    // The submenu is moved from a slot of one of its own actions
    Window w;
    RecentFiles recent(w.actionRecentFiles);
    recent.setFiles({"/data/a.yaml", "/data/b.yaml"});
    connect(&recent, &RecentFiles::fileSelected, &recent, [&](const QString &file) {
        recent.setMenu(w.fileMenu, "Recent Files");
        recent.addFile(file);
    });

    QPointer<QMenu> oldSubmenu = recent.menu();
    recent.menu()->actions().at(1)->trigger();

    // The old submenu is not deleted while its action is still emitting
    QVERIFY(!oldSubmenu.isNull());
    processDeferredDeletes();
    QVERIFY(oldSubmenu.isNull());

    QCOMPARE(recent.files(), QStringList({"/data/b.yaml", "/data/a.yaml"}));
    QCOMPARE(w.actionRecentFiles->menu(), nullptr);
    QCOMPARE(menuFileActions(recent.menu()), recent.files());
}

void TestQtNoidUiCommonRecentFiles::testDetachWithNull()
{
    Window w;
    RecentFiles recent(w.fileMenu, "Recent Files");
    recent.setFiles({"/data/a.yaml"});

    recent.setMenu(nullptr, {});
    QCOMPARE(recent.menu(), nullptr);
    QCOMPARE(w.fileMenu->actions().size(), 3);

    recent.setAction(w.actionRecentFiles);
    recent.setAction(nullptr);
    QCOMPARE(recent.menu(), nullptr);
    QCOMPARE(w.actionRecentFiles->menu(), nullptr);
    QVERIFY(w.actionRecentFiles->isEnabled());

    // The list is not touched
    QCOMPARE(recent.files(), QStringList({"/data/a.yaml"}));
}


QTEST_MAIN(TestQtNoidUiCommonRecentFiles)
#include "test_uicommon_recentfiles.moc"
