#include <QSignalSpy>
#include <QTest>
#include <QJsonObject>
#include <QJsonArray>
#include <QtNoidApp/QtNoidApp>

using namespace QtNoid::App;

class TestQtNoidAppConfigGlobal : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Basic singleton tests
    void testConfigGlobalInstanceIsNotNull();
    void testConfigGlobalInstanceIsSingleton();
    void testAppConfigMacro();

    // Config access tests
    void testConfigGlobalProvidesAccessToConfig();
    // void testConfigGlobalCanAddParameterLists();
    // void testConfigGlobalCanAccessParameterLists();
    // void testConfigGlobalCanRemoveParameterLists();

    // // Nested parameter access tests
    // void testConfigGlobalNestedParameterAccess();
    // void testConfigGlobalNestedValueAccess();

    // // JSON serialization tests
    // void testConfigGlobalJsonSerialization();
    // void testConfigGlobalJsonDeserialization();

    // // Persistence across calls
    // void testConfigGlobalPersistsAcrossMultipleCalls();
    // void testConfigGlobalModificationsArePersistent();

    // // Signal forwarding tests
    // void testConfigGlobalSignals();
};


void TestQtNoidAppConfigGlobal::initTestCase()
{
}

void TestQtNoidAppConfigGlobal::cleanupTestCase()
{
}

void TestQtNoidAppConfigGlobal::init()
{
    // Clean up any existing state in the global config before each tests
    Config* cfg = ConfigGlobal::instance();
    cfg->clear();
    // qDebug() << __func__;
}

void TestQtNoidAppConfigGlobal::cleanup()
{
}

void TestQtNoidAppConfigGlobal::testConfigGlobalInstanceIsNotNull()
{
    Config* instance = ConfigGlobal::instance();
    QVERIFY(instance != nullptr);
}

void TestQtNoidAppConfigGlobal::testConfigGlobalInstanceIsSingleton()
{
    Config* instance1 = ConfigGlobal::instance();
    Config* instance2 = ConfigGlobal::instance();

    // Both calls should return the same instance
    QCOMPARE(instance1, instance2);
}

void TestQtNoidAppConfigGlobal::testAppConfigMacro()
{
    QVERIFY(appConfig != nullptr);
    // Test that the appConfig macro works
    Config* expected = ConfigGlobal::instance();
    QCOMPARE(appConfig, expected);
}

void TestQtNoidAppConfigGlobal::testConfigGlobalProvidesAccessToConfig()
{
    // Test that we can access Config methods
    QVERIFY(appConfig->isEmpty());
    QCOMPARE(appConfig->count(), 0);
}


QTEST_MAIN(TestQtNoidAppConfigGlobal)
#include "test_configglobal.moc"
