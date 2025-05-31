#include <QTest>
#include <QtNoidCore/QtNoidCore>
#include <QSignalSpy>

class TestCore : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    
    // Test di base
    void testVersion();
    void testBuildInfo();

private:
    QtNoid::Core* m_core;
};

void TestCore::initTestCase()
{
    // Setup globale per tutti i test
    qDebug() << "Starting QtNoid::Core tests";
}

void TestCore::cleanupTestCase()
{
    // Cleanup globale
    qDebug() << "Finished QtNoid::Core tests";
}

void TestCore::init()
{
    // Setup per ogni singolo test
    m_core = new QtNoid::Core(this);
}

void TestCore::cleanup()
{
    // Cleanup per ogni singolo test
    delete m_core;
    m_core = nullptr;
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
