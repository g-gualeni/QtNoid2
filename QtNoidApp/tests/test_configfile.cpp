#include <QSignalSpy>
#include <QTest>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>
#include <QtNoidApp/QtNoidApp>

using namespace QtNoid::App;

class TestQtNoidAppConfigFile : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Basic constructor tests
    void testConfigFileDefaultConstructor();
    void testConfigFileConstructorWithFileName();

    // FileName property tests
    void testConfigFileFileNameProperty();
    void testConfigFileFileNameChanged();
    void testConfigFileFileNameNoEmitForSameValue();
    void testConfigFileSetFileName();
    void testConfigFileBindableFileName();

    // Integration with Config base class
    void testConfigFileInheritsFromConfig();

    // File operations tests
    void testConfigFileIsValid();
    void testConfigFileIsNotValidWithEmptyFileName();
    void testConfigFileSaveAndLoad();
    void testConfigFileSaveWithEmptyFileName();
    void testConfigFileSaveWithEmptyConfig();
    void testConfigFileLoadNonExistentFile();
    void testConfigFileLoadInvalidJson();
    void testConfigFileInit_Load_Modify_Save();

    void testConfigFileShouldLoadFileOnCreation();
    void testConfigFileShouldSaveFileOnDestruction();

    // Debug output test
    void testConfigFileDebugOutput();
};

void TestQtNoidAppConfigFile::initTestCase()
{
}

void TestQtNoidAppConfigFile::cleanupTestCase()
{
}

void TestQtNoidAppConfigFile::init()
{
}

void TestQtNoidAppConfigFile::cleanup()
{
}

void TestQtNoidAppConfigFile::testConfigFileDefaultConstructor()
{
    ConfigFile configFile(this);
    // qDebug() << __func__ << configFile.fileName();

    // Should be empty with the default constructor
    QVERIFY(configFile.fileName().isEmpty());

    // Should inherit Config properties
    QCOMPARE(configFile.count(), 0);
    QVERIFY(configFile.isEmpty());
}

void TestQtNoidAppConfigFile::testConfigFileConstructorWithFileName()
{
    QString testFileName = "test_config.json";
    ConfigFile configFile(testFileName, this);

    // Check fileName was set
    QCOMPARE(configFile.fileName(), testFileName);

    // Should inherit Config properties
    QCOMPARE(configFile.count(), 0);
    QVERIFY(configFile.isEmpty());
}

void TestQtNoidAppConfigFile::testConfigFileFileNameProperty()
{
    ConfigFile configFile(this);

    // Test setting fileName
    QString newFileName = "new_config.json";
    configFile.setFileName(newFileName);
    QCOMPARE(configFile.fileName(), newFileName);
}

void TestQtNoidAppConfigFile::testConfigFileFileNameChanged()
{
    ConfigFile configFile(this);
    QSignalSpy spy(&configFile, &ConfigFile::fileNameChanged);

    QVERIFY(spy.isValid());

    // Change the fileName and verify the signal is emitted
    QString expectedFileName = "changed_config.json";
    configFile.setFileName(expectedFileName);
    QCOMPARE(spy.count(), 1);

    // Check the signal argument
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), expectedFileName);
}

void TestQtNoidAppConfigFile::testConfigFileFileNameNoEmitForSameValue()
{
    QString testFileName = "same_config.json";
    ConfigFile configFile(testFileName, this);
    QSignalSpy spy(&configFile, &ConfigFile::fileNameChanged);

    // Same value, no signal should be emitted
    configFile.setFileName(testFileName);
    QCOMPARE(spy.count(), 0);
}

void TestQtNoidAppConfigFile::testConfigFileSetFileName()
{
    ConfigFile configFile(this);
    QSignalSpy spy(&configFile, &ConfigFile::fileNameChanged);

    // Set fileName
    QString fileName1 = "config1.json";
    configFile.setFileName(fileName1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(configFile.fileName(), fileName1);

    // Set different fileName
    QString fileName2 = "config2.json";
    configFile.setFileName(fileName2);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(configFile.fileName(), fileName2);
}

void TestQtNoidAppConfigFile::testConfigFileBindableFileName()
{
    ConfigFile configFile("initial.json", this);

    // Get bindable fileName
    auto bindableFileName = configFile.bindableFileName();
    QVERIFY(bindableFileName.isValid());
    QCOMPARE(bindableFileName.value(), "initial.json");

    // Test binding to another QProperty
    QProperty<QString> externalProperty;
    externalProperty.setBinding([&]() { return configFile.bindableFileName().value(); });
    QCOMPARE(externalProperty.value(), "initial.json");

    // Change fileName and verify binding updates
    configFile.setFileName("updated.json");
    QCOMPARE(externalProperty.value(), "updated.json");

    // Test setting fileName through bindable
    bindableFileName.setValue("final.json");
    QCOMPARE(configFile.fileName(), "final.json");
    QCOMPARE(externalProperty.value(), "final.json");
}

void TestQtNoidAppConfigFile::testConfigFileInheritsFromConfig()
{
    QString testFileName = __func__ + QString(".json");
    QFile::remove(testFileName);

    ConfigFile configFile(testFileName, this);

    // Test that ConfigFile can use Config methods
    QVERIFY(configFile.isEmpty());
    QCOMPARE(configFile.count(), 0);

    // Add a ParameterList
    ParameterList* paramList = configFile.emplace("TestList", "Test description");
    QVERIFY(paramList != nullptr);
    QCOMPARE(configFile.count(), 1);
    QVERIFY(!configFile.isEmpty());

    // Verify fileName is still accessible
    QCOMPARE(configFile.fileName(), testFileName);

    // Test name property from Config
    configFile.setName("TestConfig");
    QCOMPARE(configFile.name(), "TestConfig");
}

void TestQtNoidAppConfigFile::testConfigFileIsValid()
{
    // Test with valid fileName
    ConfigFile configFile("valid_config.json", this);
    QVERIFY(configFile.isValid());

    // Test with another valid fileName
    ConfigFile configFile2("another_config.json", this);
    QVERIFY(configFile2.isValid());
}

void TestQtNoidAppConfigFile::testConfigFileIsNotValidWithEmptyFileName()
{
    // Test with empty fileName
    ConfigFile configFile(this);
    QVERIFY(!configFile.isValid());

    // Set a fileName and verify it becomes valid
    configFile.setFileName("now_valid.json");
    QVERIFY(configFile.isValid());
}

void TestQtNoidAppConfigFile::testConfigFileSaveAndLoad()
{
    QString testFileName = __func__ + QStringLiteral(".json");
    QFile::remove(testFileName);

    // Create a ConfigFile and populate it with data
    ConfigFile configFile(testFileName, this);
    configFile.setName("TestConfig");

    // Add 2 Parameters
    configFile.saveValue("Volume", 100.0);
    configFile.saveValue("Theme", QStringLiteral("light"));

    // Save to file
    bool saveResult = configFile.save();
    QVERIFY(saveResult);

    // Create a new ConfigFile and load from the same file
    ConfigFile loadedConfig(testFileName, this);
    // qDebug() << __func__ << configFile;
    // qDebug() << __func__ << loadedConfig;

    QCOMPARE(loadedConfig.name(), "TestConfig");
    QCOMPARE(loadedConfig.count(), 1);
    QCOMPARE(loadedConfig.parametersCount(), 2);

    auto volume = loadedConfig.restoreAsDouble("Volume", 0.0);
    QCOMPARE(volume, 100.0);

    auto theme = loadedConfig.restoreAsString("Theme", "");
    QCOMPARE(theme, "light");

}

void TestQtNoidAppConfigFile::testConfigFileSaveWithEmptyFileName()
{
    // Test save with empty fileName should fail
    ConfigFile configFile(this);

    // Add some data
    configFile.emplace("TestList", "Test description");

    // Try to save without a fileName
    bool saveResult = configFile.save();
    QVERIFY(!saveResult);
}

void TestQtNoidAppConfigFile::testConfigFileSaveWithEmptyConfig()
{
    QString testFileName = __func__ + QString(".json");

    // Test save with empty config should fail
    ConfigFile configFile(testFileName, this);
    configFile.setName("EmptyConfig");

    // Try to save empty config
    bool saveResult = configFile.save();
    QVERIFY(!saveResult);

}

void TestQtNoidAppConfigFile::testConfigFileLoadNonExistentFile()
{
    QString nonExistentFile = "non_existent_file.json";

    // Ensure file doesn't exist
    QFile::remove(nonExistentFile);

    ConfigFile configFile(nonExistentFile, this);

    // Try to load non-existent file
    bool loadResult = configFile.load();
    QVERIFY(!loadResult);
}

void TestQtNoidAppConfigFile::testConfigFileLoadInvalidJson()
{
    QString testFileName = __func__ + QString(".json");

    // Create a file with invalid JSON
    QFile file(testFileName);
    QVERIFY(file.open(QIODevice::WriteOnly));
    QTextStream out(&file);
    out << "{ this is not valid json }";
    file.close();

    ConfigFile configFile(testFileName, this);

    // Try to load invalid JSON
    bool loadResult = configFile.load();
    QVERIFY(!loadResult);

}

void TestQtNoidAppConfigFile::testConfigFileInit_Load_Modify_Save()
{
    QString testFileName = __func__ + QString(".json");
    QFile::remove(testFileName);

    // Create initial config and save
    ConfigFile configFile1(testFileName, this);
    configFile1.setName("ModifyTest");

    auto res = configFile1.saveValue("Brightness", 50.00);
    QVERIFY(res);
    res = configFile1.save();
    QVERIFY(res);


    // Load the config and verify brightness
    ConfigFile configFile2(testFileName, this);
    QCOMPARE(configFile2.name(), "ModifyTest");
    QCOMPARE(configFile2.restoreAsDouble("Brightness", 0.0), 50.00);

    // Modify configFile2
    configFile2.saveValue("Brightness", 100.00);
    QCOMPARE(configFile2.restoreAsDouble("Brightness", 0.0), 100.00);

    // Add a new parameter
    res = configFile2.saveValue("DarkMode", true);
    QVERIFY(res);

    // Save the modified config
    res = configFile2.save();
    QVERIFY(res);

    // Load again and verify modifications were saved
    ConfigFile configFile3(testFileName, this);
    QCOMPARE(configFile3.restoreAsDouble("Brightness", 0.0), 100.00);
    QCOMPARE(configFile3.restoreAsBool("DarkMode", false), true);

}

void TestQtNoidAppConfigFile::testConfigFileShouldLoadFileOnCreation()
{

    QString fileName = __func__ + QStringLiteral(".json");
    QFile::remove(fileName);
    ConfigFile cfg1(fileName, this);
    cfg1.setName(qAppName());
    cfg1.saveValue("JustOneValue", QStringLiteral("ABC"));
    cfg1.save();
    qDebug() << cfg1.fileName() << cfg1;

    // cfg2 should load the file automatically
    ConfigFile cfg2(fileName, this);
    QString actual = cfg2.restoreAsString("JustOneValue", "");
    QCOMPARE(actual, "ABC");
}

void TestQtNoidAppConfigFile::testConfigFileShouldSaveFileOnDestruction()
{

    QString fileName = __func__ + QStringLiteral(".json");
    QFile::remove(fileName);

    ConfigFile* cfg1 = new ConfigFile(fileName, this);
    cfg1->setName(qAppName());
    cfg1->saveValue("TestOnDestruction", QStringLiteral("CBA"));

    // Test that destruction save to a file using delete or when out of scope
    delete cfg1;

    // cfg2 should load the file automatically
    ConfigFile cfg2(fileName, this);
    QString actual = cfg2.restoreAsString("TestOnDestruction", "");
    QCOMPARE(actual, "CBA");

}

void TestQtNoidAppConfigFile::testConfigFileDebugOutput()
{
    QString fileName = __func__ + QStringLiteral(".json");
    QFile::remove(fileName);
    ConfigFile* configFile = nullptr;
    QString dbgOutput = QDebug::toString(configFile);

    QVERIFY(!dbgOutput.isEmpty());
    QVERIFY(dbgOutput.contains("ConfigFile(nullptr)"));

    configFile = new ConfigFile(fileName, this);
    dbgOutput = QDebug::toString(configFile);
    QVERIFY(dbgOutput.contains("Config"));

    configFile->saveValue("RunCounter", 123);
    configFile->saveValue("Active", true);
    dbgOutput = QDebug::toString(configFile);

    QVERIFY(dbgOutput.contains("RunCounter"));
    QVERIFY(dbgOutput.contains("Active"));
    qDebug() << __func__ << dbgOutput;
}


QTEST_MAIN(TestQtNoidAppConfigFile)
#include "test_configfile.moc"
