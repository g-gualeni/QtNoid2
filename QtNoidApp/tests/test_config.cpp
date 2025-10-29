#include <QJsonArray>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>
#include <QtNoidApp/QtNoidApp>

using namespace QtNoid::App;

class TestQtNoidAppConfig : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Constructor tests
    void testDefaultConstructor();
    void testConstructorWithName();
    void testConstructorWithJson();

    // Property tests
    void testNameProperty();
    void testDescriptionProperty();
    void testTooltipProperty();
    void testCountProperty();

    // JSON serialization tests
    void testToJsonValues();
    void testToJsonSchema();
    void testConfigValuesFromJson();
    void testConfigSchemaFromJson();

    // Container management tests
    void testAppendPage();
    void testAppendWithJson();
    void testEmplacePage();
    void testEmplaceWithJson();
    void testRemovePage();
    void testRemoveNonExistentPage();
    void testClearPages();
    void testIsEmpty();

    // Access methods tests
    void testPageByIndex();
    void testPageByName();
    void testIndexOfPage();
    void testContainsPage();
    void testPagesList();
    void testParameterAndPageRenameShouldAutomaticallyUpdate();

    // Parameter access tests
    void testParameterAccess();
    void testParametersCount();

    // Operator tests
    void testOperatorStreamInsert();

    // Save/restore value tests
    void testSaveAndRestoreInt();
    void testSaveAndRestoreDouble();
    void testSaveAndRestoreString();
    void testSaveAndRestoreBool();
    void testSaveAndRestoreStringList();
    void testSaveAndRestoreVariant();
    void testSaveAndRestoreByteArray();

    // Signal tests
    void testNameChangedSignal();
    void testDescriptionChangedSignal();
    void testTooltipChangedSignal();
    void testCountChangedSignal();
    void testPageAddedSignal();
    void testPageRemovedSignal();
    void testPageRenameErrorSignal();

    // RecentFiles tests
    void testAddRecentFile();
    void testAddRecentFileShouldDoNothingIfAnEmptyString();
    void testRestoreRecentFiles();
    void testClearRecentFiles();

    // Debug output test
    void testConfigDebugOutput();
};


void TestQtNoidAppConfig::initTestCase()
{
}


void TestQtNoidAppConfig::cleanupTestCase()
{
}


void TestQtNoidAppConfig::init()
{
}


void TestQtNoidAppConfig::cleanup()
{
}


void TestQtNoidAppConfig::testAddRecentFile()
{
    Config config("TestConfig", this);

    // Test 1: Add first file
    bool result = config.addRecentFile("C:/test/file1.txt");
    QCOMPARE(result, true);
    QStringList recentFiles = config.restoreAsStringList("RecentFiles", QStringList());
    QCOMPARE(recentFiles.size(), 1);
    QCOMPARE(recentFiles.at(0), "C:/test/file1.txt");

    // Test 2: Add multiple files (most recent should be first)
    config.addRecentFile("C:/test/file2.txt");
    config.addRecentFile("C:/test/file3.txt");
    recentFiles = config.restoreAsStringList("RecentFiles", QStringList());
    QCOMPARE(recentFiles.size(), 3);
    QCOMPARE(recentFiles.at(0), "C:/test/file3.txt");
    QCOMPARE(recentFiles.at(1), "C:/test/file2.txt");
    QCOMPARE(recentFiles.at(2), "C:/test/file1.txt");

    // Test 3: Add duplicate - should move to front and not create duplicate
    config.addRecentFile("C:/test/file1.txt");
    recentFiles = config.restoreAsStringList("RecentFiles", QStringList());
    QCOMPARE(recentFiles.size(), 3);
    QCOMPARE(recentFiles.at(0), "C:/test/file1.txt");
    QCOMPARE(recentFiles.at(1), "C:/test/file3.txt");
    QCOMPARE(recentFiles.at(2), "C:/test/file2.txt");

    // Test 4: Test max limit (default 10)
    Config config2("TestConfig2", this);
    for (int i = 1; i <= 15; i++) {
        config2.addRecentFile(QString("C:/test/file%1.txt").arg(i));
    }
    recentFiles = config2.restoreAsStringList("RecentFiles", QStringList());
    QCOMPARE(recentFiles.size(), 10);
    QCOMPARE(recentFiles.at(0), "C:/test/file15.txt");
    QCOMPARE(recentFiles.at(9), "C:/test/file6.txt");

    // Test 5: Test custom max limit
    Config config3("TestConfig3", this);
    for (int i = 1; i <= 8; i++) {
        config3.addRecentFile(QString("C:/test/file%1.txt").arg(i), 5);
    }
    recentFiles = config3.restoreAsStringList("RecentFiles", QStringList());
    QCOMPARE(recentFiles.size(), 5);
    QCOMPARE(recentFiles.at(0), "C:/test/file8.txt");
    QCOMPARE(recentFiles.at(4), "C:/test/file4.txt");

    // Test 6: Test custom parameter name
    Config config4("TestConfig4", this);
    config4.addRecentFile("C:/custom/file.txt", 10, "MyRecentFiles");
    QStringList defaultFiles = config4.restoreAsStringList("RecentFiles", QStringList());
    QStringList customFiles = config4.restoreAsStringList("MyRecentFiles", QStringList());
    QCOMPARE(defaultFiles.size(), 0);
    QCOMPARE(customFiles.size(), 1);
    QCOMPARE(customFiles.at(0), "C:/custom/file.txt");

    // Test 7: Test custom list name
    Config config5("TestConfig5", this);
    config5.addRecentFile("C:/custom/file.txt", 10, "RecentFiles", "CustomList");
    QStringList settingsList = config5.restoreAsStringList("RecentFiles", QStringList(), "Settings");
    QStringList customList = config5.restoreAsStringList("RecentFiles", QStringList(), "CustomList");
    QCOMPARE(settingsList.size(), 0);
    QCOMPARE(customList.size(), 1);
    QCOMPARE(customList.at(0), "C:/custom/file.txt");
}


void TestQtNoidAppConfig::testAddRecentFileShouldDoNothingIfAnEmptyString()
{
    Config config("TestConfig", this);

    // Test 1: Add first file
    bool res = config.addRecentFile("C:/test/file1.txt");
    QVERIFY(res);
    QStringList recentFiles = config.restoreAsStringList("RecentFiles", QStringList());
    QCOMPARE(recentFiles.size(), 1);
    QCOMPARE(recentFiles.at(0), "C:/test/file1.txt");

    // Empty String shoudl fail and leave the list untouched
    res = !config.addRecentFile("");
    QVERIFY(res);
    recentFiles = config.restoreAsStringList("RecentFiles", QStringList());
    QCOMPARE(recentFiles.size(), 1);
    QCOMPARE(recentFiles.at(0), "C:/test/file1.txt");
}


void TestQtNoidAppConfig::testRestoreRecentFiles()
{
    // Test 1: Restore with default value when parameter doesn't exist
    Config config1("TestConfig1", this);
    QStringList defaultValue = {"C:/default/file1.txt", "C:/default/file2.txt"};
    QStringList result = config1.restoreRecentFiles(defaultValue);
    QCOMPARE(result.size(), 2);
    QCOMPARE(result, defaultValue);

    // Test 2: Restore with empty default when parameter doesn't exist
    Config config2("TestConfig2", this);
    QStringList emptyResult = config2.restoreRecentFiles();
    QCOMPARE(emptyResult.size(), 0);
    QVERIFY(emptyResult.isEmpty());

    // Test 3: Restore after adding files
    Config config3("TestConfig3", this);
    config3.addRecentFile("C:/test/file1.txt");
    config3.addRecentFile("C:/test/file2.txt");
    config3.addRecentFile("C:/test/file3.txt");
    QStringList recentFiles = config3.restoreRecentFiles();
    QCOMPARE(recentFiles.size(), 3);
    QCOMPARE(recentFiles.at(0), "C:/test/file3.txt");
    QCOMPARE(recentFiles.at(1), "C:/test/file2.txt");
    QCOMPARE(recentFiles.at(2), "C:/test/file1.txt");

    // Test 4: Restore with custom parameter name
    Config config4("TestConfig4", this);
    config4.addRecentFile("C:/custom/file.txt", 10, "MyRecentFiles");
    QStringList defaultFiles = config4.restoreRecentFiles(QStringList(), "RecentFiles");
    QStringList customFiles = config4.restoreRecentFiles(QStringList(), "MyRecentFiles");
    QCOMPARE(defaultFiles.size(), 0);
    QCOMPARE(customFiles.size(), 1);
    QCOMPARE(customFiles.at(0), "C:/custom/file.txt");

    // Test 5: Restore with custom list name
    Config config5("TestConfig5", this);
    config5.addRecentFile("C:/custom/file.txt", 10, "RecentFiles", "CustomList");
    QStringList settingsList = config5.restoreRecentFiles(QStringList(), "RecentFiles", "Settings");
    QStringList customList = config5.restoreRecentFiles(QStringList(), "RecentFiles", "CustomList");
    QCOMPARE(settingsList.size(), 0);
    QCOMPARE(customList.size(), 1);
    QCOMPARE(customList.at(0), "C:/custom/file.txt");

    // Test 6: Restore returns default when list doesn't exist
    Config config6("TestConfig6", this);
    QStringList defaultForNonExistent = {"C:/fallback/file.txt"};
    QStringList resultNonExistent = config6.restoreRecentFiles(defaultForNonExistent, "RecentFiles", "NonExistentList");
    QCOMPARE(resultNonExistent, defaultForNonExistent);

    // Test 7: Restore after adding and removing duplicates
    Config config7("TestConfig7", this);
    config7.addRecentFile("C:/test/file1.txt");
    config7.addRecentFile("C:/test/file2.txt");
    config7.addRecentFile("C:/test/file1.txt"); // Duplicate - should move to front
    QStringList filesAfterDupe = config7.restoreRecentFiles();
    QCOMPARE(filesAfterDupe.size(), 2);
    QCOMPARE(filesAfterDupe.at(0), "C:/test/file1.txt");
    QCOMPARE(filesAfterDupe.at(1), "C:/test/file2.txt");

    // Test 8: Restore after reaching max limit
    Config config8("TestConfig8", this);
    for (int i = 1; i <= 15; i++) {
        config8.addRecentFile(QString("C:/test/file%1.txt").arg(i));
    }
    QStringList limitedFiles = config8.restoreRecentFiles();
    QCOMPARE(limitedFiles.size(), 10);
    QCOMPARE(limitedFiles.at(0), "C:/test/file15.txt");
    QCOMPARE(limitedFiles.at(9), "C:/test/file6.txt");

    // Test 9: Restore with custom max limit
    Config config9("TestConfig9", this);
    for (int i = 1; i <= 8; i++) {
        config9.addRecentFile(QString("C:/test/file%1.txt").arg(i), 5);
    }
    QStringList customMaxFiles = config9.restoreRecentFiles();
    QCOMPARE(customMaxFiles.size(), 5);
    QCOMPARE(customMaxFiles.at(0), "C:/test/file8.txt");
    QCOMPARE(customMaxFiles.at(4), "C:/test/file4.txt");
}


void TestQtNoidAppConfig::testClearRecentFiles()
{
    // Test 1: Clear recent files after adding some files
    Config config1("TestConfig1", this);
    config1.addRecentFile("C:/test/file1.txt");
    config1.addRecentFile("C:/test/file2.txt");
    config1.addRecentFile("C:/test/file3.txt");

    // Verify files were added
    QStringList recentFiles = config1.restoreRecentFiles();
    QCOMPARE(recentFiles.size(), 3);

    // Clear the recent files
    config1.clearRecentFiles();

    // Verify the list is now empty
    QStringList clearedFiles = config1.restoreRecentFiles();
    QCOMPARE(clearedFiles.size(), 0);
    QVERIFY(clearedFiles.isEmpty());

    // Test 2: Clear on empty list (should not crash)
    config1.clearRecentFiles();
    QStringList emptyList = config1.restoreRecentFiles();
    QCOMPARE(emptyList.size(), 0);

    // Test 3: Clear with custom parameter name
    Config config3("TestConfig3", this);
    config3.addRecentFile("C:/custom/file1.txt", 10, "MyRecentFiles");
    config3.addRecentFile("C:/custom/file2.txt", 10, "MyRecentFiles");

    // Clear the custom parameter
    config3.clearRecentFiles("MyRecentFiles");
    QStringList clearedCustomFiles = config3.restoreRecentFiles(QStringList(), "MyRecentFiles");
    QCOMPARE(clearedCustomFiles.size(), 0);

    // Test 4: Clear with custom list name
    Config config4("TestConfig4", this);
    config4.addRecentFile("C:/custom/file.txt", 10, "RecentFiles", "CustomList");

    QStringList customListFiles = config4.restoreRecentFiles(QStringList(), "RecentFiles", "CustomList");
    QCOMPARE(customListFiles.size(), 1);

    // Clear the custom list
    config4.clearRecentFiles("RecentFiles", "CustomList");

    QStringList clearedCustomList = config4.restoreRecentFiles(QStringList(), "RecentFiles", "CustomList");
    QCOMPARE(clearedCustomList.size(), 0);

    // Test 5: Clear does not affect other parameters
    Config config5("TestConfig5", this);
    config5.addRecentFile("C:/test/file1.txt");
    config5.saveValue("OtherParam", QStringLiteral("XXXXValue"));
    config5.clearRecentFiles();

    QStringList recentAfterClear = config5.restoreRecentFiles();
    QString otherParam = config5.restoreAsString("OtherParam", "");

    QCOMPARE(recentAfterClear.size(), 0);
    QCOMPARE(otherParam, "XXXXValue"); // Other parameter should remain unchanged

    // Test 6: Clear non-existent parameter (should not crash)
    Config config6("TestConfig6", this);
    config6.clearRecentFiles("NonExistent");

    // Should complete without error

    // Test 7: Clear non-existent list (should not crash)
    Config config7("TestConfig7", this);
    config7.clearRecentFiles("RecentFiles", "NonExistentList");
    // Should complete without error

    // Test 8: Add files after clearing
    Config config8("TestConfig8", this);
    config8.addRecentFile("C:/test/file1.txt");
    config8.addRecentFile("C:/test/file2.txt");
    config8.clearRecentFiles();

    // Add new files after clearing
    config8.addRecentFile("C:/test/file3.txt");
    config8.addRecentFile("C:/test/file4.txt");

    QStringList filesAfterClear = config8.restoreRecentFiles();
    QCOMPARE(filesAfterClear.size(), 2);
    QCOMPARE(filesAfterClear.at(0), "C:/test/file4.txt");
    QCOMPARE(filesAfterClear.at(1), "C:/test/file3.txt");
}


void TestQtNoidAppConfig::testConfigDebugOutput()
{
    // Test with empty config
    Config emptyConfig(this);
    emptyConfig.setName("EmptyConfig");

    QString dbgOutput = QDebug::toString(&emptyConfig);
    // qDebug() << __func__ << dbgOutput;

    // Verify output contains the config name and count
    QVERIFY(!dbgOutput.isEmpty());
    QVERIFY(dbgOutput.contains("EmptyConfig"));
    QVERIFY(dbgOutput.contains("count: 0"));

    // Test with populated config
    Config config;
    config.setName("DebugTest");
    config.setDescription("Test config for debug output");
    config.setTooltip("Test config tooltip");

    // Add multiple parameter lists
    config.saveValue("Volume", 75.0);
    config.saveValue("Theme", "light");

    dbgOutput = QDebug::toString(&config);
    // qDebug() << __func__ << dbgOutput;

    // Verify output contains expected information
    QVERIFY(!dbgOutput.isEmpty());
    QVERIFY(dbgOutput.contains("DebugTest"));
    QVERIFY(dbgOutput.contains("description"));
    QVERIFY(dbgOutput.contains("tooltip"));

    QVERIFY(dbgOutput.contains("pages: ["));
    QVERIFY(dbgOutput.contains("Settings"));
    QVERIFY(dbgOutput.contains("count: 2, visible: true"));
    QVERIFY(dbgOutput.contains("Volume"));
    QVERIFY(dbgOutput.contains("Theme"));


    // Test pointer version
    Config* ptrConfig = new Config(this);
    ptrConfig->setName("PtrTest");

    QString ptrOutput;
    QDebug ptrDebug(&ptrOutput);
    ptrDebug << ptrConfig;

    QVERIFY(!ptrOutput.isEmpty());
    QVERIFY(ptrOutput.contains("Config("));
    QVERIFY(ptrOutput.contains("PtrTest"));

    delete ptrConfig;

    // Test nullptr
    Config* nullConfig = nullptr;
    QString nullOutput;
    QDebug nullDebug(&nullOutput);
    nullDebug << nullConfig;

    QVERIFY(!nullOutput.isEmpty());
    QVERIFY(nullOutput.contains("Config(nullptr)"));

}

// Constructor tests
void TestQtNoidAppConfig::testDefaultConstructor()
{
    Config config(this);

    QVERIFY(config.name().isEmpty());
    QVERIFY(config.description().isEmpty());
    QVERIFY(config.tooltip().isEmpty());
    QCOMPARE(config.count(), 0);
    QVERIFY(config.isEmpty());
}

void TestQtNoidAppConfig::testConstructorWithName()
{
    Config config("MyConfig", this);

    QCOMPARE(config.name(), "MyConfig");
}

void TestQtNoidAppConfig::testConstructorWithJson()
{
    // Create schema JSON
    QJsonObject page1Details({{"description", "Page 1"}});
    QJsonObject page1Schema({{"Settings", page1Details}});
    QJsonArray pagesArray({page1Schema});

    QJsonObject schemaMain({
        {"description", "Test description"},
        {"tooltip", "Test tooltip"},
        {"pages", pagesArray}
    });
    QJsonObject schema({{"TestConfig", schemaMain}});
    // qDebug() << __func__ << "Schema:" << schema;

    // Create value JSON
    QJsonObject param1({{"Volume", 50}});
    QJsonObject param2({{"Theme", "light"}});
    QJsonArray params({param1, param2});
    QJsonObject page1Main({{"parameters", params}});
    QJsonObject page1({{"Settings", page1Main}});
    QJsonObject valueMain({{"pages", QJsonArray({page1})}});
    QJsonObject value({{"TestConfig", valueMain}});
    // qDebug() << __func__ << "Value:" << value;

    // Create config from JSON
    Config config(schema, value, this);
    // qDebug() << __func__ << config;

    QCOMPARE(config.name(), "TestConfig");
    QCOMPARE(config.description(), "Test description");
    QCOMPARE(config.tooltip(), "Test tooltip");

    auto volume = config.restoreAsInt("Volume", 0);
    QCOMPARE(volume, 50);
    auto theme = config.restoreAsString("Theme", "");
    QCOMPARE(theme, "light");

    QCOMPARE(config.count(), 1);
    QVERIFY(config.contains("Settings"));
    QCOMPARE(config.parametersCount("Settings"), 2);
}

// Property tests
void TestQtNoidAppConfig::testNameProperty()
{
    Config config(this);

    QVERIFY(config.name().isEmpty());

    config.setName("TestName");
    QCOMPARE(config.name(), "TestName");

    config.setName("NewName");
    QCOMPARE(config.name(), "NewName");

    // Test bindable
    QBindable<QString> bindableName = config.bindableName();
    QVERIFY(bindableName.isValid());
    QCOMPARE(bindableName.value(), "NewName");
}

void TestQtNoidAppConfig::testDescriptionProperty()
{
    Config config(this);

    QVERIFY(config.description().isEmpty());

    config.setDescription("Test description");
    QCOMPARE(config.description(), "Test description");

    config.setDescription("New description");
    QCOMPARE(config.description(), "New description");

    // Test bindable
    QBindable<QString> bindableDesc = config.bindableDescription();
    QVERIFY(bindableDesc.isValid());
    QCOMPARE(bindableDesc.value(), "New description");
}

void TestQtNoidAppConfig::testTooltipProperty()
{
    Config config(this);

    QVERIFY(config.tooltip().isEmpty());

    config.setTooltip("Test tooltip");
    QCOMPARE(config.tooltip(), "Test tooltip");

    config.setTooltip("New tooltip");
    QCOMPARE(config.tooltip(), "New tooltip");

    // Test bindable
    QBindable<QString> bindableTooltip = config.bindableTooltip();
    QVERIFY(bindableTooltip.isValid());
    QCOMPARE(bindableTooltip.value(), "New tooltip");
}

void TestQtNoidAppConfig::testCountProperty()
{
    Config config(this);

    QCOMPARE(config.count(), 0);

    config.emplace("Page1");
    QCOMPARE(config.count(), 1);

    config.emplace("Page2");
    QCOMPARE(config.count(), 2);

    config.remove("Page1");
    QCOMPARE(config.count(), 1);

    config.clear();
    QCOMPARE(config.count(), 0);
}

// JSON serialization tests
void TestQtNoidAppConfig::testToJsonValues()
{
    Config config("TestConfig", this);
    config.saveValue("Volume", 75);
    config.saveValue("Theme", "dark");

    QJsonObject json = config.toJsonValues();
    // qDebug() << __func__ << config;
    // qDebug() << __func__ << json;

    QVERIFY(json.contains("TestConfig"));
    QJsonObject jsonMain = json["TestConfig"].toObject();
    QJsonArray pagesArray = jsonMain["pages"].toArray();
    QVERIFY(pagesArray.size() == 1);
}

void TestQtNoidAppConfig::testToJsonSchema()
{
    Config config("TestConfig", this);
    config.setDescription("Test description");
    config.setTooltip("Test tooltip");
    config.emplace("Page1", "Page 1 description");

    QJsonObject json = config.toJsonSchema();

    QVERIFY(json.contains("TestConfig"));
    QJsonObject schemaMain = json["TestConfig"].toObject();
    QCOMPARE(schemaMain["description"].toString(), "Test description");
    QCOMPARE(schemaMain["tooltip"].toString(), "Test tooltip");
    QVERIFY(schemaMain.contains("pages"));
}


void TestQtNoidAppConfig::testConfigValuesFromJson()
{
    Config config("TestConfig", this);

    // Create value JSON
    QJsonObject param1({{"Volume", 80}});
    QJsonObject param2({{"Intensity", 500}});
    QJsonArray params({param1, param2});
    QJsonObject pageValue({{"parameters", params}});
    QJsonObject pageMain({{"Settings", pageValue}});
    QJsonArray pagesArray({pageMain});
    QJsonObject valueMain({{"pages", pagesArray}});
    QJsonObject value({{"TestConfig", valueMain}});

    // qDebug() << __func__ << value;
    bool result = config.valuesFromJson(value);
    // qDebug() << __func__ << config;

    QVERIFY(result);
    QCOMPARE(config.name(), "TestConfig");
    QCOMPARE(config.restoreAsInt("Volume", 0), 80);
    QCOMPARE(config.restoreAsInt("Intensity", 0), 500);

    // Test using a Config object with 0 pages
    Config config2(this);
    QJsonObject valueMain2({{"pages", QJsonArray()}});
    QJsonObject value2({{"TestConfig2", valueMain2}});

    // qDebug() << __func__ << value2;
    result = config2.valuesFromJson(value2);
    // qDebug() << __func__ << config2;

    QVERIFY(result);
    QCOMPARE(config2.name(), "TestConfig2");
    QCOMPARE(config2.count(), 0);
}


void TestQtNoidAppConfig::testConfigSchemaFromJson()
{
    Config config(this);

    // Create schema JSON
    QJsonObject schema;

    QJsonObject pageDetails({{"description", "Settings page"}});
    QJsonObject pageSchema({{"Settings",pageDetails}});
    QJsonArray pagesArray({pageSchema});
    QJsonObject schemaMain({{"description", "Loaded description"},
                            {"tooltip", "Loaded tooltip"},
                            {"pages", pagesArray}});
    schema["LoadedConfig"] = schemaMain;

    // qDebug() << __func__ << schema;
    bool result = config.schemaFromJson(schema);
    // qDebug() << __func__ << config;

    QVERIFY(result);
    QCOMPARE(config.name(), "LoadedConfig");
    QCOMPARE(config.description(), "Loaded description");
    QCOMPARE(config.tooltip(), "Loaded tooltip");
    QVERIFY(config.contains("Settings"));
    QCOMPARE(config.parametersCount(), 0);

}

// Container management tests
void TestQtNoidAppConfig::testAppendPage()
{
    Config config(this);
    QSignalSpy pageSpy(&config, &Config::pageAdded);
    QVERIFY(pageSpy.isValid());

    ParameterList* page = new ParameterList("Page1", &config);
    bool result = config.append(page);

    QVERIFY(result);
    QCOMPARE(pageSpy.count(), 1);
    auto argument = pageSpy.takeFirst();
    auto actual = argument.at(0).value<ParameterList*>();
    QCOMPARE(actual, page);

    QCOMPARE(config.count(), 1);
    QVERIFY(config.contains("Page1"));

    // Test appending nullptr
    result = config.append(nullptr);
    QVERIFY(!result);
    QCOMPARE(config.count(), 1);

    // Test appending page with duplicate name
    ParameterList* duplicate = new ParameterList("Page1", &config);
    result = config.append(duplicate);
    QVERIFY(!result);
    QCOMPARE(config.count(), 1);
    delete duplicate;

    // Test appending page with empty name
    ParameterList* emptyName = new ParameterList("", &config);
    result = config.append(emptyName);
    QVERIFY(!result);
    delete emptyName;

}

void TestQtNoidAppConfig::testAppendWithJson()
{
    Config config(this);

    QJsonObject pageSchema({{"description", "Page description"}});
    QJsonObject schema({{"Page1", pageSchema}});

    QJsonObject param1({{"Key","Value"}});
    QJsonObject param2({{"Int", 123}});
    QJsonArray params({{param1, param2}});
    QJsonObject valueMain({{"parameters", params}});
    QJsonObject value({{"Page1", valueMain}});

    bool result = config.append(schema, value);
    // qDebug() << __func__ << config;

    QVERIFY(result);
    QCOMPARE(config.count(), 1);
    QVERIFY(config.contains("Page1"));
    QCOMPARE(config.restoreAsString("Key", "", "Page1"), "Value");
    QCOMPARE(config.restoreAsInt("Int", 0, "Page1"), 123);
}

void TestQtNoidAppConfig::testEmplacePage()
{
    Config config(this);

    ParameterList* page = config.emplace("Page1", "Page 1 description");

    QVERIFY(page != nullptr);
    QCOMPARE(config.count(), 1);
    QCOMPARE(page->name(), "Page1");
    QCOMPARE(page->description(), "Page 1 description");
    QVERIFY(config.contains("Page1"));

    // Test emplace with empty name
    ParameterList* emptyPage = config.emplace("");
    QVERIFY(emptyPage == nullptr);
    QCOMPARE(config.count(), 1);

    // Test emplace with duplicate name
    ParameterList* duplicate = config.emplace("Page1");
    QVERIFY(duplicate == nullptr);
    QCOMPARE(config.count(), 1);
}

void TestQtNoidAppConfig::testEmplaceWithJson()
{
    Config config(this);

    QJsonObject schemaPageMain({{"description", "Page description"},
                                {"tooltip", "Page Tooltip"}});
    QJsonObject schemaPage({{"DriversPage", schemaPageMain}});

    QJsonObject value1({{"Citizenship", true}});
    QJsonObject value2({{"DrivingLicenseNumber", 12345}});
    QJsonObject valueMain({{"parameters", QJsonArray({value1, value2})}});
    QJsonObject value({{"DriversPage", valueMain}});

    ParameterList* page = config.emplace(schemaPage, value);
    // qDebug() << __func__ << page;

    QVERIFY(page != nullptr);
    QCOMPARE(config.count(), 1);
    QVERIFY(config.contains("DriversPage"));

    QCOMPARE(config.restoreAsBool("Citizenship", false, "DriversPage"), true);
    QCOMPARE(config.restoreAsInt("DrivingLicenseNumber", 0, "DriversPage"), 12345);
}

void TestQtNoidAppConfig::testRemovePage()
{
    Config config(this);
    QSignalSpy spy(&config, &Config::pageRemoved);

    ParameterList* page1 = config.emplace("Page1");
    ParameterList* page2 = config.emplace("Page2");
    ParameterList* page3 = config.emplace("Page3");
    QCOMPARE(config.count(), 3);

    config.remove(page1);   // Remove by reference
    QCOMPARE(spy.count(), 1);
    auto args = spy.takeFirst();
    auto actual = args.at(0).value<ParameterList*>();
    QCOMPARE(actual, page1);


    config.remove("Page2"); // Remove by Name
    QCOMPARE(spy.count(), 1);
    args = spy.takeFirst();
    actual = args.at(0).value<ParameterList*>();
    QCOMPARE(actual, page2);

    // I left Page3 on purpose
    QCOMPARE(config.count(), 1);
    QVERIFY(!config.contains(page1));
    QVERIFY(!config.contains(page2));
    QVERIFY(config.contains(page3));
    QVERIFY(config.contains("Page3"));

}

void TestQtNoidAppConfig::testRemoveNonExistentPage()
{
    Config config(this);

    config.emplace("Page1");
    config.emplace("Page2");

    // Test removing nullptr
    config.remove(static_cast<ParameterList*>(nullptr));
    QCOMPARE(config.count(), 2);

    config.remove("NonExistentPage");
    QCOMPARE(config.count(), 2);

    // Test removing not related page
    ParameterList list(this);
    config.remove(&list);
    QCOMPARE(config.count(), 2);
}

void TestQtNoidAppConfig::testClearPages()
{
    Config config(this);

    config.emplace("Page1");
    config.emplace("Page2");
    config.emplace("Page3");

    QCOMPARE(config.count(), 3);

    config.clear();

    QCOMPARE(config.count(), 0);
    QVERIFY(config.isEmpty());

    // Test clearing empty config
    config.clear();
    QCOMPARE(config.count(), 0);

}

void TestQtNoidAppConfig::testIsEmpty()
{
    Config config(this);
    QVERIFY(config.isEmpty());

    config.emplace("Page1");
    QVERIFY(!config.isEmpty());

    config.clear();
    QVERIFY(config.isEmpty());

}

// Access methods tests
void TestQtNoidAppConfig::testPageByIndex()
{
    Config config(this);

    ParameterList* page1 = config.emplace("Page1");
    ParameterList* page2 = config.emplace("Page2");

    QCOMPARE(config.page(0), page1);
    QCOMPARE(config.page(1), page2);

    // Test out of bounds
    QVERIFY(config.page(10) == nullptr);
    QVERIFY(config.page(-1) == nullptr);
}

void TestQtNoidAppConfig::testPageByName()
{
    Config config(this);

    ParameterList* page1 = config.emplace("Page1");
    ParameterList* page2 = config.emplace("Page2");

    QCOMPARE(config.page("Page1"), page1);
    QCOMPARE(config.page("Page2"), page2);

    // Test non-existent page
    QVERIFY(config.page("NonExistent") == nullptr);

}

void TestQtNoidAppConfig::testIndexOfPage()
{
    Config config(this);

    ParameterList* page1 = config.emplace("Page1");
    ParameterList* page2 = config.emplace("Page2");

    // index using object
    QCOMPARE(config.indexOf(page1), 0);
    QCOMPARE(config.indexOf(page2), 1);

    // index using name
    QCOMPARE(config.indexOf("Page1"), 0);
    QCOMPARE(config.indexOf("Page2"), 1);

    // Test non-existent page
    ParameterList* external = new ParameterList("External");
    QCOMPARE(config.indexOf(external), -1);
    QCOMPARE(config.indexOf("external"), -1);

    delete external;

}


void TestQtNoidAppConfig::testContainsPage()
{
    Config config(this);

    ParameterList* page1 = config.emplace("Page1");

    QVERIFY(config.contains(page1));
    QVERIFY(config.contains("Page1"));

    // Test non-existent page
    ParameterList* external = new ParameterList("External");
    QVERIFY(!config.contains(external));
    QVERIFY(!config.contains("external"));
    delete external;

}

void TestQtNoidAppConfig::testPagesList()
{
    Config config(this);

    ParameterList* page1 = config.emplace("Page1");
    ParameterList* page2 = config.emplace("Page2");
    ParameterList* page3 = config.emplace("Page3");

    QList<ParameterList*> pages = config.pages();


    QCOMPARE(pages.size(), 3);
    QVERIFY(pages.contains(page1));
    QVERIFY(pages.contains(page2));
    QVERIFY(pages.contains(page3));

}

void TestQtNoidAppConfig::testParameterAndPageRenameShouldAutomaticallyUpdate()
{
    Config config("AppConfig", this);

    // Create parameter and page
    Parameter parameter(123, "PARAM", this);
    ParameterList page("PAGE", this);
    page << parameter;
    config << page;
    // qDebug() << __func__ << config;

    // Try to rename the value
    parameter.setName("ParamNewName");
    page.setName("PageNewName");
    // qDebug() << __func__ << config;

    // Check the new names are in place
    auto actual = config.restoreAsInt("ParamNewName", -1, "PageNewName");
    QCOMPARE(actual, 123);
}


// Parameter access tests
void TestQtNoidAppConfig::testParameterAccess()
{
    Config config(this);

    config.saveValue("Volume", 75);

    Parameter* param = config.parameter("Volume", "Settings");
    QVERIFY(param != nullptr);
    QCOMPARE(param->name(), "Volume");
    QCOMPARE(param->value(), 75);


    // Test non-existent parameter
    Parameter* nonExistent = config.parameter("NonExistent", "Settings");
    QVERIFY(nonExistent == nullptr);


    // Test non-existent page
    Parameter* nonExistentPage = config.parameter("Volume", "NonExistentPage");
    QVERIFY(nonExistentPage == nullptr);
}

void TestQtNoidAppConfig::testParametersCount()
{
    Config config(this);

    // Test 1: Count with default page name "Settings" - page doesn't exist yet
    int count = config.parametersCount();
    QCOMPARE(count, 0);

    // Test 2: Count after adding parameters to default "Settings" page
    config.saveValue("Volume", 75);
    config.saveValue("Theme", "dark");
    config.saveValue("Language", "en");
    count = config.parametersCount();
    QCOMPARE(count, 3);

    // Test 3: Count with explicit "Settings" page name
    count = config.parametersCount("Settings");
    QCOMPARE(count, 3);

    // Test 4: Count for non-existent page
    count = config.parametersCount("NonExistentPage");
    QCOMPARE(count, 0);

    // Test 5: Count with custom page name
    config.saveValue("Width", 1920, "Display");
    config.saveValue("Height", 1080, "Display");
    count = config.parametersCount("Display");
    QCOMPARE(count, 2);

    // Verify Settings page still has 3 parameters
    count = config.parametersCount("Settings");
    QCOMPARE(count, 3);

    // Test 6: Count for empty page
    config.emplace("EmptyPage");
    count = config.parametersCount("EmptyPage");
    QCOMPARE(count, 0);

    // Test 7: Count after adding parameters to previously empty page
    config.saveValue("Key1", "Value1", "EmptyPage");
    config.saveValue("Key2", "Value2", "EmptyPage");
    count = config.parametersCount("EmptyPage");
    QCOMPARE(count, 2);

    // Test 8: Verify count doesn't change when updating existing parameter
    config.saveValue("Volume", 50); // Update existing
    count = config.parametersCount("Settings");
    QCOMPARE(count, 3); // Should still be 3
}

void TestQtNoidAppConfig::testOperatorStreamInsert()
{
    Config config(this);
    QSignalSpy pageAddedSpy(&config, &Config::pageAdded);
    QSignalSpy countSpy(&config, &Config::countChanged);

    // Test 1: operator<< with ParameterList reference
    ParameterList page1("Page1", &config);
    config << page1;

    QCOMPARE(config.count(), 1);
    QVERIFY(config.contains("Page1"));
    QCOMPARE(config.page(0), &page1);
    QCOMPARE(pageAddedSpy.count(), 1);
    QCOMPARE(countSpy.count(), 1);

    // Test 2: operator<< with ParameterList pointer
    ParameterList* page2 = new ParameterList("Page2", &config);
    config << page2;

    QCOMPARE(config.count(), 2);
    QVERIFY(config.contains("Page2"));
    QCOMPARE(config.page(1), page2);
    QCOMPARE(pageAddedSpy.count(), 2);
    QCOMPARE(countSpy.count(), 2);

    // Test 3: Chaining operator<< calls
    ParameterList* page3 = new ParameterList("Page3", &config);
    ParameterList* page4 = new ParameterList("Page4", &config);
    config << page3 << page4;

    QCOMPARE(config.count(), 4);
    QVERIFY(config.contains("Page3"));
    QVERIFY(config.contains("Page4"));
    QCOMPARE(pageAddedSpy.count(), 4);
    QCOMPARE(countSpy.count(), 4);

    // Test 4: operator<< with nullptr pointer (should do nothing)
    ParameterList* nullPage = nullptr;
    config << nullPage;

    QCOMPARE(config.count(), 4); // Count should not change
    QCOMPARE(pageAddedSpy.count(), 4); // No signal should be emitted

    // Test 5: operator<< with duplicate name (should fail but not crash)
    ParameterList* duplicate = new ParameterList("Page1", &config);
    config << duplicate;

    QCOMPARE(config.count(), 4); // Count should not change
    QCOMPARE(pageAddedSpy.count(), 4); // No signal should be emitted
    delete duplicate; // Clean up since it wasn't added

    // Test 6: operator<< with empty name (should fail)
    ParameterList* emptyName = new ParameterList("", &config);
    config << emptyName;

    QCOMPARE(config.count(), 4); // Count should not change
    QCOMPARE(pageAddedSpy.count(), 4); // No signal should be emitted
    delete emptyName; // Clean up since it wasn't added

    // Test 7: Verify that operator<< returns Config reference for chaining
    Config config2(this);
    ParameterList page5("Page5", &config2);
    ParameterList page6("Page6", &config2);

    Config& result = (config2 << page5 << page6);

    QCOMPARE(&result, &config2); // Should return reference to same object
    QCOMPARE(config2.count(), 2);

    // Test 8: Mixed reference and pointer chaining
    Config config3(this);
    ParameterList pageRef("PageRef", &config3);
    ParameterList* pagePtr = new ParameterList("PagePtr", &config3);

    config3 << pageRef << pagePtr;

    QCOMPARE(config3.count(), 2);
    QVERIFY(config3.contains("PageRef"));
    QVERIFY(config3.contains("PagePtr"));
}

// Save/restore value tests
void TestQtNoidAppConfig::testSaveAndRestoreInt()
{
    Config config(this);

    bool result = config.saveValue("Volume", 75);
    QVERIFY(result);

    int value = config.restoreAsInt("Volume", 0);
    QCOMPARE(value, 75);

    // Test default value
    int defaultValue = config.restoreAsInt("NonExistent", 100);
    QCOMPARE(defaultValue, 100);

    // Test updating value
    config.saveValue("Volume", 50);
    value = config.restoreAsInt("Volume", 0);
    QCOMPARE(value, 50);
}

void TestQtNoidAppConfig::testSaveAndRestoreDouble()
{
    Config config(this);

    bool result = config.saveValue("Opacity", 0.75);
    QVERIFY(result);

    double value = config.restoreAsDouble("Opacity", 0.0);
    QCOMPARE(value, 0.75);

    // Test default value
    double defaultValue = config.restoreAsDouble("NonExistent", 1.0);
    QCOMPARE(defaultValue, 1.0);
}

void TestQtNoidAppConfig::testSaveAndRestoreString()
{
    Config config(this);

    bool result = config.saveValue("Theme", QString("dark"));
    QVERIFY(result);

    QString value = config.restoreAsString("Theme", "");
    QCOMPARE(value, "dark");

    // Test default value
    QString defaultValue = config.restoreAsString("NonExistent", "light");
    QCOMPARE(defaultValue, "light");
}

void TestQtNoidAppConfig::testSaveAndRestoreBool()
{
    Config config(this);

    bool result = config.saveValue("Enabled", true);
    QVERIFY(result);

    bool value = config.restoreAsBool("Enabled", false);
    QCOMPARE(value, true);

    // Test default value
    bool defaultValue = config.restoreAsBool("NonExistent", false);
    QCOMPARE(defaultValue, false);
}

void TestQtNoidAppConfig::testSaveAndRestoreStringList()
{
    Config config(this);

    QStringList list = {"item1", "item2", "item3"};
    bool result = config.saveValue("Items", list);
    QVERIFY(result);

    QStringList value = config.restoreAsStringList("Items", QStringList());
    QCOMPARE(value.size(), 3);
    QCOMPARE(value, list);

    // Test default value
    QStringList defaultList = {"default1"};
    QStringList defaultValue = config.restoreAsStringList("NonExistent", defaultList);
    QCOMPARE(defaultValue, defaultList);
}

void TestQtNoidAppConfig::testSaveAndRestoreVariant()
{
    Config config(this);

    QVariant variant(42);
    bool result = config.saveValue("Value", variant);
    QVERIFY(result);

    QVariant value = config.restoreAsVariant("Value", QVariant());
    QCOMPARE(value.toInt(), 42);

    // Test default value
    QVariant defaultVariant("default");
    QVariant defaultValue = config.restoreAsVariant("NonExistent", defaultVariant);
    QCOMPARE(defaultValue.toString(), "default");
}

void TestQtNoidAppConfig::testSaveAndRestoreByteArray()
{
    Config config(this);

    QByteArray data = "Hello World";
    bool result = config.saveValue("Data", data);
    QVERIFY(result);

    QByteArray value = config.restoreAsByteArray("Data", QByteArray());
    QCOMPARE(value, data);

    // Test default value
    QByteArray defaultData = "default";
    QByteArray defaultValue = config.restoreAsByteArray("NonExistent", defaultData);
    QCOMPARE(defaultValue, defaultData);
}

// Signal tests
void TestQtNoidAppConfig::testNameChangedSignal()
{
    Config config(this);
    QSignalSpy spy(&config, &Config::nameChanged);

    config.setName("TestName");
    QCOMPARE(spy.count(), 1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "TestName");

    config.setName("NewName");
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "NewName");

}

void TestQtNoidAppConfig::testDescriptionChangedSignal()
{
    Config config(this);
    QSignalSpy spy(&config, &Config::descriptionChanged);

    config.setDescription("Test description");
    QCOMPARE(spy.count(), 1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "Test description");
}


void TestQtNoidAppConfig::testTooltipChangedSignal()
{
    Config config(this);
    QSignalSpy spy(&config, &Config::tooltipChanged);

    config.setTooltip("Test tooltip");
    QCOMPARE(spy.count(), 1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "Test tooltip");

}

void TestQtNoidAppConfig::testCountChangedSignal()
{
    Config config(this);
    QSignalSpy spy(&config, &Config::countChanged);

    config.emplace("Page1");
    QCOMPARE(spy.count(), 1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 1);

    config.emplace("Page2");
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 2);

    config.clear();
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 0);
}


void TestQtNoidAppConfig::testPageAddedSignal()
{
    Config config(this);
    QSignalSpy spy(&config, &Config::pageAdded);

    auto page1 = config.emplace("Page1");
    QCOMPARE(spy.count(), 1);
    auto param = spy.takeFirst();
    QCOMPARE(param.at(0).value<ParameterList*>(), page1);

    auto page2 = config.emplace("Page2");
    QCOMPARE(spy.count(), 1);
    param = spy.takeFirst();
    QCOMPARE(param.at(0).value<ParameterList*>(), page2);
}


void TestQtNoidAppConfig::testPageRemovedSignal()
{
    Config config(this);
    QSignalSpy spy(&config, &Config::pageRemoved);

    ParameterList* page1 = config.emplace("Page1");
    ParameterList* page2 = config.emplace("Page2");

    config.remove(page1);
    QCOMPARE(spy.count(), 1);
    auto param = spy.takeFirst();
    QCOMPARE(param.at(0).value<ParameterList*>(), page1);


    config.remove("Page2");
    QCOMPARE(spy.count(), 1);
    param = spy.takeFirst();
    QCOMPARE(param.at(0).value<ParameterList*>(), page2);

}

void TestQtNoidAppConfig::testPageRenameErrorSignal()
{
    Config config(this);

    config.emplace("Page1");
    config.emplace("Page2");

    QSignalSpy spy(&config, &Config::pageRenameError);

    ParameterList* page1 = config.page("Page1");

    // Try to rename to an existing name - should trigger error
    page1->setName("Page2");


    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "Page1");
    QCOMPARE(arguments.at(1).toString(), "Page2");

}


QTEST_MAIN(TestQtNoidAppConfig)
#include "test_config.moc"
