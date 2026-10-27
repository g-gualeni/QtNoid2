#include <QComboBox>
#include <QJsonArray>
#include <QTest>
#include <QtNoidUiCommon/QtNoidUiCommon>


class TestQtNoidUiCommonCore : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // ComboBox tests
    void testSaveComboBoxTextItems();
    void testRestoreComboBoxTextItems();


private:


};

using namespace QtNoid::UiCommon;


void TestQtNoidUiCommonCore::initTestCase()
{}

void TestQtNoidUiCommonCore::cleanupTestCase()
{}

void TestQtNoidUiCommonCore::init()
{}

void TestQtNoidUiCommonCore::cleanup()
{}

void TestQtNoidUiCommonCore::testSaveComboBoxTextItems()
{
    // Sanity check: Handle null pointer gracefully (should not crash)
    QComboBox* cboNull = nullptr;
    QJsonObject cboNullJson = Core::saveComboBoxTextItems(cboNull);
    qDebug() << cboNullJson;

    // Test 0: Save empty combo box
    QComboBox* cbo0 = new QComboBox();
    QJsonObject cbo0Json = Core::saveComboBoxTextItems(cbo0);
    // qDebug() << cbo0Json;
    QVERIFY(cbo0Json.contains("Items"));
    QVERIFY(cbo0Json.contains("CurrentText"));
    delete cbo0;

    // Test 1: Save combo box items
    QComboBox* cbo1 = new QComboBox();
    cbo1->addItems({"Option1", "Option2", "Option3"});
    QJsonObject cbo1Json = Core::saveComboBoxTextItems(cbo1);
    // qDebug() << cbo1Json;

    const QJsonArray itemsArray = cbo1Json["Items"].toArray();
    QCOMPARE(itemsArray.size(), 3);
    QCOMPARE(itemsArray.at(0), "Option1");
    QCOMPARE(itemsArray.at(1), "Option2");
    QCOMPARE(itemsArray.at(2), "Option3");

    // Verify current item was saved as "CurrentText"
    QString cbo1CurrentText = cbo1Json["CurrentText"].toString();
    QCOMPARE(cbo1->currentText(), cbo1CurrentText);

    delete cbo1;
}

void TestQtNoidUiCommonCore::testRestoreComboBoxTextItems()
{
    // Create a json object and initialize a combo box
    QJsonObject combo1;
    combo1["CurrentText"] = "Item1";
    combo1["Items"] = QJsonArray::fromStringList({"Item0", "Item1"});
    QComboBox* cbo1 = new QComboBox();
    auto res1 = Core::restoreComboBoxTextItems(cbo1, combo1);
    QVERIFY(res1);
    QCOMPARE(cbo1->itemText(0), "Item0");
    QCOMPARE(cbo1->itemText(1), "Item1");
    QCOMPARE(cbo1->currentText(), "Item1");
    delete cbo1;

    // Create a JSON object and try with a non existing current text
    QJsonObject combo2;
    combo2["CurrentText"] = "Item5";
    combo2["Items"] = QJsonArray::fromStringList({"Item0", "Item1"});
    QComboBox* cbo2 = new QComboBox();
    auto res2 = Core::restoreComboBoxTextItems(cbo2, combo2);
    QVERIFY(!res2);
    QCOMPARE(cbo2->itemText(0), "Item0");
    QCOMPARE(cbo2->itemText(1), "Item1");
    QCOMPARE(cbo2->currentText(), "Item0");
    delete cbo2;

    // Init with an empty json
    QJsonObject combo3;
    QComboBox* cbo3 = new QComboBox();
    auto res3 = Core::restoreComboBoxTextItems(cbo3, combo3);
    QVERIFY(!res3);
    QCOMPARE(cbo3->count(), 0);
    delete cbo3;

    // An empty json should not change the content
    QComboBox* cbo4 = new QComboBox();
    cbo4->addItems({"Item0", "Item1", "Item2"});
    QJsonObject combo4;
    auto res4 = Core::restoreComboBoxTextItems(cbo4, combo4);
    QVERIFY(!res4);
    QCOMPARE(cbo4->count(), 3);
    QCOMPARE(cbo4->itemText(0), "Item0");
    QCOMPARE(cbo4->itemText(1), "Item1");
    QCOMPARE(cbo4->itemText(2), "Item2");
    QCOMPARE(cbo4->currentText(), "Item0");
    delete cbo4;
}



QTEST_MAIN(TestQtNoidUiCommonCore)
#include "test_uicommon_core.moc"

