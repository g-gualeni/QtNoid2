#include <QSignalSpy>
#include <QTest>
#include <QJsonObject>
#include <QJsonArray>
#include <QtNoidApp/QtNoidApp>

using namespace QtNoid::App;

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
    void testAppendingParameterFirstElementInTheListShouldBeNumber0();
    void testEmplaceWithNameDescriptionValue();
    void testEmplaceWithDefaultParameters();
    void testEmplaceWithJsonObjects();

    void testRemovingParameters();
    void testParameterListClear();
    void testParameterAccess();
    void testParameterListSetValueConvenienceMethods();
    void testParameterDestruction();
    void testBindableNameProperty();
    void testParameterListDescription();
    void testBindableDescriptionProperty();
    void testParameterListTooltip();
    void testBindableTooltipProperty();

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

    void testChangingParamterNameShouldUpdateTheParameterList();
    void testParameterRenameError();
    void testIsEmpty();
    void testApplyPreset();
    void testOperatorLeftShiftWithParameterReference();

    void testApplyJsonValuesToExistingParameterListShouldUpdateValues();
    void testApplyJsonSchemaToExistingParameterListShouldUpdateSchema();

    void testListOwnershipDeleteListDestroyParameters();
};



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
    QCOMPARE(list.description(), QString());
    QCOMPARE(list.tooltip(), QString());

    ParameterList namedList("Application Configuration", this);
    QCOMPARE(namedList.count(), 0);
    QCOMPARE(namedList.name(), "Application Configuration");
    QCOMPARE(namedList.description(), QString());
    QCOMPARE(namedList.tooltip(), QString());
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
    
    auto param1 = new Parameter(100.0, "Param1", this);
    list.append(param1);
    QCOMPARE(list.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);

    QCOMPARE(list.contains(param1), true);
    QCOMPARE(list.contains("Param1"), true);
    
    auto param2 = new Parameter(200.0, "Param2", this);
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

    auto param1 = new Parameter(100.0, "Param1", this);
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

    auto param1 = new Parameter(100.0, "Param1", this);
    list.append(param1);

    auto sameNameParam1= new Parameter(123, "Param1", this);
    bool res = list.append(sameNameParam1);

    // qDebug() << list.toJsonValues() << list.toJsonSchema();

    QCOMPARE(res, false);
    QCOMPARE(list.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);

    // Try changing paramter1 name should fail anyway
    param1->setName("Param1-Rename");
    res = list.append(param1);
    QCOMPARE(res, false);
    QCOMPARE(list.count(), 1);
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
    tempSchema["visible"] = true;
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

void TestQtNoidAppParameterList::testAppendingParameterFirstElementInTheListShouldBeNumber0()
{
    ParameterList list(this);
    auto param0 = new Parameter(100.0, "Param0", this);
    list.append(param0);
    QCOMPARE(list.parameter(0), param0);
}

void TestQtNoidAppParameterList::testEmplaceWithNameDescriptionValue()
{
    ParameterList list(this);
    QSignalSpy countSpy(&list, &ParameterList::countChanged);
    QSignalSpy addedSpy(&list, &ParameterList::parameterAdded);

    Parameter* param = list.emplace(25.5, "Temperature", "Ambient temperature");

    QVERIFY(param != nullptr);
    QCOMPARE(list.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);

    QCOMPARE(param->name(), "Temperature");
    QCOMPARE(param->description(), "Ambient temperature");
    QCOMPARE(param->value().toDouble(), 25.5);

    QCOMPARE(list.contains("Temperature"), true);
    QCOMPARE(list.parameter("Temperature"), param);

    Parameter* duplicate = list.emplace(30.0, "Temperature", "Another temperature");
    QCOMPARE(duplicate, nullptr);
    QCOMPARE(list.count(), 1);
}

void TestQtNoidAppParameterList::testEmplaceWithDefaultParameters()
{
    ParameterList list(this);
    QSignalSpy countSpy(&list, &ParameterList::countChanged);
    QSignalSpy addedSpy(&list, &ParameterList::parameterAdded);

    Parameter* param0 = list.emplace(0.0, {}, {});
    QVERIFY(param0 == nullptr);

    Parameter* param1 = list.emplace({}, "Param1");
    QVERIFY(param1 != nullptr);
    QCOMPARE(param1->name(), "Param1");
    QCOMPARE(param1->description(), QString());
    QCOMPARE(param1->value(), QVariant());

    Parameter* param2 = list.emplace({}, "Param2", "A description");
    QVERIFY(param2 != nullptr);
    QCOMPARE(param2->name(), "Param2");
    QCOMPARE(param2->description(), "A description");
    QCOMPARE(param2->value(), QVariant());

    Parameter* param3 = list.emplace( 42, "Param3", QString());
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
    schemaObject["visible"] = true;
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
    
    auto param1 = new Parameter(100.0, "Param1", this);
    auto param2 = new Parameter(200.0, "Param2", this);
    
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

    auto param1 = new Parameter(100.0, "Param1", this);
    auto param2 = new Parameter(200.0, "Param2", this);
    auto param3 = new Parameter(300.0, "Param3", this);

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
    auto param1 = new Parameter(25.0, "Temperature", this);
    auto param2 = new Parameter(1013.25, "Pressure", this);
    auto param3 = new Parameter( 60.0, "Humidity",this);
    
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
    auto param1 = new Parameter(25.0, "Temperature", this);
    auto param2 = new Parameter(1013.25, "Pressure", this);
    
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
    
    auto param1 = new Parameter(100.0, "Param1", this);
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
    auto param1 = new Parameter(25.5, "Temperature", this);
    page.append(param1);
    auto param2 = new Parameter(1013.25, "Pressure", this);
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
    auto param1 = new Parameter(25.5, "Temperature", "temp", this);
    param1->setRange(0,100);
    param1->setUnit("°C");
    param1->setReadOnly(true);
    page.append(param1);


    auto param2 = new Parameter(1013.25, "Pressure", "pres" , this);
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
    Parameter param("test value", "Mode", this);

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
    tempSchema["visible"] = true;
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
    pressSchema["visible"] = true;
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
    QCOMPARE(actualTemp, expectedTemp);

    // Verify Pressure in list
    QJsonObject actualPress = list.parameter("Pressure")->toJsonSchema();
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
    tempSchema["visible"] = true;
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
    pressSchema["visible"] = true;
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

void TestQtNoidAppParameterList::testChangingParamterNameShouldUpdateTheParameterList()
{
    Parameter par(123, "OriginalName", this);
    QCOMPARE(par.name(), "OriginalName");
    ParameterList list;
    list.append(&par);
    QVERIFY(list.contains("OriginalName"));

    par.setName("NewName");
    QVERIFY(list.contains("NewName"));
}

void TestQtNoidAppParameterList::testParameterListDescription()
{
    ParameterList list(this);
    QSignalSpy spy(&list, &ParameterList::descriptionChanged);

    // Initial description should be empty
    QCOMPARE(list.description(), QString());

    // Set description and verify signal emission
    list.setDescription("Configuration parameters for the application");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(list.description(), "Configuration parameters for the application");

    // Check signal argument
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "Configuration parameters for the application");

    // Same value, no signal should be emitted
    list.setDescription("Configuration parameters for the application");
    QCOMPARE(spy.count(), 0);

    // Change to different value
    list.setDescription("Updated description");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(list.description(), "Updated description");

    // Clear description
    list.setDescription("");
    QCOMPARE(spy.count(), 2);
    QCOMPARE(list.description(), QString());
}

void TestQtNoidAppParameterList::testBindableDescriptionProperty()
{
    // Create parameter list with initial description
    ParameterList list(this);
    list.setDescription("Initial description");

    // Get the bindable property
    auto bindableDescription = list.bindableDescription();
    QVERIFY(bindableDescription.isValid());
    QCOMPARE(bindableDescription.value(), "Initial description");

    // Test binding to another property
    QProperty<QString> externalProperty;
    externalProperty.setBinding([&]() { return bindableDescription.value(); });
    QCOMPARE(externalProperty.value(), "Initial description");

    // Change parameter description and verify binding updates
    list.setDescription("Updated description");
    QCOMPARE(externalProperty.value(), "Updated description");

    // Test setting description through bindable
    bindableDescription.setValue("Final description");
    QCOMPARE(list.description(), "Final description");
    QCOMPARE(externalProperty.value(), "Final description");

    // Create a reverse binding from externalProperty to list
    QSignalSpy spy(&list, &ParameterList::descriptionChanged);
    bindableDescription.setBinding([&]() { return externalProperty.value(); });
    externalProperty.setValue("Externally set description");
    QCOMPARE(bindableDescription.value(), "Externally set description");
    QCOMPARE(list.description(), "Externally set description");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().toString(), "Externally set description");
}

void TestQtNoidAppParameterList::testParameterListTooltip()
{
    ParameterList list(this);
    QSignalSpy spy(&list, &ParameterList::tooltipChanged);

    // Initial tooltip should be empty
    QCOMPARE(list.tooltip(), QString());

    // Set tooltip and verify signal emission
    list.setTooltip("This is a helpful tooltip for the parameter list");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(list.tooltip(), "This is a helpful tooltip for the parameter list");

    // Check signal argument
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "This is a helpful tooltip for the parameter list");

    // Same value, no signal should be emitted
    list.setTooltip("This is a helpful tooltip for the parameter list");
    QCOMPARE(spy.count(), 0);

    // Change to different value
    list.setTooltip("Updated tooltip text");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(list.tooltip(), "Updated tooltip text");

    // Clear tooltip
    list.setTooltip("");
    QCOMPARE(spy.count(), 2);
    QCOMPARE(list.tooltip(), QString());
}

void TestQtNoidAppParameterList::testBindableTooltipProperty()
{
    // Create parameter list with initial tooltip
    ParameterList list(this);
    list.setTooltip("Initial tooltip");

    // Get the bindable property
    auto bindableTooltip = list.bindableTooltip();
    QVERIFY(bindableTooltip.isValid());
    QCOMPARE(bindableTooltip.value(), "Initial tooltip");

    // Test binding to another property
    QProperty<QString> externalProperty;
    externalProperty.setBinding([&]() { return bindableTooltip.value(); });
    QCOMPARE(externalProperty.value(), "Initial tooltip");

    // Change parameter tooltip and verify binding updates
    list.setTooltip("Updated tooltip");
    QCOMPARE(externalProperty.value(), "Updated tooltip");

    // Test setting tooltip through bindable
    bindableTooltip.setValue("Final tooltip");
    QCOMPARE(list.tooltip(), "Final tooltip");
    QCOMPARE(externalProperty.value(), "Final tooltip");

    // Create a reverse binding from externalProperty to list
    QSignalSpy spy(&list, &ParameterList::tooltipChanged);
    bindableTooltip.setBinding([&]() { return externalProperty.value(); });
    externalProperty.setValue("Externally set tooltip");
    QCOMPARE(bindableTooltip.value(), "Externally set tooltip");
    QCOMPARE(list.tooltip(), "Externally set tooltip");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().toString(), "Externally set tooltip");
}

void TestQtNoidAppParameterList::testParameterRenameError()
{
    ParameterList *list = new ParameterList("MyList", this);
    QSignalSpy renameErrorSpy(list, &ParameterList::parameterRenameError);

    // Create two parameters with different names
    Parameter *param1 = new Parameter(100.0, "Param1", this);
    Parameter *param2 = new Parameter(200.0, "Param2", this);

    list->append(param1);
    list->append(param2);

    // Test case 1: Try to rename to an existing name (should emit error)
    param1->setName("Param2");

    QCOMPARE(renameErrorSpy.count(), 1);
    QList<QVariant> arguments = renameErrorSpy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "Param1");  // oldName
    QCOMPARE(arguments.at(1).toString(), "Param2");  // newName

    // Verify the parameter name wasn't actually changed in the list
    QVERIFY(list->contains("Param1"));
    QVERIFY(list->contains("Param2"));
    QCOMPARE(list->parameter("Param2"), param2);

    // Test case 2: Create a scenario where oldName is not in the list
    // (This is harder to trigger naturally, but we can simulate it)
    // by removing a parameter from the list but keeping the object alive
    list->removeParameter(param1);

    // Now if param1 tries to rename itself, it should not trigger an error
    // because "Param1" is no longer in the list
    param1->setName("Param2");
    QCOMPARE(renameErrorSpy.count(), 0);

    delete param1;
    delete param2;
    delete list;
}

void TestQtNoidAppParameterList::testIsEmpty()
{
    ParameterList list(this);

    // Test empty list
    QVERIFY(list.isEmpty());
    QCOMPARE(list.count(), 0);

    // Add a parameter
    Parameter* param1 = new Parameter(100.0, "Param1", this);
    list.append(param1);

    // Test non-empty list
    QVERIFY(!list.isEmpty());
    QCOMPARE(list.count(), 1);

    // Remove one parameter
    list.removeParameter(param1);
    QVERIFY(list.isEmpty());

    // Test clear() method
    Parameter* param3 = new Parameter(300.0, "Param3", this);
    Parameter* param4 = new Parameter(400.0, "Param4", this);
    list.append(param3);
    list.append(param4);

    QVERIFY(!list.isEmpty());
    QCOMPARE(list.count(), 2);

    list.clear();
    QVERIFY(list.isEmpty());
    QCOMPARE(list.count(), 0);
}

void TestQtNoidAppParameterList::testApplyPreset()
{
    ParameterList list(this);

    // Create parameters with presets
    Parameter param1(50.0, "Temperature", this);
    param1.setPreset("Low", 10.0);
    param1.setPreset("High", 80.0);

    Parameter param2(1000.0, "Pressure", this);
    param2.setPreset("Low", 900.0);
    param2.setPreset("High", 1100.0);

    Parameter param3(30.0, "Humidity", this);
    param3.setPreset("Low", 20.0);
    param3.setPreset("High", 90.0);

    list.append(&param1);
    list.append(&param2);
    list.append(&param3);

    // Test applying "Low" preset - all parameter should change
    QSignalSpy spy1(&param1, &Parameter::valueChanged);
    QSignalSpy spy2(&param2, &Parameter::valueChanged);
    QSignalSpy spy3(&param3, &Parameter::valueChanged);

    list.applyPreset("Low");

    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 1);
    QCOMPARE(spy3.count(), 1);
    QCOMPARE(param1.value().toDouble(), 10.0);
    QCOMPARE(param2.value().toDouble(), 900.0);
    QCOMPARE(param3.value().toDouble(), 20.0);

    // Check all valueChangedSignals
    auto currentRes = spy1.takeFirst();
    double current = currentRes.first().toDouble();
    QCOMPARE(current, 10);

    currentRes = spy2.takeFirst();
    current = currentRes.first().toDouble();
    QCOMPARE(current, 900);

    currentRes = spy3.takeFirst();
    current = currentRes.first().toDouble();
    QCOMPARE(current, 20);

    // Test applying "High" preset
    list.applyPreset("High");
    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 1);
    QCOMPARE(spy3.count(), 1);

    // Test applying non-existent preset - no parameters should change
    spy1.clear();
    spy2.clear();
    spy3.clear();
    list.applyPreset("NonExistent");
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    // all should use High preset unchanged
    QCOMPARE(param1.value().toDouble(), 80.0);
    QCOMPARE(param2.value().toDouble(), 1100.0);
    QCOMPARE(param3.value().toDouble(), 90.0);

    // Test empty list
    ParameterList emptyList(this);
    emptyList.applyPreset("AnyPreset"); // Should not crash
}

void TestQtNoidAppParameterList::testOperatorLeftShiftWithParameterReference()
{
    ParameterList list(this);
    QSignalSpy countSpy(&list, &ParameterList::countChanged);
    QSignalSpy addedSpy(&list, &ParameterList::parameterAdded);

    // Create parameters to add
    Parameter param1(100.0, "Temperature", this);
    Parameter param2(1013.25, "Pressure", this);
    Parameter param3(60.0, "Humidity", this);

    // Test chaining operator<< with parameter references
    list << param1 << param2 << param3;

    // Verify all parameters were added
    QCOMPARE(list.count(), 3);
    QCOMPARE(countSpy.count(), 3);
    QCOMPARE(addedSpy.count(), 3);

    // Verify parameters are accessible
    QCOMPARE(list.contains("Temperature"), true);
    QCOMPARE(list.contains("Pressure"), true);
    QCOMPARE(list.contains("Humidity"), true);
    QCOMPARE(list.parameter("Temperature"), &param1);
    QCOMPARE(list.parameter("Pressure"), &param2);
    QCOMPARE(list.parameter("Humidity"), &param3);

    // Verify parameter values
    QCOMPARE(list.value("Temperature").toDouble(), 100.0);
    QCOMPARE(list.value("Pressure").toDouble(), 1013.25);
    QCOMPARE(list.value("Humidity").toDouble(), 60.0);

    // Test adding duplicate parameter with operator<< (should not add)
    int originalCount = list.count();
    list << param1;
    QCOMPARE(list.count(), originalCount);
    QCOMPARE(countSpy.count(), 3); // No new count change signal

    // Test return value allows chaining
    ParameterList& result = (list << param1);
    QCOMPARE(&result, &list);

    // Verify I can change paramter and see the changed value
    param1.setValue(99);
    QCOMPARE(list.value("Temperature"), 99);
    list.setValue("Pressure", 1000);
    QCOMPARE(param2.value(), 1000);
}

void TestQtNoidAppParameterList::testListOwnershipDeleteListDestroyParameters()
{
    // Create 2 objects and verify they are destroyed with the list
    ParameterList *list = new ParameterList("MyBeautifullList");

    Parameter *parameter1 = new Parameter(1, "Paramter1", list);
    QSignalSpy parameter1Spy(parameter1, &QObject::destroyed);

    Parameter *parameter2 = new Parameter(2, "Paramter2", list);
    QSignalSpy parameter2Spy(parameter2, &QObject::destroyed);
    *list << parameter1 << parameter2;

    delete list;
    QCOMPARE(parameter1Spy.count(), 1);
    QCOMPARE(parameter2Spy.count(), 1);
}

void TestQtNoidAppParameterList::testApplyJsonValuesToExistingParameterListShouldUpdateValues()
{
    // Create an existing ParameterList with some parameters
    ParameterList list("SensorConfig", this);
    list.emplace(20.0, "Temperature");
    list.emplace(1000.0, "Pressure");

    QCOMPARE(list.count(), 2);
    QCOMPARE(list.value("Temperature").toDouble(), 20.0);
    QCOMPARE(list.value("Pressure").toDouble(), 1000.0);

    // Create JSON values to apply to the existing list
    QJsonArray valuesArray;
    QJsonObject tempValue;
    tempValue["Temperature"] = 25.5;
    valuesArray.append(tempValue);

    QJsonObject pressValue;
    pressValue["Pressure"] = 1013.25;
    valuesArray.append(pressValue);

    // Add a new parameter via JSON
    QJsonObject humidityValue;
    humidityValue["Humidity"] = 60.0;
    valuesArray.append(humidityValue);

    QJsonObject jsonValues;
    jsonValues["SensorConfig"] = valuesArray;

    QSignalSpy countSpy(&list, &ParameterList::countChanged);
    QSignalSpy addedSpy(&list, &ParameterList::parameterAdded);

    // Apply JSON values to existing list
    bool result = list.valuesFromJson(jsonValues);

    // Verify the operation succeeded
    QVERIFY(result);

    // Verify existing parameters were updated
    QCOMPARE(list.value("Temperature"), 25.5);
    QCOMPARE(list.value("Pressure"), 1013.25);

    // Verify new parameter was added
    QCOMPARE(list.count(), 3);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);
    QCOMPARE(list.value("Humidity"), 60.0);
}

void TestQtNoidAppParameterList::testApplyJsonSchemaToExistingParameterListShouldUpdateSchema()
{
    // Create an existing ParameterList with some parameters
    ParameterList list("DeviceConfig", this);
    list.emplace(50.0, "Temperature", "Description");
    QCOMPARE(list.count(), 1);

    // Create JSON schema to apply to the existing list
    QJsonArray schemaArray;

    // Temperature parameter schema
    QJsonObject temperatureObject;
    temperatureObject["description"] = "Temperature sensor reading";
    temperatureObject["tooltip"] = "Current ambient temperature";
    temperatureObject["unit"] = "°C";
    temperatureObject["min"] = -40.0;
    temperatureObject["max"] = 85.0;
    temperatureObject["readOnly"] = true;
    temperatureObject["visible"] = true;

    QJsonObject temperatureSchema;
    temperatureSchema["Temperature"] = temperatureObject;
    schemaArray.append(temperatureSchema);

    // Pressure parameter schema
    QJsonObject pressureObject;
    pressureObject["description"] = "Atmospheric pressure";
    pressureObject["tooltip"] = "Current atmospheric pressure reading";
    pressureObject["unit"] = "hPa";
    pressureObject["min"] = 800.0;
    pressureObject["max"] = 1200.0;
    pressureObject["readOnly"] = false;
    pressureObject["visible"] = true;

    QJsonObject pressureSchema;
    pressureSchema["Pressure"] = pressureObject;
    schemaArray.append(pressureSchema);

    QJsonObject jsonSchema;
    jsonSchema["DeviceConfig"] = schemaArray;

    QSignalSpy countSpy(&list, &ParameterList::countChanged);
    QSignalSpy addedSpy(&list, &ParameterList::parameterAdded);

    // Apply JSON schema to existing list
    bool result = list.schemaFromJson(jsonSchema);

    // Verify the operation succeeded
    QVERIFY(result);

    // Verify new parameters were added from schema
    QCOMPARE(list.count(), 2);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);

    // Verify Temperature parameter was updated from schema
    Parameter* tempParam = list.parameter("Temperature");
    QVERIFY(tempParam != nullptr);
    QCOMPARE(tempParam->description(), "Temperature sensor reading");
    QCOMPARE(tempParam->tooltip(), "Current ambient temperature");
    QCOMPARE(tempParam->unit(), "°C");
    QCOMPARE(tempParam->min().toDouble(), -40.0);
    QCOMPARE(tempParam->max().toDouble(), 85.0);
    QCOMPARE(tempParam->readOnly(), true);
    QCOMPARE(tempParam->visible(), true);

    // Verify Pressure parameter was created correctly from schema
    Parameter* pressParam = list.parameter("Pressure");
    QVERIFY(pressParam != nullptr);
    QCOMPARE(pressParam->description(), "Atmospheric pressure");
    QCOMPARE(pressParam->tooltip(), "Current atmospheric pressure reading");
    QCOMPARE(pressParam->unit(), "hPa");
    QCOMPARE(pressParam->min().toDouble(), 800.0);
    QCOMPARE(pressParam->max().toDouble(), 1200.0);
    QCOMPARE(pressParam->readOnly(), false);
    QCOMPARE(pressParam->visible(), true);
}


QTEST_MAIN(TestQtNoidAppParameterList)
#include "test_parameterlist.moc"
