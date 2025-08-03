#include <QTest>
#include <QtNoidApp/QtNoidApp>
#include <QImage>
#include <QPainter>
#include <QPdfWriter>

class TestSettings : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private:

};

using namespace QtNoid::App;


void TestSettings::initTestCase()
{}

void TestSettings::cleanupTestCase()
{}

void TestSettings::init()
{}

void TestSettings::cleanup()
{}



QTEST_MAIN(TestSettings)
#include "test_settings.moc"


