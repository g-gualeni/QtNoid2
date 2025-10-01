#include <QTest>
#include <QtNoidJson/QtNoidJson>

class TestQtNoidJsonTxt2Json : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testPlainTextIsString_data();
    void testPlainTextIsString();

    void testPlainTextIsArray_data();
    void testPlainTextIsArray();

    void testPlainTextIsNumber_data();
    void testPlainTextIsNumber();

    void testTextArrayToJson_data();
    void testTextArrayToJson();

    void testPlainTextToJson_data();
    void testPlainTextToJson();

    void testPlainTextFromJson_data();
    void testPlainTextFromJson();

    void textPlainTextToJosnCornerCases();
    void testPlainTextToJsonFromListOfObjects();


private:
};

void TestQtNoidJsonTxt2Json::initTestCase()
{
}

void TestQtNoidJsonTxt2Json::cleanupTestCase()
{
}

void TestQtNoidJsonTxt2Json::init()
{
}

void TestQtNoidJsonTxt2Json::cleanup()
{
}

void TestQtNoidJsonTxt2Json::testPlainTextIsString_data()
{
    QTest::addColumn<QString>("in");
    QTest::addColumn<bool>("res");

    QTest::newRow("Empty String") << "" << false;
    QTest::newRow("String") << "This is the end" << true;
    QTest::newRow("IP String1") << "0.0.0.0" << true;
    QTest::newRow("IP String2") << "192.168.0.100" << true;
    QTest::newRow("bool-1") << "true" << false;
    QTest::newRow("bool-2") << "false" << false;
    QTest::newRow("null") << "null" << false;
    QTest::newRow("Array") << "[1,2,3]" << false;
    QTest::newRow("Integer") << "255" << false;
    QTest::newRow("Integer+") << "+1000" << false;
    QTest::newRow("Integer-") << "-255" << false;
    QTest::newRow("Real+") << "2.55" << false;
    QTest::newRow("Real-") << "-2.55" << false;
}

void TestQtNoidJsonTxt2Json::testPlainTextIsString()
{
    QFETCH(QString, in);
    QFETCH(bool, res);

    QCOMPARE(QtNoid::Json::Txt2Json::plainTextIsString(in), res);

}

void TestQtNoidJsonTxt2Json::testPlainTextIsArray_data()
{
    QTest::addColumn<QString>("in");
    QTest::addColumn<bool>("expected");
    QTest::newRow("Object end afterArray") << "[0 1 2 3]}" << true;
    QTest::newRow("Space Separated Array") << "[0 1 2 3]" << true;
    QTest::newRow("String Array") << "[\"AA\", BB, CC]" << true;
    QTest::newRow("Null Array") << "[null, null, null]" << true;
    QTest::newRow("Bool Array") << "[true, false, true]" << true;
    QTest::newRow("Int Array") << "[1, -100, +1000]" << true;
    QTest::newRow("Real Array") << "[-1.0, 1.23, +1000]" << true;
}

void TestQtNoidJsonTxt2Json::testPlainTextIsArray()
{
    QFETCH(QString, in);
    QFETCH(bool, expected);

    QCOMPARE(QtNoid::Json::Txt2Json::plainTextIsArray(in), expected);
}

void TestQtNoidJsonTxt2Json::testTextArrayToJson_data()
{

    QVERIFY(0);

    QTest::addColumn<QString>("in");
    QTest::addColumn<QStringList>("res");
    QTest::newRow("Space Separated Array") << "[0 1 2 3]" << QStringList({"0", "1", "2", "3"});
    QTest::newRow("String Array") << "[\"AA\", BB, CC]" << QStringList({"AA", "BB", "CC"});
    QTest::newRow("Null Array") << "[null, null, null]" << QStringList({"null", "null", "null"});
    QTest::newRow("Bool Array") << "[true, false, true]" << QStringList({"true", "false", "true"});
    QTest::newRow("Int Array") << "[1, -100, +1000]" << QStringList({"1", "-100", "+1000"});
    QTest::newRow("Real Array") << "[-1.0, 1.23, +1000]" << QStringList({"-1.0", "1.23", "+1000"});
}

void TestQtNoidJsonTxt2Json::testTextArrayToJson()
{
    QVERIFY(0);

    QFETCH(QString, in);
    QFETCH(QStringList, res);
    QCOMPARE(QtNoid::Json::Txt2Json::textArrayToJson(in), res);
}

void TestQtNoidJsonTxt2Json::testPlainTextIsNumber_data()
{
    QTest::addColumn<QString>("in");
    QTest::addColumn<bool>("res");

    QTest::newRow("String") << "This is the end" << false;
    QTest::newRow("IP String1") << "0.0.0.0" << false;
    QTest::newRow("IP String2") << "192.168.0.100" << false;
    QTest::newRow("bool-1") << "true" << false;
    QTest::newRow("bool-2") << "false" << false;
    QTest::newRow("null") << "null" << false;
    QTest::newRow("Array") << "[1,2,3]" << false;
    QTest::newRow("Integer") << "100" << true;
    QTest::newRow("Integer+") << "+255" << true;
    QTest::newRow("Integer-") << "-255" << true;
    QTest::newRow("Real") << "2.55" << true;
    QTest::newRow("Real+") << "+12.13" << true;
    QTest::newRow("Real-") << "-2.55" << true;

}

void TestQtNoidJsonTxt2Json::testPlainTextIsNumber()
{
    QFETCH(QString, in);
    QFETCH(bool, res);
    QCOMPARE(QtNoid::Json::Txt2Json::plainTextIsNumber(in), res);
}

void TestQtNoidJsonTxt2Json::textPlainTextToJosnCornerCases()
{
    // Empty String
    QString emtpyLine = "";
    QJsonObject jj = QtNoid::Json::Txt2Json::plainTextToJson({emtpyLine});
    QVERIFY(jj.isEmpty());

    // Empty Lines
    QString crLine = "\n";
    jj = QtNoid::Json::Txt2Json::plainTextToJson({crLine});
    QVERIFY(jj.isEmpty());

    // Multiple Empty Lines
    jj = QtNoid::Json::Txt2Json::plainTextToJson({crLine, crLine});
    QVERIFY(jj.isEmpty());
}

void TestQtNoidJsonTxt2Json::testPlainTextToJsonFromListOfObjects()
{
    QStringList def;
    def << "ObjName:";
    def << "{Array01: [ 1  2  3 ],";
    def << "Array02: [ 10  20  30 ]}";

    QJsonObject jj = QtNoid::Json::Txt2Json::plainTextToJson(def);

    qDebug() << jj;

    QVERIFY(0);

}

void TestQtNoidJsonTxt2Json::testPlainTextToJson_data()
{
    QTest::addColumn<QStringList>("in");
    QTest::addColumn<QString>("key");
    QTest::addColumn<QJsonValue>("val");

    QTest::newRow("Bool 1") << QStringList({"default: true"}) << "default" << QJsonValue(true);
    QTest::newRow("Bool 2") << QStringList({"default: false"}) << "default" << QJsonValue(false);
    QTest::newRow("IP Address") << QStringList({"default: 0.0.0.0"}) << "default"
                                << QJsonValue("0.0.0.0");
    QTest::newRow("Title") << QStringList({"title  : Enable At Startup"}) << "title"
                           << QJsonValue("Enable At Startup");
    QTest::newRow("Int Val") << QStringList({"max  : 655"}) << "max" << QJsonValue(655);
    QTest::newRow("Int Val-") << QStringList({"min  : -123"}) << "min" << QJsonValue(-123);
    QTest::newRow("Int Val+") << QStringList({"max  : +655"}) << "max" << QJsonValue(655);

    QTest::newRow("String Array") << QStringList({"values  : [CR, LF, CRLF]"}) << "values"
                                  << QJsonValue(QJsonArray({"CR", "LF", "CRLF"}));

    QTest::newRow("Bool Array") << QStringList({"values  : [true, false, true]"}) << "values"
                                << QJsonValue(QJsonArray({true, false, true}));

    QTest::newRow("Int Array") << QStringList({"values  : [100, -123, 12345]"}) << "values"
                               << QJsonValue(QJsonArray({100, -123, 12345}));

    QTest::newRow("Real Array") << QStringList({"values  : [+1.00, -1.23, 12.45]"}) << "values"
                                << QJsonValue(QJsonArray({1.00, -1.23, 12.45}));

    QTest::newRow("Folder Path") << QStringList({"folder : c:/database"}) << "folder"
                                 << QJsonValue("c:/database");

    QTest::newRow("Missing :") << QStringList({"folder /database"}) << "folder"
                               << QJsonValue();
}

void TestQtNoidJsonTxt2Json::testPlainTextToJson()
{
    QFETCH(QStringList, in);
    QFETCH(QString, key);
    QFETCH(QJsonValue, val);

    QJsonObject jj = QtNoid::Json::Txt2Json::plainTextToJson(in);

    // qDebug() << "Expected:" << val;
    // qDebug() << "Result  :" << jj[key];

    QCOMPARE(jj[key], val);

}

void TestQtNoidJsonTxt2Json::testPlainTextFromJson_data()
{
    QTest::addColumn<QStringList>("in");
    QTest::newRow("Bool 1") << QStringList({"default: true"});
    QTest::newRow("Bool 2") << QStringList({"default: false"});
    QTest::newRow("IP Address") << QStringList({"default: 0.0.0.0"});
    QTest::newRow("Title") << QStringList({"title: Enable At Startup"});
    QTest::newRow("Int Val") << QStringList({"max: 655"});
    QTest::newRow("Int Val-") << QStringList({"min: -123"});
    QTest::newRow("String Array") << QStringList({"values: [CR, LF, CRLF]"});
    QTest::newRow("Bool Array") << QStringList({"values: [true, false, true]"});
    QTest::newRow("Int Array") << QStringList({"values: [100, -123, 12345]"});
    QTest::newRow("Real Array") << QStringList({"values: [1.01, -1.23, 12.45]"});
    QTest::newRow("Folder Path") << QStringList({"folder: c:/database"});
    QTest::newRow("Two Lines") << QStringList({"folder: c:/database", "values: [1.01, -1.23, 12.45]"});
}

void TestQtNoidJsonTxt2Json::testPlainTextFromJson()
{
    QFETCH(QStringList, in);

    QJsonObject jj = QtNoid::Json::Txt2Json::plainTextToJson(in);
    QStringList res = QtNoid::Json::Txt2Json::plainTextFromJson(jj);

    // qDebug() << res;
    QCOMPARE(in, res);
}




QTEST_MAIN(TestQtNoidJsonTxt2Json)
#include "test_txt2json.moc"
