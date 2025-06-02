#include <QTest>
#include <QtNoidCommon/QtNoidCommon>

class TestText : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testIsValidUrl_data();
    void testIsValidUrl();

    void testTokenizeSnakeCase_data();
    void testTokenizeSnakeCase();
    void testTokenizeSnakeCaseList_data();
    void testTokenizeSnakeCaseList();

    void testTokenizeCamelCase_data();
    void testTokenizeCamelCase();
    void testTokenizeCamelCaseList_data();
    void testTokenizeCamelCaseList();

    void testTokenizeNumberBlocks_data();
    void testTokenizeNumberBlocks();

    void testTokenizeNumberBlocksList_data();
    void testTokenizeNumberBlocksList();

};

using namespace QtNoid::Common;

void TestText::initTestCase()
{
}

void TestText::cleanupTestCase()
{
}

void TestText::init()
{
}

void TestText::cleanup()
{
}

void TestText::testIsValidUrl_data()
{
    QTest::addColumn<QString>("url");
    QTest::addColumn<bool>("expected");

    QTest::newRow("valid_http") << "http://example.com" << true;
    QTest::newRow("valid_https") << "https://www.example.com" << true;
    QTest::newRow("valid_with_path") << "https://example.com/path/to/page" << true;
    QTest::newRow("valid_ftp") << "ftp://files.example.com" << true;
    QTest::newRow("invalid_no_scheme") << "example.com" << false;
    QTest::newRow("invalid_no_host") << "http://" << false;
    QTest::newRow("Plain text") << "Plain text" << false;
    QTest::newRow("empty") << "" << false;
}

void TestText::testIsValidUrl()
{
    QFETCH(QString, url);
    QFETCH(bool, expected);

    auto result = Text::isValidUrl(url);
    QCOMPARE(result, expected);

}

void TestText::testTokenizeSnakeCase_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QStringList>("expected");
    QTest::newRow("simple_text") << "simple_text" << QStringList({"simple", "text"});
}

void TestText::testTokenizeSnakeCase()
{
    QFETCH(QString, text);
    QFETCH(QStringList, expected);

    auto result = Text::tokenizeSnakeCase(text);
    QCOMPARE(result, expected);
}

void TestText::testTokenizeSnakeCaseList_data()
{
    QTest::addColumn<QStringList>("list");
    QTest::addColumn<QStringList>("expected");
    QTest::newRow("A_B C_D") << QStringList({"A_B", "C_D"}) << QStringList({"A", "B", "C", "D"});
}

void TestText::testTokenizeSnakeCaseList()
{
    QFETCH(QStringList, list);
    QFETCH(QStringList, expected);

    auto result = Text::tokenizeSnakeCase(list);
    QCOMPARE(result, expected);

}

void TestText::testTokenizeCamelCase_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QStringList>("expected");
    QTest::newRow("SimpleText") << "SimpleText" << QStringList({"Simple", "Text"});
}

void TestText::testTokenizeCamelCase()
{
    QFETCH(QString, text);
    QFETCH(QStringList, expected);

    auto result = Text::tokenizeCamelCase(text);
    QCOMPARE(result, expected);
}

void TestText::testTokenizeCamelCaseList_data()
{
    QTest::addColumn<QStringList>("list");
    QTest::addColumn<QStringList>("expected");
    QTest::newRow("AaBb CcDd") << QStringList({"AaBb", "CcDd"}) << QStringList({"Aa", "Bb", "Cc", "Dd"});

}

void TestText::testTokenizeCamelCaseList()
{
    QFETCH(QStringList, list);
    QFETCH(QStringList, expected);

    auto result = Text::tokenizeCamelCase(list);
    QCOMPARE(result, expected);
}

void TestText::testTokenizeNumberBlocks_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QStringList>("expected");
    QTest::newRow("Simple1") << "Simple1" << QStringList({"Simple1"});
    QTest::newRow("Simple01") << "Simple01" << QStringList({"Simple", "01"});
}

void TestText::testTokenizeNumberBlocks()
{
    QFETCH(QString, text);
    QFETCH(QStringList, expected);

    auto result = Text::tokenizeNumberBlocks(text);
    QCOMPARE(result, expected);
}

void TestText::testTokenizeNumberBlocksList_data()
{
    QTest::addColumn<int>("minNumBlock");
    QTest::addColumn<QStringList>("list");

    QTest::addColumn<QStringList>("expected");
    QTest::newRow("A1 C01") << 1 << QStringList({"A1", "C01"}) << QStringList({"A", "1", "C", "01"});
    QTest::newRow("A1 B02 C003") << 3 << QStringList({"A1", "B02", "C003"})
                                 << QStringList({"A1", "B02", "C", "003"});
}

void TestText::testTokenizeNumberBlocksList()
{
    QFETCH(int, minNumBlock);
    QFETCH(QStringList, list);
    QFETCH(QStringList, expected);

    auto result = Text::tokenizeNumberBlocks(list, minNumBlock);
    QCOMPARE(result, expected);
}




QTEST_MAIN(TestText)
#include "test_text.moc"
