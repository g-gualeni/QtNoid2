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
    void testConfigGlobalProvidesAccessToConfig();

    // More
    void testAppConfigFileName();
    void testAppConfigSaveAndLoad();
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

void TestQtNoidAppConfigGlobal::testAppConfigFileName()
{
    QString expected = Settings::filePathAsAppSibling();
    // qDebug() << appConfig->fileName();
    QCOMPARE(appConfig->fileName(), expected);
}

void TestQtNoidAppConfigGlobal::testAppConfigSaveAndLoad()
{
    // Set config name and add a parameter
    qDebug() << __func__ << qAppName();
    appConfig->setName(qAppName());
    ParameterList* paramList = appConfig->emplace("Settings", "Settings");
    paramList->emplace(42.0, "TestParam", "Test parameter");

    // Save
    bool saveResult = appConfig->save();
    QVERIFY(saveResult);

    // Modify value
    appConfig->saveValue("TestParam", 99.0);
    QCOMPARE(appConfig->restoreAsVariant("TestParam", "DefValue").toDouble(), 99.0);

    // Load - should restore original value
    bool loadResult = appConfig->load();
    QVERIFY(loadResult);
    QCOMPARE(appConfig->restoreAsVariant("TestParam", "DefVal").toDouble(), 42.0);
}


QTEST_MAIN(TestQtNoidAppConfigGlobal)
#include "test_configglobal.moc"
