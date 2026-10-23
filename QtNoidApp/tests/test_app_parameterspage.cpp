#include <QSignalSpy>
#include <QTest>
#include <QJsonObject>
#include <QJsonArray>
#include <QtNoidApp/QtNoidApp>

using namespace QtNoid::App;

class TestQtNoidAppParametersPage : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testParamtersPageUniqueId();
    void testCreatingParametersPage();
    void testParametersPageName();
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
    void testRemoveParameterByNameAtIndexZeroShouldUpdateCountAndIndex();
    void testParametersPageClear();
    void testEmplaceAfterRemoveParameterShouldAddAtIndexZero();
    void testAppendAfterRemoveParameterShouldAddAtIndexZero();

    void testUsingDeleteDirectlyOnTheParameterPointer();

    void testParameterAccess();

    void testParametersPageSetValueConvenienceMethods();
    void testParametersPageSetValueWhileReadOnlyShouldFail();

    void testParameterDestruction();

    void testBindableNameProperty();

    void testParametersPageLabel();
    void testBindableLabelProperty();

    void testParametersPageDescription();
    void testBindableDescriptionProperty();

    void testParametersPageTooltip();
    void testBindableTooltipProperty();

    void testParametersPageVisible();
    void testBindableParametersPageVisible();
    void testParametersPageBindableCountProperty();

    void testParametersPageReadOnly();
    void testBindableParametersPageReadOnly();

    void testParametersPageIsValueModifiedChanged();
    void testBindableIsValueModifiedProperty();
    void testParametersPageResetValueModifications();
    void testParametersPageResetValueModificationsDoesNotAffectSchema();

    // setValueModified() tests
    void testParametersPageSetValueModifiedCascadesTrueToAllParameters();
    void testParametersPageSetValueModifiedFalseCapturesCurrentStateAsNewBaselineForAllParameters();
    void testParametersPageSetValueModifiedOverrideClearedByResetValueModifications();
    void testParametersPageSetValueModifiedWithMixedParameterStates();

    void testParametersPageIsSchemaModified();
    void testBindableIsSchemaModifiedProperty();
    void testParametersPageResetSchemaModifications();
    void testParametersPageResetSchemaModificationsDoesNotAffectValues();

    // setSchemaModified() tests
    void testParametersPageSetSchemaModifiedForcesTrueRegardlessOfActualChanges();
    void testParametersPageSetSchemaModifiedFalseCapturesCurrentStateAsNewBaseline();
    void testParametersPageSetSchemaModifiedOverrideClearedByResetSchemaModifications();
    void testParametersPageSetSchemaModifiedInteractionWithActualSchemaChanges();
    void testParametersPageSetSchemaModifiedDoesNotCascadeToParameters();

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

    void testParametersPageLabelShouldSurviveJsonRoundTrip();
    void testParametersPageReadOnlyShouldSurviveJsonRoundTrip();


    void testChangingParamterNameShouldUpdateTheParametersPage();
    void testParameterRenameError();
    void testIsEmpty();
    void testApplyPreset();
    void testApplyPresetShouldBypassReadOnly();

    void testOperatorLeftShiftWithParameterReference();

    void testApplyJsonValuesToExistingParametersPageShouldUpdateValues();
    void testApplyJsonSchemaToExistingParametersPageShouldUpdateSchema();
    void testDeletingParamtersPageDestroyOwnedParamters();

    // Iterator tests
    void testParametersPageBeginAndEndAndRangeLoop();
    void testParametersPageConstIteratorsAndConstRangeLoop();
    void testParametersPageReverseIteratorsAndRangeLoop();
    void testParametersPageConstReverseIterators();

    // QDebug operator<< tests
    void testQDebugOperator();
    void testParametersPageWithQDebugOperatorWithPointer();


    // Testing aboutToBeDestroyed
    void testAboutToBeDestroyedSignal();
};



void TestQtNoidAppParametersPage::initTestCase()
{}

void TestQtNoidAppParametersPage::cleanupTestCase()
{}

void TestQtNoidAppParametersPage::init()
{}

void TestQtNoidAppParametersPage::cleanup()
{}

void TestQtNoidAppParametersPage::testParamtersPageUniqueId()
{
    // Test that each ParametersPage instance gets a unique ID
    ParametersPage page1(this);
    ParametersPage page2(QJsonObject(), QJsonObject(), this);
    ParametersPage page3("NamedList", this);

    int id1 = page1.uniqueId();
    int id2 = page2.uniqueId();
    int id3 = page3.uniqueId();

    // Verify that IDs are different
    QVERIFY(id1 != id2);
    QVERIFY(id2 != id3);
    QVERIFY(id1 != id3);

    // Verify that ID doesn't change for the same instance
    QCOMPARE(page1.uniqueId(), id1);
    QCOMPARE(page2.uniqueId(), id2);
    QCOMPARE(page3.uniqueId(), id3);

    // Test that IDs are sequential (based on the implementation)
    QCOMPARE(id2, id1 + 1);
    QCOMPARE(id3, id2 + 1);

    // Test with constructor that takes schema and value JSON objects
    QJsonArray schemaArray;
    QJsonObject tempSchema;
    tempSchema["description"] = "Temperature";
    tempSchema["tooltip"] = "Temperature of the sensor ";
    QJsonObject tempSchemaWrapper;
    tempSchemaWrapper["Temperature"] = tempSchema;
    schemaArray.append(tempSchemaWrapper);

    QJsonObject schemaList;
    schemaList["TestConfig"] = schemaArray;

    QJsonArray valuesArray;
    QJsonObject tempValue;
    tempValue["Temperature"] = 25.5;
    valuesArray.append(tempValue);

    QJsonObject valueList;
    valueList["TestConfig"] = valuesArray;

    ParametersPage page4(schemaList, valueList, this);
    int id4 = page4.uniqueId();

    // Verify that this constructor also assigns a unique ID
    QVERIFY(id4 != id1);
    QVERIFY(id4 != id2);
    QVERIFY(id4 != id3);
    QCOMPARE(id4, id3 + 1);
}

void TestQtNoidAppParametersPage::testCreatingParametersPage()
{
    ParametersPage page(this);
    QCOMPARE(page.count(), 0);
    QCOMPARE(page.name(), QString());
    QCOMPARE(page.description(), QString());
    QCOMPARE(page.tooltip(), QString());

    ParametersPage namedPage("Application Configuration", this);
    QCOMPARE(namedPage.count(), 0);
    QCOMPARE(namedPage.name(), "Application Configuration");
    QCOMPARE(namedPage.description(), QString());
    QCOMPARE(namedPage.tooltip(), QString());
}

void TestQtNoidAppParametersPage::testParametersPageName()
{
    ParametersPage page("MyPage", this);
    QSignalSpy spyChanged(&page, &ParametersPage::nameChanged);
    QSignalSpy spyEdited(&page, &ParametersPage::nameEdited);
    
    page.setName("Application Configuration");
    QCOMPARE(spyChanged.count(), 1);
    QCOMPARE(spyEdited.count(), 1);
    QCOMPARE(page.name(), "Application Configuration");
    
    // Check signal argument
    auto arguments = spyChanged.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "Application Configuration");
    arguments = spyEdited.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "MyPage");
    QCOMPARE(arguments.at(1).toString(), "Application Configuration");

    // Same value, no signal
    page.setName("Application Configuration");
    QCOMPARE(spyChanged.count(), 0);
    QCOMPARE(spyEdited.count(), 0);
}

void TestQtNoidAppParametersPage::testAppendingParameters()
{
    ParametersPage page(this);
    QSignalSpy countSpy(&page, &ParametersPage::countChanged);
    QSignalSpy addedSpy(&page, &ParametersPage::parameterAdded);
    
    auto param1 = new Parameter(100.0, "Param1", this);
    page << param1;
    QCOMPARE(page.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);

    QCOMPARE(page.contains(param1), true);
    QCOMPARE(page.contains("Param1"), true);
    
    auto param2 = new Parameter(200.0, "Param2", this);
    page << param2;
    QCOMPARE(page.count(), 2);
    QCOMPARE(countSpy.count(), 2);
    QCOMPARE(addedSpy.count(), 2);
    
    // Adding same parameter again should not change anything
    page << param1;
    QCOMPARE(page.count(), 2);
    QCOMPARE(countSpy.count(), 2);
    QCOMPARE(addedSpy.count(), 2);
    
    // Adding nullptr should not change anything
    page.append(nullptr);
    QCOMPARE(page.count(), 2);
}

void TestQtNoidAppParametersPage::testAppendingDuplicatedParametersShouldFail()
{
    ParametersPage page(this);
    QSignalSpy countSpy(&page, &ParametersPage::countChanged);
    QSignalSpy addedSpy(&page, &ParametersPage::parameterAdded);

    auto param1 = new Parameter(100.0, "Param1", this);
    page.append(param1);

    bool res = page.append(param1);
    QCOMPARE(res, false);
    QCOMPARE(page.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);
}

void TestQtNoidAppParametersPage::testAppendingSameNameParametersShouldFail()
{
    ParametersPage page(this);
    QSignalSpy countSpy(&page, &ParametersPage::countChanged);
    QSignalSpy addedSpy(&page, &ParametersPage::parameterAdded);

    auto param1 = new Parameter(100.0, "Param1", this);
    page.append(param1);

    auto sameNameParam1= new Parameter(123, "Param1", this);
    bool res = page.append(sameNameParam1);

    // qDebug() << list.toJsonValues() << list.toJsonSchema();

    QCOMPARE(res, false);
    QCOMPARE(page.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);

    // Try changing paramter1 name should fail anyway
    param1->setName("Param1-Rename");
    res = page.append(param1);
    QCOMPARE(res, false);
    QCOMPARE(page.count(), 1);
}

void TestQtNoidAppParametersPage::testAppendingNoNameParametersShouldFail()
{
    auto param = new Parameter(this);
    ParametersPage page(this);
    bool res = page.append(param);
    QCOMPARE(res, false);
}

void TestQtNoidAppParametersPage::testAppendingParameterFromJsonObjects()
{
    ParametersPage page(this);
    QSignalSpy countSpy(&page, &ParametersPage::countChanged);
    QSignalSpy addedSpy(&page, &ParametersPage::parameterAdded);

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
    tempSchema["label"] = "TS";
    schema["Temperature"] = tempSchema;

    QJsonObject value;
    value["Temperature"] = 25.5;

    bool result = page.append(schema, value);
    QCOMPARE(result, true);
    QCOMPARE(page.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);

    QCOMPARE(page.parameter(0)->toJsonSchema(), schema);
    QCOMPARE(page.parameter(0)->toJsonValue(), value);
}

void TestQtNoidAppParametersPage::testAppendingParameterFromBadJsonObjectsShouldFail()
{
    ParametersPage page(this);
    QJsonObject schema;
    QJsonObject value;
    bool result = page.append(schema, value);
    QCOMPARE(result, false);
}

void TestQtNoidAppParametersPage::testAppendingParameterFirstElementInTheListShouldBeNumber0()
{
    ParametersPage page(this);
    auto param0 = new Parameter(100.0, "Param0", this);
    page.append(param0);
    QCOMPARE(page.parameter(0), param0);
}

void TestQtNoidAppParametersPage::testEmplaceWithNameDescriptionValue()
{
    ParametersPage page(this);
    QSignalSpy countSpy(&page, &ParametersPage::countChanged);
    QSignalSpy addedSpy(&page, &ParametersPage::parameterAdded);

    Parameter* param = page.emplace(25.5, "Temperature", "Ambient temperature");

    QVERIFY(param != nullptr);
    QCOMPARE(page.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);

    QCOMPARE(param->name(), "Temperature");
    QCOMPARE(param->description(), "Ambient temperature");
    QCOMPARE(param->value().toDouble(), 25.5);

    QCOMPARE(page.contains("Temperature"), true);
    QCOMPARE(page.parameter("Temperature"), param);

    Parameter* duplicate = page.emplace(30.0, "Temperature", "Another temperature");
    QCOMPARE(duplicate, nullptr);
    QCOMPARE(page.count(), 1);
}

void TestQtNoidAppParametersPage::testEmplaceWithDefaultParameters()
{
    ParametersPage page(this);
    QSignalSpy countSpy(&page, &ParametersPage::countChanged);
    QSignalSpy addedSpy(&page, &ParametersPage::parameterAdded);

    Parameter* param0 = page.emplace(0.0, {}, {});
    QVERIFY(param0 == nullptr);

    Parameter* param1 = page.emplace({}, "Param1");
    QVERIFY(param1 != nullptr);
    QCOMPARE(param1->name(), "Param1");
    QCOMPARE(param1->description(), QString());
    QCOMPARE(param1->value(), QVariant());

    Parameter* param2 = page.emplace({}, "Param2", "A description");
    QVERIFY(param2 != nullptr);
    QCOMPARE(param2->name(), "Param2");
    QCOMPARE(param2->description(), "A description");
    QCOMPARE(param2->value(), QVariant());

    Parameter* param3 = page.emplace( 42, "Param3", QString());
    QVERIFY(param3 != nullptr);
    QCOMPARE(param3->name(), "Param3");
    QCOMPARE(param3->description(), QString());
    QCOMPARE(param3->value().toInt(), 42);

    QCOMPARE(page.count(), 3);
    QCOMPARE(countSpy.count(), 3);
    QCOMPARE(addedSpy.count(), 3);
}

void TestQtNoidAppParametersPage::testEmplaceWithJsonObjects()
{
    ParametersPage page(this);
    QSignalSpy countSpy(&page, &ParametersPage::countChanged);
    QSignalSpy addedSpy(&page, &ParametersPage::parameterAdded);

    QJsonObject schema;
    QJsonObject schemaObject;
    schemaObject["description"] = "Temperature sensor";
    schemaObject["tooltip"] = "Temperature of the sensor ";
    schemaObject["unit"] = "°C";
    schemaObject["min"] = -50.0;
    schemaObject["max"] = 100.0;
    schemaObject["readOnly"] = true;
    schemaObject["visible"] = true;
    schemaObject["label"] = "TS";

    schema["Temperature"] = schemaObject;

    QJsonObject value;
    value["Temperature"] = 25.5;

    Parameter* param = page.emplace(schema, value);

    QVERIFY(param != nullptr);
    QCOMPARE(page.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);

    QCOMPARE(param->name(), "Temperature");
    QCOMPARE(param->description(), "Temperature sensor");
    QCOMPARE(param->unit(), "°C");
    QCOMPARE(param->value().toDouble(), 25.5);
    QCOMPARE(param->min().toDouble(), -50.0);
    QCOMPARE(param->max().toDouble(), 100.0);
    QCOMPARE(param->readOnly(), true);

    QCOMPARE(page.contains("Temperature"), true);
    QCOMPARE(page.parameter("Temperature"), param);

    QCOMPARE(param->toJsonSchema(), schema);
    QCOMPARE(param->toJsonValue(), value);

    QJsonObject badSchema;
    QJsonObject badValue;
    Parameter* failedParam = page.emplace(badSchema, badValue);
    QCOMPARE(failedParam, nullptr);
    QCOMPARE(page.count(), 1);
}

void TestQtNoidAppParametersPage::testRemovingParameters()
{
    ParametersPage page(this);
    
    auto param1 = new Parameter(100.0, "Param1", this);
    auto param2 = new Parameter(200.0, "Param2", this);
    
    page.append(param1);
    page.append(param2);
    QSignalSpy countSpy(&page, &ParametersPage::countChanged);
    QSignalSpy removedSpy(&page, &ParametersPage::parameterRemoved);
    
    page.removeParameter(param1);
    QCOMPARE(page.count(), 1);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(removedSpy.count(), 1);
    QCOMPARE(page.contains(param1), false);
    QCOMPARE(page.contains("Param1"), false);
    
    // Remove by name
    page.removeParameter("Param2");
    QCOMPARE(page.count(), 0);
    QCOMPARE(countSpy.count(), 2);
    QCOMPARE(removedSpy.count(), 2);
}

void TestQtNoidAppParametersPage::testRemoveParameterByNameAtIndexZeroShouldUpdateCountAndIndex()
{
    ParametersPage page(this);

    auto paramAt0 = new Parameter(100.0, "paramAt0", this);
    auto paramAt1 = new Parameter(200.0, "paramAt1", this);

    page.append(paramAt0);
    page.append(paramAt1);

    page.removeParameter("paramAt0");

    qDebug() << page;

    QCOMPARE(page.isEmpty(), false);

    QCOMPARE(page.contains(paramAt0), false);
    QCOMPARE(page.contains("paramAt0"), false);
    QCOMPARE(page.indexOf(paramAt0), -1);
    QCOMPARE(page.parameter(0), nullptr);

    QCOMPARE(page.count(), 1);

    // The list should contain just paramAt1
    QCOMPARE(page.parameter("paramAt1"), paramAt1);
    QList<Parameter*> remaining = page.parameters();
    QCOMPARE(remaining.count(), 1);
    QCOMPARE(remaining.first(), paramAt1);


}

void TestQtNoidAppParametersPage::testParametersPageClear()
{
    ParametersPage page(this);

    auto param1 = new Parameter(100.0, "Param1", this);
    auto param2 = new Parameter(200.0, "Param2", this);
    auto param3 = new Parameter(300.0, "Param3", this);

    page.append(param1);
    page.append(param2);
    page.append(param3);
    QCOMPARE(page.count(), 3);

    QSignalSpy countSpy(&page, &ParametersPage::countChanged);
    QSignalSpy removedSpy(&page, &ParametersPage::parameterRemoved);
    page.clear();

    // Verify list is empty
    QCOMPARE(page.count(), 0);

    // Verify signals were emitted for each parameter removal
    QCOMPARE(removedSpy.count(), 3);
    QCOMPARE(countSpy.count(), 1); // Final count change signal

    // Verify final count change signal has correct value
    QList<QVariant> arguments = countSpy.takeLast();
    QCOMPARE(arguments.at(0).toInt(), 0);

    // Test clearing empty list (should not emit signals)
    countSpy.clear();
    removedSpy.clear();
    page.clear();
    QCOMPARE(countSpy.count(), 0);
    QCOMPARE(removedSpy.count(), 0);
}

void TestQtNoidAppParametersPage::testEmplaceAfterRemoveParameterShouldAddAtIndexZero()
{
    ParametersPage page(this);

    Parameter* first = page.emplace(10, "Param1");
    QCOMPARE(page.indexOf(first), 0);

    page.removeParameter(first);
    QCOMPARE(page.count(), 0);

    Parameter* second = page.emplace(20, "Param2");
    QCOMPARE(page.indexOf(second), 0);
    QCOMPARE(page.parameter(0), second);

    page.clear();
    Parameter* third = page.emplace(30, "Param3");
    QCOMPARE(page.indexOf(third), 0);
    QCOMPARE(page.parameter(0), third);
}

void TestQtNoidAppParametersPage::testAppendAfterRemoveParameterShouldAddAtIndexZero()
{
    ParametersPage page(this);

    Parameter* first = new Parameter(10, "Param1", this);
    QVERIFY(page.append(first));
    QCOMPARE(page.indexOf(first), 0);
    QCOMPARE(page.count(), 1);

    page.removeParameter(first);
    QCOMPARE(page.count(), 0);

    Parameter* second = new Parameter(20, "Param2", this);
    QVERIFY(page.append(second));
    QCOMPARE(page.count(), 1);
    QCOMPARE(page.indexOf(second), 0);
    QCOMPARE(page.parameter(0), second);

    page.clear();
    Parameter* third = new Parameter(30, "Param3", this);
    QVERIFY(page.append(third));
    QCOMPARE(page.indexOf(third), 0);
    QCOMPARE(page.parameter(0), third);
}

void TestQtNoidAppParametersPage::testUsingDeleteDirectlyOnTheParameterPointer()
{
    Parameter* par = new Parameter(10, "par", this);
    ParametersPage page("myPage", this);

    page.append(par);
    par->setValue(100);
    QCOMPARE(page.isValueModified(), true);

    QSignalSpy removedSpy(&page, &ParametersPage::parameterRemoved);
    QSignalSpy countSpy(&page, &ParametersPage::countChanged);

    delete par;

    QCOMPARE(page.count(), 0);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(removedSpy.count(), 1);

    // Check the isValueChanged of the list is false
    QCOMPARE(page.isValueModified(), false);

    // Check the the first parameter of an empty list is 0
    Parameter* emPlace = page.emplace(123, "Emplace");
    QCOMPARE(page.indexOf(emPlace), 0);
}

void TestQtNoidAppParametersPage::testParameterAccess()
{
    ParametersPage page(this);
    auto param1 = new Parameter(25.0, "Temperature", this);
    auto param2 = new Parameter(1013.25, "Pressure", this);
    auto param3 = new Parameter( 60.0, "Humidity",this);
    
    page << param1 << param2 << param3;
    
    // Test access by index
    QCOMPARE(page.parameter(0), param1);
    QCOMPARE(page.parameter(1), param2);
    QCOMPARE(page.parameter(2), param3);
    QCOMPARE(page.parameter(-1), nullptr);
    QCOMPARE(page.parameter(3), nullptr);
    
    // Test access by name
    QCOMPARE(page.parameter("Temperature"), param1);
    QCOMPARE(page.parameter("Pressure"), param2);
    QCOMPARE(page.parameter("Humidity"), param3);
    QCOMPARE(page.parameter("NonExistent"), nullptr);
    
    // Test indexOf
    QCOMPARE(page.indexOf(param1), 0);
    QCOMPARE(page.indexOf(param2), 1);
    QCOMPARE(page.indexOf("Temperature"), 0);
    QCOMPARE(page.indexOf("Pressure"), 1);
    QCOMPARE(page.indexOf("NonExistent"), -1);
    
    // Test parameters() method
    QList<Parameter*> params = page.parameters();
    QCOMPARE(params.count(), 3);
    QCOMPARE(params.contains(param1), true);
    QCOMPARE(params.contains(param2), true);
    QCOMPARE(params.contains(param3), true);
}

void TestQtNoidAppParametersPage::testParametersPageSetValueConvenienceMethods()
{
    ParametersPage page(this);
    auto param1 = new Parameter(25.0, "Temperature", this);
    auto param2 = new Parameter(1013.25, "Pressure", this);
    
    page.append(param1);
    page.append(param2);
    
    // Test value() method
    QCOMPARE(page.value("Temperature"), 25.0);
    QCOMPARE(page.value("Pressure"), 1013.25);
    QCOMPARE(page.value("NonExistent"), QVariant());
    
    // Test setValue() method
    QCOMPARE(page.setValue("Temperature", 30.0), true);
    QCOMPARE(param1->value(), 30.0);
    QCOMPARE(page.setValue("NonExistent", 123), false);
}

void TestQtNoidAppParametersPage::testParametersPageSetValueWhileReadOnlyShouldFail()
{
    ParametersPage page(this);
    Parameter param1(25.0, "Temperature", this);
    page << param1;

    // setValue should fail and not modify the parameter
    page.setReadOnly(true);
    QSignalSpy writeAttemptSpy(&page, &ParametersPage::writeAttemptedWhileReadOnly);
    QCOMPARE(page.setValue("Temperature", 99.0), false);
    QCOMPARE(page.value("Temperature"), 25.0);
    QCOMPARE(writeAttemptSpy.count(), 1);

    // Signal must carry the name of the parameter whose modification was blocked
    QList<QVariant> arguments = writeAttemptSpy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "Temperature");

    // A non-existent parameter should just fail silently: no write-attempt signal
    QCOMPARE(page.setValue("NonExistent", 1), false);
    QCOMPARE(writeAttemptSpy.count(), 0);

    // Turning readOnly off should let setValue succeed again
    page.setReadOnly(false);
    QCOMPARE(page.setValue("Temperature", 99.0), true);
    QCOMPARE(page.value("Temperature"), 99.0);
    QCOMPARE(writeAttemptSpy.count(), 0);


}

void TestQtNoidAppParametersPage::testParameterDestruction()
{
    ParametersPage page(this);
    QSignalSpy countSpy(&page, &ParametersPage::countChanged);
    QSignalSpy removedSpy(&page, &ParametersPage::parameterRemoved);
    
    auto param1 = new Parameter(100.0, "Param1", this);
    page.append(param1);
    countSpy.clear();
    removedSpy.clear();
    
    // Delete parameter - should be automatically removed from list
    delete param1;
    
    QCOMPARE(page.count(), 0);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(removedSpy.count(), 1);
}

void TestQtNoidAppParametersPage::testBindableNameProperty()
{
    // Get the bindable property
    ParametersPage page("File Settings", this);
    auto bindableName = page.bindableName();
    QVERIFY(bindableName.isValid());
    QCOMPARE(bindableName.value(), "File Settings");

    // Test binding to another property
    QProperty<QString> externalProperty;
    externalProperty.setBinding([&]() { return bindableName.value(); });
    QCOMPARE(externalProperty.value(), "File Settings");

    // Change parameter name and verify binding updates
    page.setName("NewName");
    QCOMPARE(externalProperty.value(), "NewName");

    // Test setting name through bindable
    bindableName.setValue("FinalName");
    QCOMPARE(page.name(), "FinalName");
    QCOMPARE(externalProperty.value(), "FinalName");

    // Create a reverse binding from externalProperty to page
    // Change external property and verify binding works
    QSignalSpy spy(&page, &ParametersPage::nameChanged);
    bindableName.setBinding([&]() { return externalProperty.value(); });
    externalProperty.setValue("Updated External Value");
    QCOMPARE(bindableName.value(), "Updated External Value");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().toString(), "Updated External Value");
}

void TestQtNoidAppParametersPage::testParametersPageLabel()
{
    ParametersPage page(this);
    QSignalSpy spy(&page, &ParametersPage::labelChanged);

    // Initial label should be empty
    QCOMPARE(page.label(), QString());

    // Set label and verify signal emission
    page.setLabel("Configuration");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(page.label(), "Configuration");

    // Check signal argument
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "Configuration");

    // Same value, no signal should be emitted
    page.setLabel("Configuration");
    QCOMPARE(spy.count(), 0);

    // Change to different value
    page.setLabel("Updated label");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(page.label(), "Updated label");

}

void TestQtNoidAppParametersPage::testBindableLabelProperty()
{
    // Create parameter page with initial label
    ParametersPage page(this);
    page.setLabel("Initial label");

    // Get the bindable property
    auto bindableLabel = page.bindableLabel();
    QVERIFY(bindableLabel.isValid());
    QCOMPARE(bindableLabel.value(), "Initial label");

    // Test binding to another property
    QProperty<QString> externalProperty;
    externalProperty.setBinding([&]() { return bindableLabel.value(); });
    QCOMPARE(externalProperty.value(), "Initial label");

    // Change parameter label and verify binding updates
    page.setLabel("Updated label");
    QCOMPARE(externalProperty.value(), "Updated label");

    // Test setting label through bindable
    bindableLabel.setValue("Final label");
    QCOMPARE(page.label(), "Final label");
    QCOMPARE(externalProperty.value(), "Final label");

    // Create a reverse binding from externalProperty to page
    QSignalSpy spy(&page, &ParametersPage::labelChanged);
    bindableLabel.setBinding([&]() { return externalProperty.value(); });
    externalProperty.setValue("Externally set label");
    QCOMPARE(bindableLabel.value(), "Externally set label");
    QCOMPARE(page.label(), "Externally set label");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().toString(), "Externally set label");
}

void TestQtNoidAppParametersPage::testToJsonValues()
{
    // Create a parameter list with parameters
    ParametersPage page("Configuration", this);
    auto param1 = new Parameter(25.5, "Temperature", this);
    page.append(param1);
    auto param2 = new Parameter(1013.25, "Pressure", this);
    page.append(param2);

    QJsonObject json = page.toJsonValues();
    // qDebug() << __func__ << json;

    // Verify JSON structure
    QVERIFY(json.contains("Configuration"));
    QJsonObject main = json["Configuration"].toObject();
    QJsonArray parametersArray = main["parameters"].toArray();
    QCOMPARE(parametersArray.size(), 2);
    QVERIFY(parametersArray.contains(param1->toJsonValue()));
    QVERIFY(parametersArray.contains(param2->toJsonValue()));
}

void TestQtNoidAppParametersPage::testToJsonValuesNoName()
{
    ParametersPage page(this);
    QJsonObject json = page.toJsonValues();
    // qDebug() << __func__ << json;
    QVERIFY(json.contains("pageName"));
}

void TestQtNoidAppParametersPage::TestToJsonSchema()
{
    ParametersPage page("Configuration", this);
    page.setTooltip("Set the app configuration");
    page.setDescription("This is your's app config page");
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

    QJsonObject jsonMain = jsonSchema["Configuration"].toObject();

    QCOMPARE(jsonMain["tooltip"], "Set the app configuration");
    QCOMPARE(jsonMain["description"], "This is your's app config page");
    QCOMPARE(jsonMain["visible"], true);

    QJsonArray parametersArray = jsonMain["parameters"].toArray();
    QCOMPARE(parametersArray.size(), 2);

    QVERIFY(parametersArray.contains(param1->toJsonSchema()));
    QVERIFY(parametersArray.contains(param2->toJsonSchema()));

}

void TestQtNoidAppParametersPage::TestToJsonSchemaNoName()
{
    ParametersPage page(this);
    QJsonObject json = page.toJsonSchema();
    // qDebug() << __func__ << json;
    QVERIFY(json.contains("PageName"));
}

void TestQtNoidAppParametersPage::testToJsonSchemaWithPresets()
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

void TestQtNoidAppParametersPage::testValuesFromJsonWithEmptyNameShouldTakeTheJsonName()
{
    // Create JSON objects (simulating what Parameter::toJsonValue() would return)
    QJsonObject pressureJson({{"Pressure", 1013.25}});
    QJsonObject temperatureJson({{"Temperature", 25.5}});
    QJsonArray parametersArray({pressureJson, temperatureJson});
    QJsonObject jsonMain({{"parameters", parametersArray}});

    QJsonObject json({{"MyTestPage", jsonMain}});
    // qDebug() << __func__ << json;

    ParametersPage page(this);
    auto res = page.valuesFromJson(json);
    QVERIFY(res);
    // qDebug() << __func__ << list;


    // Verify list properties
    QCOMPARE(page.name(), "MyTestPage");
    QCOMPARE(page.count(), 2);

    auto actual = page.parameter("Pressure")->toJsonValue();
    QCOMPARE(actual, pressureJson);

    actual = page.parameter("Temperature")->toJsonValue();
    QCOMPARE(actual, temperatureJson);

}

void TestQtNoidAppParametersPage::testValuesFromJsonWithWrongNameShouldFail()
{
    QJsonObject json;
    json["MyTestPage"] = QJsonArray();

    ParametersPage page("NotMyTestPage", this);
    QCOMPARE(page.valuesFromJson(json), false);
}

void TestQtNoidAppParametersPage::testValuesFromJsonWithCorrectName()
{
    QJsonObject json;
    json["MyTestPage"] = QJsonArray();
    // qDebug() << __func__ << json;

    // Test successful fromJson
    ParametersPage list("MyTestPage", this);
    QCOMPARE(list.valuesFromJson(json), true);
}

void TestQtNoidAppParametersPage::testSchemaFromJson()
{
    // Create JSON schema with multiple parameters and use it
    // to load into a ParametersPage via schemaFromJson()

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
    tempSchema["label"] = "CT";

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
    pressSchema["label"] = "AP";
    QJsonObject expectedPress;
    expectedPress["Pressure"] = pressSchema;
    parametersArray.append(expectedPress);

    // Complete schema JSON
    QJsonObject schemaMain;
    schemaMain["parameters"] = parametersArray;
    QJsonObject json;
    json["TestConfiguration"] = schemaMain;

    // Test with empty-named parameter page (should take JSON name)
    ParametersPage page(this);
    QVERIFY(page.schemaFromJson(json));

    // Verify list properties
    QCOMPARE(page.name(), "TestConfiguration");
    QCOMPARE(page.count(), 2);

    // Verify Temperature in page
    QJsonObject actualTemp = page.parameter("Temperature")->toJsonSchema();
    QCOMPARE(actualTemp, expectedTemp);

    // Verify Pressure in page
    QJsonObject actualPress = page.parameter("Pressure")->toJsonSchema();
    QCOMPARE(actualPress, expectedPress);
}

void TestQtNoidAppParametersPage::testSchemaFromJsonWithWrongNameShouldFail()
{
    QJsonObject json;
    json["WrongName"] = QJsonArray();
    // qDebug() << __func__ << json;

    // If there is no name if is ok
    ParametersPage page2(this);
    QVERIFY(page2.schemaFromJson(json));
    QCOMPARE(page2.name(), "WrongName");

    // IF there is a name schemaFromJson() fails
    ParametersPage page("MyPage", this);
    QCOMPARE(page.schemaFromJson(json), false);
}

void TestQtNoidAppParametersPage::testSchemaFromJsonWithCorrectName()
{
    QJsonObject json;
    json["CorrectName"] = QJsonArray();
    // qDebug() << __func__ << json;

    ParametersPage page("CorrectName", this);
    QVERIFY(page.schemaFromJson(json));
}

void TestQtNoidAppParametersPage::testSchemaFromDuplicatedJsonOverwriteAndNotFail()
{
    ParametersPage original("TestList", this);
    original.emplace(123, "Pressure", "pressure parameter");
    QJsonObject schema = original.toJsonSchema();

    ParametersPage fromJson(this);

    QCOMPARE(fromJson.schemaFromJson(schema), true);
    QCOMPARE(fromJson.schemaFromJson(schema), true);
    QCOMPARE(fromJson.count(), 1);
    QCOMPARE(fromJson.parameter("Pressure")->description(), "pressure parameter");
}

void TestQtNoidAppParametersPage::testConstructorWithSchemaAndValueJsonObjects()
{
    // Create schema JSON with two parameters

    // Temperature parameter schema
    QJsonObject temperatureSchema{{"description", "Current temperature"},
                            {"tooltip", "Temperature of the sensor"},
                            {"unit", "°C"},
                            {"readOnly", true},
                            {"visible", true},
                            {"min", -50.0},
                            {"max", 100.0},
                            {"label", "CT"}
    };
    QJsonObject temperatureSchemaMain{{"Temperature", temperatureSchema}};

    // Pressure parameter schema
    QJsonObject pressureSchema{{"description", "Atmospheric pressure"},
                                {"tooltip", "This is the atmospheric pressure"},
                                {"unit", "hPa"},
                                {"readOnly", false},
                                {"visible", true},
                                {"min", 800.0},
                                {"max", 1100.0},
                                {"label", "AP"}
    };
    QJsonObject pressureSchemaMain{{"Pressure", pressureSchema}};


    QJsonObject schemaMain{{"visible", false},
                        {"description", "Desc"},
                        {"tooltip", "toollllllttiiiipp"},
                        {"label", "UI Name"},
                        {"readOnly",false},
                        {"parameters", QJsonArray{temperatureSchemaMain, pressureSchemaMain}}
    };

    QJsonObject schema{{"SensorConfiguration", schemaMain}};
    // qDebug() << __func__ << schema;


    // Create values JSON
    QJsonObject temperatureValue{{"Temperature", 25.5}};
    QJsonObject pressureValue{{"Pressure", 1013.25}};
    QJsonObject valueMain({{"parameters", QJsonArray{temperatureValue, pressureValue}}});
    QJsonObject value{{"SensorConfiguration", valueMain}};
    // qDebug() << __func__ << "JSON-Value:" << value;

    // Test constructor
    ParametersPage page(schema, value, this);
    // qDebug() << __func__ << list;
    // qDebug() << __func__ << "Temperature:" << list.value("Temperature");
    // qDebug() << __func__ << "Pressure:" << list.value("Pressure");

    // Verify the list was created correctly
    QCOMPARE(page.name(), "SensorConfiguration");
    QCOMPARE(page.count(), 2);

    QCOMPARE(page.description(), "Desc");
    QCOMPARE(page.tooltip(), "toollllllttiiiipp");
    QCOMPARE(page.visible(), false);

    // Verify Temperature parameter
    Parameter* temperature = page.parameter("Temperature");
    // qDebug() << __func__ << temperature;

    QVERIFY(temperature != nullptr);
    QCOMPARE(temperature->description(), "Current temperature");
    QCOMPARE(temperature->tooltip(), "Temperature of the sensor");
    QCOMPARE(temperature->unit(), "°C");
    QCOMPARE(temperature->readOnly(), true);
    QCOMPARE(temperature->visible(), true);
    QCOMPARE(temperature->min().toDouble(), -50.0);
    QCOMPARE(temperature->max().toDouble(), 100.0);
    QCOMPARE(temperature->value().toDouble(), 25.5);

    // Verify Pressure parameter
    Parameter* pressParam = page.parameter("Pressure");
    QVERIFY(pressParam != nullptr);
    QCOMPARE(pressParam->value().toDouble(), 1013.25);
    QCOMPARE(pressParam->description(), "Atmospheric pressure");
    QCOMPARE(pressParam->unit(), "hPa");
    QCOMPARE(pressParam->readOnly(), false);
    QCOMPARE(pressParam->min().toDouble(), 800.0);
    QCOMPARE(pressParam->max().toDouble(), 1100.0);

    // Verify JSON serialization matches input
    QCOMPARE(page.toJsonSchema(), schema);
    QCOMPARE(page.toJsonValues(), value);
}

void TestQtNoidAppParametersPage::testParametersPageLabelShouldSurviveJsonRoundTrip()
{
    ParametersPage original("Configuration", this);
    original.setLabel("App Configuration");

    QJsonObject schema = original.toJsonSchema();
    QJsonObject values = original.toJsonValues();

    // Ricarico da zero, come al riavvio dell'applicazione
    ParametersPage restored(schema, values, this);

    // check label
    QCOMPARE(restored.label(), "App Configuration");

    // check the restore from a json object
    ParametersPage fromJson;
    fromJson.schemaFromJson(schema);
    QCOMPARE(fromJson.label(), "App Configuration");

}

void TestQtNoidAppParametersPage::testParametersPageReadOnlyShouldSurviveJsonRoundTrip()
{
    ParametersPage original("Configuration", this);
    original.setReadOnly(true);

    QJsonObject schema = original.toJsonSchema();
    QJsonObject values = original.toJsonValues();

    // Ricarico da zero, come al riavvio dell'applicazione
    ParametersPage restored(schema, values, this);

    // check label
    QCOMPARE(restored.readOnly(), true);

    // check the restore from a json object
    ParametersPage fromJson;
    fromJson.schemaFromJson(schema);
    QCOMPARE(fromJson.readOnly(), true);
}

void TestQtNoidAppParametersPage::testChangingParamterNameShouldUpdateTheParametersPage()
{
    Parameter par(123, "OriginalName", this);
    QCOMPARE(par.name(), "OriginalName");
    ParametersPage page;
    page.append(&par);
    QVERIFY(page.contains("OriginalName"));

    par.setName("NewName");
    QVERIFY(page.contains("NewName"));
}

void TestQtNoidAppParametersPage::testParametersPageDescription()
{
    ParametersPage page(this);
    QSignalSpy spy(&page, &ParametersPage::descriptionChanged);

    // Initial description should be empty
    QCOMPARE(page.description(), QString());

    // Set description and verify signal emission
    page.setDescription("Configuration parameters for the application");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(page.description(), "Configuration parameters for the application");

    // Check signal argument
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "Configuration parameters for the application");

    // Same value, no signal should be emitted
    page.setDescription("Configuration parameters for the application");
    QCOMPARE(spy.count(), 0);

    // Change to different value
    page.setDescription("Updated description");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(page.description(), "Updated description");

    // Clear description
    page.setDescription("");
    QCOMPARE(spy.count(), 2);
    QCOMPARE(page.description(), QString());
}

void TestQtNoidAppParametersPage::testBindableDescriptionProperty()
{
    // Create parameter page with initial description
    ParametersPage page(this);
    page.setDescription("Initial description");

    // Get the bindable property
    auto bindableDescription = page.bindableDescription();
    QVERIFY(bindableDescription.isValid());
    QCOMPARE(bindableDescription.value(), "Initial description");

    // Test binding to another property
    QProperty<QString> externalProperty;
    externalProperty.setBinding([&]() { return bindableDescription.value(); });
    QCOMPARE(externalProperty.value(), "Initial description");

    // Change parameter description and verify binding updates
    page.setDescription("Updated description");
    QCOMPARE(externalProperty.value(), "Updated description");

    // Test setting description through bindable
    bindableDescription.setValue("Final description");
    QCOMPARE(page.description(), "Final description");
    QCOMPARE(externalProperty.value(), "Final description");

    // Create a reverse binding from externalProperty to page
    QSignalSpy spy(&page, &ParametersPage::descriptionChanged);
    bindableDescription.setBinding([&]() { return externalProperty.value(); });
    externalProperty.setValue("Externally set description");
    QCOMPARE(bindableDescription.value(), "Externally set description");
    QCOMPARE(page.description(), "Externally set description");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().toString(), "Externally set description");
}

void TestQtNoidAppParametersPage::testParametersPageTooltip()
{
    ParametersPage page(this);
    QSignalSpy spy(&page, &ParametersPage::tooltipChanged);

    // Initial tooltip should be empty
    QCOMPARE(page.tooltip(), QString());

    // Set tooltip and verify signal emission
    page.setTooltip("This is a helpful tooltip for the parameter list");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(page.tooltip(), "This is a helpful tooltip for the parameter list");

    // Check signal argument
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "This is a helpful tooltip for the parameter list");

    // Same value, no signal should be emitted
    page.setTooltip("This is a helpful tooltip for the parameter list");
    QCOMPARE(spy.count(), 0);

    // Change to different value
    page.setTooltip("Updated tooltip text");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(page.tooltip(), "Updated tooltip text");

    // Clear tooltip
    page.setTooltip("");
    QCOMPARE(spy.count(), 2);
    QCOMPARE(page.tooltip(), QString());
}

void TestQtNoidAppParametersPage::testBindableTooltipProperty()
{
    // Create parameter page with initial tooltip
    ParametersPage page(this);
    page.setTooltip("Initial tooltip");

    // Get the bindable property
    auto bindableTooltip = page.bindableTooltip();
    QVERIFY(bindableTooltip.isValid());
    QCOMPARE(bindableTooltip.value(), "Initial tooltip");

    // Test binding to another property
    QProperty<QString> externalProperty;
    externalProperty.setBinding([&]() { return bindableTooltip.value(); });
    QCOMPARE(externalProperty.value(), "Initial tooltip");

    // Change parameter tooltip and verify binding updates
    page.setTooltip("Updated tooltip");
    QCOMPARE(externalProperty.value(), "Updated tooltip");

    // Test setting tooltip through bindable
    bindableTooltip.setValue("Final tooltip");
    QCOMPARE(page.tooltip(), "Final tooltip");
    QCOMPARE(externalProperty.value(), "Final tooltip");

    // Create a reverse binding from externalProperty to page
    QSignalSpy spy(&page, &ParametersPage::tooltipChanged);
    bindableTooltip.setBinding([&]() { return externalProperty.value(); });
    externalProperty.setValue("Externally set tooltip");
    QCOMPARE(bindableTooltip.value(), "Externally set tooltip");
    QCOMPARE(page.tooltip(), "Externally set tooltip");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().toString(), "Externally set tooltip");
}

void TestQtNoidAppParametersPage::testParametersPageVisible()
{
    ParametersPage page(this);
    QSignalSpy spy(&page, &ParametersPage::visibleChanged);
    page.setVisible(false);
    QCOMPARE(spy.count(), 1);
    auto arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toBool(), false);

    page.setVisible(false);
    QCOMPARE(spy.count(), 0);
}

void TestQtNoidAppParametersPage::testBindableParametersPageVisible()
{
    ParametersPage page(this);
    auto bindableVisible = page.bindableVisible();
    QVERIFY(bindableVisible.isValid());
    QCOMPARE(bindableVisible.value(), true);

    // Test binding to another property
    QProperty<bool> externalProperty;
    externalProperty.setBinding([&]() { return bindableVisible.value(); });
    QCOMPARE(externalProperty.value(), true);

    // Change parameter visible and verify binding updates
    page.setVisible(false);
    QCOMPARE(externalProperty.value(), false);

    // Test setting visible through bindable
    bindableVisible.setValue(true);
    QCOMPARE(page.visible(), true);
    QCOMPARE(externalProperty.value(), true);

    // Create a reverse binding from externalProperty to page
    QSignalSpy spy(&page, &ParametersPage::visibleChanged);
    bindableVisible.setBinding([&]() { return externalProperty.value(); });
    externalProperty.setValue(false);
    QCOMPARE(bindableVisible.value(), false);
    QCOMPARE(page.visible(), false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first(), false);
}

void TestQtNoidAppParametersPage::testParametersPageBindableCountProperty()
{
    ParametersPage page(this);

    // Get the bindable property
    auto bindableCount = page.bindableCount();
    QVERIFY(bindableCount.isValid());
    QCOMPARE(bindableCount.value(), 0);

    // Test binding to another property
    QProperty<int> externalProperty;
    externalProperty.setBinding([&]() { return bindableCount.value(); });
    QCOMPARE(externalProperty.value(), 0);

    // Add parameters and verify binding updates
    QSignalSpy countChangedSpy(&page, &ParametersPage::countChanged);

    // Add a parameter using emplace
    page.emplace(100, "Param1");
    QCOMPARE(page.count(), 1);
    QCOMPARE(bindableCount.value(), 1);
    QCOMPARE(externalProperty.value(), 1);
    QCOMPARE(countChangedSpy.count(), 1);

    // Add a parameter using append
    Parameter* param2 = new Parameter(200.0, "Param2", this);
    page.append(param2);
    QCOMPARE(page.count(), 2);
    QCOMPARE(bindableCount.value(), 2);
    QCOMPARE(externalProperty.value(), 2);
    QCOMPARE(countChangedSpy.count(), 2);

    // Add a parameter using emplace from JSON
    QJsonObject param3{{"Param3", 300.0}};
    page.emplace({}, param3);
    QCOMPARE(page.value("Param3"), 300.0);
    QCOMPARE(page.count(), 3);
    QCOMPARE(bindableCount.value(), 3);
    QCOMPARE(externalProperty.value(), 3);
    QCOMPARE(countChangedSpy.count(), 3);

    // Remove a parameter by reference
    page.removeParameter(param2);
    QCOMPARE(page.count(), 2);
    QCOMPARE(bindableCount.value(), 2);
    QCOMPARE(externalProperty.value(), 2);
    QCOMPARE(countChangedSpy.count(), 4);

    // Remove a parameter by name
    page.removeParameter("Param3");
    QCOMPARE(page.count(), 1);
    QCOMPARE(bindableCount.value(), 1);
    QCOMPARE(externalProperty.value(), 1);
    QCOMPARE(countChangedSpy.count(), 5);

    // Clear list and verify binding updates
    page.clear();
    QCOMPARE(page.count(), 0);
    QCOMPARE(bindableCount.value(), 0);
    QCOMPARE(externalProperty.value(), 0);
    QCOMPARE(countChangedSpy.count(), 6);

    // Verify signal was emitted with correct values
    QList<QVariant> arguments = countChangedSpy.at(0);
    QCOMPARE(arguments.at(0).toInt(), 1);
    arguments = countChangedSpy.at(1);
    QCOMPARE(arguments.at(0).toInt(), 2);
    arguments = countChangedSpy.at(2);
    QCOMPARE(arguments.at(0).toInt(), 3);
    arguments = countChangedSpy.at(3);
    QCOMPARE(arguments.at(0).toInt(), 2);
    arguments = countChangedSpy.at(4);
    QCOMPARE(arguments.at(0).toInt(), 1);
    arguments = countChangedSpy.at(5);
    QCOMPARE(arguments.at(0).toInt(), 0);
}

void TestQtNoidAppParametersPage::testParametersPageReadOnly()
{
    ParametersPage page(this);
    QSignalSpy spy(&page, &ParametersPage::readOnlyChanged);

    // Default should be false
    QCOMPARE(page.readOnly(), false);

    page.setReadOnly(true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(page.readOnly(), true);

    // Check signal argument
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toBool(), true);

    // Same value, no signal should be emitted
    page.setReadOnly(true);
    QCOMPARE(spy.count(), 0);

    // Change back to false
    page.setReadOnly(false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(page.readOnly(), false);

}

void TestQtNoidAppParametersPage::testBindableParametersPageReadOnly()
{
    ParametersPage page(this);
    auto bindableReadOnly = page.bindableReadOnly();
    QVERIFY(bindableReadOnly.isValid());
    QCOMPARE(bindableReadOnly.value(), false);

    // Test binding to another property
    QProperty<bool> externalProperty;
    externalProperty.setBinding([&]() { return bindableReadOnly.value(); });
    QCOMPARE(externalProperty.value(), false);

    // Change parameter readOnly and verify binding updates
    page.setReadOnly(true);
    QCOMPARE(externalProperty.value(), true);

    // Test setting readOnly through bindable
    bindableReadOnly.setValue(false);
    QCOMPARE(page.readOnly(), false);
    QCOMPARE(externalProperty.value(), false);

    // Create a reverse binding from externalProperty to page
    QSignalSpy spy(&page, &ParametersPage::readOnlyChanged);
    bindableReadOnly.setBinding([&]() { return externalProperty.value(); });
    externalProperty.setValue(true);
    QCOMPARE(bindableReadOnly.value(), true);
    QCOMPARE(page.readOnly(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first(), true);

}

void TestQtNoidAppParametersPage::testParametersPageIsValueModifiedChanged()
{
    ParametersPage page(this);
    Parameter* p1 = page.emplace(10, "Param1");
    Parameter* p2 = page.emplace(5, "Param2");

    // No parameter changed yet
    QCOMPARE(page.isValueModified(), false);

    QSignalSpy spy(&page, &ParametersPage::isValueModifiedChanged);

    // Changing one parameter should flip the aggregate to true
    p1->setValue(99);
    QCOMPARE(page.isValueModified(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);

    // A second parameter changing too should not emit again (already true)
    p2->setValue(50);
    QCOMPARE(page.isValueModified(), true);
    QCOMPARE(spy.count(), 0);

    // Reverting the first parameter: the second is still changed, aggregate stays true
    p1->setValue(10);
    QCOMPARE(page.isValueModified(), true);
    QCOMPARE(spy.count(), 0);

    // Reverting the last changed parameter: aggregate must go back to false
    p2->setValue(5);
    QCOMPARE(page.isValueModified(), false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);


    // Adding a modified paramter should activate the isValueChanged flag
    Parameter modPar_p3(0, "ModifiedParameter", this);
    modPar_p3.setValue(100);
    auto res = page.append(&modPar_p3);
    QCOMPARE(res, true);
    QCOMPARE(page.isValueModified(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);

    page.removeParameter(&modPar_p3);
    QCOMPARE(page.isValueModified(), false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);
}

void TestQtNoidAppParametersPage::testBindableIsValueModifiedProperty()
{
    ParametersPage page(this);
    Parameter* p1 = page.emplace(10, "Param1");

    auto bindableIsValueChanged = page.bindableIsValueModified();
    QVERIFY(bindableIsValueChanged.isValid());
    QCOMPARE(bindableIsValueChanged.value(), false);

    // Test binding to another property
    QProperty<bool> externalProperty;
    externalProperty.setBinding([&]() { return bindableIsValueChanged.value(); });
    QCOMPARE(externalProperty.value(), false);

    // Changing the child parameter's value should propagate through the binding
    p1->setValue(20);
    QCOMPARE(bindableIsValueChanged.value(), true);
    QCOMPARE(externalProperty.value(), true);

    // Reverting the value should propagate back to false
    p1->setValue(10);
    QCOMPARE(bindableIsValueChanged.value(), false);
    QCOMPARE(externalProperty.value(), false);
}

void TestQtNoidAppParametersPage::testParametersPageResetValueModifications()
{
    ParametersPage page(this);
    Parameter* p1 = page.emplace(10, "Param1");
    Parameter* p2 = page.emplace(5, "Param2");

    p1->setValue(99);
    p2->setValue(50);
    QCOMPARE(page.isValueModified(), true);

    page.resetValueModifications();

    QCOMPARE(p1->value(), 10);
    QCOMPARE(p2->value(), 5);
    QCOMPARE(p1->isValueModified(), false);
    QCOMPARE(p2->isValueModified(), false);
    QCOMPARE(page.isValueModified(), false);
}

void TestQtNoidAppParametersPage::testParametersPageResetValueModificationsDoesNotAffectSchema()
{
    ParametersPage page(this);
    Parameter* p1 = page.emplace(10, "Param1");
    p1->setValue(99);
    p1->setLabel("New label");
    page.setDescription("Page description");

    QCOMPARE(page.isValueModified(), true);
    QCOMPARE(page.isSchemaModified(), true);

    page.resetValueModifications();

    QCOMPARE(p1->value(), 10);
    QCOMPARE(page.isValueModified(), false);
    // resetAllValues() only touches values: schema changes on both the page
    // and the contained parameter must survive.
    QCOMPARE(p1->label(), QString("New label"));
    QCOMPARE(page.description(), QString("Page description"));
    QCOMPARE(page.isSchemaModified(), true);
}

void TestQtNoidAppParametersPage::testParametersPageSetValueModifiedCascadesTrueToAllParameters()
{
    // ParametersPage::setValueModified() has no override flag of its own: it
    // cascades to every contained Parameter, and the page's own aggregate
    // flag follows automatically through the counter already wired up.
    ParametersPage page(this);
    Parameter* p1 = page.emplace(10, "Param1");
    Parameter* p2 = page.emplace(5, "Param2");
    QCOMPARE(page.isValueModified(), false);

    QSignalSpy spy(&page, &ParametersPage::isValueModifiedChanged);

    page.setValueModified(true);
    QCOMPARE(p1->isValueModified(), true);
    QCOMPARE(p2->isValueModified(), true);
    QCOMPARE(page.isValueModified(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);
}

void TestQtNoidAppParametersPage::testParametersPageSetValueModifiedFalseCapturesCurrentStateAsNewBaselineForAllParameters()
{
    ParametersPage page(this);
    Parameter* p1 = page.emplace(10, "Param1");
    Parameter* p2 = page.emplace(5, "Param2");

    p1->setValue(99);
    QCOMPARE(page.isValueModified(), true);

    page.setValueModified(false);
    QCOMPARE(p1->isValueModified(), false);
    QCOMPARE(p2->isValueModified(), false);
    QCOMPARE(page.isValueModified(), false);

    // Each parameter's new baseline is its own current value (99 for p1, 5
    // for p2), not whatever it started with: resetValueModifications() must
    // restore to it, not revert it.
    p2->setValue(6);
    page.resetValueModifications();
    QCOMPARE(p1->value(), 99);
    QCOMPARE(p2->value(), 5);
    QCOMPARE(page.isValueModified(), false);
}

void TestQtNoidAppParametersPage::testParametersPageSetValueModifiedOverrideClearedByResetValueModifications()
{
    ParametersPage page(this);
    Parameter* p1 = page.emplace(10, "Param1");
    Parameter* p2 = page.emplace(5, "Param2");

    page.setValueModified(true);
    QCOMPARE(page.isValueModified(), true);

    // Neither parameter actually changed, so cascading
    // resetValueModifications() alone must bring everything back to false.
    page.resetValueModifications();
    QCOMPARE(p1->isValueModified(), false);
    QCOMPARE(p2->isValueModified(), false);
    QCOMPARE(page.isValueModified(), false);
}

void TestQtNoidAppParametersPage::testParametersPageSetValueModifiedWithMixedParameterStates()
{
    ParametersPage page(this);
    Parameter* p1 = page.emplace(10, "Param1");
    Parameter* p2 = page.emplace(5, "Param2");

    // p1 is genuinely modified, p2 is not.
    p1->setValue(99);
    QCOMPARE(p1->isValueModified(), true);
    QCOMPARE(p2->isValueModified(), false);
    QCOMPARE(page.isValueModified(), true);

    // Forcing true cascades to every parameter, including the one already modified.
    page.setValueModified(true);
    QCOMPARE(p1->isValueModified(), true);
    QCOMPARE(p2->isValueModified(), true);
    QCOMPARE(page.isValueModified(), true);

    // Clearing cascades too: each parameter's current value becomes its own
    // new baseline, regardless of whether it had a real change beforehand.
    page.setValueModified(false);
    QCOMPARE(p1->isValueModified(), false);
    QCOMPARE(p2->isValueModified(), false);
    QCOMPARE(page.isValueModified(), false);

    // p1's new baseline is 99, not the original 10.
    p1->resetValueModifications();
    QCOMPARE(p1->value(), 99);
}

void TestQtNoidAppParametersPage::testParametersPageIsSchemaModified()
{
    ParametersPage page(this);
    Parameter* p1 = page.emplace(10, "Param1");
    Parameter* p2 = page.emplace(5, "Param2");

    // Nothing changed yet
    QCOMPARE(page.isSchemaModified(), false);

    QSignalSpy spy(&page, &ParametersPage::isSchemaModifiedChanged);

    // Changing the page's own schema property flips the aggregate to true
    page.setLabel("New label");
    QCOMPARE(page.isSchemaModified(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);

    // Restoring that own property back to the baseline value (without going
    // through resetSchemaModifications()) must recompute the flag back to false:
    // isSchemaModified compares against the baseline, it is not a sticky flag.
    page.setLabel(QString());
    QCOMPARE(page.isSchemaModified(), false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);

    // A contained Parameter's schema change should aggregate too
    p1->setLabel("Param label");
    QCOMPARE(page.isSchemaModified(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);

    // A second parameter changing too should not emit again (already true)
    p2->setLabel("Param2 label");
    QCOMPARE(page.isSchemaModified(), true);
    QCOMPARE(spy.count(), 0);

    // Reverting the first parameter: the second is still changed, aggregate stays true
    p1->resetSchemaModifications();
    QCOMPARE(page.isSchemaModified(), true);
    QCOMPARE(spy.count(), 0);

    // Reverting the last changed parameter: aggregate must go back to false
    p2->resetSchemaModifications();
    QCOMPARE(page.isSchemaModified(), false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);

    // Adding a parameter whose schema is already changed activates the flag
    Parameter modPar_p3(0, "ModifiedParameter", this);
    modPar_p3.setLabel("already changed");
    auto res = page.append(&modPar_p3);
    QCOMPARE(res, true);
    QCOMPARE(page.isSchemaModified(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);

    page.removeParameter(&modPar_p3);
    QCOMPARE(page.isSchemaModified(), false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);
}

void TestQtNoidAppParametersPage::testBindableIsSchemaModifiedProperty()
{
    ParametersPage page(this);
    Parameter* p1 = page.emplace(10, "Param1");

    auto bindableIsSchemaModified = page.bindableIsSchemaModified();
    QVERIFY(bindableIsSchemaModified.isValid());
    QCOMPARE(bindableIsSchemaModified.value(), false);

    // Test binding to another property
    QProperty<bool> externalProperty;
    externalProperty.setBinding([&]() { return bindableIsSchemaModified.value(); });
    QCOMPARE(externalProperty.value(), false);

    // Changing the child parameter's schema should propagate through the binding
    p1->setLabel("New label");
    QCOMPARE(bindableIsSchemaModified.value(), true);
    QCOMPARE(externalProperty.value(), true);

    // Reverting the parameter's schema should propagate back to false
    p1->resetSchemaModifications();
    QCOMPARE(bindableIsSchemaModified.value(), false);
    QCOMPARE(externalProperty.value(), false);
}

void TestQtNoidAppParametersPage::testParametersPageResetSchemaModifications()
{
    ParametersPage page("TestPage", this);
    Parameter* p1 = page.emplace(10, "Param1", "Original description");
    p1->setLabel("Param label");
    page.setLabel("Page label");
    page.setDescription("Page description");
    page.setTooltip("Page tooltip");
    page.setReadOnly(true);
    page.setVisible(false);

    QCOMPARE(page.isSchemaModified(), true);

    page.resetSchemaModifications();

    QCOMPARE(page.label(), QString());
    QCOMPARE(page.description(), QString());
    QCOMPARE(page.tooltip(), QString());
    QCOMPARE(page.readOnly(), false);
    QCOMPARE(page.visible(), true);
    QCOMPARE(page.isSchemaModified(), false);

    // Cascades down to every contained Parameter too.
    QCOMPARE(p1->label(), QString());
    QCOMPARE(p1->description(), QString("Original description"));
    QCOMPARE(p1->isSchemaModified(), false);
}

void TestQtNoidAppParametersPage::testParametersPageResetSchemaModificationsDoesNotAffectValues()
{
    ParametersPage page(this);
    Parameter* p1 = page.emplace(10, "Param1");
    p1->setValue(99);
    p1->setLabel("New label");
    page.setDescription("Page description");

    QCOMPARE(page.isValueModified(), true);
    QCOMPARE(page.isSchemaModified(), true);

    page.resetSchemaModifications();

    // resetSchemaModifications() only touches schema properties: values stay as set.
    QCOMPARE(p1->value(), 99);
    QCOMPARE(page.isValueModified(), true);
    QCOMPARE(page.description(), QString());
    QCOMPARE(p1->label(), QString());
    QCOMPARE(page.isSchemaModified(), false);
}

void TestQtNoidAppParametersPage::testParametersPageSetSchemaModifiedForcesTrueRegardlessOfActualChanges()
{
    // Mirrors QTextDocument::setModified(true): force isSchemaModified() to
    // report true even though nothing tracked actually differs from baseline.
    ParametersPage page("TestPage", this);
    QCOMPARE(page.isSchemaModified(), false);

    QSignalSpy spy(&page, &ParametersPage::isSchemaModifiedChanged);

    page.setSchemaModified(true);
    QCOMPARE(page.isSchemaModified(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);

    // No tracked property changed: the override alone is forcing the flag.
    QCOMPARE(page.name(), QString("TestPage"));
    QCOMPARE(page.label(), QString());
}

void TestQtNoidAppParametersPage::testParametersPageSetSchemaModifiedFalseCapturesCurrentStateAsNewBaseline()
{
    // Mirrors QTextDocument::setModified(false): accept the page's own
    // current properties as the new reference schema.
    ParametersPage page("TestPage", this);
    page.setLabel("Changed label");
    QCOMPARE(page.isSchemaModified(), true);

    QSignalSpy spy(&page, &ParametersPage::isSchemaModifiedChanged);
    page.setSchemaModified(false);
    QCOMPARE(page.isSchemaModified(), false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);

    // The new baseline is "label: Changed label", not the original empty
    // label: resetSchemaModifications() must restore to it, not revert it.
    page.resetSchemaModifications();
    QCOMPARE(page.label(), QString("Changed label"));
    QCOMPARE(page.isSchemaModified(), false);
}

void TestQtNoidAppParametersPage::testParametersPageSetSchemaModifiedOverrideClearedByResetSchemaModifications()
{
    ParametersPage page("TestPage", this);
    QSignalSpy spy(&page, &ParametersPage::isSchemaModifiedChanged);

    page.setSchemaModified(true);
    QCOMPARE(page.isSchemaModified(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);

    // Nothing is actually tracked as changed, so clearing the override alone
    // must bring isSchemaModified() back to false.
    page.resetSchemaModifications();
    QCOMPARE(page.isSchemaModified(), false);
}

void TestQtNoidAppParametersPage::testParametersPageSetSchemaModifiedInteractionWithActualSchemaChanges()
{
    ParametersPage page("TestPage", this);

    // Override true + a real schema change: still reported as modified.
    page.setSchemaModified(true);
    page.setLabel("X");
    QCOMPARE(page.isSchemaModified(), true);

    // Clearing the override re-baselines using the current state (label "X"),
    // so isSchemaModified() goes back to false even though label was edited.
    page.setSchemaModified(false);
    QCOMPARE(page.isSchemaModified(), false);
    QCOMPARE(page.label(), QString("X"));

    // A further real schema change is still detected normally afterwards.
    page.setLabel("Y");
    QCOMPARE(page.isSchemaModified(), true);
}

void TestQtNoidAppParametersPage::testParametersPageSetSchemaModifiedDoesNotCascadeToParameters()
{
    // Unlike setValueModified(), setSchemaModified() mirrors Parameter's own
    // behavior directly: it only forces the page's own flag and only
    // re-baselines the page's own properties. It never touches contained
    // Parameters, which keep their own, independent override.
    ParametersPage page("TestPage", this);
    Parameter* p1 = page.emplace(10, "Param1");

    page.setSchemaModified(true);
    QCOMPARE(page.isSchemaModified(), true);
    QCOMPARE(p1->isSchemaModified(), false);

    page.setSchemaModified(false);
    QCOMPARE(page.isSchemaModified(), false);
    QCOMPARE(p1->isSchemaModified(), false);

    // A real schema change on the child still aggregates up normally,
    // independently of the page's own override mechanism.
    p1->setLabel("Param label");
    QCOMPARE(p1->isSchemaModified(), true);
    QCOMPARE(page.isSchemaModified(), true);
}


void TestQtNoidAppParametersPage::testParameterRenameError()
{
    ParametersPage *page = new ParametersPage("MyList", this);
    QSignalSpy renameErrorSpy(page, &ParametersPage::parameterRenameError);

    // Create two parameters with different names
    Parameter *param1 = new Parameter(100.0, "Param1", this);
    Parameter *param2 = new Parameter(200.0, "Param2", this);

    page->append(param1);
    page->append(param2);

    // Test case 1: Try to rename to an existing name (should emit error)
    param1->setName("Param2");

    QCOMPARE(renameErrorSpy.count(), 1);
    QList<QVariant> arguments = renameErrorSpy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "Param1");  // oldName
    QCOMPARE(arguments.at(1).toString(), "Param2");  // newName

    // Verify the parameter name wasn't actually changed in the list
    QVERIFY(page->contains("Param1"));
    QVERIFY(page->contains("Param2"));
    QCOMPARE(page->parameter("Param2"), param2);

    // Test case 2: Create a scenario where oldName is not in the list
    // (This is harder to trigger naturally, but we can simulate it)
    // by removing a parameter from the list but keeping the object alive
    page->removeParameter(param1);

    // Now if param1 tries to rename itself, it should not trigger an error
    // because "Param1" is no longer in the list
    param1->setName("Param2");
    QCOMPARE(renameErrorSpy.count(), 0);

    delete param1;
    delete param2;
    delete page;
}

void TestQtNoidAppParametersPage::testIsEmpty()
{
    ParametersPage page(this);

    // Test empty list
    QVERIFY(page.isEmpty());
    QCOMPARE(page.count(), 0);

    // Add a parameter
    Parameter* param1 = new Parameter(100.0, "Param1", this);
    page.append(param1);

    // Test non-empty list
    QVERIFY(!page.isEmpty());
    QCOMPARE(page.count(), 1);

    // Remove one parameter
    page.removeParameter(param1);
    QVERIFY(page.isEmpty());

    // Test clear() method
    Parameter* param3 = new Parameter(300.0, "Param3", this);
    Parameter* param4 = new Parameter(400.0, "Param4", this);
    page.append(param3);
    page.append(param4);

    QVERIFY(!page.isEmpty());
    QCOMPARE(page.count(), 2);

    page.clear();
    QVERIFY(page.isEmpty());
    QCOMPARE(page.count(), 0);
}

void TestQtNoidAppParametersPage::testApplyPreset()
{
    ParametersPage page(this);

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

    page.append(&param1);
    page.append(&param2);
    page.append(&param3);

    // Test applying "Low" preset - all parameter should change
    QSignalSpy spy1(&param1, &Parameter::valueChanged);
    QSignalSpy spy2(&param2, &Parameter::valueChanged);
    QSignalSpy spy3(&param3, &Parameter::valueChanged);

    page.applyPreset("Low");

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
    page.applyPreset("High");
    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 1);
    QCOMPARE(spy3.count(), 1);

    // Test applying non-existent preset - no parameters should change
    spy1.clear();
    spy2.clear();
    spy3.clear();
    page.applyPreset("NonExistent");
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
    QCOMPARE(spy3.count(), 0);
    // all should use High preset unchanged
    QCOMPARE(param1.value().toDouble(), 80.0);
    QCOMPARE(param2.value().toDouble(), 1100.0);
    QCOMPARE(param3.value().toDouble(), 90.0);

    // Test empty list
    ParametersPage emptyPage(this);
    emptyPage.applyPreset("AnyPreset"); // Should not crash
}

void TestQtNoidAppParametersPage::testApplyPresetShouldBypassReadOnly()
{
    ParametersPage page(this);

    Parameter param1(50.0, "Temperature", this);
    param1.setPreset("Low", 10.0);

    page << param1;
    page.setReadOnly(true);

    QSignalSpy valueChangedSpy(&param1, &Parameter::valueChanged);
    QSignalSpy writeAttemptSpy(&page, &ParametersPage::writeAttemptedWhileReadOnly);

    // applyPreset must still work even if the list is readOnly
    page.applyPreset("Low");

    QCOMPARE(page.value("Temperature"), 10.0);
    QCOMPARE(valueChangedSpy.count(), 1);
    QCOMPARE(writeAttemptSpy.count(), 0);
}

void TestQtNoidAppParametersPage::testOperatorLeftShiftWithParameterReference()
{
    ParametersPage page(this);
    QSignalSpy countSpy(&page, &ParametersPage::countChanged);
    QSignalSpy addedSpy(&page, &ParametersPage::parameterAdded);

    // Create parameters to add
    Parameter param1(100.0, "Temperature", this);
    Parameter param2(1013.25, "Pressure", this);
    Parameter param3(60.0, "Humidity", this);

    // Test chaining operator<< with parameter references
    page << param1 << param2 << param3;

    // Verify all parameters were added
    QCOMPARE(page.count(), 3);
    QCOMPARE(countSpy.count(), 3);
    QCOMPARE(addedSpy.count(), 3);

    // Verify parameters are accessible
    QCOMPARE(page.contains("Temperature"), true);
    QCOMPARE(page.contains("Pressure"), true);
    QCOMPARE(page.contains("Humidity"), true);
    QCOMPARE(page.parameter("Temperature"), &param1);
    QCOMPARE(page.parameter("Pressure"), &param2);
    QCOMPARE(page.parameter("Humidity"), &param3);

    // Verify parameter values
    QCOMPARE(page.value("Temperature").toDouble(), 100.0);
    QCOMPARE(page.value("Pressure").toDouble(), 1013.25);
    QCOMPARE(page.value("Humidity").toDouble(), 60.0);

    // Test adding duplicate parameter with operator<< (should not add)
    int originalCount = page.count();
    page << param1;
    QCOMPARE(page.count(), originalCount);
    QCOMPARE(countSpy.count(), 3); // No new count change signal

    // Test return value allows chaining
    ParametersPage& result = (page << param1);
    QCOMPARE(&result, &page);

    // Verify I can change paramter and see the changed value
    param1.setValue(99);
    QCOMPARE(page.value("Temperature"), 99);
    page.setValue("Pressure", 1000);
    QCOMPARE(param2.value(), 1000);
}

void TestQtNoidAppParametersPage::testDeletingParamtersPageDestroyOwnedParamters()
{
    // Create 2 objects and verify they are destroyed with the list
    ParametersPage *page = new ParametersPage("MyBeautifullList");

    Parameter *parameter1 = new Parameter(1, "Paramter1", page);
    QSignalSpy parameter1Spy(parameter1, &QObject::destroyed);

    Parameter *parameter2 = new Parameter(2, "Paramter2", page);
    QSignalSpy parameter2Spy(parameter2, &QObject::destroyed);
    *page << parameter1 << parameter2;

    delete page;
    QCOMPARE(parameter1Spy.count(), 1);
    QCOMPARE(parameter2Spy.count(), 1);
}

void TestQtNoidAppParametersPage::testApplyJsonValuesToExistingParametersPageShouldUpdateValues()
{
    // Create an existing ParametersPage with some parameters
    ParametersPage page("SensorConfig", this);
    page.emplace(20.0, "Temperature");
    page.emplace(1000.0, "Pressure");

    QCOMPARE(page.count(), 2);
    QCOMPARE(page.value("Temperature").toDouble(), 20.0);
    QCOMPARE(page.value("Pressure").toDouble(), 1000.0);

    // Create JSON values to update existing paramters
    QJsonObject temperatureValue{{"Temperature", 25.5}};
    QJsonObject pressValue{{"Pressure", 1013.25}};

    // Add a new parameter that should be add to the list
    QJsonObject humidityValue{{"Humidity", 60.0}};
    QJsonArray valuesArray{temperatureValue, pressValue, humidityValue};

    QJsonObject jsonMain{{"parameters", valuesArray}};
    QJsonObject jsonValues{{"SensorConfig", jsonMain}};

    QSignalSpy countSpy(&page, &ParametersPage::countChanged);
    QSignalSpy addedSpy(&page, &ParametersPage::parameterAdded);

    // Apply JSON values to existing page
    bool result = page.valuesFromJson(jsonValues);
    // qDebug() << __func__<< list;

    // Verify the operation succeeded
    QVERIFY(result);

    // Verify existing parameters were updated
    QCOMPARE(page.value("Temperature"), 25.5);
    QCOMPARE(page.value("Pressure"), 1013.25);

    // Verify new parameter was added
    QCOMPARE(page.count(), 3);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);
    // qDebug() << __func__ << list.value("Humidiy");
    QCOMPARE(page.value("Humidity"), 60.0);
}

void TestQtNoidAppParametersPage::testApplyJsonSchemaToExistingParametersPageShouldUpdateSchema()
{
    // Create an existing ParametersPage with some parameters
    ParametersPage page("DeviceConfig", this);
    page.emplace(50.0, "Temperature", "Description");
    QCOMPARE(page.count(), 1);

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
    jsonSchema["DeviceConfig"] = QJsonObject({{"parameters", schemaArray}});

    QSignalSpy countSpy(&page, &ParametersPage::countChanged);
    QSignalSpy addedSpy(&page, &ParametersPage::parameterAdded);

    // Apply JSON schema to existing page
    bool result = page.schemaFromJson(jsonSchema);

    // Verify the operation succeeded
    QVERIFY(result);

    // qDebug() << list;
    // qDebug() << jsonSchema;

    // Verify new parameters were added from schema
    QCOMPARE(page.count(), 2);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(addedSpy.count(), 1);

    // Verify Temperature parameter was updated from schema
    Parameter* tempParam = page.parameter("Temperature");
    QVERIFY(tempParam != nullptr);
    QCOMPARE(tempParam->description(), "Temperature sensor reading");
    QCOMPARE(tempParam->tooltip(), "Current ambient temperature");
    QCOMPARE(tempParam->unit(), "°C");
    QCOMPARE(tempParam->min().toDouble(), -40.0);
    QCOMPARE(tempParam->max().toDouble(), 85.0);
    QCOMPARE(tempParam->readOnly(), true);
    QCOMPARE(tempParam->visible(), true);

    // Verify Pressure parameter was created correctly from schema
    Parameter* pressParam = page.parameter("Pressure");
    QVERIFY(pressParam != nullptr);
    QCOMPARE(pressParam->description(), "Atmospheric pressure");
    QCOMPARE(pressParam->tooltip(), "Current atmospheric pressure reading");
    QCOMPARE(pressParam->unit(), "hPa");
    QCOMPARE(pressParam->min().toDouble(), 800.0);
    QCOMPARE(pressParam->max().toDouble(), 1200.0);
    QCOMPARE(pressParam->readOnly(), false);
    QCOMPARE(pressParam->visible(), true);
}

void TestQtNoidAppParametersPage::testParametersPageBeginAndEndAndRangeLoop()
{
    ParametersPage page("Test Page", this);
    // Test empty list
    QVERIFY(page.begin() == page.end());

    // Add some parameters
    page.emplace(10.0, "First", "First parameter");
    page.emplace(20.0, "Second", "Second parameter");
    page.emplace(30.0, "Third", "Third parameter");

    // Test that begin() != end() for non-empty list
    QVERIFY(page.begin() != page.end());

    // Test iterator dereferencing
    ParametersPage::iterator it = page.begin();
    QVERIFY(it != page.end());

    Parameter* firstParam = *it;
    QVERIFY(firstParam != nullptr);
    QCOMPARE(firstParam->name(), "First");
    QCOMPARE(firstParam->value().toDouble(), 10.0);

    // Test iterator increment
    ++it;
    QVERIFY(it != page.end());
    Parameter* secondParam = *it;
    QVERIFY(secondParam != nullptr);
    QCOMPARE(secondParam->name(), "Second");
    QCOMPARE(secondParam->value().toDouble(), 20.0);

    // Test post-increment
    auto prevIt = it++;
    QVERIFY(it != page.end());
    QVERIFY(prevIt != it);
    Parameter* thirdParam = *it;
    QVERIFY(thirdParam != nullptr);
    QCOMPARE(thirdParam->name(), "Third");
    QCOMPARE(thirdParam->value().toDouble(), 30.0);

    // Test iterator index() method
    QCOMPARE(page.begin().index(), 0);
    auto secondIt = page.begin();
    ++secondIt;
    QCOMPARE(secondIt.index(), 1);

    // Test bidirectional iterator (decrement)
    auto lastIt = page.end();
    --lastIt;
    Parameter* lastParam = *lastIt;
    QVERIFY(lastParam != nullptr);
    QCOMPARE(lastParam->name(), "Third");
    QCOMPARE(lastParam->value().toDouble(), 30.0);

    // Test post-decrement
    // int b = 10;
    // int a = b--;
    // qDebug() << a; // => a=10
    auto postDecIt = lastIt--;
    QCOMPARE((*postDecIt)->name(), "Third");
    QCOMPARE((*lastIt)->name(), "Second");

    // Test range-based for loop
    QStringList expectedList = {"First", "Second", "Third"};
    QStringList nameList;
    for(auto par : page){
        nameList << par->name();
    }
    QCOMPARE(nameList, expectedList);
}

void TestQtNoidAppParametersPage::testParametersPageConstIteratorsAndConstRangeLoop()
{
    ParametersPage page("Test Page", this);

    // Add some parameters
    page.emplace(100.0, "Alpha", "Alpha parameter");
    page.emplace(200.0, "Beta", "Beta parameter");
    page.emplace(300.0, "Gamma", "Gamma parameter");

    // Create a const reference to the page
    const ParametersPage& constPage = page;

    // ===== Test begin() const and end() const =====
    // Test empty vs non-empty
    QVERIFY(constPage.begin() != constPage.end());

    // Test const iterator dereferencing
    ParametersPage::const_iterator cit = constPage.begin();
    const Parameter* firstParam = *cit;
    QVERIFY(firstParam != nullptr);
    QCOMPARE(firstParam->name(), "Alpha");
    QCOMPARE(firstParam->value().toDouble(), 100.0);

    // Test const iterator increment
    ++cit;
    QVERIFY(cit != constPage.end());
    const Parameter* secondParam = *cit;
    QVERIFY(secondParam != nullptr);
    QCOMPARE(secondParam->name(), "Beta");
    QCOMPARE(secondParam->value().toDouble(), 200.0);

    // Test post-increment
    auto prevCit = cit++;
    QVERIFY(prevCit != cit);
    const Parameter* thirdParam = *cit;
    QVERIFY(thirdParam != nullptr);
    QCOMPARE(thirdParam->name(), "Gamma");
    QCOMPARE(thirdParam->value().toDouble(), 300.0);

    // Test reaching end
    ++cit;
    QVERIFY(cit == constPage.end());



    // ===== Test cbegin() and cend() =====
    // Test that cbegin() != cend() for non-empty list
    QVERIFY(constPage.cbegin() != constPage.cend());

    // Test cbegin() dereferencing
    auto cbegIt = constPage.cbegin();
    const Parameter* cbegParam = *cbegIt;
    QVERIFY(cbegParam != nullptr);
    QCOMPARE(cbegParam->name(), "Alpha");
    QCOMPARE(cbegParam->value().toDouble(), 100.0);

    // Test cbegin() index method
    QCOMPARE(constPage.cbegin().index(), 0);
    auto secondCbegIt = constPage.cbegin();
    ++secondCbegIt;
    QCOMPARE(secondCbegIt.index(), 1);


    // ===== Test bidirectional const iterator operations =====
    // Test decrement from end
    auto lastCit = constPage.cend();
    --lastCit;
    const Parameter* lastParam = *lastCit;
    QVERIFY(lastParam != nullptr);
    QCOMPARE(lastParam->name(), "Gamma");
    QCOMPARE(lastParam->value().toDouble(), 300.0);

    // Test post-decrement
    auto postDecCit = lastCit--;
    QCOMPARE((*postDecCit)->name(), "Gamma");
    QCOMPARE((*lastCit)->name(), "Beta");

    // ===== Test iterator conversion from mutable to const =====

    // Test that mutable iterator can be converted to const iterator
    ParametersPage::iterator mutIt = page.begin();
    ParametersPage::const_iterator constFromMut = mutIt;

    QCOMPARE((*constFromMut)->name(), "Alpha");
    QCOMPARE((*constFromMut)->value().toDouble(), 100.0);

    // Verify they point to the same element
    QVERIFY(*mutIt == *constFromMut);

    // ===== Test const range-based for loop =====
    QStringList constNames;
    QList<double> constValues;

    // Test const range-based for loop
    for (const Parameter* param : constPage) {
        QVERIFY(param != nullptr);
        constNames << param->name();
        constValues << param->value().toDouble();
    }

    QCOMPARE(constNames.size(), 3);
    QCOMPARE(constNames, QStringList({"Alpha", "Beta", "Gamma"}));
    QCOMPARE(constValues, QList<double>({100.0, 200.0, 300.0}));

    // ===== Test that const iterators prevent modification =====
    // This should compile (reading)
    auto constIt = constPage.cbegin();
    QString paramName = (*constIt)->name();
    QCOMPARE(paramName, "Alpha");

    // This would NOT compile (const Parameter* cannot call non-const methods):
    // (*constIt)->setValue(999.0);  // Compilation error - good!

    // ===== Test empty const list =====
    ParametersPage emptyPage("EmptyTest", this);
    const ParametersPage& constEmptyPage = emptyPage;

    QVERIFY(constEmptyPage.begin() == constEmptyPage.end());
    QVERIFY(constEmptyPage.cbegin() == constEmptyPage.cend());

    // Test const range-based for loop with empty list
    int constCount = 0;
    for ([[maybe_unused]] const Parameter* param : constEmptyPage) {
        constCount++;
    }
    QCOMPARE(constCount, 0);
}

void TestQtNoidAppParametersPage::testParametersPageReverseIteratorsAndRangeLoop()
{
    ParametersPage page("TestList", this);
    // Add some parameters
    page.emplace(100.0, "First", "First parameter");
    page.emplace(200.0, "Second", "Second parameter");
    page.emplace(300.0, "Third", "Third parameter");
    page.emplace(400.0, "Fourth", "Fourth parameter");

    // ===== Test rbegin() and rend() basic functionality =====
    QVERIFY(page.rbegin() != page.rend());

    // Test reverse iterator dereferencing - should start from last element
    ParametersPage::reverse_iterator rit = page.rbegin();
    Parameter* lastParam = *rit;
    QVERIFY(lastParam != nullptr);
    QCOMPARE(lastParam->name(), "Fourth");  // Should be the last element
    QCOMPARE(lastParam->value().toDouble(), 400.0);

    // ===== Test reverse iterator increment (moves backward through list) =====
    ++rit;
    QVERIFY(rit != page.rend());
    Parameter* thirdParam = *rit;
    QVERIFY(thirdParam != nullptr);
    QCOMPARE(thirdParam->name(), "Third");
    QCOMPARE(thirdParam->value().toDouble(), 300.0);

    // Test post-increment
    auto prevRit = rit++;
    QVERIFY(rit != page.rend());
    QVERIFY(prevRit != rit);
    Parameter* secondParam = *rit;
    QVERIFY(secondParam != nullptr);
    QCOMPARE(secondParam->name(), "Second");
    QCOMPARE(secondParam->value().toDouble(), 200.0);

    // Continue to first element
    ++rit;
    QVERIFY(rit != page.rend());
    Parameter* firstParam = *rit;
    QVERIFY(firstParam != nullptr);
    QCOMPARE(firstParam->name(), "First");
    QCOMPARE(firstParam->value().toDouble(), 100.0);

    // Test reaching rend()
    ++rit;
    QVERIFY(rit == page.rend());

    // ===== Test reverse iterator range-based for loop simulation =====

    QStringList reverseNames;
    QList<double> reverseValues;
    // Manually iterate through reverse iterators
    for (auto revIt = page.rbegin(); revIt != page.rend(); ++revIt) {
        Parameter* param = *revIt;
        QVERIFY(param != nullptr);
        reverseNames << param->name();
        reverseValues << param->value().toDouble();
    }

    // Verify reverse order
    QCOMPARE(reverseNames.size(), 4);
    QCOMPARE(reverseNames, QStringList({"Fourth", "Third", "Second", "First"}));
    QCOMPARE(reverseValues, QList<double>({400.0, 300.0, 200.0, 100.0}));


    // ===== Test reverse iterator arrow operator =====
    auto rbeginIt = page.rbegin();
    QCOMPARE((*rbeginIt)->name(), "Fourth");
    QCOMPARE((*rbeginIt)->value().toDouble(), 400.0);

    // ===== Test relationship between normal and reverse iterators =====
    // rbegin() should correspond to the element before end()
    auto normalEnd = page.end();
    --normalEnd;
    auto reverseBegin = page.rbegin();
    QCOMPARE((*normalEnd)->name(), (*reverseBegin)->name());
    QCOMPARE((*normalEnd)->value(), (*reverseBegin)->value());

    // ===== Test empty list reverse iterators =====
    ParametersPage emptyPage("EmptyTest", this);
    QVERIFY(emptyPage.rbegin() == emptyPage.rend());
    // Test reverse iteration over empty list
    int reverseCount = 0;
    for (auto revIt = emptyPage.rbegin(); revIt != emptyPage.rend(); ++revIt) {
        reverseCount++;
    }
    QCOMPARE(reverseCount, 0);

    // ===== Test reverse iterator with std::reverse algorithm simulation =====
    // Use reverse iterators to create a reversed copy
    QStringList normalOrder;
    QStringList reversedOrder;
    // Normal order
    for (auto it = page.begin(); it != page.end(); ++it) {
        normalOrder << (*it)->name();
    }

    // Reversed order using reverse iterators
    for (auto rit = page.rbegin(); rit != page.rend(); ++rit) {
        reversedOrder << (*rit)->name();
    }

    // Verify they are opposite
    QCOMPARE(normalOrder, QStringList({"First", "Second", "Third", "Fourth"}));
    QCOMPARE(reversedOrder, QStringList({"Fourth", "Third", "Second", "First"}));

    // ===== Test reverse iterator decrement (moves forward through list) =====

    // Start from rbegin() and use decrement to move "forward" in reverse direction
    auto rLastIt = page.rbegin();
    ++rLastIt; ++rLastIt; ++rLastIt; // Move to "First"
    QCOMPARE((*rLastIt)->name(), "First");

    // Test pre-decrement (should move to "Second")
    --rLastIt;
    QCOMPARE((*rLastIt)->name(), "Second");

    // Test post-decrement
    auto postDecRit = rLastIt--;
    QCOMPARE((*postDecRit)->name(), "Second");
    QCOMPARE((*rLastIt)->name(), "Third");
}

void TestQtNoidAppParametersPage::testParametersPageConstReverseIterators()
{
    ParametersPage page("TestList", this);

    // Add some parameters
    page.emplace(100.0, "First", "First parameter");
    page.emplace(200.0, "Second", "Second parameter");
    page.emplace(300.0, "Third", "Third parameter");
    page.emplace(400.0, "Fourth", "Fourth parameter");

    // Create a const reference to the page
    const ParametersPage& constPage = page;

    // ===== Test rbegin() const and rend() const =====

    // Test that const rbegin() != rend() for non-empty list
    QVERIFY(constPage.rbegin() != constPage.rend());

    // Test const reverse iterator dereferencing - should start from last element
    ParametersPage::const_reverse_iterator crit = constPage.rbegin();
    QVERIFY(crit != constPage.rend());

    const Parameter* lastParam = *crit;
    QVERIFY(lastParam != nullptr);
    QCOMPARE(lastParam->name(), "Fourth");  // Should be the last element
    QCOMPARE(lastParam->value().toDouble(), 400.0);

    // ===== Test const reverse iterator increment (moves backward through list) =====

    // Test pre-increment (moves to previous element in normal order)
    ++crit;
    QVERIFY(crit != constPage.rend());
    const Parameter* thirdParam = *crit;
    QVERIFY(thirdParam != nullptr);
    QCOMPARE(thirdParam->name(), "Third");
    QCOMPARE(thirdParam->value().toDouble(), 300.0);

    // Test post-increment
    auto prevCrit = crit++;
    QVERIFY(crit != constPage.rend());
    QVERIFY(prevCrit != crit);
    const Parameter* secondParam = *crit;
    QVERIFY(secondParam != nullptr);
    QCOMPARE(secondParam->name(), "Second");
    QCOMPARE(secondParam->value().toDouble(), 200.0);

    // Continue to first element
    ++crit;
    QVERIFY(crit != constPage.rend());
    const Parameter* firstParam = *crit;
    QVERIFY(firstParam != nullptr);
    QCOMPARE(firstParam->name(), "First");
    QCOMPARE(firstParam->value().toDouble(), 100.0);

    // Test reaching rend()
    ++crit;
    QVERIFY(crit == constPage.rend());

    // ===== Test crbegin() and crend() =====

    // Test that crbegin() != crend() for non-empty list
    QVERIFY(constPage.crbegin() != constPage.crend());

    // Test crbegin() dereferencing - should start from last element
    ParametersPage::const_reverse_iterator crbit = constPage.crbegin();
    QVERIFY(crbit != constPage.crend());

    const Parameter* crbParam = *crbit;
    QVERIFY(crbParam != nullptr);
    QCOMPARE(crbParam->name(), "Fourth");
    QCOMPARE(crbParam->value().toDouble(), 400.0);

    // Test crbegin() increment
    ++crbit;
    QVERIFY(crbit != constPage.crend());
    const Parameter* crbSecondParam = *crbit;
    QVERIFY(crbSecondParam != nullptr);
    QCOMPARE(crbSecondParam->name(), "Third");
    QCOMPARE(crbSecondParam->value().toDouble(), 300.0);

    // ===== Test const reverse iterator range-based for loop simulation =====

    QStringList constReverseNames;
    QList<double> constReverseValues;

    // Manually iterate through const reverse iterators using rbegin() const
    for (auto constRevIt = constPage.rbegin(); constRevIt != constPage.rend(); ++constRevIt) {
        const Parameter* param = *constRevIt;
        QVERIFY(param != nullptr);
        constReverseNames << param->name();
        constReverseValues << param->value().toDouble();
    }

    // Verify const reverse order
    QCOMPARE(constReverseNames.size(), 4);
    QCOMPARE(constReverseNames, QStringList({"Fourth", "Third", "Second", "First"}));
    QCOMPARE(constReverseValues, QList<double>({400.0, 300.0, 200.0, 100.0}));

    // ===== Test const reverse iterator range-based for loop with crbegin/crend =====

    QStringList crNames;
    QList<double> crValues;

    // Manually iterate through const reverse iterators using crbegin()/crend()
    for (auto crIt = constPage.crbegin(); crIt != constPage.crend(); ++crIt) {
        const Parameter* param = *crIt;
        QVERIFY(param != nullptr);
        crNames << param->name();
        crValues << param->value().toDouble();
    }

    // Verify crbegin/crend produces same result as rbegin/rend const
    QCOMPARE(crNames, constReverseNames);
    QCOMPARE(crValues, constReverseValues);

    // ===== Test const reverse iterator arrow operator =====

    auto crbeginIt = constPage.crbegin();
    QCOMPARE((*crbeginIt)->name(), "Fourth");
    QCOMPARE((*crbeginIt)->value().toDouble(), 400.0);

    auto rbeginConstIt = constPage.rbegin();
    QCOMPARE((*rbeginConstIt)->name(), "Fourth");
    QCOMPARE((*rbeginConstIt)->value().toDouble(), 400.0);

    // ===== Test relationship between const normal and const reverse iterators =====

    // rbegin() const should correspond to the element before end() const
    auto constNormalEnd = constPage.end();
    --constNormalEnd;
    auto constReverseBegin = constPage.rbegin();

    QCOMPARE((*constNormalEnd)->name(), (*constReverseBegin)->name());
    QCOMPARE((*constNormalEnd)->value(), (*constReverseBegin)->value());

    // Same test for crbegin() and cend()
    auto constCEnd = constPage.cend();
    --constCEnd;
    auto constCRBegin = constPage.crbegin();

    QCOMPARE((*constCEnd)->name(), (*constCRBegin)->name());
    QCOMPARE((*constCEnd)->value(), (*constCRBegin)->value());

    // ===== Test empty const list reverse iterators =====

    ParametersPage emptyPage("EmptyTest", this);
    const ParametersPage& constEmptyPage = emptyPage;

    QVERIFY(constEmptyPage.rbegin() == constEmptyPage.rend());
    QVERIFY(constEmptyPage.crbegin() == constEmptyPage.crend());

    // Test const reverse iteration over empty list
    int constReverseCount = 0;
    for (auto constRevIt = constEmptyPage.rbegin(); constRevIt != constEmptyPage.rend(); ++constRevIt) {
        constReverseCount++;
    }
    QCOMPARE(constReverseCount, 0);

    // Test crbegin/crend with empty list
    int crCount = 0;
    for (auto crIt = constEmptyPage.crbegin(); crIt != constEmptyPage.crend(); ++crIt) {
        crCount++;
    }
    QCOMPARE(crCount, 0);

    // ===== Test const correctness =====

    // This should compile (reading from const iterator)
    auto constIt = constPage.crbegin();
    QString paramName = (*constIt)->name();
    QCOMPARE(paramName, "Fourth");

    // This would NOT compile if we tried (const Parameter* cannot call non-const methods):
    // (*constIt)->setValue(999.0);  // Compilation error - good!

    // ===== Test bidirectional const reverse iterator operations =====

    // Test decrement operations on const reverse iterators
    auto constLastRevIt = constPage.rbegin();
    ++constLastRevIt; ++constLastRevIt; ++constLastRevIt; // Move to "First"
    QCOMPARE((*constLastRevIt)->name(), "First");

    // Test pre-decrement (should move to "Second")
    --constLastRevIt;
    QCOMPARE((*constLastRevIt)->name(), "Second");

    // Test post-decrement
    auto postDecConstRevIt = constLastRevIt--;
    QCOMPARE((*postDecConstRevIt)->name(), "Second");
    QCOMPARE((*constLastRevIt)->name(), "Third");
}

void TestQtNoidAppParametersPage::testQDebugOperator()
{
    ParametersPage page1("myApp Settings", this);

    QString output = QDebug::toString(&page1);
    // qDebug() << output;

    QVERIFY(output.contains("\"myApp Settings\""));
    QVERIFY(output.contains("uniqueID"));
    QVERIFY(output.contains("count: 0"));
    QVERIFY(output.contains("visible: true"));

    ParametersPage page2("myApp Settings full", this);
    page2.setDescription("this is my app settings");
    page2.setTooltip("this is my app tooltip");
    page2.emplace(37.1, "Temp", "temperature");
    page2.emplace(101.1, "Press", "pressure");
    page2.emplace(70.1, "Humidity");
    page2.setValue("Humidity", 80.8);

    output = QDebug::toString(&page2);
    // qDebug() <<__func__ << output;

    QVERIFY(output.contains("tooltip"));
    QVERIFY(output.contains("description"));
    QVERIFY(output.contains("Temp"));
    QVERIFY(output.contains("Press"));
    QVERIFY(output.contains("Humidity"));
    QVERIFY(output.contains("modified"));

}

void TestQtNoidAppParametersPage::testParametersPageWithQDebugOperatorWithPointer()
{
    // Test with valid pointer
    ParametersPage* page1 = new ParametersPage("myApp Settings", this);

    QString output = QDebug::toString(page1);
    // qDebug() << output;

    QVERIFY(output.contains("\"myApp Settings\""));
    QVERIFY(output.contains("uniqueID"));
    QVERIFY(output.contains("count: 0"));
    QVERIFY(output.contains("visible: true"));

    delete page1;

    // Test with nullptr
    ParametersPage* list2 = nullptr;
    output = QDebug::toString(list2);
    // qDebug() << output;
    QVERIFY(output.contains("ParametersPage(nullptr)"));

}

void TestQtNoidAppParametersPage::testAboutToBeDestroyedSignal()
{
    auto page = new ParametersPage("TestPage");
    const quintptr expectedAddress = reinterpret_cast<quintptr>(page);

    QSignalSpy spy(page, &ParametersPage::aboutToBeDestroyed);
    QVERIFY(spy.isValid());

    delete page;

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(reinterpret_cast<quintptr>(arguments.at(0).value<ParametersPage*>()), expectedAddress);
}



QTEST_MAIN(TestQtNoidAppParametersPage)
#include "test_app_parameterspage.moc"
