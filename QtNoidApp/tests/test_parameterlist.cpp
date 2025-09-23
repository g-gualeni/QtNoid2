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
    void testAppendingParameters();
    void testAppendingDuplicatedParametersShouldFail();
    void testAppendingSameNameParametersShouldFail();
    void testAppendingNoNameParametersShouldFail();
    void testAppendingParameterFromJsonObjects();
    void testAppendingParameterFromBadJsonObjectsShouldFail();
    void testEmplaceWithNameDescriptionValue();
    void testEmplaceWithDefaultParameters();
    void testEmplaceWithJsonObjects();

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
    void testToJsonSchemaWithPresets();
    void testValuesFromJsonWithEmptyNameShouldTakeTheJsonName();
    void testValuesFromJsonWithWrongNameShouldFail();
    void testValuesFromJsonWithCorrectName();

    void testSchemaFromJson();
    void testSchemaFromJsonWithWrongNameShouldFail();
    void testSchemaFromJsonWithCorrectName();
    void testSchemaFromDuplicatedJsonOverwriteAndNotFail();
    void testConstructorWithSchemaAndValueJsonObjects();






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

void TestQtNoidAppParameterList::testAppendingParameters()
{
    ParameterList list(this);
    QSignalSpy countSpy(&list, &ParameterList::countChanged);
    QSignalSpy addedSpy(&list, &ParameterList::parameterAdded);
    
    auto param1 = new Parameter("Param1", 100.0, this);   
    list.append(param1);
    QCOMPARE(list.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);

    qDebug() << list.contains(param1);

    QCOMPARE(list.contains(param1), true);
    QCOMPARE(list.contains("Param1"), true);
    
    auto param2 = new Parameter("Param2", 200.0, this);
    list.append(param2);
    QCOMPARE(list.count(), 2);
    QCOMPARE(countSpy.count(), 2);
    QCOMPARE(addedSpy.count(), 2);
    
    // Adding same parameter again should not change anything
    list.append(param1);
    QCOMPARE(list.count(), 2);
    QCOMPARE(countSpy.count(), 2);
    QCOMPARE(addedSpy.count(), 2);
    
    // Adding nullptr should not change anything
    list.append(nullptr);
    QCOMPARE(list.count(), 2);
}

void TestQtNoidAppParameterList::testAppendingDuplicatedParametersShouldFail()
{
    ParameterList list(this);
    QSignalSpy countSpy(&list, &ParameterList::countChanged);
    QSignalSpy addedSpy(&list, &ParameterList::parameterAdded);

    auto param1 = new Parameter("Param1", 100.0, this);
    list.append(param1);

    bool res = list.append(param1);
    QCOMPARE(res, false);
    QCOMPARE(list.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);
}

void TestQtNoidAppParameterList::testAppendingSameNameParametersShouldFail()
{
    ParameterList list(this);
    QSignalSpy countSpy(&list, &ParameterList::countChanged);
    QSignalSpy addedSpy(&list, &ParameterList::parameterAdded);

    auto param1 = new Parameter("Param1", 100.0, this);
    list.append(param1);

    auto sameNameParam1= new Parameter("Param1", 123, this);
    bool res = list.append(sameNameParam1);

    // qDebug() << list.toJsonValues() << list.toJsonSchema();

    QCOMPARE(res, false);
    QCOMPARE(list.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);
}

void TestQtNoidAppParameterList::testAppendingNoNameParametersShouldFail()
{
    auto param = new Parameter(this);
    ParameterList list(this);
    bool res = list.append(param);
    QCOMPARE(res, false);
}

void TestQtNoidAppParameterList::testAppendingParameterFromJsonObjects()
{
    ParameterList list(this);
    QSignalSpy countSpy(&list, &ParameterList::countChanged);
    QSignalSpy addedSpy(&list, &ParameterList::parameterAdded);

    // Test successful addition with valid schema and value
    QJsonObject schema;
    QJsonObject tempSchema;
    tempSchema["description"] = "Temperature sensor";
    tempSchema["tooltip"] = "Temperature of the sensor ";
    tempSchema["unit"] = "°C";
    tempSchema["min"] = -50.0;
    tempSchema["max"] = 100.0;
    tempSchema["readOnly"] = true;
    schema["Temperature"] = tempSchema;

    QJsonObject value;
    value["Temperature"] = 25.5;

    bool result = list.append(schema, value);
    QCOMPARE(result, true);
    QCOMPARE(list.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);

    QCOMPARE(list.parameter(0)->toJsonSchema(), schema);
    QCOMPARE(list.parameter(0)->toJsonValue(), value);

}

void TestQtNoidAppParameterList::testAppendingParameterFromBadJsonObjectsShouldFail()
{
    ParameterList list(this);
    QJsonObject schema;
    QJsonObject value;
    bool result = list.append(schema, value);
    QCOMPARE(result, false);
}

void TestQtNoidAppParameterList::testEmplaceWithNameDescriptionValue()
{
    ParameterList list(this);
    QSignalSpy countSpy(&list, &ParameterList::countChanged);
    QSignalSpy addedSpy(&list, &ParameterList::parameterAdded);

    Parameter* param = list.emplace("Temperature", "Ambient temperature", 25.5);

    QVERIFY(param != nullptr);
    QCOMPARE(list.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);

    QCOMPARE(param->name(), "Temperature");
    QCOMPARE(param->description(), "Ambient temperature");
    QCOMPARE(param->value().toDouble(), 25.5);

    QCOMPARE(list.contains("Temperature"), true);
    QCOMPARE(list.parameter("Temperature"), param);

    Parameter* duplicate = list.emplace("Temperature", "Another temperature", 30.0);
    QCOMPARE(duplicate, nullptr);
    QCOMPARE(list.count(), 1);
}

void TestQtNoidAppParameterList::testEmplaceWithDefaultParameters()
{
    ParameterList list(this);
    QSignalSpy countSpy(&list, &ParameterList::countChanged);
    QSignalSpy addedSpy(&list, &ParameterList::parameterAdded);

    Parameter* param0 = list.emplace({});
    QVERIFY(param0 == nullptr);

    Parameter* param1 = list.emplace("Param1");
    QVERIFY(param1 != nullptr);
    QCOMPARE(param1->name(), "Param1");
    QCOMPARE(param1->description(), QString());
    QCOMPARE(param1->value(), QVariant());

    Parameter* param2 = list.emplace("Param2", "A description");
    QVERIFY(param2 != nullptr);
    QCOMPARE(param2->name(), "Param2");
    QCOMPARE(param2->description(), "A description");
    QCOMPARE(param2->value(), QVariant());

    Parameter* param3 = list.emplace("Param3", QString(), 42);
    QVERIFY(param3 != nullptr);
    QCOMPARE(param3->name(), "Param3");
    QCOMPARE(param3->description(), QString());
    QCOMPARE(param3->value().toInt(), 42);


    QCOMPARE(list.count(), 3);
    QCOMPARE(countSpy.count(), 3);
    QCOMPARE(addedSpy.count(), 3);
}

void TestQtNoidAppParameterList::testEmplaceWithJsonObjects()
{
    ParameterList list(this);
    QSignalSpy countSpy(&list, &ParameterList::countChanged);
    QSignalSpy addedSpy(&list, &ParameterList::parameterAdded);

    QJsonObject schema;
    QJsonObject schemaObject;
    schemaObject["description"] = "Temperature sensor";
    schemaObject["tooltip"] = "Temperature of the sensor ";
    schemaObject["unit"] = "°C";
    schemaObject["min"] = -50.0;
    schemaObject["max"] = 100.0;
    schemaObject["readOnly"] = true;
    schema["Temperature"] = schemaObject;

    QJsonObject value;
    value["Temperature"] = 25.5;

    Parameter* param = list.emplace(schema, value);

    QVERIFY(param != nullptr);
    QCOMPARE(list.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);

    QCOMPARE(param->name(), "Temperature");
    QCOMPARE(param->description(), "Temperature sensor");
    QCOMPARE(param->unit(), "°C");
    QCOMPARE(param->value().toDouble(), 25.5);
    QCOMPARE(param->min().toDouble(), -50.0);
    QCOMPARE(param->max().toDouble(), 100.0);
    QCOMPARE(param->readOnly(), true);

    QCOMPARE(list.contains("Temperature"), true);
    QCOMPARE(list.parameter("Temperature"), param);

    QCOMPARE(param->toJsonSchema(), schema);
    QCOMPARE(param->toJsonValue(), value);

    QJsonObject badSchema;
    QJsonObject badValue;
    Parameter* failedParam = list.emplace(badSchema, badValue);
    QCOMPARE(failedParam, nullptr);
    QCOMPARE(list.count(), 1);
}

void TestQtNoidAppParameterList::testRemovingParameters()
{
    ParameterList list(this);
    
    auto param1 = new Parameter("Param1", 100.0, this);
    auto param2 = new Parameter("Param2", 200.0, this);
    
    list.append(param1);
    list.append(param2);
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

    list.append(param1);
    list.append(param2);
    list.append(param3);
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
    
    list.append(param1);
    list.append(param2);
    list.append(param3);
    
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
    
    list.append(param1);
    list.append(param2);
    
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
    list.append(param1);
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
    page.append(param1);
    auto param2 = new Parameter("Pressure", 1013.25, this);
    page.append(param2);

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
    page.append(param1);


    auto param2 = new Parameter("Pressure", "pres" , 1013.25, this);
    param2->setRange(0,2000);
    param2->setUnit("hPa");
    param2->setReadOnly(true);
    page.append(param2);

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

void TestQtNoidAppParameterList::testToJsonSchemaWithPresets()
{
    // Create a parameter with presets
    Parameter param("Mode", "test", this);

    // Add multiple presets
    param.setPreset("Auto", "auto");
    param.setPreset("Manual", "manual");
    param.setPreset("Eco", "eco");
    param.setPreset("Performance", "performance");

    // Get the JSON schema
    QJsonObject schema = param.toJsonSchema();

    // Verify schema structure
    QVERIFY(schema.contains("Mode"));
    QJsonObject modeSchema = schema["Mode"].toObject();

    // Verify presets are included
    QVERIFY(modeSchema.contains("presets"));
    const QJsonArray presetsArray = modeSchema["presets"].toArray();
    QCOMPARE(presetsArray.size(), 4);

    // Verify preset contents
    QSet<QString> expectedPresetNames = {"Auto", "Manual", "Eco", "Performance"};
    QSet<QString> actualPresetNames;

    for (const QJsonValue& presetValue : presetsArray) {
        const QJsonObject presetObj = presetValue.toObject();
        QCOMPARE(presetObj.keys().size(), 1);

        QStringList presetNameList = presetObj.keys();
        QString presetName = presetNameList.first();
        actualPresetNames.insert(presetName);

        // Verify specific preset values
        if (presetName == "Auto") {
            QCOMPARE(presetObj["Auto"].toString(), "auto");
        } else if (presetName == "Manual") {
            QCOMPARE(presetObj["Manual"].toString(), "manual");
        } else if (presetName == "Eco") {
            QCOMPARE(presetObj["Eco"].toString(), "eco");
        } else if (presetName == "Performance") {
            QCOMPARE(presetObj["Performance"].toString(), "performance");
        }
    }

    // Verify all expected presets are present
    QCOMPARE(actualPresetNames, expectedPresetNames);
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
    // Create JSON schema with multiple parameters and use it
    // to inizialize ParameterList list(this);

    QJsonArray parametersArray;

    // Temperature parameter schema
    QJsonObject tempSchema;
    tempSchema["description"] = "Current temperature";
    tempSchema["tooltip"] = "Temperature of the sensor ";
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
    pressSchema["tooltip"] = "This is the atmospheric pressure";
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
    QJsonObject json;
    json["WrongName"] = QJsonArray();
    // qDebug() << __func__ << json;

    // If there is no name if is ok
    ParameterList list2(this);
    QVERIFY(list2.schemaFromJson(json));
    QCOMPARE(list2.name(), "WrongName");

    // IF there is a name schemaFromJson() fails
    ParameterList list("MyPage", this);
    QCOMPARE(list.schemaFromJson(json), false);
}

void TestQtNoidAppParameterList::testSchemaFromJsonWithCorrectName()
{
    QJsonObject json;
    json["CorrectName"] = QJsonArray();
    // qDebug() << __func__ << json;

    ParameterList list("CorrectName", this);
    QVERIFY(list.schemaFromJson(json));
}

void TestQtNoidAppParameterList::testSchemaFromDuplicatedJsonOverwriteAndNotFail()
{
    QJsonObject schemaDefinition;
    schemaDefinition["min"] = 800.0;
    schemaDefinition["max"] = 1100.0;
    QJsonObject schemaItem;
    schemaItem["Pressure"] = schemaDefinition;
    QJsonArray schemaList;
    schemaList.append(schemaItem);
    QJsonObject schema;
    schema["LIST"] = schemaList;

    qDebug() << __func__ << schema;

    ParameterList list(this);
    QCOMPARE(list.schemaFromJson(schema), true);
    QCOMPARE(list.schemaFromJson(schema), true);
    QCOMPARE(list.count(), 1);

}

void TestQtNoidAppParameterList::testConstructorWithSchemaAndValueJsonObjects()
{
    // Create schema JSON with two parameters
    QJsonArray schemaArray;

    // Temperature parameter schema
    QJsonObject tempSchema;
    tempSchema["description"] = "Current temperature";
    tempSchema["tooltip"] = "Temperature of the sensor ";
    tempSchema["unit"] = "°C";
    tempSchema["readOnly"] = true;  // it should be possible initialize a read only!
    tempSchema["min"] = -50.0;
    tempSchema["max"] = 100.0;
    QJsonObject tempSchemaWrapper;
    tempSchemaWrapper["Temperature"] = tempSchema;
    schemaArray.append(tempSchemaWrapper);

    // Pressure parameter schema
    QJsonObject pressSchema;
    pressSchema["description"] = "Atmospheric pressure";
    pressSchema["tooltip"] = "This is the atmospheric pressure";
    pressSchema["unit"] = "hPa";
    pressSchema["readOnly"] = false;
    pressSchema["min"] = 800.0;
    pressSchema["max"] = 1100.0;
    QJsonObject pressSchemaWrapper;
    pressSchemaWrapper["Pressure"] = pressSchema;
    schemaArray.append(pressSchemaWrapper);

    QJsonObject schemaList;
    schemaList["SensorConfiguration"] = schemaArray;

    // Create values JSON
    QJsonArray valuesArray;
    QJsonObject tempValue;
    tempValue["Temperature"] = 25.5;
    valuesArray.append(tempValue);

    QJsonObject pressValue;
    pressValue["Pressure"] = 1013.25;
    valuesArray.append(pressValue);

    QJsonObject valueList;
    valueList["SensorConfiguration"] = valuesArray;

    // Test constructor
    ParameterList list(schemaList, valueList, this);

    // Verify the list was created correctly
    QCOMPARE(list.name(), "SensorConfiguration");
    QCOMPARE(list.count(), 2);

    // Verify Temperature parameter
    Parameter* tempParam = list.parameter("Temperature");
    QVERIFY(tempParam != nullptr);
    QCOMPARE(tempParam->value().toDouble(), 25.5);
    QCOMPARE(tempParam->description(), "Current temperature");
    QCOMPARE(tempParam->unit(), "°C");
    QCOMPARE(tempParam->readOnly(), true);
    QCOMPARE(tempParam->min().toDouble(), -50.0);
    QCOMPARE(tempParam->max().toDouble(), 100.0);

    // Verify Pressure parameter
    Parameter* pressParam = list.parameter("Pressure");
    QVERIFY(pressParam != nullptr);
    QCOMPARE(pressParam->value().toDouble(), 1013.25);
    QCOMPARE(pressParam->description(), "Atmospheric pressure");
    QCOMPARE(pressParam->unit(), "hPa");
    QCOMPARE(pressParam->readOnly(), false);
    QCOMPARE(pressParam->min().toDouble(), 800.0);
    QCOMPARE(pressParam->max().toDouble(), 1100.0);

    // Verify JSON serialization matches input
    QCOMPARE(list.toJsonSchema(), schemaList);
    QCOMPARE(list.toJsonValues(), valueList);
}






QTEST_MAIN(TestQtNoidAppParameterList)
#include "test_parameterlist.moc"
