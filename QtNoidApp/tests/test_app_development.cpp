#include <QTest>
#include <QtGui/qclipboard.h>
#include <QtGui/qshortcut.h>
#include <QtNoidApp/QtNoidApp>
#include <QtNoidCommon/QtNoidCommon>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMimeData>
#include <QDir>
#include <QFileInfo>
#include <QSysInfo>
#include <QMenu>
#include <QJsonDocument>
#include <qjsonobject.h>

class TestQtNoidAppDevelopment : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testInitFullDialogGrabShortcut();
    void testInitFullDialogGrabShortcut_withFileName();
    void testInitFullDialogGrabShortcut_withMissingDestinationFolder();
    void testInitFullDialogGrabShortcut_withNullParent();
    void testScreenshotFileName();

    void testCreateSaveCurrentConfigToProjectMenuAction();
    void testCreateSaveCurrentConfigToProjectMenuAction_withNullMenu();
    void testCreateSaveCurrentConfigToProjectMenuAction_withEmptyProjectFolder();
    void testCreateSaveCurrentConfigToProjectMenuAction_withNoQrcFile();
    void testCreateSaveCurrentConfigToProjectMenuAction_withNoMatchingConfigFile();
    void testCreateSaveCurrentConfigToProjectMenuAction_succeedsWithoutMenuParent();

    void testCreateInitConfigFromResourcesMenuAction();
    void testCreateInitConfigFromResourcesMenuAction_withMissingResourceFile();
    void testCreateInitConfigFromResourcesMenuAction_withInvalidResourceContent();

private:
    void initMainDialog(QMainWindow* frm, const QString& caption);

};

using namespace QtNoid::App;


void TestQtNoidAppDevelopment::initTestCase()
{}

void TestQtNoidAppDevelopment::cleanupTestCase()
{}

void TestQtNoidAppDevelopment::init()
{}

void TestQtNoidAppDevelopment::cleanup()
{}

void TestQtNoidAppDevelopment::initMainDialog(QMainWindow* frm, const QString& caption)
{
    frm->setWindowTitle(caption);
    frm->setCentralWidget(new QWidget());
    frm->centralWidget()->setLayout(new QHBoxLayout());
    auto myWidget = new QLabel(caption);
    frm->centralWidget()->layout()->addWidget(myWidget);
    frm->setFixedWidth(400);

    frm->show();
    Q_UNUSED(QTest::qWaitForWindowExposed(frm));
    QTest::qWait(500);
}

void TestQtNoidAppDevelopment::testInitFullDialogGrabShortcut()
{
    QMainWindow frm;
    initMainDialog(&frm, __func__);

    // Actitate shortcut
    QShortcut* shortcut = Development::initFullDialogGrabShortcut(&frm, {}, {},
                                                                  true,  // Save to clipboard
                                                                  false);

    // Check the shortcut creation
    QVERIFY(shortcut != nullptr);
    // QCOMPARE(shortcut->key(), QKeySequence(keySeq));
    QCOMPARE(shortcut->parent(), &frm);

    // fire the shortcut
    shortcut->activated();
    QTest::qWait(100);

    // Search the screenshot file knowing it'll start with __func__
    QStringList filters{{QString(__func__) + "-*.png"}};
    QDir dir;
    QStringList files = dir.entryList(filters, QDir::Files);
    // qDebug() << __func__ << files;
    QVERIFY(files.count());

    // Verify the screenshot
    QFileInfo fileInfo(files.first());
    QVERIFY2(fileInfo.exists(), "Screenshot file not found");
    QVERIFY(fileInfo.size() > 0);

    QPixmap pixmap(fileInfo.filePath());
    //qDebug() << __func__ << pixmap.size();

    // Verify the clipboard content
    QClipboard* clipboard = QApplication::clipboard();
    const QMimeData* mimeData = clipboard->mimeData();
    QVERIFY2(mimeData->hasImage(), "Clipboard should contain an image");
    auto actual = clipboard->image();
    // qDebug() << __func__ << actual.size();
    QCOMPARE(actual.size(), pixmap.size());
}


void TestQtNoidAppDevelopment::testInitFullDialogGrabShortcut_withFileName()
{
    QString sstFile = Core::appExeOrAppBundleDirPath() + "/" + __func__ + ".png";
    qDebug() << __func__ << sstFile;

    QMainWindow frm;
    initMainDialog(&frm, __func__);

    QString keySeq = "Ctrl+Shift+Q";

    // Actitate shortcut
    QShortcut* shortcut = Development::initFullDialogGrabShortcut(&frm,
                                                               keySeq,
                                                               sstFile,
                                                               false,
                                                               false);

    // Check the shortcut creation
    QVERIFY(shortcut != nullptr);
    QCOMPARE(shortcut->key(), QKeySequence(keySeq));
    QCOMPARE(shortcut->parent(), &frm);

    // fire the shortcut
    shortcut->activated();
    QTest::qWait(100);

    // Verifica che il file PNG sia stato creato
    QFileInfo fileInfo(sstFile);
    QVERIFY2(fileInfo.exists(), "Screenshot file not found");
    QVERIFY(fileInfo.size() > 0);
}


void TestQtNoidAppDevelopment::testInitFullDialogGrabShortcut_withMissingDestinationFolder()
{
    // destinationFileOrPath names a folder that does not exist yet.
    // initFullDialogGrabShortcut() currently has no QDir::mkpath() step
    // (unlike resolveAndPrepareFilePath()/saveAsTextFile()), so this is
    // expected to FAIL until that folder-creation step is added.
    QMainWindow frm;
    initMainDialog(&frm, __func__);

    QDir dir(QDir::currentPath() + "/" + __func__ + "_missingFolder");
    dir.removeRecursively();
    QVERIFY2(!dir.exists(), "Precondition failed: destination folder should not exist yet");

    QShortcut* shortcut = Development::initFullDialogGrabShortcut(&frm, {},
                                                                   dir.absolutePath(),
                                                                   false,  // Save to clipboard
                                                                   false); // Open destination path
    QVERIFY(shortcut != nullptr);

    // fire the shortcut
    shortcut->activated();
    QTest::qWait(100);

    QVERIFY2(dir.exists(), "Destination folder should have been created automatically");

    QStringList filters{{"*.png"}};
    QStringList files = dir.entryList(filters, QDir::Files);
    QVERIFY2(files.count() > 0, "Screenshot file should have been saved inside the newly created folder");
}


void TestQtNoidAppDevelopment::testInitFullDialogGrabShortcut_withNullParent()
{
    // A null parent must not crash: initFullDialogGrabShortcut() must bail
    // out early and return nullptr instead of dereferencing it.
    QShortcut* shortcut = Development::initFullDialogGrabShortcut(nullptr);
    QCOMPARE(shortcut, nullptr);
}


void TestQtNoidAppDevelopment::testScreenshotFileName()
{
    QString appDir = Core::appExeOrAppBundleDirPath();
    QString osName = QSysInfo::productType();

    // 1) A plain QWidget, with no QMainWindow ancestor, and no destination
    //    given: generic name, in the app's own folder.
    QWidget plainWidget;
    QString expectedGeneric = QDir::cleanPath(appDir + "/Screenshot-" + osName + ".png");
    QCOMPARE(Development::screenshotFileName(&plainWidget, QString()), expectedGeneric);

    // 2) A QMainWindow, no destination given: name built from the window
    //    title, in the app's own folder.
    QMainWindow mainWin;
    mainWin.setWindowTitle(__func__);
    QString expectedFromTitle = QDir::cleanPath(appDir + "/"
            + QtNoid::Common::Text::convertToCamelCase(mainWin.windowTitle())
            + "-" + osName + ".png");
    QCOMPARE(Development::screenshotFileName(&mainWin, QString()), expectedFromTitle);

    // 3) A QMainWindow, destination is a bare folder (no extension): same
    //    title-based name, but inside that folder instead of the app dir.
    QString folder = QDir::tempPath();
    QString expectedInFolder = QDir::cleanPath(folder + "/"
            + QtNoid::Common::Text::convertToCamelCase(mainWin.windowTitle())
            + "-" + osName + ".png");
    QCOMPARE(Development::screenshotFileName(&mainWin, folder), expectedInFolder);

    // 4) An explicit full file path with extension: returned as-is
    //    (absolute), the window/title is ignored entirely.
    QString filePath = QDir::tempPath() + "/myCustomShot.png";
    QCOMPARE(Development::screenshotFileName(&mainWin, filePath), QFileInfo(filePath).absoluteFilePath());
}


void TestQtNoidAppDevelopment::testCreateSaveCurrentConfigToProjectMenuAction()
{
    // Project layout: a .qrc file next to the "old" copy of the config file
    // that createSaveCurrentConfigToProjectMenuAction() is supposed to refresh.
    QDir projectDir(QDir::currentPath() + "/" + __func__ + "_project");
    projectDir.removeRecursively();
    QVERIFY(projectDir.mkpath(projectDir.absoluteFilePath("resources")));

    QFile qrcFile(projectDir.absoluteFilePath("resources/app.qrc"));
    QVERIFY(qrcFile.open(QIODevice::WriteOnly));
    qrcFile.close();

    QString configFileName = "myAppConfig.json";
    QFile oldProjectConfig(projectDir.absoluteFilePath("resources/" + configFileName));
    QVERIFY(oldProjectConfig.open(QIODevice::WriteOnly | QIODevice::Text));
    oldProjectConfig.write("{\"old\":\"content\"}");
    oldProjectConfig.close();

    // Point the live appConfig at a controlled file name (it does not need to
    // exist on disk: the method now serializes appConfig's live, in-memory
    // values with toJsonValues() directly - it never reads this file back).
    QString originalFileName = appConfig->fileName();
    QString originalName = appConfig->name();
    appConfig->setFileName(projectDir.absoluteFilePath(configFileName));
    appConfig->setName(QStringLiteral("myAppConfig"));
    appConfig->clear();
    appConfig->saveValue("Volume", 77.0);

    QWidget menuParent;
    QMenu menu(&menuParent);

    QAction* action = Development::createSaveCurrentConfigToProjectMenuAction(&menu, projectDir.absolutePath());

    QVERIFY(action != nullptr);
    QVERIFY(menu.actions().contains(action));
    QVERIFY2(!action->statusTip().isEmpty(), "The action should have a status tip");

    action->trigger();
    QTest::qWait(50);

    // The project's resource copy must now hold exactly appConfig's current,
    // live content - not whatever was in the file before, and not whatever
    // was last written to appConfig->fileName() on disk (there is nothing).
    QFile updatedProjectConfig(projectDir.absoluteFilePath("resources/" + configFileName));
    QVERIFY(updatedProjectConfig.open(QIODevice::ReadOnly));
    QByteArray expectedContent = QJsonDocument(appConfig->toJsonValues()).toJson();
    QCOMPARE(updatedProjectConfig.readAll(), expectedContent);

    appConfig->setFileName(originalFileName);
    appConfig->setName(originalName);
    appConfig->clear();
}

void TestQtNoidAppDevelopment::testCreateSaveCurrentConfigToProjectMenuAction_withNullMenu()
{
    QAction* action = Development::createSaveCurrentConfigToProjectMenuAction(nullptr, "some/path");
    QCOMPARE(action, nullptr);
}

void TestQtNoidAppDevelopment::testCreateSaveCurrentConfigToProjectMenuAction_withEmptyProjectFolder()
{
    QMenu menu;
    QAction* action = Development::createSaveCurrentConfigToProjectMenuAction(&menu, QString());
    QCOMPARE(action, nullptr);
}

void TestQtNoidAppDevelopment::testCreateSaveCurrentConfigToProjectMenuAction_withNoQrcFile()
{
    QDir projectDir(QDir::currentPath() + "/" + __func__ + "_project");
    projectDir.removeRecursively();
    QVERIFY(projectDir.mkpath(projectDir.absolutePath()));

    QMenu menu;
    QAction* action = Development::createSaveCurrentConfigToProjectMenuAction(&menu, projectDir.absolutePath());

    QCOMPARE(action, nullptr);
    // No dead action must be left attached to the menu.
    QCOMPARE(menu.actions().count(), 0);
}

void TestQtNoidAppDevelopment::testCreateSaveCurrentConfigToProjectMenuAction_withNoMatchingConfigFile()
{
    QDir projectDir(QDir::currentPath() + "/" + __func__ + "_project");
    projectDir.removeRecursively();
    QVERIFY(projectDir.mkpath(projectDir.absolutePath()));

    // A .qrc exists, but nothing next to it matches appConfig's file name/suffix.
    QFile qrcFile(projectDir.absoluteFilePath("app.qrc"));
    QVERIFY(qrcFile.open(QIODevice::WriteOnly));
    qrcFile.close();

    QMenu menu;
    QAction* action = Development::createSaveCurrentConfigToProjectMenuAction(&menu, projectDir.absolutePath());

    QCOMPARE(action, nullptr);
    QCOMPARE(menu.actions().count(), 0);
}

void TestQtNoidAppDevelopment::testCreateSaveCurrentConfigToProjectMenuAction_succeedsWithoutMenuParent()
{
    // Regression test: createSaveCurrentConfigToProjectMenuAction() used to
    // require developmentMenu->parent() to be non-null (to use as a tr()/
    // connect() context). Now that Development declares its own tr()
    // functions and connects without a context object, a parent-less QMenu
    // must work just as well.
    QDir projectDir(QDir::currentPath() + "/" + __func__ + "_project");
    projectDir.removeRecursively();
    QVERIFY(projectDir.mkpath(projectDir.absolutePath()));

    QFile qrcFile(projectDir.absoluteFilePath("app.qrc"));
    QVERIFY(qrcFile.open(QIODevice::WriteOnly));
    qrcFile.close();

    QString configFileName = "myAppConfig.json";
    QFile projectConfig(projectDir.absoluteFilePath(configFileName));
    QVERIFY(projectConfig.open(QIODevice::WriteOnly));
    projectConfig.close();

    QString originalFileName = appConfig->fileName();
    appConfig->setFileName(projectDir.absoluteFilePath(configFileName));

    QMenu menu;   // no parent
    QAction* action = Development::createSaveCurrentConfigToProjectMenuAction(&menu, projectDir.absolutePath());

    QVERIFY2(action != nullptr, "A parent-less menu must no longer be rejected");
    QVERIFY(menu.actions().contains(action));

    action->trigger();
    QTest::qWait(50);

    appConfig->setFileName(originalFileName);
}

void TestQtNoidAppDevelopment::testCreateInitConfigFromResourcesMenuAction()
{
    QDir workDir(QDir::currentPath() + "/" + __func__ + "_work");
    workDir.removeRecursively();
    QVERIFY(workDir.mkpath(workDir.absolutePath()));

    const QString configName = QStringLiteral("myAppConfig");

    // The "model" shipped with the application: a config with Volume=99.0.
    // A plain file on disk stands in for the resource file - the production
    // code only ever opens resConfigFile with QFile, resource or not.
    Config resourceConfig(configName);
    resourceConfig.saveValue("Volume", 99.0);
    QString resConfigFile = workDir.absoluteFilePath("resourceModel.json");
    QFile resFile(resConfigFile);
    QVERIFY(resFile.open(QIODevice::WriteOnly));
    resFile.write(QJsonDocument(resourceConfig.toJsonValues()).toJson());
    resFile.close();

    // The live appConfig, with a different value, pointing at its own file.
    QString originalFileName = appConfig->fileName();
    QString originalName = appConfig->name();
    appConfig->setFileName(workDir.absoluteFilePath("liveConfig.json"));
    appConfig->setName(configName);
    appConfig->clear();
    appConfig->saveValue("Volume", 1.0);

    QMenu menu;
    QAction* action = Development::createInitConfigFromResourcesMenuAction(&menu, resConfigFile);

    QVERIFY(action != nullptr);
    QVERIFY(menu.actions().contains(action));
    QVERIFY2(!action->statusTip().isEmpty(), "The action should have a status tip");

    action->trigger();
    QTest::qWait(50);

    // appConfig must now hold the resource's value: the file was replaced
    // and successfully reloaded with load().
    QCOMPARE(appConfig->restoreAsVariant("Volume", 0).toDouble(), 99.0);

    appConfig->setFileName(originalFileName);
    appConfig->setName(originalName);
    appConfig->clear();
}

void TestQtNoidAppDevelopment::testCreateInitConfigFromResourcesMenuAction_withMissingResourceFile()
{
    QMenu menu;
    QAction* action = Development::createInitConfigFromResourcesMenuAction(&menu, "/no/such/resourceModel.json");

    // developmentMenu is the only requirement checked when the action is
    // created: a missing resource file is only detected later, when triggered.
    QVERIFY(action != nullptr);
    QVERIFY(menu.actions().contains(action));

    QDir workDir(QDir::currentPath() + "/" + __func__ + "_work");
    workDir.removeRecursively();
    QVERIFY(workDir.mkpath(workDir.absolutePath()));

    QString originalFileName = appConfig->fileName();
    QString originalName = appConfig->name();
    appConfig->setFileName(workDir.absoluteFilePath("liveConfig.json"));
    appConfig->setName(QStringLiteral("myAppConfig"));
    appConfig->clear();
    appConfig->saveValue("Volume", 1.0);

    action->trigger();
    QTest::qWait(50);

    // Nothing was found to copy: appConfig must be left exactly as it was.
    QCOMPARE(appConfig->restoreAsVariant("Volume", 0).toDouble(), 1.0);

    appConfig->setFileName(originalFileName);
    appConfig->setName(originalName);
    appConfig->clear();
}

void TestQtNoidAppDevelopment::testCreateInitConfigFromResourcesMenuAction_withInvalidResourceContent()
{
    QDir workDir(QDir::currentPath() + "/" + __func__ + "_work");
    workDir.removeRecursively();
    QVERIFY(workDir.mkpath(workDir.absolutePath()));

    // A resource file whose JSON root does not match the live config's name,
    // so appConfig->load() must fail.
    QString resConfigFile = workDir.absoluteFilePath("resourceModel.json");
    QFile resFile(resConfigFile);
    QVERIFY(resFile.open(QIODevice::WriteOnly));
    resFile.write(QByteArray("{\"someOtherName\":{\"pages\":[]}}"));
    resFile.close();

    QString originalFileName = appConfig->fileName();
    QString originalName = appConfig->name();
    appConfig->setFileName(workDir.absoluteFilePath("liveConfig.json"));
    appConfig->setName(QStringLiteral("myAppConfig"));
    appConfig->clear();
    appConfig->saveValue("Volume", 1.0);

    QMenu menu;
    QAction* action = Development::createInitConfigFromResourcesMenuAction(&menu, resConfigFile);
    QVERIFY(action != nullptr);

    action->trigger();
    QTest::qWait(50);

    // The in-memory configuration is left untouched when load() fails...
    QCOMPARE(appConfig->restoreAsVariant("Volume", 0).toDouble(), 1.0);

    // ...but the file on disk has already been overwritten with the bad
    // content: this is the documented trade-off of writing before reloading.
    // It only gets corrected again by the next appConfig->save().
    QFile liveFile(appConfig->fileName());
    QVERIFY(liveFile.open(QIODevice::ReadOnly));
    QVERIFY(liveFile.readAll().contains("someOtherName"));

    appConfig->setFileName(originalFileName);
    appConfig->setName(originalName);
    appConfig->clear();
}

QTEST_MAIN(TestQtNoidAppDevelopment)
#include "test_app_development.moc"
