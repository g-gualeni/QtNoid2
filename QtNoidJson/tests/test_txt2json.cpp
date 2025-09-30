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

    // void testPlainTextIsString_data();
    // void testPlainTextIsString();

    // void testPlainTextIsArray_data();
    // void testPlainTextIsArray();

    // void testPlainTextIsNumber_data();
    // void testPlainTextIsNumber();

    void testPlainTextToJson_data();
    void testPlainTextToJson();

    void testPlainTextFromJson_data();
    void testPlainTextFromJson();


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
