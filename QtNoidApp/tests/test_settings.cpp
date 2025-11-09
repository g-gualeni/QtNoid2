#include <QTest>
#include <QtGui/qclipboard.h>
#include <QtGui/qshortcut.h>
#include <QtNoidApp/QtNoidApp>
#include <QColorSpace>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPainter>
#include <QPdfWriter>
#include <QPushButton>
#include <QMimeData>

class TestQtNoidAppSettings : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testAppExeOrAppBundleFilePath();
    void testAppExeOrAppBundleDirPath();
    void testFilePathAsAppSibling_data();
    void testFilePathAsAppSibling();
    void testFilePathAsAppSiblingWithEmptyFileName();
    void testMainWindowsFromWidget();
    void testMainWindowsFromWidget_usingTheMainWindow();
    void testGroupNameFromObjectOrClassUsingClass();
    void testGroupNameFromObjectOrClassUsingObjectName();
    void testUpdateMainWindowTitle_shouldFail();
    void testUpdateMainWindowTitle_data();
    void testUpdateMainWindowTitle();
    void testUpdateMainWindowTitle_multipleChange();

    void testFullDialogGrab_shouldBeEmpty();
    void testFullDialogGrab();

    void testInitFullDialogGrabShortcut();
    void testInitFullDialogGrabShortcut_withFileName();



private:
    void initMainDialog(QMainWindow* frm, const QString &caption);


};

using namespace QtNoid::App;


void TestQtNoidAppSettings::initTestCase()
{}

void TestQtNoidAppSettings::cleanupTestCase()
{}

void TestQtNoidAppSettings::init()
{}

void TestQtNoidAppSettings::cleanup()
{}

void TestQtNoidAppSettings::initMainDialog(QMainWindow* frm, const QString& caption)
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

void TestQtNoidAppSettings::testAppExeOrAppBundleFilePath()
{
    auto actual = QtNoid::App::Settings::appExeOrAppBundleFilePath();
    auto expected = qApp->applicationFilePath();
    QCOMPARE(actual, expected);
}

void TestQtNoidAppSettings::testAppExeOrAppBundleDirPath()
{
    auto actual = Settings::appExeOrAppBundleDirPath();
    auto expected = qApp->applicationDirPath();
    QCOMPARE(actual, expected);
}

void TestQtNoidAppSettings::testFilePathAsAppSibling_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::addRow("FileName") <<"moon.ini";
    QTest::addRow("FileName WithPath") <<"c:/a/b/c/mars.ini";
}

void TestQtNoidAppSettings::testFilePathAsAppSibling()
{
    QFETCH(QString, fileName);
    auto actual = Settings::filePathAsAppSibling(fileName);

    QString expected = qApp->applicationDirPath();
    auto split = fileName.split("/").last();
    expected +=  + "/" + split;
    QCOMPARE(actual, expected);
}

void TestQtNoidAppSettings::testFilePathAsAppSiblingWithEmptyFileName()
{
    auto actual = Settings::filePathAsAppSibling();
    auto expected = qApp->applicationDirPath() + "/" + qApp->applicationName() + ".json";
    QCOMPARE(actual, expected);
}

void TestQtNoidAppSettings::testMainWindowsFromWidget()
{
    QMainWindow frm;
    frm.setCentralWidget(new QWidget());
    frm.centralWidget()->setLayout(new QHBoxLayout());
    auto myWidget = new QLabel("testMainWindowsFromWidget");
    frm.centralWidget()->layout()->addWidget(myWidget);

    // Save the MainWindows as a dialog for debug purpose
    auto pixMap = frm.grab();
    auto path = qApp->applicationDirPath() + "/" + __func__ + ".png";
    pixMap.save(path);

    auto actual = Settings::mainWindowFromWidget(myWidget);
    auto expected = &frm;
    QCOMPARE(actual, expected);
}

void TestQtNoidAppSettings::testMainWindowsFromWidget_usingTheMainWindow()
{
    QMainWindow frm;
    frm.setCentralWidget(new QWidget());
    frm.centralWidget()->setLayout(new QHBoxLayout());
    auto myWidget = new QLabel("testMainWindowsFromWidget_usingTheMainWindow");
    frm.centralWidget()->layout()->addWidget(myWidget);

    // Save the MainWindows as a dialog for debug purpose
    auto pixMap = frm.grab();
    auto path = qApp->applicationDirPath() + "/" + __func__ + ".png";
    pixMap.save(path);

    auto actual = Settings::mainWindowFromWidget(&frm);
    auto expected = &frm;
    QCOMPARE(actual, expected);

}

void TestQtNoidAppSettings::testGroupNameFromObjectOrClassUsingClass()
{
    auto actual = Settings::groupNameFromObjectOrClass(new QLabel("Test"));
    auto expected ="QLabel";
    QCOMPARE(actual, expected);
}

void TestQtNoidAppSettings::testGroupNameFromObjectOrClassUsingObjectName()
{
    auto lbl = new QLabel("Test");
    lbl->setObjectName("MyObjectIsBetter");

    auto actual = Settings::groupNameFromObjectOrClass(lbl);
    auto expected ="MyObjectIsBetter";
    QCOMPARE(actual, expected);
}

void TestQtNoidAppSettings::testUpdateMainWindowTitle_shouldFail()
{
    auto res = Settings::updateMainWindowTitle(true, nullptr);
    QCOMPARE(res, false);
}

void TestQtNoidAppSettings::testUpdateMainWindowTitle_data()
{
    QTest::addColumn<QString>("title");
    QTest::addColumn<bool>("modified");
    QTest::addColumn<QString>("expected");

    QTest::addRow("No Changes") <<"MoonApp" << false << "MoonApp";
    QTest::addRow("Set Changed") <<"MoonApp" << true << "MoonApp*";
    QTest::addRow("With Space and Not Changed") <<"MoonApp " << false << "MoonApp";
    QTest::addRow("With Space and Set Changed") <<"MoonApp " << true << "MoonApp*";
}


void TestQtNoidAppSettings::testUpdateMainWindowTitle()
{
    QFETCH(QString, title);
    QFETCH(bool, modified);
    QFETCH(QString, expected);

    QMainWindow frm;
    frm.setWindowTitle(title);
    frm.setCentralWidget(new QWidget());
    auto res = Settings::updateMainWindowTitle(modified, frm.centralWidget());
    QCOMPARE(res, true);

    QCOMPARE(frm.windowTitle(), expected);
}

void TestQtNoidAppSettings::testUpdateMainWindowTitle_multipleChange()
{
    QMainWindow frm;

    frm.setWindowTitle("We Are Ready");
    frm.setCentralWidget(new QWidget());
    auto res = Settings::updateMainWindowTitle(true, frm.centralWidget());
    QCOMPARE(res, true);
    res = Settings::updateMainWindowTitle(false, frm.centralWidget());
    QCOMPARE(res, true);
    res = Settings::updateMainWindowTitle(true, frm.centralWidget());
    QCOMPARE(res, true);
    auto expected = "We Are Ready*";
    QCOMPARE(frm.windowTitle(), expected);
}

void TestQtNoidAppSettings::testFullDialogGrab_shouldBeEmpty()
{
    auto expected = QImage();
    auto actual = Settings::fullDialogGrab(nullptr);
    QCOMPARE(actual, expected);
}

void TestQtNoidAppSettings::testFullDialogGrab()
{
    QMainWindow frm;
    frm.setWindowTitle("MyMainWindowsCaption");
    frm.setCentralWidget(new QWidget());
    frm.centralWidget()->setLayout(new QHBoxLayout());
    auto myWidget = new QLabel("testFullDialogGrab");
    frm.centralWidget()->layout()->addWidget(myWidget);
    frm.setFixedWidth(400);

    frm.show();
    auto res = QTest::qWaitForWindowExposed(&frm);
    QCOMPARE(res, true);
    QTest::qWait(500);

    // Get the window content + decorations
    QRect windowRect = frm.frameGeometry();
    QPixmap pixMap  = frm.screen()->grabWindow(0,
                                            windowRect.x(),
                                            windowRect.y(),
                                            windowRect.width(),
                                            windowRect.height());
    // Validate the image size
    QVERIFY(pixMap.width() >= frm.width());
    QVERIFY(pixMap.height() >= frm.height());

    // Save the MainWindows as a dialog for debug purpose
    auto path = qApp->applicationDirPath() + "/" + __func__;
    QImage image = pixMap.toImage();
    image.setColorSpace(QColorSpace());

    image.save(path  + "_Expected.png");
    auto expected = image;

    auto actual = Settings::fullDialogGrab(&frm);
    actual.save(path + "_Actual.bmp");

    QCOMPARE(actual, expected);
}

void TestQtNoidAppSettings::testInitFullDialogGrabShortcut()
{
    QMainWindow frm;
    initMainDialog(&frm, __func__);

    // Actitate shortcut
    QShortcut *shortcut = Settings::initFullDialogGrabShortcut(&frm);

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
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    QVERIFY2(mimeData->hasImage(), "Clipboard should contain an image");
    auto actual = clipboard->image();
    // qDebug() << __func__ << actual.size();
    QCOMPARE(actual.size(), pixmap.size());
}


void TestQtNoidAppSettings::testInitFullDialogGrabShortcut_withFileName()
{
    QString sstFile = Settings::appExeOrAppBundleDirPath() + "/" + __func__ + ".png";
    // qDebug() << __func__ << sstFile;

    QMainWindow frm;
    initMainDialog(&frm, __func__);

    QString keySeq = "Ctrl+Shift+Q";

    // Actitate shortcut
    QShortcut *shortcut = Settings::initFullDialogGrabShortcut(&frm,
                                                               keySeq,
                                                               sstFile,
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




QTEST_MAIN(TestQtNoidAppSettings)
#include "test_settings.moc"


