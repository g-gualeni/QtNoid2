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

    void testAutoNamingNextName_data();
    void testAutoNamingNextName();

    void testAutoNaming_data();
    void testAutoNaming();

    void testAutoNamingFileInfo_data();
    void testAutoNamingFileInfo();

    void testCompareIfEqual_data();
    void testCompareIfEqual();

    void testIsTextFile_data();
    void testIsTextFile();

private:
    QDir testDataDir(const QString &testMethod, const QString &dataTag) const
    {
        QString dataPath = QDir::currentPath() +
                           QDir::separator() + metaObject()->className() +
                           QDir::separator() + testMethod;

        if (!dataTag.isEmpty()) {
            dataPath += QDir::separator() + dataTag;
        }

        return QDir(dataPath);
    }

    bool testDataDirInit(const QDir &dataDir, const QStringList &fileList) const
    {
        auto dir = dataDir;
        dir.removeRecursively();
        auto path = dataDir.absolutePath();
        dir.mkpath(path);

        if(!dir.exists()) {
            return false;
        }

        // Create files
        QFileInfo FI;
        for(const QString &fName : fileList){
            FI.setFile(dir, fName);
            dir.mkpath(FI.path());
            QFile f(FI.absoluteFilePath());
            f.open(QIODevice::WriteOnly | QIODevice::Text);
            f.write(QString(fName + " - test file").toLatin1());
            f.close();
        }
        return true;
    }

    bool testDataFileInit(const QDir &dataDir, const QString &fileName, const QByteArray &data) const
    {
        QFileInfo FI(dataDir.absoluteFilePath(fileName));
        auto path = FI.absolutePath();
        dataDir.mkpath(path);
        if(!dataDir.exists(path)) {
            return false;
        }

        if(data.isEmpty())
            return true;

        QFile f(FI.absoluteFilePath());
        if(!f.open(QIODevice::WriteOnly | QIODevice::Text)){
            return false;
        }
        f.write(data);
        f.close();
        return true;
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

void TestFile::testAutoNamingNextName_data()
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
    QTest::newRow("Full Path no Counters") << "C:/a/file.xyp" << "C:/a/file 001.xyp";
    QTest::newRow("Full file path2") << "./a/file-01-01.xyp" << "./a/file-01-02.xyp";
    QTest::newRow("No Extension") << "ReadMe01" << "ReadMe02";
}

void TestFile::testAutoNamingNextName()
{
    QFETCH(QString, currentName);
    QFETCH(QString, expected);
    auto result = File::autoNamingNextName(currentName);
    QCOMPARE(result, expected);
}

void TestFile::testAutoNaming_data()
{
    QTest::addColumn<QStringList>("fileList");
    QTest::addColumn<QString>("newFile");
    QTest::addColumn<QString>("expected");

    // Set the test case
    QTest::newRow("Single File") << QStringList({}) << "saturn.txt" << "saturn.txt";
    QTest::newRow("Single With Index") << QStringList({}) << "mars123.txt" << "mars123.txt";
    QTest::newRow("Duplicated") << QStringList({"saturn.txt", "mars.txt"}) << "saturn.txt" << "saturn 001.txt";
    QTest::newRow("Duplicated With Index") << QStringList({"saturn_001.txt", "mars.txt"}) << "saturn_001.txt" << "saturn_002.txt";
    QTest::newRow("Duplicated Multiple") << QStringList({"saturn_001.txt","saturn_002.txt", "saturn_1001.txt"}) << "saturn_001.txt" << "saturn_003.txt";
}

void TestFile::testAutoNaming()
{
    QFETCH(QStringList, fileList);
    QFETCH(QString, newFile);
    QFETCH(QString, expected);

    // Init the file folder
    QDir dir = testDataDir(__func__, QTest::currentDataTag());
    auto res = testDataDirInit(dir, fileList);
    QCOMPARE(res, true);

    auto result = File::autoNaming(dir.absoluteFilePath(newFile));

    QCOMPARE(result, dir.absoluteFilePath(expected));

}

void TestFile::testAutoNamingFileInfo_data()
{
    QTest::addColumn<QStringList>("fileList");
    QTest::addColumn<QString>("newFile");
    QTest::addColumn<QString>("expected");

    QTest::newRow("Single File") << QStringList({}) << "saturn.txt" << "saturn.txt";
    QTest::newRow("Single With Index") << QStringList({}) << "mars123.txt" << "mars123.txt";
    QTest::newRow("Duplicated") << QStringList({"saturn.txt", "mars.txt"}) << "saturn.txt" << "saturn 001.txt";
    QTest::newRow("Duplicated With Index") << QStringList({"saturn_001.txt", "mars.txt"}) << "saturn_001.txt" << "saturn_002.txt";
    QTest::newRow("Duplicated Multiple") << QStringList({"saturn_001.txt","saturn_002.txt", "saturn_1001.txt"}) << "saturn_001.txt" << "saturn_003.txt";
    QTest::newRow("Duplicated Multiple NoNumber") << QStringList({"saturn.txt",
                                                                  "saturn 001.txt",
                                                                  "saturn 002.txt",
                                                                  "saturn 1001.txt"}) << "saturn.txt" << "saturn 003.txt";
}

void TestFile::testAutoNamingFileInfo()
{
    QFETCH(QStringList, fileList);
    QFETCH(QString, newFile);
    QFETCH(QString, expected);

    // Init the file folder
    QDir dir = testDataDir(__func__, QTest::currentDataTag());
    auto res = testDataDirInit(dir, fileList);
    QCOMPARE(res, true);

    auto result = File::autoNaming(dir.absoluteFilePath(newFile));

    QCOMPARE(result, dir.absoluteFilePath(expected));


}

void TestFile::testCompareIfEqual_data()
{
    QTest::addColumn<QStringList>("fileList");
    QTest::addColumn<bool>("expected");

    QTest::newRow("BothNonExisting"  ) << QStringList({"",""}) << true;
    QTest::newRow("FirstNonExisting"  ) << QStringList({"","SECOND"}) << false;
    QTest::newRow("LastNonExisting"  ) << QStringList({"FIRST",""}) << false;
    QTest::newRow("EqualButSize"     ) << QStringList({"AA_AAA","AA_AAA "}) << false;
    QTest::newRow("ExpectIsEqual"    ) << QStringList({"AAAAA","AAAAA"}) << true;
    QTest::newRow("ExpectIsDifferent") << QStringList({"AAAAA","BBBBB"}) << false;
}

void TestFile::testCompareIfEqual()
{
    QFETCH(QStringList, fileList);
    QFETCH(bool, expected);

    QDir dir = testDataDir(__func__, QTest::currentDataTag());
    dir.removeRecursively();

    auto path1 = "path1.txt";
    auto path2 = "path2.txt";
    auto res = testDataFileInit(dir, path1, fileList.first().toLatin1());
    QCOMPARE(res, true);
    res = testDataFileInit(dir, path2, fileList.last().toLatin1());
    QCOMPARE(res, true);

    res = File::compareIfEqual(dir.absoluteFilePath(path1), dir.absoluteFilePath(path2));
    QCOMPARE(res, expected);


}

void TestFile::testIsTextFile_data()
{
    qDebug() << __func__;

    QDir dir = testDataDir(__func__, QTest::currentDataTag());
}

void TestFile::testIsTextFile()
{

}


QTEST_MAIN(TestFile)
#include "test_file.moc"
