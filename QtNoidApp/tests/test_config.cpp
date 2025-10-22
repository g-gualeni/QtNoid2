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

    QString emptyOutput;
    QDebug emptyDebug(&emptyOutput);
    emptyDebug << emptyConfig;
    // qDebug() << __func__ << emptyConfig;

    // Verify output contains the config name and count
    QVERIFY(!emptyOutput.isEmpty());
    QVERIFY(emptyOutput.contains("Config("));
    QVERIFY(emptyOutput.contains("EmptyConfig"));
    QVERIFY(emptyOutput.contains("Count: 0"));


    // Test with populated config
    Config config;
    config.setName("DebugTest");
    config.setDescription("Test config for debug output");

    // Add multiple parameter lists
    config.saveValue("Volume", 75.0);
    config.saveValue("Theme", "light");

    // Capture debug output
    QString debugOutput;
    QDebug debug(&debugOutput);
    debug << config;

    // qDebug() << __func__ << configFile;

    // Verify output contains expected information
    QVERIFY(!debugOutput.isEmpty());
    QVERIFY(debugOutput.contains("Config("));
    QVERIFY(debugOutput.contains("DebugTest"));
    QVERIFY(debugOutput.contains("Count: 2"));
    QVERIFY(debugOutput.contains("ParameterLists:"));
    QVERIFY(debugOutput.contains("Settings"));
    QVERIFY(debugOutput.contains("Count: 2"));
    QVERIFY(debugOutput.contains("Count: 1"));

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

QTEST_MAIN(TestQtNoidAppConfig)
#include "test_config.moc"
