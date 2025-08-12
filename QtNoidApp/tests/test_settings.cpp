#include <QTest>
#include <QtNoidApp/QtNoidApp>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPainter>
#include <QPdfWriter>
#include <QPushButton>

class TestQtNoidAppSettings : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testAppExeOrAppBundlePath();
    void testFilePathAsAppSibling_data();
    void testFilePathAsAppSibling();
    void testFilePathAsAppSiblingWithEmptyFileName();
    void testMainWindowsFromWidget();
    void testGroupNameFromClass();

private:

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

void TestQtNoidAppSettings::testAppExeOrAppBundlePath()
{
    auto actual = Settings::appExeOrAppBundlePath();
    auto expected = qApp->applicationFilePath();
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
    auto myWidget = new QLabel("TEST");
    frm.centralWidget()->layout()->addWidget(myWidget);

    // Save the MainWindows as a dialog for debug purpose
    auto pixMap = frm.grab();
    auto path = qApp->applicationDirPath() + "/" + __func__ + ".png";
    pixMap.save(path);

    auto actual = Settings::mainWindowFromWidget(myWidget);
    auto expected = &frm;
    QCOMPARE(actual, expected);
}

void TestQtNoidAppSettings::testGroupNameFromClass()
{
    auto actual = Settings::groupNameFromClass(new QLabel("Test"));
    auto expected ="QLabel";
    QCOMPARE(actual, expected);

}


QTEST_MAIN(TestQtNoidAppSettings)
#include "test_settings.moc"


