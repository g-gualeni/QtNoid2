#include <QTest>
#include <QtNoidCommon/QtNoidCommon>

class TestFile : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void autoNamingNextName_data();
    void autoNamingNextName();
};

using namespace QtNoid::Common;


void TestFile::initTestCase()
{}

void TestFile::cleanupTestCase()
{}

void TestFile::init()
{}

void TestFile::cleanup()
{}

void TestFile::autoNamingNextName_data()
{
    QTest::addColumn<QString>("currentName");
    QTest::addColumn<QString>("expected");
    QTest::newRow("file.tar.zip") << "file.tar.zip" << "file 001.tar.zip";
    QTest::newRow("file001Top.tar.zip") << "file001Top.tar.zip" << "file002Top.tar.zip";
    QTest::newRow("file") << "file.xyp" << "file 001.xyp";
    QTest::newRow("file00") << "file01.xyp" << "file02.xyp";
    QTest::newRow("file (1)") << "file (1).xyp" << "file (2).xyp";
    QTest::newRow("file-999") << "file-999.xyp" << "file-1000.xyp";
    QTest::newRow("file-999my") << "file-999my.xyp" << "file-1000my.xyp";
    QTest::newRow("Multiple000") << "file-01-007.xyp" << "file-01-008.xyp";
    QTest::newRow("MultipleEqual") << "file-01-01.xyp" << "file-01-02.xyp";

}

void TestFile::autoNamingNextName()
{
    QFETCH(QString, currentName);
    QFETCH(QString, expected);
    auto result = File::autoNamingNextName(currentName);
    QCOMPARE(result, expected);
}


QTEST_MAIN(TestFile)
#include "test_file.moc"
