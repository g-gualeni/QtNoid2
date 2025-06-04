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


QTEST_MAIN(TestFile)
#include "test_file.moc"
