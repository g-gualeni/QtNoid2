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

    void testTextNumberToJson_data();
    void testTextNumberToJson();

    void testTextArrayToJson_data();
    void testTextArrayToJson();

    void testPlainTextToJson_data();
    void testPlainTextToJson();

    void testPlainTextFromJson_data();
    void testPlainTextFromJson();

    void textPlainTextToJosnCornerCases();

    void testPlainTextToJsonFromListOfObjects_data();
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
    QTest::newRow("Object end afterArray2") << "[0 1 2 3] } " << true;
    QTest::newRow("Space Separated Array") << "[0 1 2 3]" << true;
    QTest::newRow("String Array") << "[\"AA\", BB, CC]" << true;
    QTest::newRow("Null Array") << "[null, null, null]" << true;
    QTest::newRow("Bool Array") << "[true, false, true]" << true;
    QTest::newRow("Int Array") << "[1, -100, +1000]" << true;
    QTest::newRow("Real Array") << "[-1.0, 1.23, +1000]" << true;
    QTest::newRow("Mixed Array") << "[-1.0, 1, \"+1000\"]" << true;
}

void TestQtNoidJsonTxt2Json::testPlainTextIsArray()
{
    QFETCH(QString, in);
    QFETCH(bool, expected);

    QCOMPARE(QtNoid::Json::Txt2Json::plainTextIsArray(in), expected);
}

void TestQtNoidJsonTxt2Json::testTextArrayToJson_data()
{
    QTest::addColumn<QString>("in");
    QTest::addColumn<QStringList>("expected");
    QTest::newRow("Space Separated Array")  << "[0 +1 -2 3]" << QStringList({"0", "1", "-2", "3"});
    QTest::newRow("String Array")           << "[\"AA\", BB, CC]" << QStringList({"\"AA\"", "\"BB\"", "\"CC\""});
    QTest::newRow("Null Array")             << "[null, null, null]" << QStringList({"null", "null", "null"});
    QTest::newRow("Bool Array")             << "[true, false, true]" << QStringList({"true", "false", "true"});
    QTest::newRow("Int Array")              << "[1, -100, +1000]" << QStringList({"1", "-100", "1000"});
    QTest::newRow("Real Array")             << "[-1.0, 1.23, +1000.2]" << QStringList({"-1.0", "1.23", "1000.2"});
    QTest::newRow("Mixed String")           << R"(["AA", BB, "CC])" << QStringList({"\"AA\"", "\"BB\"", "\"CC\""});
    QTest::newRow("Array ending with }")    << R"([AA, BB, "CC]})" << QStringList({"\"AA\"", "\"BB\"", "\"CC\""});
}

void TestQtNoidJsonTxt2Json::testTextArrayToJson()
{
    QFETCH(QString, in);
    QFETCH(QStringList, expected);
    QStringList actual = QtNoid::Json::Txt2Json::textArrayToJson(in);
    qDebug() << __func__ << actual;
    QCOMPARE(actual, expected);
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

void TestQtNoidJsonTxt2Json::testTextNumberToJson_data()
{
    QTest::addColumn<QString>("in");
    QTest::addColumn<QString>("expected");

    QTest::newRow("Zero")       << "0"      << "0";
    QTest::newRow("Leading 0")  << "00001"  << "1";
    QTest::newRow("Leading +0") << "+001"   << "1";
    QTest::newRow("Leading -0") << "-01"    << "-1";
    QTest::newRow("Double +0.") << "+0.123" << "0.123";
    QTest::newRow("Double +00.")<< "+00.123"<< "0.123";
    QTest::newRow("Double -0.") << "-0.321" << "-0.321";
    QTest::newRow("Double -00.")<< "-00.321"<< "-0.321";
    QTest::newRow("int +10")    << "+10"    << "10";
    QTest::newRow("int -10")    << "-10"    << "-10";

}

void TestQtNoidJsonTxt2Json::testTextNumberToJson()
{
    QFETCH(QString, in);
    QFETCH(QString, expected);
    QCOMPARE(QtNoid::Json::Txt2Json::textNumberToJson(in), expected);
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

void TestQtNoidJsonTxt2Json::testPlainTextToJsonFromListOfObjects_data()
{
    QTest::addColumn<QStringList>("in");
    QTest::addColumn<QString>("expected");

    // "Simple Child Object"
    // ObjName:
    //     Child01: 01
    //     Child01: 02
    QTest::newRow("Simple Child Object") << QStringList({"ObjName:", "Child01: 1", "Child02: 2", ""})
                                         << R"({"ObjName":{"Child01":1,"Child02":2}})";

    QTest::newRow("Simple Child with leading 0") << QStringList({"ObjName:", "Child01: 001", "Child02: 002", ""})
                                            << R"({"ObjName":{"Child01":1,"Child02":2}})";

    QTest::newRow("Simple Child with {}") << QStringList({"ObjName:", "{Child01: 1.1", "Child02: 2.2}", ""})
                                                 << R"({"ObjName":{"Child01":1.1,"Child02":2.2}})";

    QTest::newRow("Simple Child {} + spaces") << QStringList({"ObjName:", "{    Child01: 3.3", "   Child02: 4.4   }", ""})
                                          << R"({"ObjName":{"Child01":3.3,"Child02":4.4}})";

    QTest::newRow("Simple Child Name With Spaces") << QStringList({R"("Obj Name":)", R"({    "Child 01": 3.3)", R"(   "Child 02": 4.4   })", ""})
                                              << R"({"Obj Name":{"Child 01":3.3,"Child 02":4.4}})";


    // 2 Child Arrays
    // ObjName:
    //      Array01: [ 1  2  3 ]
    //      Array02: [ 10  20  30 ]
    //
    QTest::newRow("2 Child Arrays") << QStringList({R"(ObjName:)", R"(    Array01: [ 1  2  3 ])", R"(   Array02: [ AA  BB  CC ])", ""})
                                    << R"({"ObjName":{"Array01":[1,2,3],"Array02":["AA","BB","CC"]}})";

    QTest::newRow("2 Child Arrays with comma") << QStringList({R"(ObjName:)", R"(    Array01: [ 1  2  3 ],)", R"(   Array02: [ AA  BB  CC ])", ""})
                                               << R"({"ObjName":{"Array01":[1,2,3],"Array02":["AA","BB","CC"]}})";
    QTest::newRow("2 Child Arrays with {}") << QStringList({R"(ObjName:{)", R"(    Array01: [ 1  2  3 ])", R"(   Array02: [ AA  BB  CC ]})", ""})
                                    << R"({"ObjName":{"Array01":[1,2,3],"Array02":["AA","BB","CC"]}})";
    QTest::newRow("2 Child Arrays with {} 2") << QStringList({R"(ObjName:)", R"(    {Array01: [ 1  2  3 ])", R"(   Array02: [ AA  BB  CC ]})", ""})
                                            << R"({"ObjName":{"Array01":[1,2,3],"Array02":["AA","BB","CC"]}})";
    // Child Array of objects
    // Readings: [
    //      CodeName: Code128
    //      CodeName: CodeEAN]
    QTest::newRow("Child Array of objects") << QStringList({R"("Readings": [)", R"(    "CodeName": "Code128")", R"(    "CodeName": "CodeEAN"])"})
                                              << R"({"Readings":[{"CodeName":"Code128"},{"CodeName":"CodeEAN"}]})";


    // CodeSize: [ "5 Mils" "8 Mils" "10 Mils" "12 Mils" "15 Mils"  "20 Mils"]
    // WDLimit: [382  611 763 916 1144 1525]}
    // {CodeName: "ECC 200"
    //   CodeSize: [ "5 Mils" "8 Mils" "10 Mils" "12 Mils" "15 Mils" "20 Mils"]
    //   WDLimit: [ 230 367 458 550 687 916  ]}

}

void TestQtNoidJsonTxt2Json::testPlainTextToJsonFromListOfObjects()
{
    QFETCH(QStringList, in);
    QFETCH(QString, expected);

    QJsonObject jj = QtNoid::Json::Txt2Json::plainTextToJson(in);
    QJsonDocument doc(jj);
    QString actual = doc.toJson(QJsonDocument::Compact);

    // qDebug() << __func__ << in;
    // qDebug() << __func__ << actual;

    QCOMPARE(actual, expected);

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
