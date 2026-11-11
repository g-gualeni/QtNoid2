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

    void testCreateSaveConfigToProjectMenuAction();
    void testCreateSaveConfigToProjectMenuAction_withNullMenu();
    void testCreateSaveConfigToProjectMenuAction_withEmptyProjectFolder();
    void testCreateSaveConfigToProjectMenuAction_withNoQrcFile();
    void testCreateSaveConfigToProjectMenuAction_withNoMatchingConfigFile();
    void testCreateSaveConfigToProjectMenuAction_withNoMenuParent();

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


void TestQtNoidAppDevelopment::testCreateSaveConfigToProjectMenuAction()
{
    // Project layout: a .qrc file next to the "old" copy of the config file
    // that createSaveConfigToProjectMenuAction() is supposed to refresh.
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

    // Point the live appConfig at a controlled file, with different content,
    // that should end up copied over the project's resource copy.
    QString originalFileName = appConfig->fileName();
    QString liveConfigPath = projectDir.absoluteFilePath(configFileName);
    appConfig->setFileName(liveConfigPath);
    QFile liveConfig(liveConfigPath);
    QVERIFY(liveConfig.open(QIODevice::WriteOnly | QIODevice::Text));
    liveConfig.write("{\"new\":\"content\"}");
    liveConfig.close();

    QWidget menuParent;
    QMenu menu(&menuParent);

    QAction* action = Development::createSaveConfigToProjectMenuAction(&menu, projectDir.absolutePath());

    QVERIFY(action != nullptr);
    QVERIFY(menu.actions().contains(action));
    QVERIFY2(!action->statusTip().isEmpty(), "The action should have a status tip");

    action->trigger();
    QTest::qWait(50);

    QFile updatedProjectConfig(projectDir.absoluteFilePath("resources/" + configFileName));
    QVERIFY(updatedProjectConfig.open(QIODevice::ReadOnly | QIODevice::Text));
    QCOMPARE(updatedProjectConfig.readAll(), QByteArray("{\"new\":\"content\"}"));

    appConfig->setFileName(originalFileName);
}

void TestQtNoidAppDevelopment::testCreateSaveConfigToProjectMenuAction_withNullMenu()
{
    QAction* action = Development::createSaveConfigToProjectMenuAction(nullptr, "some/path");
    QCOMPARE(action, nullptr);
}

void TestQtNoidAppDevelopment::testCreateSaveConfigToProjectMenuAction_withEmptyProjectFolder()
{
    QMenu menu;
    QAction* action = Development::createSaveConfigToProjectMenuAction(&menu, QString());
    QCOMPARE(action, nullptr);
}

void TestQtNoidAppDevelopment::testCreateSaveConfigToProjectMenuAction_withNoQrcFile()
{
    QDir projectDir(QDir::currentPath() + "/" + __func__ + "_project");
    projectDir.removeRecursively();
    QVERIFY(projectDir.mkpath(projectDir.absolutePath()));

    QMenu menu;
    QAction* action = Development::createSaveConfigToProjectMenuAction(&menu, projectDir.absolutePath());

    QCOMPARE(action, nullptr);
    // No dead action must be left attached to the menu.
    QCOMPARE(menu.actions().count(), 0);
}

void TestQtNoidAppDevelopment::testCreateSaveConfigToProjectMenuAction_withNoMatchingConfigFile()
{
    QDir projectDir(QDir::currentPath() + "/" + __func__ + "_project");
    projectDir.removeRecursively();
    QVERIFY(projectDir.mkpath(projectDir.absolutePath()));

    // A .qrc exists, but nothing next to it matches appConfig's file name/suffix.
    QFile qrcFile(projectDir.absoluteFilePath("app.qrc"));
    QVERIFY(qrcFile.open(QIODevice::WriteOnly));
    qrcFile.close();

    QMenu menu;
    QAction* action = Development::createSaveConfigToProjectMenuAction(&menu, projectDir.absolutePath());

    QCOMPARE(action, nullptr);
    QCOMPARE(menu.actions().count(), 0);
}

void TestQtNoidAppDevelopment::testCreateSaveConfigToProjectMenuAction_withNoMenuParent()
{
    QDir projectDir(QDir::currentPath() + "/" + __func__ + "_project");
    projectDir.removeRecursively();
    QVERIFY(projectDir.mkpath(projectDir.absolutePath()));

    QFile qrcFile(projectDir.absoluteFilePath("app.qrc"));
    QVERIFY(qrcFile.open(QIODevice::WriteOnly));
    qrcFile.close();

    QString configFileName = "myAppConfig.json";
    QFile projectConfig(projectDir.absoluteFilePath(configFileName));
    QVERIFY(projectConfig.open(QIODevice::WriteOnly | QIODevice::Text));
    projectConfig.close();

    QString originalFileName = appConfig->fileName();
    appConfig->setFileName(projectDir.absoluteFilePath(configFileName));

    // A QMenu with no parent - the config file is found, but there is no
    // QObject to parent the new QAction/connection to, so it must bail out.
    QMenu menu;
    QAction* action = Development::createSaveConfigToProjectMenuAction(&menu, projectDir.absolutePath());

    QCOMPARE(action, nullptr);
    QCOMPARE(menu.actions().count(), 0);

    appConfig->setFileName(originalFileName);
}


QTEST_MAIN(TestQtNoidAppDevelopment)
#include "test_app_development.moc"
