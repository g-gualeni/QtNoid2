#include <QComboBox>
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
    void testConfigBindableCountProperty();

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

    // ComboBox tests
    void testSaveComboBoxTextItems();
    void testRestoreComboBoxTextItems();

    // Debug output test
    void testConfigDebugOutput();

    // Iterator tests
    void testConfigBeginAndEndAndRangeLoop();
    void testConfigConstIteratorsAndConstRangeLoop();
    void testConfigReverseIteratorsAndRangeLoop();
    void testConfigConstReverseIterators();
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

void TestQtNoidAppConfig::testConfigBindableCountProperty()
{    
    Config config(this);

    // Get the bindable property
    auto bindableCount = config.bindableCount();
    QVERIFY(bindableCount.isValid());
    QCOMPARE(bindableCount.value(), 0);

    // Test binding to another property
    QProperty<int> externalProperty;
    externalProperty.setBinding([&]() { return bindableCount.value(); });
    QCOMPARE(externalProperty.value(), 0);

    // Add pages and verify binding updates
    QSignalSpy countChangedSpy(&config, &Config::countChanged);

    // Add a page using emplace
    config.emplace("Page1");
    QCOMPARE(config.count(), 1);
    QCOMPARE(bindableCount.value(), 1);
    QCOMPARE(externalProperty.value(), 1);
    QCOMPARE(countChangedSpy.count(), 1);

    // Add a page using append
    ParameterList* page2 = new ParameterList("Page2", &config);
    config.append(page2);
    QCOMPARE(config.count(), 2);
    QCOMPARE(bindableCount.value(), 2);
    QCOMPARE(externalProperty.value(), 2);
    QCOMPARE(countChangedSpy.count(), 2);

    // Add a page using emplace from JSON
    QJsonObject pageSchema({{"description", "This is Page 3"}});
    QJsonObject schema({{"Page3", pageSchema}});
    QJsonObject param({{"Key", "Value"}});
    QJsonObject valueMain({{"parameters", QJsonArray{param} }});
    QJsonObject value({{"Page3", valueMain}});
    config.emplace(schema, value);
    QCOMPARE(config.count(), 3);
    QCOMPARE(bindableCount.value(), 3);
    QCOMPARE(externalProperty.value(), 3);
    QCOMPARE(countChangedSpy.count(), 3);

    // Remove a page by reference
    config.remove(page2);
    QCOMPARE(config.count(), 2);
    QCOMPARE(bindableCount.value(), 2);
    QCOMPARE(externalProperty.value(), 2);
    QCOMPARE(countChangedSpy.count(), 4);

    // Remove a page by name
    config.remove("Page3");
    QCOMPARE(config.count(), 1);
    QCOMPARE(bindableCount.value(), 1);
    QCOMPARE(externalProperty.value(), 1);
    QCOMPARE(countChangedSpy.count(), 5);

    // Clear config and verify binding updates
    config.clear();
    QCOMPARE(config.count(), 0);
    QCOMPARE(bindableCount.value(), 0);
    QCOMPARE(externalProperty.value(), 0);
    QCOMPARE(countChangedSpy.count(), 6);

    // Verify signal was emitted with correct values
    QList<QVariant> arguments = countChangedSpy.at(0);
    QCOMPARE(arguments.at(0).toInt(), 1);
    arguments = countChangedSpy.at(1);
    QCOMPARE(arguments.at(0).toInt(), 2);
    arguments = countChangedSpy.at(2);
    QCOMPARE(arguments.at(0).toInt(), 3);
    arguments = countChangedSpy.at(3);
    QCOMPARE(arguments.at(0).toInt(), 2);
    arguments = countChangedSpy.at(4);
    QCOMPARE(arguments.at(0).toInt(), 1);
    arguments = countChangedSpy.at(5);
    QCOMPARE(arguments.at(0).toInt(), 0);
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

void TestQtNoidAppConfig::testSaveComboBoxTextItems()
{

    // Test 1: Save combo box items with default page name "Settings"
    Config config1("TestConfig1", this);
    QComboBox* cbo1 = new QComboBox();
    cbo1->addItems({"Option1", "Option2", "Option3"});
    cbo1->setCurrentIndex(1); // Select "Option2"

    config1.saveComboBoxTextItems(cbo1, "MyComboBox");

    // Verify items were saved
    QStringList savedItems = config1.restoreAsStringList("MyComboBox", {});
    QCOMPARE(savedItems.size(), 3);
    QCOMPARE(savedItems.at(0), "Option1");
    QCOMPARE(savedItems.at(1), "Option2");
    QCOMPARE(savedItems.at(2), "Option3");

    // Verify current item was saved with "Current" suffix
    QString savedCurrent = config1.restoreAsString("MyComboBoxCurrent", "");
    QCOMPARE(savedCurrent, "Option2");

    delete cbo1;

    // Test 2: Save combo box items with custom page name
    Config config2("TestConfig2", this);
    QComboBox* cbo2 = new QComboBox();
    cbo2->addItems({"Red", "Green", "Blue"});
    cbo2->setCurrentIndex(2); // Select "Blue"
    config2.saveComboBoxTextItems(cbo2, "ColorPicker", "Display");
    delete cbo2;

    // Verify items were saved to correct page
    QStringList colorItems = config2.restoreAsStringList("ColorPicker", QStringList(), "Display");
    QCOMPARE(colorItems.size(), 3);
    QCOMPARE(colorItems, QStringList({"Red", "Green", "Blue"}));

    QString currentColor = config2.restoreAsString("ColorPickerCurrent", "", "Display");
    QCOMPARE(currentColor, "Blue");

    // Test 3: Save empty combo box
    Config config3("TestConfig3", this);
    QComboBox* cbo3 = new QComboBox();
    // Don't add any items

    config3.saveComboBoxTextItems(cbo3, "EmptyCombo");

    QStringList emptyItems = config3.restoreAsStringList("EmptyCombo", QStringList());
    QCOMPARE(emptyItems.size(), 0);
    QVERIFY(emptyItems.isEmpty());

    // Current text should be empty string
    QString emptyCurrent = config3.restoreAsString("EmptyComboCurrent", "DEFAULT");
    QCOMPARE(emptyCurrent, "");

    delete cbo3;

    // Test 4: Save combo box with no selection (index -1)
    Config config4("TestConfig4", this);
    QComboBox* cbo4 = new QComboBox();
    cbo4->addItems({"Item1", "Item2", "Item3"});
    cbo4->setCurrentIndex(-1); // No selection

    config4.saveComboBoxTextItems(cbo4, "NoSelection");

    QStringList items4 = config4.restoreAsStringList("NoSelection", QStringList());
    QCOMPARE(items4.size(), 3);

    // Current text should be empty when no selection
    QString current4 = config4.restoreAsString("NoSelectionCurrent", "DEFAULT");
    QCOMPARE(current4, "");

    delete cbo4;

    // Test 5: Handle null pointer gracefully (should not crash)
    Config config5("TestConfig5", this);
    QComboBox* nullCbo = nullptr;   
    config5.saveComboBoxTextItems(nullCbo, "NullCombo");

    // Should not have created any parameters
    QStringList nullItems = config5.restoreAsStringList("NullCombo", {"DEFAULT"});
    QCOMPARE(nullItems.size(), 1);
    QCOMPARE(nullItems.at(0), "DEFAULT"); // Should return default value


    // Test 6: Update existing combo box items
    Config config6("TestConfig6", this);
    QComboBox* cbo6 = new QComboBox();
    cbo6->addItems({"First", "Second"});
    cbo6->setCurrentIndex(0);
    config6.saveComboBoxTextItems(cbo6, "UpdateTest");

    QStringList initial = config6.restoreAsStringList("UpdateTest", QStringList());
    QCOMPARE(initial.size(), 2);

    // Update the combo box
    cbo6->clear();
    cbo6->addItems({"Alpha", "Beta", "Gamma", "Delta"});
    cbo6->setCurrentIndex(2); // Select "Gamma"

    config6.saveComboBoxTextItems(cbo6, "UpdateTest");

    QStringList updated = config6.restoreAsStringList("UpdateTest", QStringList());
    QCOMPARE(updated.size(), 4);
    QCOMPARE(updated, QStringList({"Alpha", "Beta", "Gamma", "Delta"}));
    QString updatedCurrent = config6.restoreAsString("UpdateTestCurrent", "");
    QCOMPARE(updatedCurrent, "Gamma");
    delete cbo6;


    // Test 7: Save combo box with editable text
    Config config7("TestConfig7", this);
    QComboBox* cbo7 = new QComboBox();
    cbo7->setEditable(true);
    cbo7->addItems({"Preset1", "Preset2"});
    cbo7->setCurrentText("CustomText"); // User typed custom text

    config7.saveComboBoxTextItems(cbo7, "EditableCombo");


    QStringList presets = config7.restoreAsStringList("EditableCombo", QStringList());
    QCOMPARE(presets.size(), 2); // Should save the preset items

    QString customText = config7.restoreAsString("EditableComboCurrent", "");
    QCOMPARE(customText, "CustomText"); // Should save the custom text

    delete cbo7;


    // Test 8: Verify separate pages don't interfere
    Config config8("TestConfig8", this);
    QComboBox* cboA = new QComboBox();
    cboA->addItems({"A1", "A2"});
    cboA->setCurrentIndex(0);


    QComboBox* cboB = new QComboBox();
    cboB->addItems({"B1", "B2", "B3"});
    cboB->setCurrentIndex(1);

    config8.saveComboBoxTextItems(cboA, "Combo", "PageA");
    config8.saveComboBoxTextItems(cboB, "Combo", "PageB");

    QStringList itemsA = config8.restoreAsStringList("Combo", QStringList(), "PageA");
    QStringList itemsB = config8.restoreAsStringList("Combo", QStringList(), "PageB");

    QCOMPARE(itemsA.size(), 2);
    QCOMPARE(itemsB.size(), 3);
    QCOMPARE(config8.restoreAsString("ComboCurrent", "", "PageA"), "A1");
    QCOMPARE(config8.restoreAsString("ComboCurrent", "", "PageB"), "B2");

    delete cboA;
    delete cboB;

}

void TestQtNoidAppConfig::testRestoreComboBoxTextItems()
{
    // Test 1: Restore combo box with previously saved items
    Config config1("TestConfig1", this);
    QComboBox* cbo1 = new QComboBox();
    cbo1->addItems({"Original1", "Original2"});
    cbo1->setCurrentIndex(1);

    // Save first
    config1.saveComboBoxTextItems(cbo1, "RestoredCombo");

    // Clear and restore
    cbo1->clear();
    QCOMPARE(cbo1->count(), 0);

    config1.restoreComboBoxTextItems(cbo1, "RestoredCombo", {});

    // Verify items were restored
    QCOMPARE(cbo1->count(), 2);
    QCOMPARE(cbo1->itemText(0), "Original1");
    QCOMPARE(cbo1->itemText(1), "Original2");
    QCOMPARE(cbo1->currentText(), "Original2");
    QCOMPARE(cbo1->currentIndex(), 1);
    delete cbo1;



    // Test 2: Restore with default values when parameter doesn't exist
    Config config2("TestConfig2", this);
    QComboBox* cbo2 = new QComboBox();
    QStringList defaultItems = {"Default1", "Default2", "Default3"};

    config2.restoreComboBoxTextItems(cbo2, "NonExistent", defaultItems);

    QCOMPARE(cbo2->count(), 3);
    QCOMPARE(cbo2->itemText(0), "Default1");
    QCOMPARE(cbo2->itemText(1), "Default2");
    QCOMPARE(cbo2->itemText(2), "Default3");
    QCOMPARE(cbo2->currentText(), "Default1");
    QCOMPARE(cbo2->currentIndex(), 0);

    delete cbo2;

    // Test 3: Restore with custom page name
    Config config3("TestConfig3", this);
    QComboBox* cbo3 = new QComboBox();
    cbo3->addItems({"Color1", "Color2", "Color3"});
    cbo3->setCurrentIndex(2);

    config3.saveComboBoxTextItems(cbo3, "Colors", "Display");

    cbo3->clear();
    config3.restoreComboBoxTextItems(cbo3, "Colors", {}, "Display");

    QCOMPARE(cbo3->count(), 3);
    QCOMPARE(cbo3->currentText(), "Color3");

    delete cbo3;

    // Test 4: Handle null pointer gracefully
    // Should not crash: Test passes if no crash occurred
    Config config4("TestConfig4", this);
    QComboBox* nullCbo = nullptr;
    config4.restoreComboBoxTextItems(nullCbo, "NullTest", {});

    // Test 5: Restore empty combo box (saved with no items)
    Config config5("TestConfig5", this);
    QComboBox* cbo5a = new QComboBox();
    // Don't add any items
    config5.saveComboBoxTextItems(cbo5a, "EmptyCombo");
    QCOMPARE(cbo5a->count(), 0);
    delete cbo5a;


    // // Test 5b:  Should be cleared to match saved state
    QComboBox* cbo5b = new QComboBox();
    cbo5b->addItems({"Temp1", "Temp2"}); // Add some temp items
    config5.restoreComboBoxTextItems(cbo5b, "EmptyCombo", {});
    QCOMPARE(cbo5b->count(), 0);
    delete cbo5b;


    // Test 6: Restore when only items exist but no current selection saved
    Config config6("TestConfig6", this);
    // Manually save just the items list without the "Current" parameter
    QStringList items6 = {"Item1", "Item2", "Item3"};
    config6.saveValue("TestCombo", items6);
    // Intentionally not saving "TestComboCurrent"
    QComboBox* cbo6 = new QComboBox();
    config6.restoreComboBoxTextItems(cbo6, "TestCombo", {});

    QCOMPARE(cbo6->count(), 3);
    QCOMPARE(cbo6->itemText(0), "Item1");
    QCOMPARE(cbo6->itemText(1), "Item2");
    QCOMPARE(cbo6->itemText(2), "Item3");
    QCOMPARE(cbo6->currentIndex(), 0);
    delete cbo6;


    // Test 7: Restore preserves current text even if not in items
    Config config7("TestConfig7", this);
    QComboBox* cbo7a = new QComboBox();
    cbo7a->setEditable(true);
    cbo7a->addItems({"Preset1", "Preset2"});
    cbo7a->setCurrentText("CustomValue");

    config7.saveComboBoxTextItems(cbo7a, "EditableCombo");
    delete cbo7a;

    QComboBox* cbo7b = new QComboBox();
    cbo7b->setEditable(true);
    config7.restoreComboBoxTextItems(cbo7b, "EditableCombo", {});

    QCOMPARE(cbo7b->count(), 2);
    QCOMPARE(cbo7b->currentText(), "CustomValue"); // Custom text should be restored

    delete cbo7b;

    // Test 8: Restore doesn't affect other combo boxes on different pages
    Config config8("TestConfig8", this);

    QComboBox* cboPageA = new QComboBox();
    cboPageA->addItems({"A1", "A2"});
    cboPageA->setCurrentIndex(1);
    config8.saveComboBoxTextItems(cboPageA, "Combo", "PageA");

    QComboBox* cboPageB = new QComboBox();
    cboPageB->addItems({"B1", "B2", "B3"});
    cboPageB->setCurrentIndex(0);
    config8.saveComboBoxTextItems(cboPageB, "Combo", "PageB");

    delete cboPageA;
    delete cboPageB;

    // Restore separately
    QComboBox* restoreA = new QComboBox();
    QComboBox* restoreB = new QComboBox();

    config8.restoreComboBoxTextItems(restoreA, "Combo", QStringList(), "PageA");
    config8.restoreComboBoxTextItems(restoreB, "Combo", QStringList(), "PageB");

    QCOMPARE(restoreA->count(), 2);
    QCOMPARE(restoreA->currentText(), "A2");

    QCOMPARE(restoreB->count(), 3);
    QCOMPARE(restoreB->currentText(), "B1");

    delete restoreA;
    delete restoreB;
}

void TestQtNoidAppConfig::testConfigBeginAndEndAndRangeLoop()
{
    Config config("TestConfig", this);

    // Test empty config
    QVERIFY(config.begin() == config.end());

    // Add some pages
    config.emplace("First", "First page");
    config.emplace("Second", "Second page");
    config.emplace("Third", "Third page");

    // Test that begin() != end() for non-empty config
    QVERIFY(config.begin() != config.end());

    // Test iterator dereferencing
    Config::iterator it = config.begin();
    QVERIFY(it != config.end());

    ParameterList* firstPage = *it;
    QVERIFY(firstPage != nullptr);
    QCOMPARE(firstPage->name(), "First");
    QCOMPARE(firstPage->description(), "First page");

    // Test iterator increment
    ++it;
    QVERIFY(it != config.end());
    ParameterList* secondPage = *it;
    QVERIFY(secondPage != nullptr);
    QCOMPARE(secondPage->name(), "Second");
    QCOMPARE(secondPage->description(), "Second page");

    // Test post-increment
    auto prevIt = it++;
    QVERIFY(it != config.end());
    QVERIFY(prevIt != it);
    ParameterList* thirdPage = *it;
    QVERIFY(thirdPage != nullptr);
    QCOMPARE(thirdPage->name(), "Third");
    QCOMPARE(thirdPage->description(), "Third page");

    // Test iterator index() method
    QCOMPARE(config.begin().index(), 0);
    auto secondIt = config.begin();
    ++secondIt;
    QCOMPARE(secondIt.index(), 1);

    // Test bidirectional iterator (decrement)
    auto lastIt = config.end();
    --lastIt;
    ParameterList* lastPage = *lastIt;
    QVERIFY(lastPage != nullptr);
    QCOMPARE(lastPage->name(), "Third");
    QCOMPARE(lastPage->description(), "Third page");

    // Test post-decrement
    auto postDecIt = lastIt--;
    QCOMPARE((*postDecIt)->name(), "Third");
    QCOMPARE((*lastIt)->name(), "Second");

    // Test range-based for loop
    QStringList expectedList = {"First", "Second", "Third"};
    QStringList nameList;
    for(auto page : config){
        nameList << page->name();
    }
    QCOMPARE(nameList, expectedList);
}

void TestQtNoidAppConfig::testConfigConstIteratorsAndConstRangeLoop()
{
    Config config("TestConfig", this);

    // Add some pages
    config.emplace("Alpha", "Alpha page");
    config.emplace("Beta", "Beta page");
    config.emplace("Gamma", "Gamma page");

    // Create a const reference to the config
    const Config& constConfig = config;

    // ===== Test begin() const and end() const =====
    // Test empty vs non-empty
    QVERIFY(constConfig.begin() != constConfig.end());

    // Test const iterator dereferencing
    Config::const_iterator cit = constConfig.begin();
    const ParameterList* firstPage = *cit;
    QVERIFY(firstPage != nullptr);
    QCOMPARE(firstPage->name(), "Alpha");
    QCOMPARE(firstPage->description(), "Alpha page");

    // Test const iterator increment
    ++cit;
    QVERIFY(cit != constConfig.end());
    const ParameterList* secondPage = *cit;
    QVERIFY(secondPage != nullptr);
    QCOMPARE(secondPage->name(), "Beta");
    QCOMPARE(secondPage->description(), "Beta page");

    // Test post-increment
    auto prevCit = cit++;
    QVERIFY(prevCit != cit);
    const ParameterList* thirdPage = *cit;
    QVERIFY(thirdPage != nullptr);
    QCOMPARE(thirdPage->name(), "Gamma");
    QCOMPARE(thirdPage->description(), "Gamma page");

    // Test reaching end
    ++cit;
    QVERIFY(cit == constConfig.end());

    // ===== Test cbegin() and cend() =====
    // Test that cbegin() != cend() for non-empty config
    QVERIFY(constConfig.cbegin() != constConfig.cend());

    // Test cbegin() dereferencing
    auto cbegIt = constConfig.cbegin();
    const ParameterList* cbegPage = *cbegIt;
    QVERIFY(cbegPage != nullptr);
    QCOMPARE(cbegPage->name(), "Alpha");
    QCOMPARE(cbegPage->description(), "Alpha page");

    // Test cbegin() index method
    QCOMPARE(constConfig.cbegin().index(), 0);
    auto secondCbegIt = constConfig.cbegin();
    ++secondCbegIt;
    QCOMPARE(secondCbegIt.index(), 1);

    // ===== Test bidirectional const iterator operations =====
    // Test decrement from end
    auto lastCit = constConfig.cend();
    --lastCit;
    const ParameterList* lastPage = *lastCit;
    QVERIFY(lastPage != nullptr);
    QCOMPARE(lastPage->name(), "Gamma");
    QCOMPARE(lastPage->description(), "Gamma page");

    // Test post-decrement
    auto postDecCit = lastCit--;
    QCOMPARE((*postDecCit)->name(), "Gamma");
    QCOMPARE((*lastCit)->name(), "Beta");

    // ===== Test iterator conversion from mutable to const =====
    // Test that mutable iterator can be converted to const iterator
    Config::iterator mutIt = config.begin();
    Config::const_iterator constFromMut = mutIt;

    QCOMPARE((*constFromMut)->name(), "Alpha");
    QCOMPARE((*constFromMut)->description(), "Alpha page");

    // Verify they point to the same element
    QVERIFY(*mutIt == *constFromMut);

    // ===== Test const range-based for loop =====
    QStringList constNames;
    QStringList constDescriptions;

    // Test const range-based for loop
    for (const ParameterList* page : constConfig) {
        QVERIFY(page != nullptr);
        constNames << page->name();
        constDescriptions << page->description();
    }

    QCOMPARE(constNames.size(), 3);
    QCOMPARE(constNames, QStringList({"Alpha", "Beta", "Gamma"}));
    QCOMPARE(constDescriptions, QStringList({"Alpha page", "Beta page", "Gamma page"}));

    // ===== Test that const iterators prevent modification =====
    // This should compile (reading)
    auto constIt = constConfig.cbegin();
    QString pageName = (*constIt)->name();
    QCOMPARE(pageName, "Alpha");

    // ===== Test empty const config =====
    Config emptyConfig("EmptyTest", this);
    const Config& constEmptyConfig = emptyConfig;

    QVERIFY(constEmptyConfig.begin() == constEmptyConfig.end());
    QVERIFY(constEmptyConfig.cbegin() == constEmptyConfig.cend());

    // Test const range-based for loop with empty config
    int constCount = 0;
    for ([[maybe_unused]] const ParameterList* page : constEmptyConfig) {
        constCount++;
    }
    QCOMPARE(constCount, 0);
}

void TestQtNoidAppConfig::testConfigReverseIteratorsAndRangeLoop()
{
    Config config("TestConfig", this);

    // Add some pages
    config.emplace("First", "First page");
    config.emplace("Second", "Second page");
    config.emplace("Third", "Third page");
    config.emplace("Fourth", "Fourth page");

    // ===== Test rbegin() and rend() basic functionality =====
    QVERIFY(config.rbegin() != config.rend());

    // Test reverse iterator dereferencing - should start from last element
    Config::reverse_iterator rit = config.rbegin();
    ParameterList* lastPage = *rit;
    QVERIFY(lastPage != nullptr);
    QCOMPARE(lastPage->name(), "Fourth");  // Should be the last element
    QCOMPARE(lastPage->description(), "Fourth page");

    // ===== Test reverse iterator increment (moves backward through config) =====
    ++rit;
    QVERIFY(rit != config.rend());
    ParameterList* thirdPage = *rit;
    QVERIFY(thirdPage != nullptr);
    QCOMPARE(thirdPage->name(), "Third");
    QCOMPARE(thirdPage->description(), "Third page");

    // Test post-increment
    auto prevRit = rit++;
    QVERIFY(rit != config.rend());
    QVERIFY(prevRit != rit);
    ParameterList* secondPage = *rit;
    QVERIFY(secondPage != nullptr);
    QCOMPARE(secondPage->name(), "Second");
    QCOMPARE(secondPage->description(), "Second page");

    // Continue to first element
    ++rit;
    QVERIFY(rit != config.rend());
    ParameterList* firstPage = *rit;
    QVERIFY(firstPage != nullptr);
    QCOMPARE(firstPage->name(), "First");
    QCOMPARE(firstPage->description(), "First page");

    // Test reaching rend()
    ++rit;
    QVERIFY(rit == config.rend());

    // ===== Test reverse iterator range-based for loop simulation =====
    QStringList reverseNames;
    QStringList reverseDescriptions;

    // Manually iterate through reverse iterators
    for (auto revIt = config.rbegin(); revIt != config.rend(); ++revIt) {
        ParameterList* page = *revIt;
        QVERIFY(page != nullptr);
        reverseNames << page->name();
        reverseDescriptions << page->description();
    }

    // Verify reverse order
    QCOMPARE(reverseNames.size(), 4);
    QCOMPARE(reverseNames, QStringList({"Fourth", "Third", "Second", "First"}));
    QCOMPARE(reverseDescriptions, QStringList({"Fourth page", "Third page", "Second page", "First page"}));

    // ===== Test reverse iterator arrow operator =====
    auto rbeginIt = config.rbegin();
    QCOMPARE((*rbeginIt)->name(), "Fourth");
    QCOMPARE((*rbeginIt)->description(), "Fourth page");

    // ===== Test relationship between normal and reverse iterators =====
    // rbegin() should correspond to the element before end()
    auto normalEnd = config.end();
    --normalEnd;
    auto reverseBegin = config.rbegin();
    QCOMPARE((*normalEnd)->name(), (*reverseBegin)->name());
    QCOMPARE((*normalEnd)->description(), (*reverseBegin)->description());

    // ===== Test empty config reverse iterators =====
    Config emptyConfig("EmptyTest", this);
    QVERIFY(emptyConfig.rbegin() == emptyConfig.rend());

    // Test reverse iteration over empty config
    int reverseCount = 0;
    for (auto revIt = emptyConfig.rbegin(); revIt != emptyConfig.rend(); ++revIt) {
        reverseCount++;
    }
    QCOMPARE(reverseCount, 0);

    // ===== Test reverse iterator with std::reverse algorithm simulation =====
    // Use reverse iterators to create a reversed copy
    QStringList normalOrder;
    QStringList reversedOrder;

    // Normal order
    for (auto it = config.begin(); it != config.end(); ++it) {
        normalOrder << (*it)->name();
    }

    // Reversed order using reverse iterators
    for (auto rit = config.rbegin(); rit != config.rend(); ++rit) {
        reversedOrder << (*rit)->name();
    }

    // Verify they are opposite
    QCOMPARE(normalOrder, QStringList({"First", "Second", "Third", "Fourth"}));
    QCOMPARE(reversedOrder, QStringList({"Fourth", "Third", "Second", "First"}));

    // ===== Test reverse iterator decrement (moves forward through config) =====
    // Start from rbegin() and use decrement to move "forward" in reverse direction
    auto rLastIt = config.rbegin();
    ++rLastIt; ++rLastIt; ++rLastIt; // Move to "First"
    QCOMPARE((*rLastIt)->name(), "First");

    // Test pre-decrement (should move to "Second")
    --rLastIt;
    QCOMPARE((*rLastIt)->name(), "Second");

    // Test post-decrement
    auto postDecRit = rLastIt--;
    QCOMPARE((*postDecRit)->name(), "Second");
    QCOMPARE((*rLastIt)->name(), "Third");
}

void TestQtNoidAppConfig::testConfigConstReverseIterators()
{
    Config config("TestConfig", this);

    // Add some pages
    config.emplace("First", "First page");
    config.emplace("Second", "Second page");
    config.emplace("Third", "Third page");
    config.emplace("Fourth", "Fourth page");

    // Create a const reference to the config
    const Config& constConfig = config;

    // ===== Test rbegin() const and rend() const =====
    // Test that const rbegin() != rend() for non-empty config
    QVERIFY(constConfig.rbegin() != constConfig.rend());

    // Test const reverse iterator dereferencing - should start from last element
    Config::const_reverse_iterator crit = constConfig.rbegin();
    QVERIFY(crit != constConfig.rend());

    const ParameterList* lastPage = *crit;
    QVERIFY(lastPage != nullptr);
    QCOMPARE(lastPage->name(), "Fourth");  // Should be the last element
    QCOMPARE(lastPage->description(), "Fourth page");

    // ===== Test const reverse iterator increment (moves backward through config) =====
    // Test pre-increment (moves to previous element in normal order)
    ++crit;
    QVERIFY(crit != constConfig.rend());
    const ParameterList* thirdPage = *crit;
    QVERIFY(thirdPage != nullptr);
    QCOMPARE(thirdPage->name(), "Third");
    QCOMPARE(thirdPage->description(), "Third page");

    // Test post-increment
    auto prevCrit = crit++;
    QVERIFY(crit != constConfig.rend());
    QVERIFY(prevCrit != crit);
    const ParameterList* secondPage = *crit;
    QVERIFY(secondPage != nullptr);
    QCOMPARE(secondPage->name(), "Second");
    QCOMPARE(secondPage->description(), "Second page");

    // Continue to first element
    ++crit;
    QVERIFY(crit != constConfig.rend());
    const ParameterList* firstPage = *crit;
    QVERIFY(firstPage != nullptr);
    QCOMPARE(firstPage->name(), "First");
    QCOMPARE(firstPage->description(), "First page");

    // Test reaching rend()
    ++crit;
    QVERIFY(crit == constConfig.rend());

    // ===== Test crbegin() and crend() =====
    // Test that crbegin() != crend() for non-empty config
    QVERIFY(constConfig.crbegin() != constConfig.crend());

    // Test crbegin() dereferencing - should start from last element
    Config::const_reverse_iterator crbit = constConfig.crbegin();
    QVERIFY(crbit != constConfig.crend());

    const ParameterList* crbPage = *crbit;
    QVERIFY(crbPage != nullptr);
    QCOMPARE(crbPage->name(), "Fourth");
    QCOMPARE(crbPage->description(), "Fourth page");

    // Test crbegin() increment
    ++crbit;
    QVERIFY(crbit != constConfig.crend());
    const ParameterList* crbSecondPage = *crbit;
    QVERIFY(crbSecondPage != nullptr);
    QCOMPARE(crbSecondPage->name(), "Third");
    QCOMPARE(crbSecondPage->description(), "Third page");

    // ===== Test const reverse iterator range-based for loop simulation =====
    QStringList constReverseNames;
    QStringList constReverseDescriptions;

    // Manually iterate through const reverse iterators using rbegin() const
    for (auto constRevIt = constConfig.rbegin(); constRevIt != constConfig.rend(); ++constRevIt) {
        const ParameterList* page = *constRevIt;
        QVERIFY(page != nullptr);
        constReverseNames << page->name();
        constReverseDescriptions << page->description();
    }

    // Verify const reverse order
    QCOMPARE(constReverseNames.size(), 4);
    QCOMPARE(constReverseNames, QStringList({"Fourth", "Third", "Second", "First"}));
    QCOMPARE(constReverseDescriptions, QStringList({"Fourth page", "Third page", "Second page", "First page"}));

    // ===== Test const reverse iterator range-based for loop with crbegin/crend =====
    QStringList crNames;
    QStringList crDescriptions;

    // Manually iterate through const reverse iterators using crbegin()/crend()
    for (auto crIt = constConfig.crbegin(); crIt != constConfig.crend(); ++crIt) {
        const ParameterList* page = *crIt;
        QVERIFY(page != nullptr);
        crNames << page->name();
        crDescriptions << page->description();
    }

    // Verify crbegin/crend produces same result as rbegin/rend const
    QCOMPARE(crNames, constReverseNames);
    QCOMPARE(crDescriptions, constReverseDescriptions);

    // ===== Test const reverse iterator arrow operator =====
    auto crbeginIt = constConfig.crbegin();
    QCOMPARE((*crbeginIt)->name(), "Fourth");
    QCOMPARE((*crbeginIt)->description(), "Fourth page");

    auto rbeginConstIt = constConfig.rbegin();
    QCOMPARE((*rbeginConstIt)->name(), "Fourth");
    QCOMPARE((*rbeginConstIt)->description(), "Fourth page");

    // ===== Test relationship between const normal and const reverse iterators =====
    // rbegin() const should correspond to the element before end() const
    auto constNormalEnd = constConfig.end();
    --constNormalEnd;
    auto constReverseBegin = constConfig.rbegin();

    QCOMPARE((*constNormalEnd)->name(), (*constReverseBegin)->name());
    QCOMPARE((*constNormalEnd)->description(), (*constReverseBegin)->description());

    // Same test for crbegin() and cend()
    auto constCEnd = constConfig.cend();
    --constCEnd;
    auto constCRBegin = constConfig.crbegin();

    QCOMPARE((*constCEnd)->name(), (*constCRBegin)->name());
    QCOMPARE((*constCEnd)->description(), (*constCRBegin)->description());

    // ===== Test empty const config reverse iterators =====
    Config emptyConfig("EmptyTest", this);
    const Config& constEmptyConfig = emptyConfig;

    QVERIFY(constEmptyConfig.rbegin() == constEmptyConfig.rend());
    QVERIFY(constEmptyConfig.crbegin() == constEmptyConfig.crend());

    // Test const reverse iteration over empty config
    int constReverseCount = 0;
    for (auto constRevIt = constEmptyConfig.rbegin(); constRevIt != constEmptyConfig.rend(); ++constRevIt) {
        constReverseCount++;
    }
    QCOMPARE(constReverseCount, 0);

    // Test crbegin/crend with empty config
    int crCount = 0;
    for (auto crIt = constEmptyConfig.crbegin(); crIt != constEmptyConfig.crend(); ++crIt) {
        crCount++;
    }
    QCOMPARE(crCount, 0);

    // ===== Test const correctness =====
    // This should compile (reading from const iterator)
    auto constIt = constConfig.crbegin();
    QString pageName = (*constIt)->name();
    QCOMPARE(pageName, "Fourth");

    // ===== Test bidirectional const reverse iterator operations =====
    // Test decrement operations on const reverse iterators
    auto constLastRevIt = constConfig.rbegin();
    ++constLastRevIt; ++constLastRevIt; ++constLastRevIt; // Move to "First"
    QCOMPARE((*constLastRevIt)->name(), "First");

    // Test pre-decrement (should move to "Second")
    --constLastRevIt;
    QCOMPARE((*constLastRevIt)->name(), "Second");

    // Test post-decrement
    auto postDecConstRevIt = constLastRevIt--;
    QCOMPARE((*postDecConstRevIt)->name(), "Second");
    QCOMPARE((*constLastRevIt)->name(), "Third");
}


QTEST_MAIN(TestQtNoidAppConfig)
#include "test_config.moc"
