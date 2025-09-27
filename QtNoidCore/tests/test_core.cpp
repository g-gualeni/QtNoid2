#include <QTest>
#include <QtNoidCore/QtNoidCore>

class TestQtNoidCore : public QObject
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

void TestQtNoidCore::initTestCase()
{
}

void TestQtNoidCore::cleanupTestCase()
{
}

void TestQtNoidCore::init()
{
}

void TestQtNoidCore::cleanup()
{
}

void TestQtNoidCore::testVersion()
{
    QString version = QtNoid::Core::version();
    QVERIFY(!version.isEmpty());
    QCOMPARE(version, QString("2.0.1"));
}

void TestQtNoidCore::testBuildInfo()
{
    QString buildInfo = QtNoid::Core::buildInfo();
    // qDebug() << buildInfo;
    QVERIFY(!buildInfo.isEmpty());
    QVERIFY(buildInfo.contains("QtNoidCore"));
    QVERIFY(buildInfo.contains("2.0.1"));
    QVERIFY(buildInfo.contains("Qt"));
    QVERIFY(buildInfo.contains("Gianbattista Gualeni"));
}


QTEST_MAIN(TestQtNoidCore)
#include "test_core.moc"
