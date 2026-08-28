#include <QTest>
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

class TestQtNoidAppCore : public QObject
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



private:


};

using namespace QtNoid::App;


void TestQtNoidAppCore::initTestCase()
{}

void TestQtNoidAppCore::cleanupTestCase()
{}

void TestQtNoidAppCore::init()
{}

void TestQtNoidAppCore::cleanup()
{}

void TestQtNoidAppCore::testAppExeOrAppBundleFilePath()
{
    auto actual = QtNoid::App::Core::appExeOrAppBundleFilePath();
    auto expected = qApp->applicationFilePath();
    QCOMPARE(actual, expected);
}

void TestQtNoidAppCore::testAppExeOrAppBundleDirPath()
{
    auto actual = Core::appExeOrAppBundleDirPath();
    auto expected = qApp->applicationDirPath();
    QCOMPARE(actual, expected);
}

void TestQtNoidAppCore::testFilePathAsAppSibling_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::addRow("FileName") <<"moon.ini";
    QTest::addRow("FileName WithPath") <<"c:/a/b/c/mars.ini";
}

void TestQtNoidAppCore::testFilePathAsAppSibling()
{
    QFETCH(QString, fileName);
    auto actual = Core::filePathAsAppSibling(fileName);

    QString expected = qApp->applicationDirPath();
    auto split = fileName.split("/").last();
    expected +=  + "/" + split;
    QCOMPARE(actual, expected);
}

void TestQtNoidAppCore::testFilePathAsAppSiblingWithEmptyFileName()
{
    auto actual = Core::filePathAsAppSibling();
    auto expected = qApp->applicationDirPath() + "/" + qApp->applicationName() + ".json";
    QCOMPARE(actual, expected);
}

void TestQtNoidAppCore::testMainWindowsFromWidget()
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

    auto actual = Core::mainWindowFromWidget(myWidget);
    auto expected = &frm;
    QCOMPARE(actual, expected);
}

void TestQtNoidAppCore::testMainWindowsFromWidget_usingTheMainWindow()
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

    auto actual = Core::mainWindowFromWidget(&frm);
    auto expected = &frm;
    QCOMPARE(actual, expected);

}

void TestQtNoidAppCore::testGroupNameFromObjectOrClassUsingClass()
{
    auto actual = Core::groupNameFromObjectOrClass(new QLabel("Test"));
    auto expected ="QLabel";
    QCOMPARE(actual, expected);
}

void TestQtNoidAppCore::testGroupNameFromObjectOrClassUsingObjectName()
{
    auto lbl = new QLabel("Test");
    lbl->setObjectName("MyObjectIsBetter");

    auto actual = Core::groupNameFromObjectOrClass(lbl);
    auto expected ="MyObjectIsBetter";
    QCOMPARE(actual, expected);
}

void TestQtNoidAppCore::testUpdateMainWindowTitle_shouldFail()
{
    auto res = Core::updateMainWindowTitle(true, nullptr);
    QCOMPARE(res, false);
}

void TestQtNoidAppCore::testUpdateMainWindowTitle_data()
{
    QTest::addColumn<QString>("title");
    QTest::addColumn<bool>("modified");
    QTest::addColumn<QString>("expected");

    QTest::addRow("No Changes") <<"MoonApp" << false << "MoonApp";
    QTest::addRow("Set Changed") <<"MoonApp" << true << "MoonApp*";
    QTest::addRow("With Space and Not Changed") <<"MoonApp " << false << "MoonApp";
    QTest::addRow("With Space and Set Changed") <<"MoonApp " << true << "MoonApp*";
    QTest::addRow("Empty Title Should Not Crash") <<"" << true << "*";
}


void TestQtNoidAppCore::testUpdateMainWindowTitle()
{
    QFETCH(QString, title);
    QFETCH(bool, modified);
    QFETCH(QString, expected);

    QMainWindow frm;
    frm.setWindowTitle(title);
    frm.setCentralWidget(new QWidget());
    auto res = Core::updateMainWindowTitle(modified, frm.centralWidget());
    QCOMPARE(res, true);

    QCOMPARE(frm.windowTitle(), expected);
}

void TestQtNoidAppCore::testUpdateMainWindowTitle_multipleChange()
{
    QMainWindow frm;

    frm.setWindowTitle("We Are Ready");
    frm.setCentralWidget(new QWidget());
    auto res = Core::updateMainWindowTitle(true, frm.centralWidget());
    QCOMPARE(res, true);
    res = Core::updateMainWindowTitle(false, frm.centralWidget());
    QCOMPARE(res, true);
    res = Core::updateMainWindowTitle(true, frm.centralWidget());
    QCOMPARE(res, true);
    auto expected = "We Are Ready*";
    QCOMPARE(frm.windowTitle(), expected);
}

void TestQtNoidAppCore::testFullDialogGrab_shouldBeEmpty()
{
    auto expected = QImage();
    auto actual = Core::fullDialogGrab(nullptr);
    QCOMPARE(actual, expected);
}

void TestQtNoidAppCore::testFullDialogGrab()
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

    auto actual = Core::fullDialogGrab(&frm);
    actual.save(path + "_Actual.bmp");

    QCOMPARE(actual, expected);
}


QTEST_MAIN(TestQtNoidAppCore)
#include "test_app_core.moc"


