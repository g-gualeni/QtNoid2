#include <QTest>
#include <QtNoidJson/QtNoidJson>

class TestQtNoidJsonYaml2Json : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();


    void testSimpleString();
    void testSimpleInteger();
    void testSimpleFloat();
    void testSimpleBoolean();
    void testSimpleNull();
    void testSimpleMap();
    void testSimpleSequence();
    void testFlowSequence();
    void testNestedMap();


    void testYaml2Json();
private:
};

void TestQtNoidJsonYaml2Json::initTestCase()
{
}

void TestQtNoidJsonYaml2Json::cleanupTestCase()
{
}

void TestQtNoidJsonYaml2Json::init()
{
}

void TestQtNoidJsonYaml2Json::cleanup()
{
}

void TestQtNoidJsonYaml2Json::testSimpleString()
{
    QtNoid::Json::Yaml2Json converter("name: John Space");

    QVERIFY(converter.isValid());
    QJsonObject expected({{"name","John Space"}});
    QCOMPARE(converter.json(), expected);
}

void TestQtNoidJsonYaml2Json::testSimpleInteger()
{
    QtNoid::Json::Yaml2Json converter("age: 55");

    QVERIFY(converter.isValid());
    QJsonObject expected({{"age", 55}});
    QCOMPARE(converter.json(), expected);
}

void TestQtNoidJsonYaml2Json::testSimpleFloat()
{
    QtNoid::Json::Yaml2Json converter("price: 55.55");

    QVERIFY(converter.isValid());
    QJsonObject expected({{"price", 55.55}});
    QCOMPARE(converter.json(), expected);

}

void TestQtNoidJsonYaml2Json::testSimpleBoolean()
{
    QtNoid::Json::Yaml2Json converter("visible: true");

    QVERIFY(converter.isValid());
    QJsonObject expected({{"visible", true}});
    QCOMPARE(converter.json(), expected);
}

void TestQtNoidJsonYaml2Json::testSimpleNull()
{
    QtNoid::Json::Yaml2Json converter("value: null");

    QVERIFY(converter.isValid());
    QJsonObject expected({{"value", QJsonValue()}});
    QCOMPARE(converter.json(), expected);
}

void TestQtNoidJsonYaml2Json::testSimpleMap()
{
    QtNoid::Json::Yaml2Json converter(
        R"(Name: Johan Koradì)"         "\n"
        R"(Age: 125)"                   "\n"
        R"(Active: true)"               "\n"
        R"(Email: Johan.Koradì@gg.com)" "\n"
    );

    QVERIFY(converter.isValid());
    QJsonObject expected({
                          {"Name", "Johan Koradì"},
                          {"Age", 125},
                          {"Active", true},
                          {"Email", "Johan.Koradì@gg.com"}
    });
    QCOMPARE(converter.json(), expected);

}

void TestQtNoidJsonYaml2Json::testSimpleSequence()
{
    QtNoid::Json::Yaml2Json converter(
        R"(ToBuy:)"     "\n"
        R"( - Sugar)"   "\n"
        R"( - Apples)"  "\n"
        R"( - Bananas)" "\n"
        );

    // qDebug() << converter.yaml();
    // qDebug() << converter.json();

    QVERIFY(converter.isValid());
    QJsonObject expected({{"ToBuy",
                           QJsonArray({"Sugar", "Apples", "Bananas"})}
    });

    // qDebug() << expected;

    QCOMPARE(converter.json(), expected);

}

void TestQtNoidJsonYaml2Json::testFlowSequence()
{
    QtNoid::Json::Yaml2Json converter("numbers: [1, 2, 3, 4, 5]");

    QVERIFY(converter.isValid());
    QJsonObject expected({{"numbers",
                           QJsonArray({1,2,3,4,5})}
    });
    QCOMPARE(converter.json(), expected);

}

void TestQtNoidJsonYaml2Json::testNestedMap()
{
    QtNoid::Json::Yaml2Json converter(
R"(User:
  - Name: Bella Lee
  - Age: 123
  - Email: bella.lee@vala.com
  - Address:
    - Street: Via Della Scimmia
    - City: Salcapperovo
)");

    qDebug() << converter.tokens();
    qDebug() << converter.yaml();
    qDebug() << converter.errorString();

    QVERIFY(converter.isValid());

    QVERIFY(0);
}


void TestQtNoidJsonYaml2Json::testYaml2Json()
{
    QVERIFY(0);
}


QTEST_MAIN(TestQtNoidJsonYaml2Json)
#include "test_yaml2json.moc"
