#include <QTest>
#include <QtNoidApp/QtNoidApp>
#include <QImage>
#include <QPainter>
#include <QPdfWriter>

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
    auto split = fileName.split("/").last();
    auto expected = qApp->applicationDirPath() + "/" + split;
    QCOMPARE(actual, expected);
}


QTEST_MAIN(TestQtNoidAppSettings)
#include "test_settings.moc"


