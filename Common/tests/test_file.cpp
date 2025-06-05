#include <QTest>
#include <QtNoidCommon/QtNoidCommon>

class TestFile : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void autoNamingNextName_data();
    void autoNamingNextName();

    void autoNaming_data();
    void autoNaming();

private:
    QString testDataPath() const {
        return QDir::currentPath() + "/TestData/" +
               metaObject()->className() + "/";
    }

};

using namespace QtNoid::Common;


void TestFile::initTestCase()
{}

void TestFile::cleanupTestCase()
{}

void TestFile::init()
{}

void TestFile::cleanup()
{}

void TestFile::autoNamingNextName_data()
{
    QTest::addColumn<QString>("currentName");
    QTest::addColumn<QString>("expected");
    QTest::newRow("file.tar.zip") << "file.tar.zip" << "file 001.tar.zip";
    QTest::newRow("file001Top.tar.zip") << "file001Top.tar.zip" << "file002Top.tar.zip";
    QTest::newRow("file") << "file.xyp" << "file 001.xyp";
    QTest::newRow("file00") << "file01.xyp" << "file02.xyp";
    QTest::newRow("file (1)") << "file (1).xyp" << "file (2).xyp";
    QTest::newRow("file-999") << "file-999.xyp" << "file-1000.xyp";
    QTest::newRow("file-999my") << "file-999my.xyp" << "file-1000my.xyp";
    QTest::newRow("Multiple000") << "file-01-007.xyp" << "file-01-008.xyp";
    QTest::newRow("MultipleEqual") << "file-01-01.xyp" << "file-01-02.xyp";
    QTest::newRow("File+Path") << "a/b\\c/file-01-01.xyp" << "a/b\\c/file-01-02.xyp";
    QTest::newRow("Full file path") << "C:/a/file-01-01.xyp" << "C:/a/file-01-02.xyp";
    QTest::newRow("Full file path2") << "./a/file-01-01.xyp" << "./a/file-01-02.xyp";
    QTest::newRow("No Extension") << "ReadMe01" << "ReadMe02";
}

void TestFile::autoNamingNextName()
{
    QFETCH(QString, currentName);
    QFETCH(QString, expected);
    auto result = File::autoNamingNextName(currentName);
    QCOMPARE(result, expected);
}

void TestFile::autoNaming_data()
{
    QTest::addColumn<QString>("root");
    QTest::addColumn<QStringList>("fileList");
    QTest::addColumn<QString>("newFile");
    QTest::addColumn<QString>("expected");

    // Init the file list
    QString path = testDataPath() + "autoNaming-";

    // Set the test case
    QTest::newRow("Single File") << path + "Single/" << QStringList({}) << "saturn.txt" << "saturn.txt";
    QTest::newRow("Single With Index") << path + "Single/" << QStringList({}) << "mars123.txt" << "mars123.txt";
    QTest::newRow("Duplicated") << path + "Duplicated/" << QStringList({"saturn.txt", "mars.txt"}) << "saturn.txt" << "saturn 001.txt";
    QTest::newRow("Duplicated With Index") << path + "Duplicated/" << QStringList({"saturn_001.txt", "mars.txt"}) << "saturn_001.txt" << "saturn_002.txt";
    QTest::newRow("Duplicated Multiple") << path  + "DuplicatedMultiple/" << QStringList({"saturn_001.txt","saturn_002.txt", "saturn_1001.txt"}) << "saturn_001.txt" << "saturn_003.txt";
}

void TestFile::autoNaming()
{
    QFETCH(QString, root);
    QFETCH(QStringList, fileList);
    QFETCH(QString, newFile);
    QFETCH(QString, expected);

    // Cleanup
    QDir dir(root);
    QCOMPARE(dir.removeRecursively(), true);
    QCOMPARE(dir.mkpath(root), true);

    // Create folder and files
    QFileInfo FI;
    for(const QString &fName : fileList){
        FI.setFile(dir.absoluteFilePath(fName));
        QCOMPARE(dir.mkpath(FI.path()), true);
        QFile f(FI.absoluteFilePath());
        QCOMPARE(f.open(QIODevice::WriteOnly | QIODevice::Text), true);
        f.write( "fileInfoAutoNaming - test file");
        f.close();
    }

    // Test Phase
    auto filePath = dir.absoluteFilePath(newFile);
    auto result = File::fileAutoNaming(filePath);

    QCOMPARE(result, expected);

}


QTEST_MAIN(TestFile)
#include "test_file.moc"
