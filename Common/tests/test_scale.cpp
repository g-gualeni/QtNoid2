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

    void testPrefixList();

    void testMultiplier_data();
    void testMultiplier();

    void testMultiplierPrefix_data();
    void testMultiplierPrefix();

    void testAutoUpToExaByte_data();
    void testAutoUpToExaByte();

    void testAutoUpToExa_data();
    void testAutoUpToExa();

    void testNanoSecsUpToDays_data();
    void testNanoSecsUpToDays();
};

using namespace QtNoid::Common;


void TestScale::initTestCase()
{}

void TestScale::cleanupTestCase()
{}

void TestScale::init()
{}

void TestScale::cleanup()
{}


void TestScale::testPrefixList()
{
    auto result = Scale::prefixList();
    QCOMPARE(result.isEmpty(), false);
}


void TestScale::testMultiplier_data()
{
    QTest::addColumn<QString>("prefix");
    QTest::addColumn<qint64>("expected");
    QTest::newRow("Invalid") << "u" << 0LL;
    QTest::newRow("Kilo") << "k" << 1000LL;
    QTest::newRow("Mega") << "M" << 1000LL * 1000;
    QTest::newRow("Giga") << "G" << 1000LL * 1000 * 1000;
    QTest::newRow("Tera") << "T" << 1000LL * 1000 * 1000 * 1000;
    QTest::newRow("Peta") << "P" << 1000LL * 1000 * 1000 * 1000 * 1000;
    QTest::newRow("Exa") << "E" << 1000LL * 1000 * 1000 * 1000 * 1000 * 1000;
}
void TestScale::testMultiplier()
{
    QFETCH(QString, prefix);
    QFETCH(qint64, expected);
    auto result = Scale::multiplier(prefix);
    QCOMPARE(result, expected);

}


void TestScale::testMultiplierPrefix_data()
{
    QTest::addColumn<qint64>("value");
    QTest::addColumn<QString>("expected");

    QTest::newRow("Invalid") << 0LL << "";
    QTest::newRow("Kilo") << 1000LL << "k";
    QTest::newRow("Mega") << 1000LL * 1000 << "M";
    QTest::newRow("Giga") << 1000LL * 1000 * 1000 << "G";
    QTest::newRow("Tera") << 1000LL * 1000 * 1000 * 1000 << "T";
    QTest::newRow("Peta") << 1000LL * 1000 * 1000 * 1000 * 1000 << "P";
    QTest::newRow("Exa") << 1000LL * 1000 * 1000 * 1000 * 1000 * 1000 << "E";
    QTest::newRow("1.001k") << 1001LL << "k";
    QTest::newRow("500k") << 500LL * 1000L << "k";

}
void TestScale::testMultiplierPrefix()
{
    QFETCH(qint64, value);
    QFETCH(QString, expected);
    auto result = Scale::multiplierPrefix(value);
    QCOMPARE(result, expected);
}


void TestScale::testAutoUpToExaByte_data()
{
    QTest::addColumn<qint64>("value");
    QTest::addColumn<QString>("expected");

    QTest::newRow("Invalid") << 0LL << "0.00 [Byte]";
    QTest::newRow("-1Exa") << -1024LL * 1024 * 1024 * 1024 * 1024 * 1024 << "-1.00 [EByte]";

    QTest::newRow("1.12kB") << static_cast<qint64>(1.12 * 1024LL) << "1.12 [kByte]";
    QTest::newRow("0.99MB") << static_cast<qint64>(0.99 * 1024LL * 1024) << "0.99 [MByte]";
    QTest::newRow("1GB") << 1024LL * 1024 * 1024 << "1.00 [GByte]";
    QTest::newRow("1TB") << 1024LL * 1024 * 1024 * 1024 << "1.00 [TByte]";
    QTest::newRow("1PB") << 1024LL * 1024 * 1024 * 1024 * 1024 << "1.00 [PByte]";
    QTest::newRow("1EB") << 1024LL * 1024 * 1024 * 1024 * 1024 * 1024 << "1.00 [EByte]";

}
void TestScale::testAutoUpToExaByte()
{
    QFETCH(qint64, value);
    QFETCH(QString, expected);
    auto result = Scale::autoUpToExaByte(value);
    QCOMPARE(result, expected);
}


void TestScale::testAutoUpToExa_data()
{
    QTest::addColumn<qint64>("value");
    QTest::addColumn<QString>("measUnit");
    QTest::addColumn<QString>("expected");

    QTest::newRow("1.23kg") << static_cast<qint64>(1.23 * 1000) << "[g]"
                            << "1.23 [kg]";
    QTest::newRow("-1.9kg") << -1900LL << "g"
                            << "-1.90 [kg]";

    QTest::newRow("2kg") << 2 * 1000LL + 489 << "g"
                         << "2.49 [kg]";
    QTest::newRow("2GJ") << static_cast<qint64>(2.5 * 1000 * 1000 * 1000) << "Joule"
                         << "2.50 [GJoule]";
    QTest::newRow("9EW") << 9156LL * 1000 * 1000 * 1000 * 1000 * 1000 << "Watt"
                         << "9.16 [EWatt]";


}
void TestScale::testAutoUpToExa()
{
    QFETCH(qint64, value);
    QFETCH(QString, measUnit);
    QFETCH(QString, expected);
    auto result = Scale::autoUpToExa(value, measUnit);
    QCOMPARE(result, expected);

}

void TestScale::testNanoSecsUpToDays_data()
{
    QTest::addColumn<qint64>("value");
    QTest::addColumn<QString>("expected");
    QTest::newRow("125 ns") << 125LL << "125 [ns]";
    QTest::newRow("1999 ns") << 1999LL << "2.00 [µs]";
    QTest::newRow("999 µs") << 999LL * 1000 << "999 [µs]";
    QTest::newRow("1776 µs") << 1776LL * 1000 << "1.78 [ms]";
    QTest::newRow("800 ms") << 800LL * 1000 * 1000 << "800 [ms]";
    QTest::newRow("1 m 5s") << 65LL * 1000 * 1000 * 1000 << "00:01:05";
    QTest::newRow("4 h") << 4LL * 60 * 60 * 1000 * 1000 * 1000 << "04:00:00";
    QTest::newRow("2 gg 2h") << 50LL * 60 * 60 * 1000 * 1000 * 1000 << "002 02:00:00";
    QTest::newRow("59.22 s") << 59LL * 1000 * 1000 * 1000 + 220 * 1000 * 1000 << "59.22 [s]";

}

void TestScale::testNanoSecsUpToDays()
{
    QFETCH(qint64, value);
    QFETCH(QString, expected);
    auto result = Scale::nanoSecsUpToDays(value);
    QCOMPARE(result, expected);

}


QTEST_MAIN(TestScale)
#include "test_scale.moc"
