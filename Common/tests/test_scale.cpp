#include <QTest>
#include <QtNoidCommon/QtNoidCommon>

class TestScale : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

};

using namespace QtNoid::Common;


void TestScale::initTestCase()
{

}

void TestScale::cleanupTestCase()
{

}

void TestScale::init()
{

}

void TestScale::cleanup()
{

}

QTEST_MAIN(TestScale)
#include "test_scale.moc"
