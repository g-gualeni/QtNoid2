#include <QSignalSpy>
#include <QTest>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>
#include <QtNoidApp/QtNoidApp>
#include <qtestcase.h>

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
    void testConfigFileSaveWithEmptyFileNameShouldEmitSaveError();
    void testConfigFileSaveOnWrongFileNameShoulEmitSaveError();
    void testConfigFileSaveErrorSignalNotEmittedOnSuccess();
    void testConfigFileSaveEmptyFileResetTheFileContent();
    void testConfigFileLoadNonExistentFile();
    void testConfigFileLoadInvalidJson();
    void testConfigFileLoadErrorSignalOnNonObjectJson();
    void testConfigFileLoadErrorSignalNotEmittedOnSuccess();
    void testConfigFileInit_Load_Modify_Save();
    void testConfigFileLoadShouldReplacesContent();
    void testConfigFileLoadingTheWrongFileShouldNotEraseExistingContent();

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

    // Add a ParametersPage
    ParametersPage* paramList = configFile.emplace("TestList", "Test description");
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


void TestQtNoidAppConfigFile::testConfigFileSaveWithEmptyFileNameShouldEmitSaveError()
{
    // save() with no fileName set must fail and report why via fileSaveError.
    ConfigFile configFile(this);
    QSignalSpy errorSpy(&configFile, &ConfigFile::fileSaveError);

    bool saveResult = configFile.save();
    QVERIFY(!saveResult);
    QCOMPARE(errorSpy.count(), 1);

    auto arguments = errorSpy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString());
    QVERIFY(!arguments.at(1).toString().isEmpty());
}

void TestQtNoidAppConfigFile::testConfigFileSaveOnWrongFileNameShoulEmitSaveError()
{
    // A fileName inside a directory that doesn't exist can't be opened for
    // writing: save() must fail and report why via fileSaveError.
    QString testFileName = QStringLiteral("no_such_directory_for_configfile_test/") + __func__ + QStringLiteral(".json");

    ConfigFile configFile(this);
    configFile.setFileName(testFileName);
    QSignalSpy errorSpy(&configFile, &ConfigFile::fileSaveError);

    bool saveResult = configFile.save();
    QVERIFY(!saveResult);
    QCOMPARE(errorSpy.count(), 1);

    auto arguments = errorSpy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), testFileName);
    QVERIFY(!arguments.at(1).toString().isEmpty());
}

void TestQtNoidAppConfigFile::testConfigFileSaveErrorSignalNotEmittedOnSuccess()
{
    QString testFileName = __func__ + QString(".json");
    QFile::remove(testFileName);

    ConfigFile configFile(testFileName, this);
    configFile.saveValue("Volume", 50.0);

    QSignalSpy errorSpy(&configFile, &ConfigFile::fileSaveError);
    QVERIFY(configFile.save());
    QCOMPARE(errorSpy.count(), 0);
}

void TestQtNoidAppConfigFile::testConfigFileSaveEmptyFileResetTheFileContent()
{
    QString testFileName = __func__ + QString(".json");
    QFile::remove(testFileName);

    // Create a configuration file
    ConfigFile configFile(testFileName, this);
    configFile.setName("Settings File");
    configFile.saveValue("Volume", 100.0);
    QVERIFY(configFile.save());

    // Reset the ConfigFile content
    configFile.clear();
    QVERIFY(configFile.isEmpty());

    // Save the file
    QVERIFY(configFile.save());

    // Check that the file is empty
    ConfigFile reloadFile(testFileName, this);
    QVERIFY(reloadFile.isEmpty());
    QCOMPARE(reloadFile.toJsonValues(), configFile.toJsonValues());

}

void TestQtNoidAppConfigFile::testConfigFileLoadNonExistentFile()
{
    QString nonExistentFile = "non_existent_file.json";

    // Ensure file doesn't exist
    QFile::remove(nonExistentFile);

    ConfigFile configFile(this);
    QSignalSpy errorSpy(&configFile, &ConfigFile::fileLoadError);
    QSignalSpy loadedSpy(&configFile, &ConfigFile::fileLoaded);
    configFile.setFileName(nonExistentFile);

    // Try to load non-existent file
    bool loadResult = configFile.load();
    QVERIFY(!loadResult);

    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(loadedSpy.count(), 0);
    auto arguments = errorSpy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), nonExistentFile);
    QVERIFY(!arguments.at(1).toString().isEmpty());
}

void TestQtNoidAppConfigFile::testConfigFileLoadInvalidJson()
{
    QString testFileName = __func__ + QString(".json");

    // Create a file with syntactically invalid JSON.
    QFile file(testFileName);
    QVERIFY(file.open(QIODevice::WriteOnly));
    QTextStream out(&file);
    out << "{ this is not valid json }";
    file.close();

    // Use the default constructor + setFileName() rather than the
    // fileName constructor, which would call load() immediately and fire
    // the signal before the spy below gets a chance to connect to it.
    ConfigFile configFile(this);
    QSignalSpy errorSpy(&configFile, &ConfigFile::fileLoadError);
    QSignalSpy loadedSpy(&configFile, &ConfigFile::fileLoaded);

    configFile.setFileName(testFileName);
    bool loadResult = configFile.load();
    QVERIFY(!loadResult);

    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(loadedSpy.count(), 0);
    auto arguments = errorSpy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), testFileName);
    QVERIFY(!arguments.at(1).toString().isEmpty());
}

void TestQtNoidAppConfigFile::testConfigFileLoadErrorSignalOnNonObjectJson()
{
    QString testFileName = __func__ + QString(".json");

    // Syntactically valid JSON, but not an object: load() must still fail
    // without crashing, and fileLoadError must fire explaining why.
    QFile file(testFileName);
    QVERIFY(file.open(QIODevice::WriteOnly));
    QTextStream out(&file);
    out << "[1, 2, 3]";
    file.close();

    ConfigFile configFile(this);
    configFile.setFileName(testFileName);

    QSignalSpy errorSpy(&configFile, &ConfigFile::fileLoadError);

    QVERIFY(!configFile.load());
    QCOMPARE(errorSpy.count(), 1);

    auto arguments = errorSpy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), testFileName);
    QVERIFY(!arguments.at(1).toString().isEmpty());
}

void TestQtNoidAppConfigFile::testConfigFileLoadErrorSignalNotEmittedOnSuccess()
{
    QString testFileName = __func__ + QString(".json");
    QFile::remove(testFileName);

    // Save a valid, well-formed file first.
    ConfigFile configFile(testFileName, this);
    configFile.saveValue("Volume", 50.0);
    QVERIFY(configFile.save());

    QSignalSpy errorSpy(&configFile, &ConfigFile::fileLoadError);
    QSignalSpy loadedSpy(&configFile, &ConfigFile::fileLoaded);

    QVERIFY(configFile.load());
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(loadedSpy.count(), 1);
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

void TestQtNoidAppConfigFile::testConfigFileLoadShouldReplacesContent()
{
    // Reading a configFile should reset the Config content
    QString testFileName = __func__ + QString(".json");
    QFile::remove(testFileName);
    ConfigFile configFile(testFileName, this);
    configFile.saveValue("Volume", 50.0, "Page");
    auto res = configFile.save();
    QVERIFY(res);

    ConfigFile configFileA(testFileName, this);
    configFileA.saveValue("Bass", 50.0, "PageA");
    QCOMPARE(configFileA.count(), 2);

    configFileA.load(); // Load reset the ConfigFile content
    QCOMPARE(configFileA.count(), 1);
    QVERIFY(configFileA.contains("Page"));
    QVERIFY(!configFileA.contains("PageA"));

}

void TestQtNoidAppConfigFile::testConfigFileLoadingTheWrongFileShouldNotEraseExistingContent()
{
    // A load() that is rejected because the file's content doesn't match
    // this Config's name must not touch what was already in memory: the
    // validation has to happen before clear(), not after.
    QString testFileName = __func__ + QString(".json");
    QFile::remove(testFileName);

    // Save a file under a name that will NOT match the config under test.
    ConfigFile other(testFileName, this);
    other.setName("OtherConfig");
    other.saveValue("Volume", 50.0);
    QVERIFY(other.save());

    // This config already has its own name and content before attempting
    // to load the mismatched file.
    ConfigFile configFile(this);
    configFile.setName("MyConfig");
    configFile.saveValue("Brightness", 75.0);
    QCOMPARE(configFile.count(), 1);

    configFile.setFileName(testFileName);
    QSignalSpy errorSpy(&configFile, &ConfigFile::fileLoadError);
    QSignalSpy loadedSpy(&configFile, &ConfigFile::fileLoaded);

    bool loadResult = configFile.load();
    QVERIFY(!loadResult);
    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(loadedSpy.count(), 0);

    // The failed load must not have wiped the content that was already there.
    QCOMPARE(configFile.name(), "MyConfig");
    QCOMPARE(configFile.count(), 1);
    QCOMPARE(configFile.restoreAsDouble("Brightness", 0.0), 75.0);
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
#include "test_app_configfile.moc"
