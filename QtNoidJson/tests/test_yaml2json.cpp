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

void TestQtNoidJsonYaml2Json::testYaml2Json()
{
    QVERIFY(0);
}


QTEST_MAIN(TestQtNoidJsonYaml2Json)
#include "test_yaml2json.moc"
