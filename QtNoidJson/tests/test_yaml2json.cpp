#include <QTest>
#include <QSignalSpy>
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
    void testNestedSequenceOrArrayOfNestedObjects();
    void testNestedMapOrNestedObjects();

    void testDeepNesting();          // Nesting profondo (5+ livelli)

    void testMixedStructures_data();
    void testMixedStructures();

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

    QJsonObject expected({{"name","John Space"}});
    QCOMPARE(converter.json(), expected);
    QVERIFY(converter.isValid());
}


void TestQtNoidJsonYaml2Json::testSimpleInteger()
{
    QtNoid::Json::Yaml2Json converter("age: 55");

    QJsonObject expected({{"age", 55}});
    QCOMPARE(converter.json(), expected);
    QVERIFY(converter.isValid());
}


void TestQtNoidJsonYaml2Json::testSimpleFloat()
{
    QtNoid::Json::Yaml2Json converter("price: 55.55");

    QJsonObject expected({{"price", 55.55}});
    QCOMPARE(converter.json(), expected);
    QVERIFY(converter.isValid());
}


void TestQtNoidJsonYaml2Json::testSimpleBoolean()
{
    QtNoid::Json::Yaml2Json converter("visible: true");

    QJsonObject expected({{"visible", true}});
    QCOMPARE(converter.json(), expected);
    QVERIFY(converter.isValid());
}


void TestQtNoidJsonYaml2Json::testSimpleNull()
{
    QtNoid::Json::Yaml2Json converter("value: null");

    QJsonObject expected({{"value", QJsonValue()}});
    QCOMPARE(converter.json(), expected);
    QVERIFY(converter.isValid());
}


void TestQtNoidJsonYaml2Json::testSimpleMap()
{
    QtNoid::Json::Yaml2Json converter(
        R"(Name: Johan Koradì)"         "\n"
        R"(Age: 125)"                   "\n"
        R"(Active: true)"               "\n"
        R"(Email: Johan.Koradì@gg.com)" "\n"
    );

    QJsonObject expected({
                          {"Name", "Johan Koradì"},
                          {"Age", 125},
                          {"Active", true},
                          {"Email", "Johan.Koradì@gg.com"}
    });
    QCOMPARE(converter.json(), expected);
    QVERIFY(converter.isValid());
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

    QJsonObject expected({{"ToBuy",
                           QJsonArray({"Sugar", "Apples", "Bananas"})}
    });

    // qDebug() << expected;

    QCOMPARE(converter.json(), expected);
    QVERIFY(converter.isValid());
}


void TestQtNoidJsonYaml2Json::testFlowSequence()
{
    QtNoid::Json::Yaml2Json converter("numbers: [1, 2, 3, 4, 5]");

    QJsonObject expected({{"numbers",
                           QJsonArray({1,2,3,4,5})}
    });
    QCOMPARE(converter.json(), expected);
    QVERIFY(converter.isValid());
}


void TestQtNoidJsonYaml2Json::testNestedSequenceOrArrayOfNestedObjects()
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

    QJsonArray address = {
        QJsonObject({{"Street", "Via Della Scimmia"}}),
        QJsonObject({{"City", "Salcapperovo"}})

    };

    QJsonObject expected({{"User", QJsonArray({
                    QJsonObject({ {"Name", "Bella Lee"} }),
                    QJsonObject({ {"Age", 123} }),
                    QJsonObject({ {"Email", "bella.lee@vala.com"} }),
                    QJsonObject({ {"Address", address} }),
                })
    }});

    QCOMPARE(converter.json(), expected);

    // qDebug() << converter.tokens();
    // qDebug() << converter.yaml();
    // qDebug() << converter.json();
    // qDebug() << expected;

    QVERIFY(converter.isValid());

}

void TestQtNoidJsonYaml2Json::testNestedMapOrNestedObjects()
{
    QtNoid::Json::Yaml2Json converter(
R"(
User Object:
  Name: Bella Lee
  Age: 123
  Email: bella.lee@vala.com
  Address:
    Street: Via Della Scimmia
    City: Salcapperovo
)");

    // qDebug() << converter.tokens();
    // qDebug() << converter.json();

    QJsonObject address = {
        {"Street", "Via Della Scimmia"},
        {"City", "Salcapperovo"}
    };

    QJsonObject expected({{"User Object", QJsonObject({
                                    {"Name", "Bella Lee"} ,
                                    {"Age", 123} ,
                                    {"Email", "bella.lee@vala.com"} ,
                                    {"Address", address}
                        })
    }});
    // qDebug() << expected;

    QCOMPARE(converter.json(), expected);

    QVERIFY(converter.isValid());

}


void TestQtNoidJsonYaml2Json::testDeepNesting()
{
    QtNoid::Json::Yaml2Json converter(
        "a:\n"
        "  b:\n"
        "    c:\n"
        "      d:\n"
        "        e:\n"
        "          f: deep\n");

    QVERIFY(converter.isValid());
    QJsonObject obj = converter.json();
    QJsonValue v = obj;
    for (const char *key : {"a", "b", "c", "d", "e"})
        v = v.toObject().value(QLatin1String(key));
    QCOMPARE(v.toObject().value("f").toString(), QString("deep"));
}


void TestQtNoidJsonYaml2Json::testMixedStructures_data()
{
    QTest::addColumn<QString>("yaml");
    QTest::addColumn<QString>("path");      // keys separated by '/', array index as number
    QTest::addColumn<QVariant>("expected");

    QString yaml;

    // Array of multiple objects
    yaml = R"(
users:
  - name: Alice
    age: 30
    active: true
  - name: Bob
    age: 25
    active: false
  - name: Charlie
    age: 35
    active: true
)";
    QTest::newRow("array of objects") << yaml << "users/1/name" << QVariant("Bob");
    QTest::newRow("array of objects bool") << yaml << "users/2/active" << QVariant(true);

    // Array and Objects mixed
    yaml = R"(
product:
  name: Laptop
  price: 999.99
  tags: [electronics, computers, portable]
  specs:
    cpu: Intel i7
    ram: 16
  reviews:
    - rating: 5
      comment: Excellent
    - rating: 4
      comment: Good value
)";
    QTest::newRow("mixed double") << yaml << "product/price" << QVariant(999.99);
    QTest::newRow("mixed flow seq") << yaml << "product/tags/2" << QVariant("portable");
    QTest::newRow("mixed seq of maps") << yaml << "product/reviews/1/rating" << QVariant(4);

    // Configuration File
    yaml = R"(
server:
  host: localhost
  port: 8080
  endpoints:
    - path: /api/users
      methods: [GET, POST]
      auth: true
    - path: /api/products
      methods: [GET]
      auth: false
  database:
    connections:
      - name: primary
        url: postgres://localhost:5432
        pool: 10
      - name: cache
        url: redis://localhost:6379
        pool: 5
)";
    QTest::newRow("config url") << yaml << "server/database/connections/1/url" << QVariant("redis://localhost:6379");
    QTest::newRow("config methods") << yaml << "server/endpoints/0/methods/1" << QVariant("POST");
    QTest::newRow("config int") << yaml << "server/port" << QVariant(8080);
}

void TestQtNoidJsonYaml2Json::testMixedStructures()
{
    QFETCH(QString, yaml);
    QFETCH(QString, path);
    QFETCH(QVariant, expected);

    QtNoid::Json::Yaml2Json converter(yaml);
    QVERIFY2(converter.isValid(), qPrintable(converter.errorString()));

    QJsonValue value = converter.json();
    const QStringList keys = path.split('/');
    for (const QString &key : keys) {
        bool isIndex = false;
        int index = key.toInt(&isIndex);
        value = isIndex ? value.toArray().at(index) : value.toObject().value(key);
    }
    QCOMPARE(value.toVariant(), expected);
}

void TestQtNoidJsonYaml2Json::testYaml2Json()
{
    // Error reporting
    QtNoid::Json::Yaml2Json converter;
    QSignalSpy spy(&converter, &QtNoid::Json::Yaml2Json::errorOccurred);
    converter.setYaml("a: [1, 2\nb: 3\n");
    QVERIFY(!converter.isValid());
    QVERIFY(converter.errorString().startsWith("YAML error at line 2, column 1"));
    QVERIFY(converter.json().isEmpty());
    QCOMPARE(spy.count(), 1);

    // Recovery after a valid text
    converter.setYaml("ok: true");
    QVERIFY(converter.isValid());
    QVERIFY(converter.errorString().isEmpty());
    QCOMPARE(converter.json()["ok"].toBool(), true);

    // Empty text gives an empty object
    converter.setYaml("");
    QVERIFY(converter.isValid());
    QVERIFY(converter.json().isEmpty());

    // Anchors and aliases are resolved
    converter.setYaml("base: &b {x: 1}\ncopy: *b\n");
    QVERIFY(converter.isValid());
    QCOMPARE(converter.json().value("copy").toObject().value("x").toInt(), 1);

    // Root sequence
    converter.setYaml("- 1\n- two\n");
    QVERIFY(converter.isValid());
    QVERIFY(converter.document().isArray());
    QCOMPARE(converter.jsonArray().at(1).toString(), QString("two"));

    // Static helper
    QString error;
    QJsonDocument doc = QtNoid::Json::Yaml2Json::convert("k: v", &error);
    QVERIFY(error.isEmpty());
    QCOMPARE(doc.object()["k"].toString(), QString("v"));
}


QTEST_MAIN(TestQtNoidJsonYaml2Json)
#include "test_yaml2json.moc"
