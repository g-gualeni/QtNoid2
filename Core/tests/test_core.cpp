#include <QTest>
#include <QtNoidCore/QtNoidCore>

class TestCore : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    void testVersion();
    void testBuildInfo();

private:
    QtNoid::Core* m_core;
};

void TestCore::initTestCase()
{
}

void TestCore::cleanupTestCase()
{
}

void TestCore::init()
{
}

void TestCore::cleanup()
{
}

void TestCore::testVersion()
{
    QString version = QtNoid::Core::version();
    QVERIFY(!version.isEmpty());
    QCOMPARE(version, QString("2.0.0"));
}

void TestCore::testBuildInfo()
{
    QString buildInfo = QtNoid::Core::buildInfo();
    qDebug() << buildInfo;
    QVERIFY(!buildInfo.isEmpty());
    QVERIFY(buildInfo.contains("QtNoidCore"));
    QVERIFY(buildInfo.contains("2.0.0"));
    QVERIFY(buildInfo.contains("Qt"));
    QVERIFY(buildInfo.contains("Gianbattista Gualeni"));
}


QTEST_MAIN(TestCore)
#include "test_core.moc"
