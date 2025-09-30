#include <QTest>
#include <QtNoidCommon/QtNoidCommon>

class TestQtNoidCommonText : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testIsValidUrl_data();
    void testIsValidUrl();
    void testIsValidEmail_data();
    void testIsValidEmail();

    void testSanitizeString_data();
    void testSanitizeString();

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

    void testTokenize_data();
    void testTokenize();

    void testConvertToCamelCase_data();
    void testConvertToCamelCase();

    void testConvertToSnakeCase_data();
    void testConvertToSnakeCase();

};

using namespace QtNoid::Common;

void TestQtNoidCommonText::initTestCase()
{
}

void TestQtNoidCommonText::cleanupTestCase()
{
}

void TestQtNoidCommonText::init()
{
}

void TestQtNoidCommonText::cleanup()
{
}

void TestQtNoidCommonText::testIsValidUrl_data()
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

void TestQtNoidCommonText::testIsValidUrl()
{
    QFETCH(QString, url);
    QFETCH(bool, expected);

    auto result = Text::isValidUrl(url);
    QCOMPARE(result, expected);
}

void TestQtNoidCommonText::testIsValidEmail_data()
{
    QTest::addColumn<QString>("email");
    QTest::addColumn<bool>("expected");

    QTest::newRow("valid_simple") << "test@example.com" << true;
    QTest::newRow("valid_with_dots") << "user.name@domain.co.uk" << true;
    QTest::newRow("valid_with_plus") << "user+tag@example.org" << true;
    QTest::newRow("invalid_no_at") << "invalid.email" << false;
    QTest::newRow("invalid_no_domain") << "user@" << false;
    QTest::newRow("invalid_no_user") << "@domain.com" << false;
    QTest::newRow("invalid_no_tld") << "user@domain" << false;
    QTest::newRow("empty") << "" << false;
}

void TestQtNoidCommonText::testIsValidEmail()
{
    QFETCH(QString, email);
    QFETCH(bool, expected);

    auto result = Text::isValidEmail(email);
    QCOMPARE(result, expected);
}

void TestQtNoidCommonText::testSanitizeString_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("expected");

    QTest::newRow("empty") << "" << "";
    QTest::newRow("normal") << "Hello World" << "Hello World";
    QTest::newRow("with_spaces") << "  Hello   World  " << "Hello World";
    QTest::newRow("with_tabs") << "\tHello\tWorld\t" << "Hello World";
    QTest::newRow("with_newlines") << "Hello\nWorld\r\n" << "Hello World";
    QTest::newRow("control_chars") << "Hello\x01\x02World" << "HelloWorld";
    QTest::newRow("multiple_spaces") << "Hello     World" << "Hello World";
}

void TestQtNoidCommonText::testSanitizeString()
{
    QFETCH(QString, input);
    QFETCH(QString, expected);

    QString result = Text::sanitizeString(input);
    QCOMPARE(result, expected);
}

void TestQtNoidCommonText::testTokenizeSnakeCase_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QStringList>("expected");
    QTest::newRow("simple_text") << "simple_text" << QStringList({"simple", "text"});
}

void TestQtNoidCommonText::testTokenizeSnakeCase()
{
    QFETCH(QString, text);
    QFETCH(QStringList, expected);

    auto result = Text::tokenizeSnakeCase(text);
    QCOMPARE(result, expected);
}

void TestQtNoidCommonText::testTokenizeSnakeCaseList_data()
{
    QTest::addColumn<QStringList>("list");
    QTest::addColumn<QStringList>("expected");
    QTest::newRow("A_B C_D") << QStringList({"A_B", "C_D"}) << QStringList({"A", "B", "C", "D"});
}

void TestQtNoidCommonText::testTokenizeSnakeCaseList()
{
    QFETCH(QStringList, list);
    QFETCH(QStringList, expected);

    auto result = Text::tokenizeSnakeCase(list);
    QCOMPARE(result, expected);
}

void TestQtNoidCommonText::testTokenizeCamelCase_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QStringList>("expected");
    QTest::newRow("SimpleText") << "SimpleText" << QStringList({"Simple", "Text"});
}

void TestQtNoidCommonText::testTokenizeCamelCase()
{
    QFETCH(QString, text);
    QFETCH(QStringList, expected);

    auto result = Text::tokenizeCamelCase(text);
    QCOMPARE(result, expected);
}

void TestQtNoidCommonText::testTokenizeCamelCaseList_data()
{
    QTest::addColumn<QStringList>("list");
    QTest::addColumn<QStringList>("expected");
    QTest::newRow("AaBb CcDd") << QStringList({"AaBb", "CcDd"}) << QStringList({"Aa", "Bb", "Cc", "Dd"});

}

void TestQtNoidCommonText::testTokenizeCamelCaseList()
{
    QFETCH(QStringList, list);
    QFETCH(QStringList, expected);

    auto result = Text::tokenizeCamelCase(list);
    QCOMPARE(result, expected);
}

void TestQtNoidCommonText::testTokenizeNumberBlocks_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QStringList>("expected");
    QTest::newRow("Simple1") << "Simple1" << QStringList({"Simple1"});
    QTest::newRow("Simple01") << "Simple01" << QStringList({"Simple", "01"});
}

void TestQtNoidCommonText::testTokenizeNumberBlocks()
{
    QFETCH(QString, text);
    QFETCH(QStringList, expected);

    auto result = Text::tokenizeNumberBlocks(text);
    QCOMPARE(result, expected);
}

void TestQtNoidCommonText::testTokenizeNumberBlocksList_data()
{
    QTest::addColumn<int>("minNumBlock");
    QTest::addColumn<QStringList>("list");

    QTest::addColumn<QStringList>("expected");
    QTest::newRow("A1 C01") << 1 << QStringList({"A1", "C01"}) << QStringList({"A", "1", "C", "01"});
    QTest::newRow("A1 B02 C003") << 3 << QStringList({"A1", "B02", "C003"})
                                 << QStringList({"A1", "B02", "C", "003"});
}

void TestQtNoidCommonText::testTokenizeNumberBlocksList()
{
    QFETCH(int, minNumBlock);
    QFETCH(QStringList, list);
    QFETCH(QStringList, expected);

    auto result = Text::tokenizeNumberBlocks(list, minNumBlock);
    QCOMPARE(result, expected);
}

void TestQtNoidCommonText::testTokenize_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("splittersString");
    QTest::addColumn<bool>("splitCamelCase");
    QTest::addColumn<int>("numberBlockLen");
    QTest::addColumn<QStringList>("expected");

    QTest::newRow("Multiple   Spaces   -1Splitters-NoCamelCase-NoNumbers")
        << "Multiple   Spaces   "
        << " " << false << 0 << QStringList({"Multiple", "Spaces"});

    QTest::newRow("FileName012-1Splitters-NoCamelCase-NoNumbers")
        << "FileTCPName012.png"
        << "." << false << 0 << QStringList({"FileTCPName012", "png"});
    QTest::newRow("FileName012-1Splitters-CamelCase-NoNumbers")
        << "FileTCPName012.png"
        << "." << true << 0 << QStringList({"File", "TCP", "Name012", "png"});
    QTest::newRow("FileName012-1Splitters-CamelCase-2Numbers")
        << "FileTCPName012.png"
        << "." << true << 2 << QStringList({"File", "TCP", "Name", "012", "png"});

    QTest::newRow("FileName012aa-1Splitters-CamelCase-2Numbers")
        << "FileTCPName012aa.png"
        << "." << true << 2 << QStringList({"File", "TCP", "Name", "012", "aa", "png"});

    QTest::newRow("FileName-5Splitters-NoCamelCase-NoNumbers")
        << "ThisIsCamelFileName99 Sec+01++00-2--00_3__004..png"
        << " +-_." << false << 0
        << QStringList({"ThisIsCamelFileName99", "Sec", "01", "00", "2", "00", "3", "004", "png"});

    QTest::newRow("FileName-5Splitters-CamelCase-NoNumbers")
        << "ThisIsCamelFileName99 Sec+01++01-2--00_3__004..png"
        << " +-_." << true << 0
        << QStringList({"This",
                        "Is",
                        "Camel",
                        "File",
                        "Name99",
                        "Sec",
                        "01",
                        "01",
                        "2",
                        "00",
                        "3",
                        "004",
                        "png"});

    QTest::newRow("FileName-5Splitters-CamelCase-2Numbers")
        << "ThisIsCamelFileName99 Sec+01++01-2--00_3__004..png"
        << " +-_." << true << 2
        << QStringList({"This",
                        "Is",
                        "Camel",
                        "File",
                        "Name",
                        "99",
                        "Sec",
                        "01",
                        "01",
                        "2",
                        "00",
                        "3",
                        "004",
                        "png"});

    QTest::newRow("FilePath-7Splitters-NoCamelCase-NoNumbers")
        << "this\\is\\my/folder/ThisIsCamelFileName Sec01+002-003_004.png"
        << " +-_.\\/" << false << 0
        << QStringList({"this",
                        "is",
                        "my",
                        "folder",
                        "ThisIsCamelFileName",
                        "Sec01",
                        "002",
                        "003",
                        "004",
                        "png"});

    QTest::newRow("LogEntry-2Splitters-NoCamelCase-NoNumbers")
        << "2020-08-23\t01:27:17\t871 TZ01.ABC011-KK1-2"
        << "\t " << false << 0
        << QStringList({"2020-08-23", "01:27:17", "871", "TZ01.ABC011-KK1-2"});
    QTest::newRow("LogEntry-2Splitters-CamelCase-NoNumbers")
        << "2020-08-23\t01:27:17\t871 ProductionLotTZ01.ABC011-KK1-2"
        << "\t " << true << 0
        << QStringList({"2020-08-23",
                        "01:27:17",
                        "871",
                        "Production",
                        "Lot",
                        "T",
                        "Z01.",
                        "AB",
                        "C011-",
                        "K",
                        "K1-2"});
    QTest::newRow("LogEntry-2Splitters-CamelCase-2Numbers")
        << "2020-08-23\t01:27:17\t871 ProductionLotTZ01.ABC011-KK1-2"
        << "\t" << true << 2
        << QStringList({"2020", "-",  "08",  "-", "23",         "01",  ":", "27",
                        ":",    "17", "871", " ", "Production", "Lot", "T", "Z",
                        "01",   ".",  "AB",  "C", "011",        "-",   "K", "K1-2"});

    QTest::newRow("LetterNumber-1Split-NoCamelCase-NoNumbers")
        << "2020-08-23 CamelCase333 22Sup"
        << " " << false << 0 << QStringList({"2020-08-23", "CamelCase333", "22Sup"});

    QTest::newRow("LetterNumber-1Split-CamelCase-NoNumbers")
        << "2020-08-23 CamelCase333 22Sup"
        << " " << true << 0 << QStringList({"2020-08-23", "Camel", "Case333", "22", "Sup"});

    QTest::newRow("LetterNumber-1Split-CamelCase-2Numbers")
        << "2020-08-23 CamelCase333 22Sup"
        << " " << true << 2
        << QStringList({"2020", "-", "08", "-", "23", "Camel", "Case", "333", "22", "Sup"});
}

void TestQtNoidCommonText::testTokenize()
{
    QFETCH(QString, input);
    QFETCH(QString, splittersString);
    QFETCH(bool, splitCamelCase);
    QFETCH(int, numberBlockLen);
    QFETCH(QStringList, expected);

    auto result = Text::tokenize(input, splittersString, splitCamelCase,
                                 numberBlockLen);

    // qDebug() << QtNoidCommon::fastTokenize(input, splittersString, splitCamelCase, numberBlockLen);
    QCOMPARE(result, expected);
}

void TestQtNoidCommonText::testConvertToCamelCase_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("expected");

    QTest::newRow("_this_snake_case") << "_this_is_a_snake_case"
                                      << "ThisIsASnakeCase";
    QTest::newRow("snake123_turtle_") << "snake123_turtle_"
                                      << "Snake123Turtle";
}

void TestQtNoidCommonText::testConvertToCamelCase()
{
    QFETCH(QString, input);
    QFETCH(QString, expected);
    auto result = Text::convertToCamelCase(input);
    QCOMPARE(result, expected);
}

void TestQtNoidCommonText::testConvertToSnakeCase_data()
{
    QTest::addColumn<int>("minNumBlockLen");
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("expected");

    QTest::newRow("CamelNotation-0Numbers")<< 0 << "thisIsACamelNotation123"
                                            << "this_is_a_camel_notation123";
    QTest::newRow("Snake1Turtle-2Numbers") << 2 << "Snake1Turtle"
                                           << "snake1_turtle";
    QTest::newRow("TCPSnakeIp127-2Numbers") << 2 << "TCPSnakeIp127"
                                            << "tcp_snake_ip_127";
}

void TestQtNoidCommonText::testConvertToSnakeCase()
{
    QFETCH(int, minNumBlockLen);
    QFETCH(QString, input);
    QFETCH(QString, expected);
    auto result = Text::convertToSnakeCase(input, minNumBlockLen);
    QCOMPARE(result, expected);
}



QTEST_MAIN(TestQtNoidCommonText)
#include "test_text.moc"
