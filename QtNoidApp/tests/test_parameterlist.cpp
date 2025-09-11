#include <QSignalSpy>
#include <QTest>
#include <QtNoidApp/QtNoidApp>

class TestQtNoidAppParameterList : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testCreatingParameterList();
    void testParameterListName();
    void testAddingParameters();
    void testRemovingParameters();
    void testParameterListClear();
    void testParameterAccess();
    void testParameterListSetValueConvenienceMethods();
    void testParameterDestruction();

private:

};

using namespace QtNoid::App;


void TestQtNoidAppParameterList::initTestCase()
{}

void TestQtNoidAppParameterList::cleanupTestCase()
{}

void TestQtNoidAppParameterList::init()
{}

void TestQtNoidAppParameterList::cleanup()
{}

void TestQtNoidAppParameterList::testCreatingParameterList()
{
    ParameterList list(this);
    QCOMPARE(list.count(), 0);
    QCOMPARE(list.name(), QString());
    
    ParameterList namedList("Application Configuration", this);
    QCOMPARE(namedList.count(), 0);
    QCOMPARE(namedList.name(), "Application Configuration");
}

void TestQtNoidAppParameterList::testParameterListName()
{
    ParameterList list(this);
    QSignalSpy spy(&list, &ParameterList::nameChanged);
    
    list.setName("Application Configuration");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(list.name(), "Application Configuration");
    
    // Check signal argument
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "Application Configuration");
    
    // Same value, no signal
    list.setName("Application Configuration");
    QCOMPARE(spy.count(), 0);
}

void TestQtNoidAppParameterList::testAddingParameters()
{
    ParameterList list(this);
    QSignalSpy countSpy(&list, &ParameterList::countChanged);
    QSignalSpy addedSpy(&list, &ParameterList::parameterAdded);
    
    auto param1 = new Parameter("Param1", 100.0, this);   
    list.addParameter(param1);
    QCOMPARE(list.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);
    QCOMPARE(list.contains(param1), true);
    QCOMPARE(list.contains("Param1"), true);
    
    auto param2 = new Parameter("Param2", 200.0, this);
    list.addParameter(param2);
    QCOMPARE(list.count(), 2);
    QCOMPARE(countSpy.count(), 2);
    QCOMPARE(addedSpy.count(), 2);
    
    // Adding same parameter again should not change anything
    list.addParameter(param1);
    QCOMPARE(list.count(), 2);
    QCOMPARE(countSpy.count(), 2);
    QCOMPARE(addedSpy.count(), 2);
    
    // Adding nullptr should not change anything
    list.addParameter(nullptr);
    QCOMPARE(list.count(), 2);
}

void TestQtNoidAppParameterList::testRemovingParameters()
{
    ParameterList list(this);
    
    auto param1 = new Parameter("Param1", 100.0, this);
    auto param2 = new Parameter("Param2", 200.0, this);
    
    list.addParameter(param1);
    list.addParameter(param2);
    QSignalSpy countSpy(&list, &ParameterList::countChanged);
    QSignalSpy removedSpy(&list, &ParameterList::parameterRemoved);
    
    list.removeParameter(param1);
    QCOMPARE(list.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(removedSpy.count(), 1);
    QCOMPARE(list.contains(param1), false);
    QCOMPARE(list.contains("Param1"), false);
    
    // Remove by name
    list.removeParameter("Param2");
    QCOMPARE(list.count(), 0);
    QCOMPARE(countSpy.count(), 2);
    QCOMPARE(removedSpy.count(), 2);
}

void TestQtNoidAppParameterList::testParameterListClear()
{
    ParameterList list(this);

    auto param1 = new Parameter("Param1", 100.0, this);
    auto param2 = new Parameter("Param2", 200.0, this);
    auto param3 = new Parameter("Param3", 300.0, this);

    list.addParameter(param1);
    list.addParameter(param2);
    list.addParameter(param3);
    QCOMPARE(list.count(), 3);

    QSignalSpy countSpy(&list, &ParameterList::countChanged);
    QSignalSpy removedSpy(&list, &ParameterList::parameterRemoved);
    list.clear();

    // Verify list is empty
    QCOMPARE(list.count(), 0);

    // Verify signals were emitted for each parameter removal
    QCOMPARE(removedSpy.count(), 3);
    QCOMPARE(countSpy.count(), 1); // Final count change signal

    // Verify final count change signal has correct value
    QList<QVariant> arguments = countSpy.takeLast();
    QCOMPARE(arguments.at(0).toInt(), 0);

    // Test clearing empty list (should not emit signals)
    countSpy.clear();
    removedSpy.clear();
    list.clear();
    QCOMPARE(countSpy.count(), 0);
    QCOMPARE(removedSpy.count(), 0);
}

void TestQtNoidAppParameterList::testParameterAccess()
{
    ParameterList list(this);
    auto param1 = new Parameter("Temperature", 25.0, this);
    auto param2 = new Parameter("Pressure", 1013.25, this);
    auto param3 = new Parameter("Humidity", 60.0, this);
    
    list.addParameter(param1);
    list.addParameter(param2);
    list.addParameter(param3);
    
    // Test access by index
    QCOMPARE(list.parameter(0), param1);
    QCOMPARE(list.parameter(1), param2);
    QCOMPARE(list.parameter(2), param3);
    QCOMPARE(list.parameter(-1), nullptr);
    QCOMPARE(list.parameter(3), nullptr);
    
    // Test access by name
    QCOMPARE(list.parameter("Temperature"), param1);
    QCOMPARE(list.parameter("Pressure"), param2);
    QCOMPARE(list.parameter("Humidity"), param3);
    QCOMPARE(list.parameter("NonExistent"), nullptr);
    
    // Test indexOf
    QCOMPARE(list.indexOf(param1), 0);
    QCOMPARE(list.indexOf(param2), 1);
    QCOMPARE(list.indexOf("Temperature"), 0);
    QCOMPARE(list.indexOf("Pressure"), 1);
    QCOMPARE(list.indexOf("NonExistent"), -1);
    
    // Test parameters() method
    QList<Parameter*> params = list.parameters();
    QCOMPARE(params.count(), 3);
    QCOMPARE(params.contains(param1), true);
    QCOMPARE(params.contains(param2), true);
    QCOMPARE(params.contains(param3), true);
}

void TestQtNoidAppParameterList::testParameterListSetValueConvenienceMethods()
{
    ParameterList list(this);
    auto param1 = new Parameter("Temperature", 25.0, this);
    auto param2 = new Parameter("Pressure", 1013.25, this);
    
    list.addParameter(param1);
    list.addParameter(param2);
    
    // Test value() method
    QCOMPARE(list.value("Temperature"), 25.0);
    QCOMPARE(list.value("Pressure"), 1013.25);
    QCOMPARE(list.value("NonExistent"), QVariant());
    
    // Test setValue() method
    QCOMPARE(list.setValue("Temperature", 30.0), true);
    QCOMPARE(param1->value(), 30.0);
    QCOMPARE(list.setValue("NonExistent", 123), false);
}

void TestQtNoidAppParameterList::testParameterDestruction()
{
    ParameterList list(this);
    QSignalSpy countSpy(&list, &ParameterList::countChanged);
    QSignalSpy removedSpy(&list, &ParameterList::parameterRemoved);
    
    auto param1 = new Parameter("Param1", 100.0);
    list.addParameter(param1);
    countSpy.clear();
    removedSpy.clear();
    
    // Delete parameter - should be automatically removed from list
    delete param1;
    
    QCOMPARE(list.count(), 0);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(removedSpy.count(), 1);
}


QTEST_MAIN(TestQtNoidAppParameterList)
#include "test_parameterlist.moc"
