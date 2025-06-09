#include <QTest>
#include <QtNoidCommon/QtNoidCommon>
#include <QImage>
#include <QPainter>
#include <QPdfWriter>

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

    void testIsTextFileShouldBeTrue_data();
    void testIsTextFileShouldBeTrue();

    void testIsTextFile_ImagesShouldBeFalse_data();
    void testIsTextFile_ImagesShouldBeFalse();

    void testIsTextFile_PdfShouldBeFalse();

    void testIsTestFile_EmptyFileShouldBeFalse();

    void testSaveAsTextFile4String_data();
    void testSaveAsTextFile4String();


    void testSaveAsTextFile4StringList_data();
    void testSaveAsTextFile4StringList();

    void testSaveAsTextFileWithSubFolder_data();
    void testSaveAsTextFileWithSubFolder();

    void testReadAsStringList();


    void testListPathRecursively_data();
    void testListPathRecursively();

    void testListPathRecursivelyStartingFromAFileInTheFolder();



private:
    QDir testDataDir(const QString &testMethod, const QString &dataTag = {}) const
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

void TestFile::testIsTextFileShouldBeTrue_data()
{
    QTest::addColumn<bool>("enableBom");
    QTest::addColumn<QStringConverter::Encoding>("codec");
    //     enum Encoding {
    //         Utf8,
    // #ifndef QT_BOOTSTRAPPED
    //         Utf16,
    //         Utf16LE,
    //         Utf16BE,
    //         Utf32,
    //         Utf32LE,
    //         Utf32BE,
    // #endif
    //         Latin1,
    //         System,

    // These test cases are not significant since a correct decoding is not possible
    // QTest::newRow("NoBom-Utf16"     ) << false << QStringConverter::Utf16;
    // QTest::newRow("NoBom-Utf16LE"   ) << false << QStringConverter::Utf16LE;
    // QTest::newRow("NoBom-Utf16BE"   ) << false << QStringConverter::Utf16BE;
    // QTest::newRow("NoBom-Utf32"     ) << false << QStringConverter::Utf32;
    // QTest::newRow("NoBom-Utf32LE"   ) << false << QStringConverter::Utf32LE;
    // QTest::newRow("NoBom-Utf32BE"   ) << false << QStringConverter::Utf32BE;

    QTest::newRow("NoBom-Utf8"      ) << false << QStringConverter::Utf8;
    QTest::newRow("NoBom-Latin1"    ) << false << QStringConverter::Latin1;
    QTest::newRow("NoBom-System"    ) << false << QStringConverter::System;

    QTest::newRow("Bom-Utf8"      ) << true << QStringConverter::Utf8;
    QTest::newRow("Bom-Utf16"     ) << true << QStringConverter::Utf16;
    QTest::newRow("Bom-Utf16LE"   ) << true << QStringConverter::Utf16LE;
    QTest::newRow("Bom-Utf16BE"   ) << true << QStringConverter::Utf16BE;
    QTest::newRow("Bom-Utf32"     ) << true << QStringConverter::Utf32;
    QTest::newRow("Bom-Utf32LE"   ) << true << QStringConverter::Utf32LE;
    QTest::newRow("Bom-Utf32BE"   ) << true << QStringConverter::Utf32BE;
    QTest::newRow("Bom-Latin1"    ) << true << QStringConverter::Latin1;
    QTest::newRow("Bom-System"    ) << true << QStringConverter::System;

}


void TestFile::testIsTextFileShouldBeTrue()
{
    QFETCH(bool, enableBom);
    QFETCH(QStringConverter::Encoding, codec);

    QDir dir = testDataDir(__func__);
    // dir.removeRecursively();
    QCOMPARE(dir.mkpath(dir.absolutePath()), true);
    auto path = dir.absoluteFilePath( QTest::currentDataTag() + QString(".txt"));
    auto file = QFile(path);

    QCOMPARE(file.open(QIODevice::WriteOnly | QIODevice::Text), true);

    QTextStream out(&file);
    out.setGenerateByteOrderMark(enableBom);
    out.setEncoding(codec);
    out << "pippoàé pippoàéñ 🚀"<< Qt::endl;
    out << "Testo con BOM UTF-8" << Qt::endl;
    out << "Caratteri speciali: àèìòù 🚀" << Qt::endl;
    QString goodLuck = "祝你好运";
    out << goodLuck << Qt::endl;

    QCOMPARE(File::isTextFile(path), true);
}

void TestFile::testIsTextFile_ImagesShouldBeFalse_data()
{
    QTest::addColumn<QString>("format");

    QTest::newRow("Image BMP"    ) << ".BMP" ;
    QTest::newRow("Image PNG"    ) << ".PNG" ;
    QTest::newRow("Image JPG"    ) << ".JPG" ;
    QTest::newRow("Image TIF"    ) << ".TIF" ;
}

void TestFile::testIsTextFile_ImagesShouldBeFalse()
{
    QFETCH(QString, format);

    QDir dir = testDataDir(__func__);
    QCOMPARE(dir.mkpath(dir.absolutePath()), true);

    auto path = dir.absoluteFilePath( QTest::currentDataTag() + format);


    QSize size(640,480);
    QRect rect({0,0}, size);
    QPixmap pixmap(size);
    QPainter painter(&pixmap);
    QLinearGradient gradient(rect.topLeft(), rect.bottomRight());
    gradient.setColorAt(0, Qt::green);
    gradient.setColorAt(1, Qt::darkGreen);
    painter.fillRect(rect, gradient);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 30, 900, true));
    painter.drawText(10, 50, "Sample Image Qt");

    painter.setPen(QPen(Qt::red, 3));
    painter.drawEllipse(rect.center(), 50, 50);
    QRect redRect(0,0, 200, 150);
    redRect.moveCenter(rect.center());
    painter.drawRect(redRect);


    // QImage::save() - guess the encoding using the file suffix
    QCOMPARE(pixmap.save(path), true);


    QCOMPARE(File::isTextFile(path), false);

}

void TestFile::testIsTextFile_PdfShouldBeFalse()
{
    QDir dir = testDataDir(__func__);
    QCOMPARE(dir.mkpath(dir.absolutePath()), true);
    auto path = dir.absoluteFilePath("PdfShouldBeFalse.pdf");

    QPdfWriter writer(path);
    writer.setPageSize(QPageSize::A4);
    writer.setResolution(300);
    writer.setTitle("PDF Test document");
    writer.setCreator("QtNoid::Common::File -> TestFile");
    QPainter painter(&writer);
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(100, 200, "automated test report");
    painter.end();


    QCOMPARE(File::isTextFile(path), false);

}

void TestFile::testIsTestFile_EmptyFileShouldBeFalse()
{
    QDir dir = testDataDir(__func__);
    QCOMPARE(dir.mkpath(dir.absolutePath()), true);
    auto path = dir.absoluteFilePath("EmptyShouldBeFalse.txt");

    auto file = QFile(path);

    QCOMPARE(file.open(QIODevice::WriteOnly | QIODevice::Text), true);
    file.close();

    QCOMPARE(File::isTextFile(path), false);

}

void TestFile::testSaveAsTextFile4String_data()
{
    QTest::addColumn<QString>("filePath");
    QTest::addColumn<QString>("fileSuffix");
    QTest::addColumn<QString>("expectedFileName");

    QTest::newRow("TxtFileNoSuffix"         ) << "TxtFile" << "" << "TxtFile";
    QTest::newRow("TxtFileRemoveSuffix"     ) << "TxtFileRemoveSuffix.abc" << "" << "TxtFileRemoveSuffix";

    QTest::newRow("TxtFile+SuffixNoDot"     ) << "TxtFileSuffix.ini" << "txt" <<"TxtFileSuffix.txt" ;
    QTest::newRow("TxtFile+DotSuffix"       ) << "TxtFileDotSuffix.ini" << ".txt" << "TxtFileDotSuffix.txt";
    QTest::newRow("TxtFile+DotDubleSuffix"  ) << "TxtFileDotSuffix.tar.ini" << ".txt" << "TxtFileDotSuffix.tar.txt";
    QTest::newRow("TxtFile+BadSuffix"       ) << "TxtFile+BadSuffix" << " .txt" << "TxtFile+BadSuffix.txt";
    QTest::newRow("TxtFile+BadSuffix2"      ) << "TxtFile+BadSuffix2" << " txt" << "TxtFile+BadSuffix2.txt";
}

void TestFile::testSaveAsTextFile4String()
{
    QFETCH(QString, filePath);
    QFETCH(QString, fileSuffix);
    QFETCH(QString, expectedFileName);

    QDir dir = testDataDir(__func__);
    QString data = "AA BB CC";
    auto resPath = File::saveAsTextFile(data, filePath, dir.absolutePath(), fileSuffix);

    QFileInfo fileInfo(resPath);
    auto out = fileInfo.fileName();

    QCOMPARE(out, expectedFileName);


    // Check there is the file
    QCOMPARE(fileInfo.exists(), true);

    // Check the file content is the same
    QFile f(resPath);
    QCOMPARE(f.open(QIODevice::ReadOnly | QIODevice::Text), true);
    QTextStream stream(&f);
    auto outText = f.readAll();
    QCOMPARE(outText, data);
}

void TestFile::testSaveAsTextFile4StringList_data()
{
    QTest::addColumn<QString>("filePath");
    QTest::addColumn<QString>("fileSuffix");
    QTest::addColumn<QString>("expectedFileName");

    QTest::newRow("TxtFile+DotDubleSuffix"  ) << "TxtFileDotSuffix.tar.ini" << ".txt" << "TxtFileDotSuffix.tar.txt";

}

void TestFile::testSaveAsTextFile4StringList()
{

    QFETCH(QString, filePath);
    QFETCH(QString, fileSuffix);
    QFETCH(QString, expectedFileName);

    QDir dir = testDataDir(__func__);
    QStringList data = {"AA BB CC", "KK"};
    auto resPath = File::saveAsTextFile(data, filePath, dir.absolutePath(), fileSuffix);

    QFileInfo fileInfo(resPath);
    auto out = fileInfo.fileName();

    QCOMPARE(out, expectedFileName);


    // Check there is the file
    QCOMPARE(fileInfo.exists(), true);

    // Check the file content is the same
    QFile f(resPath);
    QCOMPARE(f.open(QIODevice::ReadOnly | QIODevice::Text), true);
    QTextStream stream(&f);
    QStringList outText;
    outText << f.readLine().replace("\n", "");
    outText << f.readLine();

    QCOMPARE(outText, data);

}

void TestFile::testSaveAsTextFileWithSubFolder_data()
{
    QTest::addColumn<QString>("filePath");
    QTest::addColumn<QString>("fileSuffix");
    QTest::addColumn<QString>("expectedRelativePath");

    QTest::newRow("TxtFileWithSubFolder"  ) << "a/TxtFileWithSubFolder" << "" << "a/TxtFileWithSubFolder";
    QTest::newRow("IniFile+SubFolderNo"  ) << "a/IniFile+SubFolder.txt" << ".ini" << "a/IniFile+SubFolder.ini";

}

void TestFile::testSaveAsTextFileWithSubFolder()
{
    QFETCH(QString, filePath);
    QFETCH(QString, fileSuffix);
    QFETCH(QString, expectedRelativePath);

    QDir dir = testDataDir(__func__);
    QStringList data = {"AA BB CC", "KK"};
    auto resPath = File::saveAsTextFile(data, filePath, dir.absolutePath(), fileSuffix);

    auto relPath = dir.relativeFilePath(resPath);
    QCOMPARE(relPath, expectedRelativePath);

    // QTest::newRow("TxtFileWithSubFolder"  ) << "/a/TxtFileWithSubFolder" << "" << "TxtFileWithSubFolderNo";
}


void TestFile::testReadAsStringList()
{
    QDir dir = testDataDir(__func__);
    dir.mkpath(dir.absolutePath());

    auto path = dir.absoluteFilePath("readAsStringList.txt");

    QFile f(path);
    QCOMPARE(f.open(QIODevice::WriteOnly | QIODevice::Text), true);
    QTextStream stream(&f);
    QStringList data = {"AA BB CC", "KK"};
    stream << data.join("\n");
    f.close();

    auto out = File::readAsStringList(path);

    QCOMPARE(out, data);


}

void TestFile::testListPathRecursively_data()
{

    QTest::addColumn<QStringList>("fileList");


    QTest::newRow("EmptyFolder"         ) << QStringList();
    QTest::newRow("SingleFile"          ) << QStringList({"lst.txt"});
    QTest::newRow("MultipleFilea"       ) << QStringList({"lst1.txt", "lst2.txt"});
    QTest::newRow("NestedDirectories"   ) << QStringList({"lst1.txt", "lst2.txt",
                                                          "a/lst1.txt", "a/lst2.txt"});


    // void testListPathRecursively_EmptyDirectory();
    // void testListPathRecursively_SingleFile();
    // void testListPathRecursively_MultipleFiles();
    // void testListPathRecursively_NestedDirectories();
    // void testListPathRecursively_WithFilters();
    // void testListPathRecursively_PathIsFile();
    // void testListPathRecursively_InvalidPath();

}

void TestFile::testListPathRecursively()
{
    QFETCH(QStringList, fileList);

    QDir dir = testDataDir(__func__, QTest::currentDataTag());
    auto res = testDataDirInit(dir, fileList);
    QCOMPARE(res, true);

    QStringList expected;
    for(const auto &item : std::as_const(fileList)) {
        expected << dir.absoluteFilePath(item);
    }

    File QtNoidFIle;
    auto resList = QtNoidFIle.listPathRecursively(dir.absolutePath());
    expected.sort();
    resList.sort();
    QCOMPARE(resList, expected);

    QCOMPARE(false, true);
}

void TestFile::testListPathRecursivelyStartingFromAFileInTheFolder()
{
    QCOMPARE(false, true);
}



QTEST_MAIN(TestFile)
#include "test_file.moc"


