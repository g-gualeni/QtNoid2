#include <QSignalSpy>
#include <QTest>
#include <QJsonObject>
#include <QJsonArray>
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
    void testBindableNameProperty();

    void testToJsonValues();
    void testToJsonValuesNoName();
    void TestToJsonSchema();
    void TestToJsonSchemaNoName();
    void testValuesFromJsonWithEmptyNameShouldTakeTheJsonName();
    void testValuesFromJsonWithWrongNameShouldFail();
    void testValuesFromJsonWithCorrectName();

    void testSchemaFromJson();
    void testSchemaFromJsonWithWrongNameShouldFail();



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

void TestQtNoidAppParameterList::testBindableNameProperty()
{
    // Get the bindable property
    ParameterList list("File Settings", this);
    auto bindableName = list.bindableName();
    QVERIFY(bindableName.isValid());
    QCOMPARE(bindableName.value(), "File Settings");

    // Test binding to another property
    QProperty<QString> externalProperty;
    externalProperty.setBinding([&]() { return bindableName.value(); });
    QCOMPARE(externalProperty.value(), "File Settings");

    // Change parameter name and verify binding updates
    list.setName("NewName");
    QCOMPARE(externalProperty.value(), "NewName");

    // Test setting name through bindable
    bindableName.setValue("FinalName");
    QCOMPARE(list.name(), "FinalName");
    QCOMPARE(externalProperty.value(), "FinalName");

    // Create a reverse binding from externalProperty to list
    // Change external property and verify binding works
    QSignalSpy spy(&list, &ParameterList::nameChanged);
    bindableName.setBinding([&]() { return externalProperty.value(); });
    externalProperty.setValue("Updated External Value");
    QCOMPARE(bindableName.value(), "Updated External Value");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().toString(), "Updated External Value");
}

void TestQtNoidAppParameterList::testToJsonValues()
{
    // Create a parameter list with parameters
    ParameterList page("Configuration", this);
    auto param1 = new Parameter("Temperature", 25.5, this);
    page.addParameter(param1);
    auto param2 = new Parameter("Pressure", 1013.25, this);
    page.addParameter(param2);

    QJsonObject json = page.toJsonValues();
    // qDebug() << __func__ << json;

    // Verify JSON structure
    QVERIFY(json.contains("Configuration"));
    QJsonArray parametersArray = json["Configuration"].toArray();
    QCOMPARE(parametersArray.size(), 2);
    QVERIFY(parametersArray.contains(param1->toJsonValue()));
    QVERIFY(parametersArray.contains(param2->toJsonValue()));
}

void TestQtNoidAppParameterList::testToJsonValuesNoName()
{
    ParameterList page(this);
    QJsonObject json = page.toJsonValues();
    // qDebug() << __func__ << json;
    QVERIFY(json.contains("PageName"));

}

void TestQtNoidAppParameterList::TestToJsonSchema()
{
    ParameterList page("Configuration", this);
    auto param1 = new Parameter("Temperature", "temp", 25.5, this);
    param1->setRange(0,100);
    param1->setUnit("°C");
    param1->setReadOnly(true);
    page.addParameter(param1);


    auto param2 = new Parameter("Pressure", "pres" , 1013.25, this);
    param2->setRange(0,2000);
    param2->setUnit("hPa");
    param2->setReadOnly(true);
    page.addParameter(param2);

    QJsonObject jsonSchema = page.toJsonSchema();
    // qDebug() << __func__ << jsonSchema;

    QVERIFY(jsonSchema.contains("Configuration"));
    QJsonArray parametersArray = jsonSchema["Configuration"].toArray();
    QCOMPARE(parametersArray.size(), 2);

    QVERIFY(parametersArray.contains(param1->toJsonSchema()));
    QVERIFY(parametersArray.contains(param2->toJsonSchema()));
}

void TestQtNoidAppParameterList::TestToJsonSchemaNoName()
{
    ParameterList page(this);
    QJsonObject json = page.toJsonSchema();
    // qDebug() << __func__ << json;
    QVERIFY(json.contains("PageName"));
}

void TestQtNoidAppParameterList::testValuesFromJsonWithEmptyNameShouldTakeTheJsonName()
{
    // Create JSON objects (simulating what Parameter::toJsonValue() would return)
    QJsonArray parametersArray;

    QJsonObject pressureJson;
    pressureJson["Pressure"]= 1013.25;
    parametersArray.append(pressureJson);
    
    QJsonObject temperatureJson;
    temperatureJson["Temperature"]= 25;
    parametersArray.append(temperatureJson);

    QJsonObject json;
    json["MyTestPage"] = parametersArray;
    // qDebug() << __func__ << json;

    // Test successful fromJson
    ParameterList list(this);
    QVERIFY(list.valuesFromJson(json));

    // Verify list properties
    QCOMPARE(list.name(), "MyTestPage");
    QCOMPARE(list.count(), 2);

    auto actual = list.parameter("Pressure")->toJsonValue();
    QCOMPARE(actual, pressureJson);
    actual = list.parameter("Temperature")->toJsonValue();
    QCOMPARE(actual, temperatureJson);
}

void TestQtNoidAppParameterList::testValuesFromJsonWithWrongNameShouldFail()
{
    QJsonObject json;
    json["MyTestPage"] = QJsonArray();

    ParameterList list("NotMyTestPage", this);
    QCOMPARE(list.valuesFromJson(json), false);
}

void TestQtNoidAppParameterList::testValuesFromJsonWithCorrectName()
{
    QJsonObject json;
    json["MyTestPage"] = QJsonArray();
    // qDebug() << __func__ << json;

    // Test successful fromJson
    ParameterList list("MyTestPage", this);
    QCOMPARE(list.valuesFromJson(json), true);
}

void TestQtNoidAppParameterList::testSchemaFromJson()
{
    // Create JSON schema with multiple parameters
    QJsonArray parametersArray;

    // Temperature parameter schema
    QJsonObject tempSchema;
    tempSchema["description"] = "Current temperature";
    tempSchema["unit"] = "°C";
    tempSchema["readOnly"] = true;
    tempSchema["min"] = -50.0;
    tempSchema["max"] = 100.0;

    QJsonObject expectedTemp;
    expectedTemp["Temperature"] = tempSchema;
    parametersArray.append(expectedTemp);

    // Pressure parameter schema
    QJsonObject pressSchema;
    pressSchema["description"] = "Atmospheric pressure";
    pressSchema["unit"] = "hPa";
    pressSchema["readOnly"] = false;
    pressSchema["min"] = 800.0;
    pressSchema["max"] = 1100.0;
    QJsonObject expectedPress;
    expectedPress["Pressure"] = pressSchema;
    parametersArray.append(expectedPress);

    // Complete schema JSON
    QJsonObject json;
    json["TestConfiguration"] = parametersArray;

    // Test with empty-named parameter list (should take JSON name)
    ParameterList list(this);
    QVERIFY(list.schemaFromJson(json));

    // Verify list properties
    QCOMPARE(list.name(), "TestConfiguration");
    QCOMPARE(list.count(), 2);

    // Verify Temperature in list
    QJsonObject actualTemp = list.parameter("Temperature")->toJsonSchema();
    qDebug() << actualTemp;
    QCOMPARE(actualTemp, expectedTemp);

    // Verify Pressure in list
    QJsonObject actualPress = list.parameter("Pressure")->toJsonSchema();
    qDebug() << actualPress;
    QCOMPARE(actualPress, expectedPress);

}

void TestQtNoidAppParameterList::testSchemaFromJsonWithWrongNameShouldFail()
{
    // Create JSON schema with multiple parameters
    QJsonObject json;
    json["WrongName"] = QJsonArray();


    ParameterList list("", this);
    QVERIFY(list.schemaFromJson(json));




}



QTEST_MAIN(TestQtNoidAppParameterList)
#include "test_parameterlist.moc"
