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
    void testConfigFileLoadAndModifyAndSave();

    void testConfigFileShouldLoadFileOnCreation();
    void testConfigFileShouldSaveFileOnDestruction();
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
    QString testFileName = "test.json";
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
    QString testFileName = __func__ + QString(".json");

    // Create a ConfigFile and populate it with data
    ConfigFile configFile(testFileName, this);
    configFile.setName("TestConfig");
    configFile.setDescription("Test configuration for save/load");

    // Add a ParameterList with parameters
    ParameterList* paramList = configFile.emplace("Settings", "Application settings");
    QVERIFY(paramList != nullptr);

    paramList->emplace(100.0, "Volume", "Audio volume level");
    paramList->emplace("dark", "Theme", "UI theme");

    // Save to file
    bool saveResult = configFile.save();
    QVERIFY(saveResult);

    // Create a new ConfigFile and load from the same file
    ConfigFile loadedConfig(testFileName, this);


    bool loadResult = loadedConfig.load();
    QVERIFY(loadResult);

    // Verify loaded data matches saved data
    QCOMPARE(loadedConfig.count(), 1);

    ParameterList* loadedParamList = loadedConfig.parameterList("Settings");
    QVERIFY(loadedParamList != nullptr);
    QCOMPARE(loadedParamList->count(), 2);

    Parameter* loadedParam1 = loadedParamList->parameter("Volume");
    QVERIFY(loadedParam1 != nullptr);
    QCOMPARE(loadedParam1->value().toDouble(), 100.0);

    Parameter* loadedParam2 = loadedParamList->parameter("Theme");
    QVERIFY(loadedParam2 != nullptr);
    QCOMPARE(loadedParam2->value().toString(), "dark");


    QCOMPARE(loadedConfig.name(), "TestConfig");
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

void TestQtNoidAppConfigFile::testConfigFileLoadAndModifyAndSave()
{
    QString testFileName = __func__ + QString(".json");

    // Create initial config and save
    ConfigFile configFile1(testFileName, this);
    configFile1.setName("ModifyTest");

    ParameterList* paramList1 = configFile1.emplace("Settings", "Settings");
    Parameter* param1 = paramList1->emplace(50.0, "Brightness", "Screen brightness");
    param1->setMin(0.0);
    param1->setMax(100.0);

    bool saveResult1 = configFile1.save();
    QVERIFY(saveResult1);

    // Load the config
    ConfigFile configFile2(testFileName, this);
    configFile2.setName("ModifyTest");
    bool loadResult = configFile2.load();
    QVERIFY(loadResult);

    // Modify the loaded config
    ParameterList* loadedParamList = configFile2.parameterList("Settings");
    QVERIFY(loadedParamList != nullptr);

    Parameter* loadedParam = loadedParamList->parameter("Brightness");
    QVERIFY(loadedParam != nullptr);
    QCOMPARE(loadedParam->value().toDouble(), 50.0);

    // Change the value
    loadedParam->setValue(75.0);

    // Add a new parameter
    Parameter* newParam = loadedParamList->emplace(true, "DarkMode", "Enable dark mode");
    QVERIFY(newParam != nullptr);

    // Save the modified config
    bool saveResult2 = configFile2.save();
    QVERIFY(saveResult2);

    // Load again and verify modifications were saved
    ConfigFile configFile3(testFileName, this);
    configFile3.setName("ModifyTest");
    bool loadResult2 = configFile3.load();
    QVERIFY(loadResult2);

    ParameterList* finalParamList = configFile3.parameterList("Settings");
    QVERIFY(finalParamList != nullptr);
    QCOMPARE(finalParamList->count(), 2);

    Parameter* finalBrightness = finalParamList->parameter("Brightness");
    QVERIFY(finalBrightness != nullptr);
    QCOMPARE(finalBrightness->value().toDouble(), 75.0);

    Parameter* finalDarkMode = finalParamList->parameter("DarkMode");
    QVERIFY(finalDarkMode != nullptr);
    QCOMPARE(finalDarkMode->value().toBool(), true);

}

void TestQtNoidAppConfigFile::testConfigFileShouldLoadFileOnCreation()
{
    // Test that creation load a file
    QVERIFY(0);
}

void TestQtNoidAppConfigFile::testConfigFileShouldSaveFileOnDestruction()
{
    // Test that destruction save to a file using delete or when out of scope
    QVERIFY(0);
}

QTEST_MAIN(TestQtNoidAppConfigFile)
#include "test_configfile.moc"
