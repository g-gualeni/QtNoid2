#include <QJsonArray>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>
#include <QtNoidApp/QtNoidApp>

class TestQtNoidAppParameter : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testCreatingParameter();
    void testConstructorWithNameAndInitialValue();
    void testConstructorWithNameDescriptionAndInitialValue();

    void testUniqueId();

    void testParameterValueChanged();
    void testParameterValueChangedNoEmitForSameValue();
    void testParameterMin();
    void testParameterMax();
    void testTooltip();
    void testVisible();

    void testParameterSetPresets();
    void testParameterSetPreset();
    void testParameterApplyPresetByName();
    void testParameterRemovePreset();
    void testParameterClearPreset();
    void testParameterSetPresetShouldAbideToRangeRules();
    void testParameterApplyPresetReadOnlyShouldBeIneffective();

    void testParameterRange();
    void testParameterSetRangeShouldHandleFlippingMinAndMaxSoRangeWillBeConsistent();
    void testParameterRangeIsValid();
    void testParameterName();
    void testParameterDescription();
    void testParameterUnit();
    void testParameterTooltip();
    void testParameterReadOnly();

    // Slot tests
    void testOnValueChangedSlot();
    void testOnValueChangedSlotUsingConnect();

    // Bindable properties tests
    void testBindableValue();
    void testBindableValueUsingBidirectionalNotifier();

    void testBindableMin();
    void testBindableMax();
    void testBindablePresets();
    void testBindableName();
    void testBindableDescription();
    void testBindableUnit();
    void testBindableTooltip();
    void testBindableVisible();
    void testBindableReadOnly();


    // ToJSON Schema tests
    void testParameterToJsonSchema();
    void testParameterToJsonSchemaWithNoNameAndEmptyParameters();
    void testParameterToJsonSchemaWithFullMetadataForFloatNumber();
    
    // ToJSON tests
    void testParameterToJson();
    void testParameterToJsonWithNoName();
    void testParameterToJsonWithDifferentValueTypes();
    
    // Value fromJSON deserialization tests
    void testParameterValueFromJson();
    void testParameterValueFromJsonWithEmptyName();
    void testParameterValueFromJsonWithNonExistentKey();
    void testParameterValueFromJsonWithDifferentTypes();
    
    // Schema fromJSON schema loading tests
    void testParameterSchemaFromJson();
    void testParameterSchemaFromJsonWithEmptyName();
    void testParameterSchemaFromJsonWithNonExistentKey();
    void testParameterSchemaFromJsonEmptyObject();
    void testParameterSchemaFromJsonPartialSchemaShouldSetUnspecifiedParamtersToDefault();
    void testParameterSchemaFromJsonChangeANewRangeShouldForceANewValue();

    // fromJSON constructor tests
    void testParameterConstructorFromJsonSchemaAndValue();
    void testParameterConstructorFromJsonSchemaOnly();
    void testParameterConstructorFromJsonValueOnly();
    void testParameterConstructorFromJsonEmptyObjects();

    // isValid() method tests
    void testParameterIsValid();

    // fromJson method tests
    void testParameterFromJson();
    void testParameterFromJsonShouldUpdateAnExistingParameter();
    void testParameterFromJsonWithInvalidSchemaShouldLeaveTheParameterUnchanged();
    void testParameterFromJsonWithInvalidValueShouldLeaveValueUnchanged();
    void testParameterFromJsonWithIncompleteSchemaShouldOverrideExisitingSchema();

private:

};

using namespace QtNoid::App;


void TestQtNoidAppParameter::initTestCase()
{}

void TestQtNoidAppParameter::cleanupTestCase()
{}

void TestQtNoidAppParameter::init()
{}

void TestQtNoidAppParameter::cleanup()
{}

void TestQtNoidAppParameter::testCreatingParameter()
{
    auto par = Parameter(this);
    par.setValue(123.456);
    par.setMin(-1000);
    par.setMax(1000);
    par.setName("ParameterName");
    par.setDescription("This is the first time");

    QCOMPARE(par.value(), 123.456);
    QCOMPARE(par.min(), -1000);
    QCOMPARE(par.max(), 1000);

    QCOMPARE(par.name(), "ParameterName");
    QCOMPARE(par.description(), "This is the first time");

    par.setUnit("°C");
    QCOMPARE(par.unit(), "°C");

}

void TestQtNoidAppParameter::testConstructorWithNameAndInitialValue()
{
    auto par = Parameter(456.789, "Param", this);
    
    QCOMPARE(par.name(), "Param");
    QCOMPARE(par.value(), 456.789);
    QCOMPARE(par.description(), QString());
    QCOMPARE(par.unit(), QString());
    QCOMPARE(par.readOnly(), false);
    QCOMPARE(par.min(), QVariant());
    QCOMPARE(par.max(), QVariant());
}

void TestQtNoidAppParameter::testConstructorWithNameDescriptionAndInitialValue()
{
    // I use __func__ as a description
    auto par = Parameter(789.123, "Param", __func__, this);
    
    QCOMPARE(par.name(), "Param");
    QCOMPARE(par.description(), __func__);
    QCOMPARE(par.value(), 789.123);
    QCOMPARE(par.unit(), QString());
    QCOMPARE(par.tooltip(), QString());
    QCOMPARE(par.readOnly(), false);
    QCOMPARE(par.min(), QVariant());
    QCOMPARE(par.max(), QVariant());
}

void TestQtNoidAppParameter::testUniqueId()
{
    Parameter par(this);
    // Paramter should have a uniqueID
    QVERIFY(par.uniqueId() != 0);

    Parameter newPar(par.toJsonSchema(), par.toJsonValue(), this);
    QVERIFY(par.uniqueId() != newPar.uniqueId());

    // Unique ID should be consecutive
    QCOMPARE(par.uniqueId()+1, newPar.uniqueId());
}

void TestQtNoidAppParameter::testParameterValueChanged()
{
    auto par = Parameter(1000);
    QSignalSpy spy(&par, &Parameter::valueChanged);
    QVERIFY(spy.isValid());

    // Change the value and verify the signal is emitted
    double expected = 500.00001;
    par.setValue(expected);
    QCOMPARE(spy.count(), 1);

    // check the value is the correct one
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toDouble(), expected);

}

void TestQtNoidAppParameter::testParameterValueChangedNoEmitForSameValue()
{
    Parameter par(100);
    QSignalSpy spy(&par, &Parameter::valueChanged);

    // Same value, no activation
    par.setValue(100);
    QCOMPARE(spy.count(), 0);

}

void TestQtNoidAppParameter::testParameterMin()
{
    Parameter par(100);
    QSignalSpy spyValue(&par, &Parameter::valueChanged);
    QSignalSpy spyMin(&par, &Parameter::minChanged);
    QSignalSpy spyRange(&par, &Parameter::rangeChanged);


    // Change the min check value and range
    QVariant expected = 101;
    par.setMin(expected);
    QCOMPARE(spyValue.count(), 1);
    QCOMPARE(spyValue.takeFirst().at(0), expected);

    QCOMPARE(spyMin.count(), 1);
    QCOMPARE(spyMin.takeFirst().at(0), expected);


    QCOMPARE(spyRange.count(), 1);
    auto rangeChanged = spyRange.takeFirst();
    QCOMPARE(rangeChanged.at(0), expected);
    QCOMPARE(rangeChanged.at(1), QVariant());

}

void TestQtNoidAppParameter::testParameterMax()
{
    Parameter par(100);
    QSignalSpy spyValue(&par, &Parameter::valueChanged);
    QSignalSpy spyMax(&par, &Parameter::maxChanged);
    QSignalSpy spyRange(&par, &Parameter::rangeChanged);


    // Change the max check value and range
    QVariant expected = 99;
    par.setMax(expected);
    QCOMPARE(spyValue.count(), 1);
    QCOMPARE(spyValue.takeFirst().at(0), expected);

    QCOMPARE(spyMax.count(), 1);
    QCOMPARE(spyMax.takeFirst().at(0), expected);


    QCOMPARE(spyRange.count(), 1);
    auto rangeChanged = spyRange.takeFirst();
    QCOMPARE(rangeChanged.at(0), QVariant());
    QCOMPARE(rangeChanged.at(1), expected);

}

void TestQtNoidAppParameter::testTooltip()
{
    Parameter par(42.0, "TestParam", "A test parameter");
    QSignalSpy spy(&par, &Parameter::tooltipChanged);

    // Test initial state (should be empty)
    QCOMPARE(par.tooltip(), QString());

    // Set tooltip and verify signal emission
    par.setTooltip("This is a tooltip");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(par.tooltip(), "This is a tooltip");

    // Check signal argument
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "This is a tooltip");

    // Setting same value should not emit signal
    par.setTooltip("This is a tooltip");
    QCOMPARE(spy.count(), 0);

    // Change to different value
    par.setTooltip("New tooltip");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(par.tooltip(), "New tooltip");
}

void TestQtNoidAppParameter::testVisible()
{
    Parameter par(42.0, "TestParam", "A test parameter");
    QSignalSpy spy(&par, &Parameter::visibleChanged);

    // Test initial state (should be true by default)
    QCOMPARE(par.visible(), true);

    // Set visible to false and verify signal emission
    par.setVisible(false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(par.visible(), false);

    // Check signal argument
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toBool(), false);

    // Setting same value should not emit signal
    par.setVisible(false);
    QCOMPARE(spy.count(), 0);

    // Change back to true
    par.setVisible(true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(par.visible(), true);
}

void TestQtNoidAppParameter::testParameterSetPresets()
{
    Parameter par(100.0);
    QSignalSpy spy(&par, &Parameter::presetsChanged);

    // Test initial state (should be empty)
    QCOMPARE(par.presets(), QVariantMap());

    // Create test presets
    QVariantMap expected;
    expected["Low"] = 10.0;
    expected["Medium"] = 50.0;
    expected["High"] = 100.0;

    // Set presets and verify signal is emitted
    par.setPresets(expected);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(par.presets(), expected);

    // Check signal contains the correct presets
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toMap(), expected);

    // Same presets, no signal should be emitted
    par.setPresets(expected);
    QCOMPARE(spy.count(), 0);

    qDebug() << par.presets();
}

void TestQtNoidAppParameter::testParameterSetPreset()
{
    Parameter par(100.0);
    QSignalSpy spy(&par, &Parameter::presetsChanged);

    par.setPreset("Low", 10.0);
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.first(), QVariantMap({{"Low", 10.0}}));
    QCOMPARE(par.presets(), QVariantMap({{"Low", 10.0}}));

    QCOMPARE(par.preset("Low"), 10.0);
}


void TestQtNoidAppParameter::testParameterApplyPresetByName()
{
    Parameter par(50.0);
    QSignalSpy spyValue(&par, &Parameter::valueChanged);

    par.setPreset("Low", 10.0);
    par.applyPreset("Low");
    QCOMPARE(spyValue.count(), 1);
    auto newVal = spyValue.takeFirst();
    QCOMPARE(newVal.first(), 10.0);
    QCOMPARE(par.value(), 10.0);

    // Applying the same preset, there should be no changes
    par.applyPreset("Low");
    QCOMPARE(spyValue.count(), 0);
}

void TestQtNoidAppParameter::testParameterRemovePreset()
{
    Parameter par(100.0);

    // Set up initial presets
    par.setPreset("Low", 10.0);
    par.setPreset("Medium", 50.0);
    par.setPreset("High", 100.0);

    // Remove one preset
    QSignalSpy spy(&par, &Parameter::presetsChanged);
    par.removePreset("Medium");
    QCOMPARE(spy.count(), 1);
    
    QVariantMap expected;
    expected["Low"] = 10.0;
    expected["High"] = 100.0;
    QCOMPARE(par.presets(), expected);

    // Verify signal contains updated presets
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.first().toMap(), expected);

    // Remove non-existent preset should not emit signal
    par.removePreset("NonExistent");
    QCOMPARE(spy.count(), 0);
}

void TestQtNoidAppParameter::testParameterClearPreset()
{
    Parameter par(100.0);

    // Set up initial presets
    par.setPreset("Low", 10.0);
    par.setPreset("Medium", 50.0);
    par.setPreset("High", 100.0);

    // Clear all presets
    QSignalSpy spy(&par, &Parameter::presetsChanged);
    par.clearPresets();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(par.presets(), QVariantMap());

    // Verify signal contains empty presets
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.first().toMap(), QVariantMap());

    // Clear already empty presets should not emit signal
    par.clearPresets();
    QCOMPARE(spy.count(), 0);
}

void TestQtNoidAppParameter::testParameterSetPresetShouldAbideToRangeRules()
{
    Parameter par(50.0);
    par.setRange(0.0, 100.0);
    
    // Apply preset exactly at min boundary
    par.setPreset("AtMin", 0.0);
    par.applyPreset("AtMin");
    QCOMPARE(par.value(), 0.0);

    // Apply preset exactly at max boundary
    par.setPreset("AtMax", 100.0);
    par.applyPreset("AtMax");
    QCOMPARE(par.value(), 100.0);

    // Apply preset below min range - should be clamped to min
    par.setPreset("TooLow", -10.0);
    par.applyPreset("TooLow");
    QCOMPARE(par.value(), 0.0); // Clamped to min

    // Apply preset above max range - should be clamped to max
    par.setPreset("TooHigh", 150.0);
    par.applyPreset("TooHigh");
    QCOMPARE(par.value(), 100.0); // Clamped to max

}

void TestQtNoidAppParameter::testParameterApplyPresetReadOnlyShouldBeIneffective()
{
    Parameter par(50.0, "ReadOnlyParam");
    par.setReadOnly(true);
    
    // Set up presets
    par.setPreset("Low", 10.0);
    
    // Set up signal spies
    QSignalSpy valueChangedSpy(&par, &Parameter::valueChanged);
    QSignalSpy writeAttemptSpy(&par, &Parameter::writeAttemptedWhileReadOnly);
    
    // Try to apply preset when read-only
    par.applyPreset("Low");
    // Value should not change
    QCOMPARE(par.value(), 50.0);
    QCOMPARE(valueChangedSpy.count(), 0);
    
    // writeAttemptedWhileReadOnly signal should be emitted
    QCOMPARE(writeAttemptSpy.count(), 1);
    QList<QVariant> arguments = writeAttemptSpy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "ReadOnlyParam");

    // Try to apply the same value and there should be no errors
    par.setPreset("asItIsNow", par.value());
    auto res = par.applyPreset("asItIsNow");
    QCOMPARE(res, true);
    QCOMPARE(writeAttemptSpy.count(), 0);
    QCOMPARE(valueChangedSpy.count(), 0);
}

void TestQtNoidAppParameter::testParameterRange()
{
    auto par = Parameter(1000);

    // Apply Range to the value
    par.setRange(-10, 10);
    QCOMPARE(par.value(), 10);

    // Apply a value and check it is clipped to the max
    par.setValue(100);
    QCOMPARE(par.value(), 10);

    // Apply a value and check it is clipped to the min
    par.setValue(-100);
    QCOMPARE(par.value(), -10);
}

void TestQtNoidAppParameter::testParameterSetRangeShouldHandleFlippingMinAndMaxSoRangeWillBeConsistent()
{
    Parameter par(1000, "Par", this);

    par.setRange(1200, 10);
    QVERIFY(par.isValid());
    QCOMPARE(par.min(), 10);
    QCOMPARE(par.max(), 1200);

    auto range = par.range();
    QCOMPARE(range.first, 10);
    QCOMPARE(range.second, 1200);

    // Setting Range using std::pair
    std::pair<QVariant, QVariant> newRange({1300, 11});
    par.setRange(newRange);
    QVERIFY(par.isValid());
    QCOMPARE(par.min(), 11);
    QCOMPARE(par.max(), 1300);

    // Setting Range using schema
    QJsonObject schemaObj{{"min", 222}, {"max", 2}};
    QJsonObject schema{{"Par", schemaObj}};
    par.schemaFromJson(schema);
    range = par.range();
    QCOMPARE(range.first, 2);
    QCOMPARE(range.second, 222);
}

void TestQtNoidAppParameter::testParameterRangeIsValid()
{
    Parameter par(50.0, "TestParam", this);

    // Test case 1: No range set (both min and max are invalid) - should return false
    QCOMPARE(par.rangeIsValid(), false);

    // Test case 2: Only min set (max is invalid) - should return false
    par.setMin(0.0);
    QCOMPARE(par.rangeIsValid(), false);

    // Test case 3: Only max set (min is invalid) - should return false
    par.setMin(QVariant()); // Reset min to invalid
    par.setMax(100.0);
    QCOMPARE(par.rangeIsValid(), false);

    // Test case 4: Valid range with min < max - should return true
    par.setMin(0.0);
    par.setMax(100.0);
    QCOMPARE(par.rangeIsValid(), true);

    // Test case 5: Valid range with min = max - should return true
    par.setRange(50.0, 50.0);
    QCOMPARE(par.rangeIsValid(), true);

    // Test case 6: Invalid range with min > max - should return false
    // because there is no control in consinstency
    par.setMin(100.0);
    par.setMax(0.0);
    QCOMPARE(par.rangeIsValid(), false);


    // Test case 7: Test with string ranges
    Parameter parString("middle", "StringParam", this);
    QCOMPARE(parString.rangeIsValid(), false); // No range set

    parString.setRange("a", "z");
    QCOMPARE(parString.rangeIsValid(), true); // Valid string range

    QVERIFY(0);
    parString.setRange("z", "a");
    QCOMPARE(parString.rangeIsValid(), false); // Invalid string range (z > a)

    // Test case 8: Test with negative numbers
    Parameter parNegative(-50.0, "NegativeParam", this);
    parNegative.setRange(-100.0, -10.0);
    QCOMPARE(parNegative.rangeIsValid(), true);

    parNegative.setRange(-10.0, -100.0);
    QCOMPARE(parNegative.rangeIsValid(), false);

    // Test case 9: Test edge case with zero
    Parameter parZero(0.0, "ZeroParam", this);
    parZero.setRange(-1.0, 1.0);
    QCOMPARE(parZero.rangeIsValid(), true);

    parZero.setRange(1.0, -1.0);
    QCOMPARE(parZero.rangeIsValid(), false);
}

void TestQtNoidAppParameter::testParameterName()
{
    Parameter par(12500, "EarthRadius");
    QSignalSpy spy1(&par, &Parameter::nameChanged);
    QSignalSpy spy2(&par, &Parameter::nameEdited);

    par.setName("EarthDiameter");
    QCOMPARE(spy1.count(), 1);
    QCOMPARE(spy2.count(), 1);

    // check the value is the correct one
    QList<QVariant> arguments = spy1.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "EarthDiameter");
    arguments = spy2.takeFirst();
    qDebug() << __func__ << arguments;
    QCOMPARE(arguments.at(0).toString(), "EarthRadius");
    QCOMPARE(arguments.at(1).toString(), "EarthDiameter");

    // Same value, no activation
    par.setName("EarthDiameter");
    QCOMPARE(spy1.count(), 0);
    QCOMPARE(spy2.count(), 0);
}

void TestQtNoidAppParameter::testParameterDescription()
{
    Parameter par(12500, "EarthDiameter", "this is the Earth diameter", this);
    QSignalSpy spy(&par, &Parameter::descriptionChanged);

    par.setDescription("this is the Earth diameter in km");
    QCOMPARE(spy.count(), 1);

    // check the value is the correct one
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "this is the Earth diameter in km");

    // Same value, no activation
    par.setDescription("this is the Earth diameter in km");
    QCOMPARE(spy.count(), 0);

}

void TestQtNoidAppParameter::testParameterUnit()
{
    Parameter par(25.0, "Temperature", "Current temperature", this);
    QSignalSpy spy(&par, &Parameter::unitChanged);

    par.setUnit("°C");
    QCOMPARE(spy.count(), 1);

    // check the value is the correct one
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "°C");

    // Same value, no activation
    par.setUnit("°C");
    QCOMPARE(spy.count(), 0);
}

void TestQtNoidAppParameter::testParameterTooltip()
{
    Parameter par(25.0, "Temperature", "Current temperature", this);
    QSignalSpy spy(&par, &Parameter::tooltipChanged);

    // Test initial state (should be empty)
    QCOMPARE(par.tooltip(), QString());

    par.setTooltip("This shows the current temperature reading");
    QCOMPARE(spy.count(), 1);

    // check the value is the correct one
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "This shows the current temperature reading");
    QCOMPARE(par.tooltip(), "This shows the current temperature reading");

    // Same value, no activation
    par.setTooltip("This shows the current temperature reading");
    QCOMPARE(spy.count(), 0);
}

void TestQtNoidAppParameter::testParameterReadOnly()
{
    Parameter par(100.0, "ReadOnlyParamName", "Test parameter", this);
    
    // Test initial state (should be writable)
    QCOMPARE(par.readOnly(), false);
    
    // Test setting readOnly property
    QSignalSpy readOnlyChangeSpy(&par, &Parameter::readOnlyChanged);
    par.setReadOnly(true);
    QCOMPARE(readOnlyChangeSpy.count(), 1);
    QCOMPARE(par.readOnly(), true);
    
    // Test that setValue emits writeAttemptedWhileReadOnly signal when readOnly
    QSignalSpy writeAttemptSpy(&par, &Parameter::writeAttemptedWhileReadOnly);
    QSignalSpy valueChangeSpy(&par, &Parameter::valueChanged);
    
    par.setValue(200.0);
    QCOMPARE(writeAttemptSpy.count(), 1);

    // Signal contains the parameter name
    QList<QVariant> arguments = writeAttemptSpy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "ReadOnlyParamName");


    QCOMPARE(valueChangeSpy.count(), 0);
    QCOMPARE(par.value(), 100.0); // Value should not change
    
    // Test that setValue works normally when readOnly is false
    par.setReadOnly(false);
    par.setValue(200.0);
    QCOMPARE(writeAttemptSpy.count(), 0); // No additional signal
    QCOMPARE(valueChangeSpy.count(), 1);
    QCOMPARE(par.value(), 200.0);
}

void TestQtNoidAppParameter::testOnValueChangedSlot()
{
    Parameter par(100.0, "TestParam", this);
    QCOMPARE(par.value(), 100.0);

    QSignalSpy valueChangedSpy(&par, &Parameter::valueChanged);

    // Directly Call the slot with a new value
    QVariant newValue = 250.0;
    par.onValueChanged(newValue);

    // Verify the value was changed
    QCOMPARE(par.value(), 250.0);
    QCOMPARE(valueChangedSpy.count(), 1);

    // Check that the signal was emitted with the correct argument
    QList<QVariant> arguments = valueChangedSpy.takeFirst();
    QCOMPARE(arguments.at(0), newValue);

    // Test with a string value
    QVariant stringValue = "TestString";
    par.onValueChanged(stringValue);
    QCOMPARE(par.value(), stringValue);
    QCOMPARE(valueChangedSpy.count(), 1);
    arguments = valueChangedSpy.takeFirst();
    QCOMPARE(arguments.at(0), stringValue);

    // Test with the same value should not fire valueChanged
    par.onValueChanged(stringValue);
    QCOMPARE(par.value(), stringValue);
    QCOMPARE(valueChangedSpy.count(), 0);

    // Test slot with range constraints
    par.setValue(50.0);
    par.setRange(0.0, 100.0);
    valueChangedSpy.clear();

    // Call slot with value that should be clamped
    par.onValueChanged(150.0);
    QCOMPARE(par.value(), 100.0); // Should be clamped to max
    QCOMPARE(valueChangedSpy.count(), 1);


    // Call slot with value below minimum
    valueChangedSpy.clear();
    par.onValueChanged(-10.0);
    QCOMPARE(par.value(), 0.0); // Should be clamped to min
    QCOMPARE(valueChangedSpy.count(), 1);

    // Test slot behavior with read-only parameter
    par.setReadOnly(true);
    QSignalSpy writeAttemptSpy(&par, &Parameter::writeAttemptedWhileReadOnly);
    valueChangedSpy.clear();
    par.onValueChanged(75.0);
    QCOMPARE(par.value(), 0.0); // Value should not change when read-only
    QCOMPARE(valueChangedSpy.count(), 0);
    QCOMPARE(writeAttemptSpy.count(), 1);
    QList<QVariant> writeAttemptArgs = writeAttemptSpy.takeFirst();
    // Verify the writeAttemptedWhileReadOnly signal contains the parameter name
    QCOMPARE(writeAttemptArgs.at(0).toString(), "TestParam");
}

void TestQtNoidAppParameter::testOnValueChangedSlotUsingConnect()
{
    Parameter parLeader(100.0, this);
    QCOMPARE(parLeader.value(), 100.0);

    // Create a follower object and
    Parameter parFollower(10.0, this);
    QCOMPARE(parFollower.value(), 10.0);
    QSignalSpy valueChangedSpy(&parFollower, &Parameter::valueChanged);
    connect(&parLeader, &Parameter::valueChanged, &parFollower, &Parameter::onValueChanged);

    parLeader.setValue(123.0);
    QCOMPARE(parFollower.value(), 123.0);
}

void TestQtNoidAppParameter::testBindableValue()
{
    Parameter par(100.0, this);
    
    // Get bindable value
    auto bindableValue = par.bindableValue();
    QVERIFY(bindableValue.isValid());
    
    // Test binding to another QProperty
    QProperty<QVariant> externalProperty;
    externalProperty.setBinding([&]() { return par.bindableValue().value(); });
    QCOMPARE(externalProperty.value(), 100.0);
    
    // Change parameter value and verify binding updates
    par.setValue(200.0);
    QCOMPARE(externalProperty.value(), 200.0);
    
    // Test setting value through bindable
    bindableValue.setValue(300.0);
    QCOMPARE(par.value(), 300.0);
    QCOMPARE(externalProperty.value(), 300.0);
}

void TestQtNoidAppParameter::testBindableValueUsingBidirectionalNotifier()
{
    Parameter par(1.0, this);
    QProperty<QVariant> prop(2.0);

    auto propNotifier = prop.addNotifier([&]{
        // qDebug() << "prop changed -> " << prop.value();
        par.setValue(prop.value());
    });

    auto parNotifier = par.bindableValue().addNotifier([&]{
        // qDebug() << "par changed -> " << par.value();
        prop.setValue(par.value());
    });
    //  Change the property and check the parameter
    prop.setValue(123);
    QCOMPARE(par.value(), 123);
    //  Change the parameter and check the property
    par.setValue(456);
    QCOMPARE(prop.value(), 456);
}

void TestQtNoidAppParameter::testBindableMin()
{
    Parameter par(50.0, this);
    
    // Get bindable min
    auto bindableMin = par.bindableMin();
    QVERIFY(bindableMin.isValid());
    QCOMPARE(bindableMin.value(), QVariant());
    
    // Test binding to another QProperty
    QProperty<QVariant> externalProperty;
    externalProperty.setBinding([&]() { return par.bindableMin().value(); });
    QCOMPARE(externalProperty.value(), QVariant());
    
    // Change parameter min and verify binding updates
    par.setMin(10.0);
    QCOMPARE(externalProperty.value(), 10.0);
    
    // Test setting min through bindable
    bindableMin.setValue(5.0);
    QCOMPARE(par.min(), 5.0);
    QCOMPARE(externalProperty.value(), 5.0);
}

void TestQtNoidAppParameter::testBindableMax()
{
    Parameter par(50.0, this);
    
    // Get bindable max
    auto bindableMax = par.bindableMax();
    QVERIFY(bindableMax.isValid());
    QCOMPARE(bindableMax.value(), QVariant());
    
    // Test binding to another QProperty
    QProperty<QVariant> externalProperty;
    externalProperty.setBinding([&]() { return par.bindableMax().value(); });
    QCOMPARE(externalProperty.value(), QVariant());
    
    // Change parameter max and verify binding updates
    par.setMax(100.0);
    QCOMPARE(externalProperty.value(), 100.0);
    
    // Test setting max through bindable
    bindableMax.setValue(200.0);
    QCOMPARE(par.max(), 200.0);
    QCOMPARE(externalProperty.value(), 200.0);
}

void TestQtNoidAppParameter::testBindablePresets()
{
    Parameter par(50.0, this);
    
    // Get bindable presets
    auto bindablePresets = par.bindablePresets();
    QVERIFY(bindablePresets.isValid());
    QCOMPARE(bindablePresets.value(), QVariantMap());
    
    // Test binding to another QProperty
    QProperty<QVariantMap> externalProperty;
    externalProperty.setBinding([&]() { return par.bindablePresets().value(); });
    QCOMPARE(externalProperty.value(), QVariantMap());
    
    // Change parameter presets and verify binding updates
    QVariantMap testPresets;
    testPresets["Low"] = 10.0;
    testPresets["High"] = 90.0;
    par.setPresets(testPresets);
    QCOMPARE(externalProperty.value(), testPresets);
    
    // Test setting presets through bindable
    QVariantMap newPresets;
    newPresets["Medium"] = 50.0;
    bindablePresets.setValue(newPresets);
    QCOMPARE(par.presets(), newPresets);
    QCOMPARE(externalProperty.value(), newPresets);
}

void TestQtNoidAppParameter::testBindableName()
{
    Parameter par(50.0, "TestParam", this);
    
    // Get bindable name
    auto bindableName = par.bindableName();
    QVERIFY(bindableName.isValid());
    QCOMPARE(bindableName.value(), "TestParam");
    
    // Test binding to another QProperty
    QProperty<QString> externalProperty;
    externalProperty.setBinding([&]() { return par.bindableName().value(); });
    QCOMPARE(externalProperty.value(), "TestParam");
    
    // Change parameter name and verify binding updates
    par.setName("NewName");
    QCOMPARE(externalProperty.value(), "NewName");
    
    // Test setting name through bindable
    bindableName.setValue("FinalName");
    QCOMPARE(par.name(), "FinalName");
    QCOMPARE(externalProperty.value(), "FinalName");
}

void TestQtNoidAppParameter::testBindableDescription()
{
    Parameter par(50.0, "TestParam", "Initial description", this);
    
    // Get bindable description
    auto bindableDescription = par.bindableDescription();
    QVERIFY(bindableDescription.isValid());
    QCOMPARE(bindableDescription.value(), "Initial description");
    
    // Test binding to another QProperty
    QProperty<QString> externalProperty;
    externalProperty.setBinding([&]() { return par.bindableDescription().value(); });
    QCOMPARE(externalProperty.value(), "Initial description");
    
    // Change parameter description and verify binding updates
    par.setDescription("Updated description");
    QCOMPARE(externalProperty.value(), "Updated description");
    
    // Test setting description through bindable
    bindableDescription.setValue("Final description");
    QCOMPARE(par.description(), "Final description");
    QCOMPARE(externalProperty.value(), "Final description");
}

void TestQtNoidAppParameter::testBindableUnit()
{
    Parameter par(25.0, "Temperature", this);

    // Get bindable unit
    auto bindableUnit = par.bindableUnit();
    QVERIFY(bindableUnit.isValid());
    QCOMPARE(bindableUnit.value(), QString());

    // Test binding to another QProperty
    QProperty<QString> externalProperty;
    externalProperty.setBinding([&]() { return par.bindableUnit().value(); });
    QCOMPARE(externalProperty.value(), QString());

    // Change parameter unit and verify binding updates
    par.setUnit("°C");
    QCOMPARE(externalProperty.value(), "°C");

    // Test setting unit through bindable
    bindableUnit.setValue("°F");
    QCOMPARE(par.unit(), "°F");
    QCOMPARE(externalProperty.value(), "°F");
}

void TestQtNoidAppParameter::testBindableTooltip()
{
    Parameter par(25.0, "Temperature", this);

    // Get bindable tooltip
    auto bindableTooltip = par.bindableTooltip();
    QVERIFY(bindableTooltip.isValid());
    QCOMPARE(bindableTooltip.value(), QString());

    // Test binding to another QProperty
    QProperty<QString> externalProperty;
    externalProperty.setBinding([&]() { return par.bindableTooltip().value(); });
    QCOMPARE(externalProperty.value(), QString());

    // Change parameter tooltip and verify binding updates
    par.setTooltip("Temperature sensor reading");
    QCOMPARE(externalProperty.value(), "Temperature sensor reading");

    // Test setting tooltip through bindable
    bindableTooltip.setValue("Updated tooltip");
    QCOMPARE(par.tooltip(), "Updated tooltip");
    QCOMPARE(externalProperty.value(), "Updated tooltip");
}

void TestQtNoidAppParameter::testBindableVisible()
{
    Parameter par(50.0, "TestParam", this);

    // Get bindable visible
    auto bindableVisible = par.bindableVisible();
    QVERIFY(bindableVisible.isValid());
    QCOMPARE(bindableVisible.value(), true);

    // Test binding to another QProperty
    QProperty<bool> externalProperty;
    externalProperty.setBinding([&]() { return par.bindableVisible().value(); });
    QCOMPARE(externalProperty.value(), true);

    // Change parameter visible and verify binding updates
    par.setVisible(false);
    QCOMPARE(externalProperty.value(), false);

    // Test setting visible through bindable
    bindableVisible.setValue(true);
    QCOMPARE(par.visible(), true);
    QCOMPARE(externalProperty.value(), true);
}

void TestQtNoidAppParameter::testBindableReadOnly()
{
    Parameter par(50.0, "TestParam", this);
    
    // Get bindable readOnly
    auto bindableReadOnly = par.bindableReadOnly();
    QVERIFY(bindableReadOnly.isValid());
    QCOMPARE(bindableReadOnly.value(), false);
    
    // Test binding to another QProperty
    QProperty<bool> externalProperty;
    externalProperty.setBinding([&]() { return par.bindableReadOnly().value(); });
    QCOMPARE(externalProperty.value(), false);
    
    // Change parameter readOnly and verify binding updates
    par.setReadOnly(true);
    QCOMPARE(externalProperty.value(), true);
    
    // Test setting readOnly through bindable
    bindableReadOnly.setValue(false);
    QCOMPARE(par.readOnly(), false);
    QCOMPARE(externalProperty.value(), false);
}

void TestQtNoidAppParameter::testParameterToJsonSchema()
{
    Parameter par(50.0, "Log File Size", "Parameter description", this);

    par.setUnit("kB");
    par.setTooltip("Size of the log file on disk");
    par.setMin(0.0);
    par.setMax(100.0);
    par.setReadOnly(true);
    par.setPreset("Small", 2.0);
    par.setPreset("Large", 90.0);
    
    QJsonObject schema = par.toJsonSchema();
    
    QVERIFY(schema.contains("Log File Size"));
    QJsonObject paramSchema = schema["Log File Size"].toObject();
    
    QCOMPARE(paramSchema["description"].toString(), "Parameter description");
    QCOMPARE(paramSchema["unit"].toString(), "kB");
    QCOMPARE(paramSchema["tooltip"].toString(), "Size of the log file on disk");
    QCOMPARE(paramSchema["readOnly"].toBool(), true);
    QCOMPARE(paramSchema["min"].toVariant(), QVariant(0.0));
    QCOMPARE(paramSchema["max"].toVariant(), QVariant(100.0));

    QVERIFY(paramSchema.contains("presets"));
    QJsonArray presetArray = paramSchema["presets"].toArray();
    QHash<QString, QVariant> presetMap;

    for(auto it = presetArray.constBegin(); it != presetArray.constEnd(); ++it) {
        QJsonObject element = it->toObject();
        QString key = element.begin().key();
        QVariant value = element.begin().value().toVariant();
        presetMap.emplace(key, value);
    }
    QCOMPARE(presetMap["Small"], QVariant(2.0));
    QCOMPARE(presetMap["Large"], QVariant(90.0));
}

void TestQtNoidAppParameter::testParameterToJsonSchemaWithNoNameAndEmptyParameters()
{
    Parameter par(42.0);
    
    QJsonObject schema = par.toJsonSchema();
    // qDebug() << schema;
    
    QVERIFY(schema.contains("Name"));
    QJsonObject paramSchema = schema["Name"].toObject();
    
    QCOMPARE(paramSchema["description"].toString(), QString());
    QCOMPARE(paramSchema["unit"].toString(), QString());
    QCOMPARE(paramSchema["tooltip"].toString(), QString());
    QCOMPARE(paramSchema["readOnly"].toBool(), false);
    QVERIFY(paramSchema["min"].isNull());
    QVERIFY(paramSchema["max"].isNull());
    QCOMPARE(paramSchema.contains("presets"), false);
}

void TestQtNoidAppParameter::testParameterToJsonSchemaWithFullMetadataForFloatNumber()
{
    Parameter par(75.5, "ComplexParam", "A complex parameter with all metadata", this);
    par.setUnit("°C");
    par.setMin(-273.15);
    par.setMax(1000.0);
    par.setReadOnly(false);
    par.setPreset("Small", 2.0);
    par.setPreset("Large", 90.0);
    
    QJsonObject schema = par.toJsonSchema();
    
    QVERIFY(schema.contains("ComplexParam"));
    QJsonObject paramSchema = schema["ComplexParam"].toObject();
    
    QCOMPARE(paramSchema["description"].toString(), "A complex parameter with all metadata");
    QCOMPARE(paramSchema["unit"].toString(), "°C");
    QCOMPARE(paramSchema["readOnly"].toBool(), false);
    QCOMPARE(paramSchema["min"].toVariant(), QVariant(-273.15));
    QCOMPARE(paramSchema["max"].toVariant(), QVariant(1000.0));
    QCOMPARE(paramSchema.contains("presets"), true);
}

void TestQtNoidAppParameter::testParameterToJson()
{
    Parameter par(25.5, "Temperature", "Current temperature", this);
    
    QJsonObject json = par.toJsonValue();
    // qDebug() << __func__ << json;
    
    QVERIFY(json.contains("Temperature"));
    QCOMPARE(json["Temperature"].toVariant(), QVariant(25.5));
    QCOMPARE(json.size(), 1);
}

void TestQtNoidAppParameter::testParameterToJsonWithNoName()
{
    Parameter par(42.0);
    
    QJsonObject json = par.toJsonValue();
    // qDebug() << __func__ << json;

    QVERIFY(json.contains("Name"));
    QCOMPARE(json["Name"].toVariant(), QVariant(42.0));
    QCOMPARE(json.size(), 1);
}

void TestQtNoidAppParameter::testParameterToJsonWithDifferentValueTypes()
{
    // Test with integer
    Parameter parInt(100, "IntParam", this);
    QJsonObject jsonInt = parInt.toJsonValue();
    QVERIFY(jsonInt.contains("IntParam"));
    QCOMPARE(jsonInt["IntParam"].toVariant(), QVariant(100));
    
    // Test with string
    Parameter parString("Hello World", "StringParam", this);
    QJsonObject jsonString = parString.toJsonValue();
    QVERIFY(jsonString.contains("StringParam"));
    QCOMPARE(jsonString["StringParam"].toVariant(), QVariant("Hello World"));
    
    // Test with boolean
    Parameter parBool(true, "BoolParam", this);
    QJsonObject jsonBool = parBool.toJsonValue();
    QVERIFY(jsonBool.contains("BoolParam"));
    QCOMPARE(jsonBool["BoolParam"].toVariant(), QVariant(true));
    
    // Test with negative double
    Parameter parNegative(-123.456, "NegativeParam", this);
    QJsonObject jsonNegative = parNegative.toJsonValue();
    QVERIFY(jsonNegative.contains("NegativeParam"));
    QCOMPARE(jsonNegative["NegativeParam"].toVariant(), QVariant(-123.456));
}

void TestQtNoidAppParameter::testParameterValueFromJson()
{
    Parameter par(25.0, "Temperature", this);
    QSignalSpy spy(&par, &Parameter::valueChanged);
    
    QJsonObject json;
    json["Temperature"] = 30.5;
    
    QCOMPARE(par.valueFromJson(json), true);
    QCOMPARE(par.value(), 30.5);
    QCOMPARE(spy.count(), 1);
    
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toDouble(), 30.5);
}

void TestQtNoidAppParameter::testParameterValueFromJsonWithEmptyName()
{
    Parameter par(42.0);  // No name set
    QSignalSpy spy(&par, &Parameter::valueChanged);
    QSignalSpy nameSpy(&par, &Parameter::nameChanged);
    
    QJsonObject json;
    json["NewParameterName"] = 100.0;
    
    QCOMPARE(par.valueFromJson(json), true);
    QCOMPARE(par.value(), 100.0);
    QCOMPARE(par.name(), "NewParameterName");
    QCOMPARE(spy.count(), 1);
    QCOMPARE(nameSpy.count(), 1);
}

void TestQtNoidAppParameter::testParameterValueFromJsonWithNonExistentKey()
{
    Parameter par(25.0, "Temperature", this);
    QSignalSpy spy(&par, &Parameter::valueChanged);
    
    QJsonObject json;
    json["Pressure"] = 1013.25;
    
    QCOMPARE(par.valueFromJson(json), false);
    QCOMPARE(par.value(), 25.0);  // Value should not change
    QCOMPARE(spy.count(), 0);     // No signal should be emitted
}

void TestQtNoidAppParameter::testParameterValueFromJsonWithDifferentTypes()
{
    // Test with integer
    Parameter parInt(50, "IntParam", this);
    QJsonObject jsonInt;
    jsonInt["IntParam"] = 100;
    QCOMPARE(parInt.valueFromJson(jsonInt), true);
    QCOMPARE(parInt.value().toInt(), 100);

    // Test with float
    Parameter parDouble(50.2, "DoubleParam", this);
    QJsonObject jsonDouble;
    jsonDouble["DoubleParam"] = 100.5;
    QCOMPARE(parDouble.valueFromJson(jsonDouble), true);
    QCOMPARE(parDouble.value().toDouble(), 100.5);

    // Test with string
    Parameter parString("initial", "StringParam", this);
    QJsonObject jsonString;
    jsonString["StringParam"] = "updated";
    QCOMPARE(parString.valueFromJson(jsonString), true);
    QCOMPARE(parString.value().toString(), "updated");
    
    // Test with boolean
    Parameter parBool(false, "BoolParam", this);
    QJsonObject jsonBool;
    jsonBool["BoolParam"] = true;
    QCOMPARE(parBool.valueFromJson(jsonBool), true);
    QCOMPARE(parBool.value().toBool(), true);
}

void TestQtNoidAppParameter::testParameterConstructorFromJsonSchemaAndValue()
{
    // Create schema and value JSON objects
    QJsonObject schema;
    QJsonObject temperatureSchema;
    temperatureSchema["description"] = "Current temperature";
    temperatureSchema["unit"] = "°C";
    temperatureSchema["tooltip"] = "Temperature sensor reading in Celsius";
    temperatureSchema["readOnly"] = false;
    temperatureSchema["min"] = -273.15;
    temperatureSchema["max"] = 1000.0;
    schema["Temperature"] = temperatureSchema;
    
    QJsonObject value;
    value["Temperature"] = 25.5;
    
    // Create parameter from JSON
    Parameter par(schema, value, this);
    
    // Verify all properties were set correctly
    QCOMPARE(par.name(), "Temperature");
    QCOMPARE(par.description(), "Current temperature");
    QCOMPARE(par.unit(), "°C");
    QCOMPARE(par.tooltip(), "Temperature sensor reading in Celsius");
    QCOMPARE(par.readOnly(), false);
    QCOMPARE(par.min().toDouble(), -273.15);
    QCOMPARE(par.max().toDouble(), 1000.0);
    QCOMPARE(par.value().toDouble(), 25.5);
}

void TestQtNoidAppParameter::testParameterConstructorFromJsonSchemaOnly()
{
    // Create schema without value
    QJsonObject schema;
    QJsonObject pressureSchema;
    pressureSchema["description"] = "Atmospheric pressure";
    pressureSchema["unit"] = "hPa";
    pressureSchema["readOnly"] = true;
    pressureSchema["min"] = 800.0;
    pressureSchema["max"] = 1100.0;
    schema["Pressure"] = pressureSchema;
    
    // Create parameter from schema only
    Parameter par(schema, QJsonObject(), this);
    
    // Verify schema properties were set
    QCOMPARE(par.name(), "Pressure");
    QCOMPARE(par.description(), "Atmospheric pressure");
    QCOMPARE(par.unit(), "hPa");
    QCOMPARE(par.readOnly(), true);
    QCOMPARE(par.min().toDouble(), 800.0);
    QCOMPARE(par.max().toDouble(), 1100.0);
    
    // Value should be invalid/empty since no value was provided
    QVERIFY(!par.value().isValid());
}

void TestQtNoidAppParameter::testParameterConstructorFromJsonValueOnly()
{
    // Create value without schema
    QJsonObject value;
    value["Voltage"] = 12.5;
    
    // Create parameter from value only
    Parameter par(QJsonObject(), value, this);
    
    // Verify name and value were set
    QCOMPARE(par.name(), "Voltage");
    QCOMPARE(par.value().toDouble(), 12.5);
    
    // Other properties should be default values
    QCOMPARE(par.description(), QString());
    QCOMPARE(par.unit(), QString());
    QCOMPARE(par.tooltip(), QString());
    QCOMPARE(par.readOnly(), false);
    QVERIFY(!par.min().isValid());
    QVERIFY(!par.max().isValid());
}

void TestQtNoidAppParameter::testParameterConstructorFromJsonEmptyObjects()
{
    // Create parameter from empty objects
    Parameter par(QJsonObject(), QJsonObject(), this);
    
    // All properties should be default values
    QCOMPARE(par.name(), QString());
    QCOMPARE(par.description(), QString());
    QCOMPARE(par.unit(), QString());
    QCOMPARE(par.tooltip(), QString());
    QCOMPARE(par.readOnly(), false);
    QVERIFY(!par.min().isValid());
    QVERIFY(!par.max().isValid());
    QVERIFY(!par.value().isValid());
}

void TestQtNoidAppParameter::testParameterSchemaFromJson()
{
    Parameter par(20.0, "Temperature", this);
    
    // Create schema JSON
    QJsonObject schema;
    QJsonObject temperatureSchema;
    temperatureSchema["description"] = "Current temperature";
    temperatureSchema["unit"] = "°C";
    temperatureSchema["tooltip"] = "Displays the current temperature from the sensor";
    temperatureSchema["readOnly"] = true;
    temperatureSchema["min"] = -50.0;
    temperatureSchema["max"] = 100.0;
    
    QJsonObject preset1{{"Low", -40}};
    QJsonObject preset2{{"High", 90}};
    QJsonArray presetArray{preset1, preset2};
    temperatureSchema["presets"] = presetArray;
    schema["Temperature"] = temperatureSchema;

    // Apply schema to parameter
    bool result = par.schemaFromJson(schema);
    QCOMPARE(result, true);
    
    // Verify all schema properties were applied
    QCOMPARE(par.name(), "Temperature");
    QCOMPARE(par.description(), "Current temperature");
    QCOMPARE(par.unit(), "°C");
    QCOMPARE(par.tooltip(), "Displays the current temperature from the sensor");
    QCOMPARE(par.readOnly(), true);
    QCOMPARE(par.min().toDouble(), -50.0);
    QCOMPARE(par.max().toDouble(), 100.0);
    auto presetList = par.presets();
    QCOMPARE(presetList.count(), 2);
    QCOMPARE(par.preset("Low"), -40);
    QCOMPARE(par.preset("High"), 90);
    
    // Value should remain unchanged
    QCOMPARE(par.value().toDouble(), 20.0);
}

void TestQtNoidAppParameter::testParameterSchemaFromJsonWithEmptyName()
{
    Parameter par(1000.0);  // No name set
    QSignalSpy nameSpy(&par, &Parameter::nameChanged);
    
    // Create schema JSON
    QJsonObject schema;
    QJsonObject pressureSchema;
    pressureSchema["description"] = "Atmospheric pressure";
    pressureSchema["unit"] = "hPa";
    pressureSchema["readOnly"] = false;
    pressureSchema["min"] = 800.0;
    pressureSchema["max"] = 1100.0;
    schema["Pressure"] = pressureSchema;
    
    // Apply schema - should set name from JSON key
    bool result = par.schemaFromJson(schema);
    QCOMPARE(result, true);
    
    // Verify name was set and schema applied
    QCOMPARE(par.name(), "Pressure");
    QCOMPARE(par.description(), "Atmospheric pressure");
    QCOMPARE(par.unit(), "hPa");
    QCOMPARE(par.readOnly(), false);
    QCOMPARE(par.min().toDouble(), 800.0);
    QCOMPARE(par.max().toDouble(), 1100.0);
    QCOMPARE(nameSpy.count(), 1);
    
    // Value should remain unchanged
    QCOMPARE(par.value().toDouble(), 1000.0);
}

void TestQtNoidAppParameter::testParameterSchemaFromJsonWithNonExistentKey()
{
    Parameter par(25.0, "Temperature", this);
    
    // Create schema JSON with different key
    QJsonObject schema;
    QJsonObject pressureSchema;
    pressureSchema["description"] = "Atmospheric pressure";
    schema["Pressure"] = pressureSchema;
    
    // Apply schema - should fail because key doesn't match parameter name
    bool result = par.schemaFromJson(schema);
    QCOMPARE(result, false);
    
    // Properties should remain unchanged
    QCOMPARE(par.name(), "Temperature");
    QCOMPARE(par.description(), QString());
    QCOMPARE(par.unit(), QString());
    QCOMPARE(par.readOnly(), false);
    QVERIFY(!par.min().isValid());
    QVERIFY(!par.max().isValid());
    QCOMPARE(par.value().toDouble(), 25.0);
}

void TestQtNoidAppParameter::testParameterSchemaFromJsonEmptyObject()
{
    Parameter par(25.0, "Temperature");
    
    // Apply empty schema
    QCOMPARE(par.schemaFromJson(QJsonObject()), false);
    
    // Properties should remain unchanged
    QCOMPARE(par.name(), "Temperature");
    QCOMPARE(par.description(), QString());
    QCOMPARE(par.unit(), QString());
    QCOMPARE(par.readOnly(), false);
    QVERIFY(!par.min().isValid());
    QVERIFY(!par.max().isValid());
    QCOMPARE(par.value().toDouble(), 25.0);
}

void TestQtNoidAppParameter::testParameterSchemaFromJsonPartialSchemaShouldSetUnspecifiedParamtersToDefault()
{
    Parameter par(12.0, "Voltage", this);
    par.setDescription("Original description");
    par.setUnit("mV");
    par.setReadOnly(true);
    
    // Create partial schema (only some properties)
    QJsonObject schema;
    QJsonObject voltageSchema;
    voltageSchema["description"] = "Updated description";
    voltageSchema["unit"] = "V";
    // Note: readOnly not included, min/max not included
    schema["Voltage"] = voltageSchema;
    
    // Apply partial schema
    bool result = par.schemaFromJson(schema);
    QCOMPARE(result, true);
    
    // Only specified properties should be updated
    QCOMPARE(par.name(), "Voltage");
    QCOMPARE(par.description(), "Updated description");
    QCOMPARE(par.unit(), "V");
    QCOMPARE(par.readOnly(), false);  // Should get the default
    QVERIFY(!par.min().isValid());   // Should remain unchanged
    QVERIFY(!par.max().isValid());   // Should remain unchanged
    QCOMPARE(par.value().toDouble(), 12.0);  // Should remain unchanged
}

void TestQtNoidAppParameter::testParameterSchemaFromJsonChangeANewRangeShouldForceANewValue()
{
    // Create parameter with value outside the new range we'll set
    Parameter par(150.0, "Temperature", this);  // Current value is 150
    QSignalSpy valueSpy(&par, &Parameter::valueChanged);
    QSignalSpy rangeSpy(&par, &Parameter::rangeChanged);
    
    // Create schema with new range that will force the value to be clamped
    QJsonObject schema;
    QJsonObject temperatureSchema;
    temperatureSchema["min"] = -50.0;   // New min
    temperatureSchema["max"] = 100.0;   // New max - current value (150) exceeds this
    schema["Temperature"] = temperatureSchema;
    
    // Apply schema - should clamp the current value to the new max
    bool result = par.schemaFromJson(schema);
    QCOMPARE(result, true);
    
    // Verify schema was applied
    QCOMPARE(par.min().toDouble(), -50.0);
    QCOMPARE(par.max().toDouble(), 100.0);
    
    // Value should be clamped to the new maximum
    QCOMPARE(par.value().toDouble(), 100.0);
    QCOMPARE(valueSpy.count(), 1);
    QCOMPARE(rangeSpy.count(), 2);  // Once for min change, once for max change
    
    // Test with value below new minimum
    Parameter par2(-200.0, "Voltage", this);  // Current value is -200
    QSignalSpy valueSpy2(&par2, &Parameter::valueChanged);
    
    QJsonObject schema2;
    QJsonObject voltageSchema;
    voltageSchema["min"] = -100.0;  // New min - current value (-200) is below this
    voltageSchema["max"] = 50.0;    // New max
    schema2["Voltage"] = voltageSchema;
    
    // Apply schema - should clamp the current value to the new min
    bool result2 = par2.schemaFromJson(schema2);
    QCOMPARE(result2, true);
    
    // Value should be clamped to the new minimum
    QCOMPARE(par2.value().toDouble(), -100.0);
    QCOMPARE(par2.min().toDouble(), -100.0);
    QCOMPARE(par2.max().toDouble(), 50.0);
    QCOMPARE(valueSpy2.count(), 1);
}

void TestQtNoidAppParameter::testParameterIsValid()
{
    // Test case 1: Parameter with empty name should be invalid
    Parameter parEmptyName(this);
    parEmptyName.setValue(100.0);
    QCOMPARE(parEmptyName.isValid(), false);

    // Test case 2: Parameter with name but no value should be invalid
    Parameter parNameOnly("ValidName", this);
    QCOMPARE(parNameOnly.isValid(), false);

    // Test case 3: Parameter with valid name and value should be valid
    Parameter parNameAndValue(50.0, "ValidParam", this);
    QCOMPARE(parNameAndValue.isValid(), true);

    // Test case 4: Parameter with valid range (min < max) should be valid
    Parameter parValidRange(50.0, "RangeParam", this);
    parValidRange.setMin(0.0);
    parValidRange.setMax(100.0);
    QCOMPARE(parValidRange.isValid(), true);

    // Test case 5: Parameter with invalid range (min > max) should be invalid
    Parameter parInvalidRange(50.0, "InvalidRange", this);
    parInvalidRange.setMin(100.0);
    parInvalidRange.setMax(0.0);
    QCOMPARE(parInvalidRange.isValid(), false);

    // Test case 6: Parameter with equal min and max should be valid
    Parameter parEqualRange(50.0, "EqualRange", this);
    parEqualRange.setMin(50.0);
    parEqualRange.setMax(50.0);
    QCOMPARE(parEqualRange.isValid(), true);

    // Test case 7: Parameter with value within range should be valid
    Parameter parValueInRange(25.0, "ValueInRange", this);
    parValueInRange.setMin(0.0);
    parValueInRange.setMax(100.0);
    QCOMPARE(parValueInRange.isValid(), true);

    // Test case 8: It should be not possible to set a value out of the range
    // Or to set the range without clamping the value
    Parameter parValueBelowMin(-10.0, "ValueBelowMin", this);
    parValueBelowMin.setMin(0.0);
    parValueBelowMin.setMax(100.0);
    QCOMPARE(parValueBelowMin.isValid(), true);

    // Test case 9: Parameter with value exactly at minimum should be valid
    Parameter parValueAtMin(0.0, "ValueAtMin", this);
    parValueAtMin.setMin(0.0);
    parValueAtMin.setMax(100.0);
    QCOMPARE(parValueAtMin.isValid(), true);

    // Test case 10: Parameter with value exactly at maximum should be valid
    Parameter parValueAtMax(100.0, "ValueAtMax", this);
    parValueAtMax.setMin(0.0);
    parValueAtMax.setMax(100.0);
    QCOMPARE(parValueAtMax.isValid(), true);

    // Test case 11: Parameter with only min set and value above it should be valid
    Parameter parMinOnly(50.0, "MinOnly", this);
    parMinOnly.setMin(10.0);
    QCOMPARE(parMinOnly.isValid(), true);

    // Test case 12: Parameter with invalid value (empty QVariant) should be invalid
    Parameter parInvalidValue(123, "InvalidValue", this);
    parInvalidValue.setValue(QVariant());
    QCOMPARE(parInvalidValue.isValid(), false);

    // Test case 13: Test with string values and range
    Parameter parStringRange("StringParam", "middle", this);
    parStringRange.setMin("a");
    parStringRange.setMax("z");
    QCOMPARE(parStringRange.isValid(), true);

}

void TestQtNoidAppParameter::testParameterFromJson()
{
    Parameter par(this);

    // Create schema and value JSON objects
    QJsonObject schema;
    QJsonObject temperatureSchema;
    temperatureSchema["description"] = "Test temperature parameter";
    temperatureSchema["unit"] = "°C";
    temperatureSchema["readOnly"] = false;
    temperatureSchema["min"] = -50.0;
    temperatureSchema["max"] = 150.0;
    schema["Temperature"] = temperatureSchema;

    QJsonObject value;
    value["Temperature"] = 25.5;

    // Test fromJson method
    bool result = par.fromJson(schema, value);
    QCOMPARE(result, true);

    // Verify all properties were set correctly
    QCOMPARE(par.name(), "Temperature");
    QCOMPARE(par.description(), "Test temperature parameter");
    QCOMPARE(par.unit(), "°C");
    QCOMPARE(par.readOnly(), false);
    QCOMPARE(par.min().toDouble(), -50.0);
    QCOMPARE(par.max().toDouble(), 150.0);
    QCOMPARE(par.value().toDouble(), 25.5);
}

void TestQtNoidAppParameter::testParameterFromJsonShouldUpdateAnExistingParameter()
{
    Parameter par("InitialValue", "ExistingName", "This is an existing paramter", this);
    QCOMPARE(par.name(), "ExistingName");
    QCOMPARE(par.value().toString(), "InitialValue");


    // Create schema and value for a different parameter name
    QJsonObject pressureSchema {
        {"description", "Atmospheric pressure sensor"},
        {"unit", "hPa"},
        {"readOnly", false},
        {"min", 900.0},
        {"max", 1100.0},
    };
    QJsonObject preset1{{"High", 1050.0}};
    QJsonObject preset2{{"Low", 950.0}};
    QJsonArray presetArray{preset1, preset2};
    pressureSchema["Presets"]= presetArray;
    QJsonObject schema{{"Pressure", pressureSchema}};

    QJsonObject value{{"Pressure", 1013.25}};

    // Test fromJson method - should update name and all properties
    bool result = par.fromJson(schema, value);
    QCOMPARE(result, true);

    // Verify properties were updated to match the JSON
    QCOMPARE(par.name(), "Pressure");
    QCOMPARE(par.description(), "Atmospheric pressure sensor");
    QCOMPARE(par.unit(), "hPa");
    QCOMPARE(par.readOnly(), false);
    QCOMPARE(par.min().toDouble(), 900.0);
    QCOMPARE(par.max().toDouble(), 1100.0);
    QCOMPARE(par.value().toDouble(), 1013.25);
}

void TestQtNoidAppParameter::testParameterFromJsonWithInvalidSchemaShouldLeaveTheParameterUnchanged()
{
    Parameter par(100.0, "TestParam", this);

    // Create invalid schema (empty object)
    QJsonObject emptySchema;

    QJsonObject value;
    value["TestParam"] = 200.0;

    // Test fromJson with invalid schema - should fail
    bool result = par.fromJson(emptySchema, value);
    QCOMPARE(result, false);

    // Original properties should remain unchanged
    QCOMPARE(par.name(), "TestParam");
    QCOMPARE(par.value().toDouble(), 100.0);
    QCOMPARE(par.description(), QString());
    QCOMPARE(par.unit(), QString());
    QCOMPARE(par.readOnly(), false);
}

void TestQtNoidAppParameter::testParameterFromJsonWithInvalidValueShouldLeaveValueUnchanged()
{
    Parameter par("ExistingName", "This is an existing paramter", "InitialValue", this);

    // Create valid schema
    QJsonObject testSchema{{"unit", "°K"}};
    QJsonObject schema{{"NewParameter", testSchema}};

    // Create invalid value (non-matching key)
    QJsonObject value;
    value["DifferentParam"] = 150.0;

    // Test fromJson with invalid value - should fail
    bool result = par.fromJson(schema, value);
    QCOMPARE(result, false);

    // Schema should have been applied, but value should remain invalid
    QCOMPARE(par.name(), "NewParameter");
    QCOMPARE(par.unit(), "°K");
    QVERIFY(par.isValid()); // Value should remain valid
}

void TestQtNoidAppParameter::testParameterFromJsonWithIncompleteSchemaShouldOverrideExisitingSchema()
{
    Parameter par("ExistingName", "This is an existing paramter", "InitialValue", this);
    par.setRange("A", "Z");
    par.setUnit("Char");
    par.setPreset("Low", "A");
    par.setPreset("High", "Z");

    QJsonObject testSchema;
    QJsonObject schema{{"NewParameter", testSchema}};
    QJsonObject value{{"NewParameter", "NewlValue"}};

    bool result = par.fromJson(schema, value);

    qDebug() << par.toJsonSchema();
    qDebug() << par.toJsonValue();

    QCOMPARE(result, true);

    // Original properties should remain unchanged
    QCOMPARE(par.name(), "NewParameter");
    QCOMPARE(par.value().toString(), "NewlValue");
    QCOMPARE(par.description(), QString());
    QCOMPARE(par.unit(), QString());
    QCOMPARE(par.range(), {});
    QCOMPARE(par.presets(), {});

}


QTEST_MAIN(TestQtNoidAppParameter)
#include "test_parameter.moc"
