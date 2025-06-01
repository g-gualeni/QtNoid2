#include <QTest>
#include <QtNoidCommon/QtNoidCommon>

class TestText : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    void testVersion();
    void testBuildInfo();
};

void TestText::initTestCase()
{
}

void TestText::cleanupTestCase()
{
}

void TestText::init()
{
}

void TestText::cleanup()
{
}

void TestText::testVersion()
{
}

void TestText::testBuildInfo()
{
}


QTEST_MAIN(TestText)
#include "test_text.moc"
