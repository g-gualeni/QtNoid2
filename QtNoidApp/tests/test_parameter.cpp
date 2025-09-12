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
    void testParameterValueChanged();
    void testParameterValueChangedNoEmitForSameValue();
    void testParameterMin();
    void testParameterMax();
    void testParameterSetPresets();
    void testParameterSetPreset();
    void testParameterApplyPresetByName();
    void testParameterRemovePreset();
    void testParameterClearPreset();
    void testParameterSetPresetShouldAbideToRangeRules();
    void testParameterApplyPresetReadOnlyShouldBeIneffective();

    void testParameterRange();
    void testParameterName();
    void testParameterDescription();
    void testParameterUnit();
    void testParameterReadOnly();
    
    // Bindable properties tests
    void testBindableValue();
    void testBindableValueUsingBidirectionalNotifier();

    void testBindableMin();
    void testBindableMax();
    void testBindablePresets();
    void testBindableName();
    void testBindableDescription();
    void testBindableUnit();
    void testBindableReadOnly();

    // JSON Schema tests
    void testParameterToJsonSchema();
    void testParameterToJsonSchemaWithNoNameAndEmptyParameters();
    void testParameterToJsonSchemaWithFullMetadataForFloatNumber();
    
    // JSON tests
    void testParameterToJson();
    void testParameterToJsonWithNoName();
    void testParameterToJsonWithDifferentValueTypes();

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
    auto par = Parameter("Param", 456.789, this);
    
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
    auto par = Parameter("Param", __func__, 789.123, this);
    
    QCOMPARE(par.name(), "Param");
    QCOMPARE(par.description(), __func__);
    QCOMPARE(par.value(), 789.123);
    QCOMPARE(par.unit(), QString());
    QCOMPARE(par.readOnly(), false);
    QCOMPARE(par.min(), QVariant());
    QCOMPARE(par.max(), QVariant());
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
    Parameter par("ReadOnlyParam", 50.0);
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

void TestQtNoidAppParameter::testParameterName()
{
    Parameter par("EarthRadius", 12500);
    QSignalSpy spy(&par, &Parameter::nameChanged);

    par.setName("EarthDiameter");
    QCOMPARE(spy.count(), 1);

    // check the value is the correct one
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), "EarthDiameter");

    // Same value, no activation
    par.setName("EarthDiameter");
    QCOMPARE(spy.count(), 0);
}

void TestQtNoidAppParameter::testParameterDescription()
{
    Parameter par("EarthDiameter", "this is the Earth diameter", 12500);
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
    Parameter par("Temperature", "Current temperature", 25.0);
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

void TestQtNoidAppParameter::testParameterReadOnly()
{
    Parameter par("ReadOnlyParamName", "Test parameter", 100.0);
    
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

void TestQtNoidAppParameter::testBindableValue()
{
    Parameter par(100.0);
    
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
    Parameter par(1.0);
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
    Parameter par(50.0);
    
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
    Parameter par(50.0);
    
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
    Parameter par(50.0);
    
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
    Parameter par("TestParam", 50.0);
    
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
    Parameter par("TestParam", "Initial description", 50.0);
    
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
    Parameter par("Temperature", 25.0);
    
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

void TestQtNoidAppParameter::testBindableReadOnly()
{
    Parameter par("TestParam", 50.0);
    
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
    Parameter par("Log File Size", "Parameter description", 50.0);
    par.setUnit("kB");
    par.setMin(0.0);
    par.setMax(100.0);
    par.setReadOnly(true);
    
    QJsonObject schema = par.toJsonSchema();
    // qDebug() << schema;
    
    QVERIFY(schema.contains("Log File Size"));
    QJsonObject paramSchema = schema["Log File Size"].toObject();
    
    QCOMPARE(paramSchema["description"].toString(), "Parameter description");
    QCOMPARE(paramSchema["unit"].toString(), "kB");
    QCOMPARE(paramSchema["readOnly"].toBool(), true);
    QCOMPARE(paramSchema["min"].toVariant(), QVariant(0.0));
    QCOMPARE(paramSchema["max"].toVariant(), QVariant(100.0));
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
    QCOMPARE(paramSchema["readOnly"].toBool(), false);
    QVERIFY(paramSchema["min"].isNull());
    QVERIFY(paramSchema["max"].isNull());
}

void TestQtNoidAppParameter::testParameterToJsonSchemaWithFullMetadataForFloatNumber()
{
    Parameter par("ComplexParam", "A complex parameter with all metadata", 75.5);
    par.setUnit("°C");
    par.setMin(-273.15);
    par.setMax(1000.0);
    par.setReadOnly(false);
    
    QJsonObject schema = par.toJsonSchema();
    
    QVERIFY(schema.contains("ComplexParam"));
    QJsonObject paramSchema = schema["ComplexParam"].toObject();
    
    QCOMPARE(paramSchema["description"].toString(), "A complex parameter with all metadata");
    QCOMPARE(paramSchema["unit"].toString(), "°C");
    QCOMPARE(paramSchema["readOnly"].toBool(), false);
    QCOMPARE(paramSchema["min"].toVariant(), QVariant(-273.15));
    QCOMPARE(paramSchema["max"].toVariant(), QVariant(1000.0));
}

void TestQtNoidAppParameter::testParameterToJson()
{
    Parameter par("Temperature", "Current temperature", 25.5);
    
    QJsonObject json = par.toJson();
    // qDebug() << __func__ << json;
    
    QVERIFY(json.contains("Temperature"));
    QCOMPARE(json["Temperature"].toVariant(), QVariant(25.5));
    QCOMPARE(json.size(), 1);
}

void TestQtNoidAppParameter::testParameterToJsonWithNoName()
{
    Parameter par(42.0);
    
    QJsonObject json = par.toJson();
    // qDebug() << __func__ << json;

    QVERIFY(json.contains("Name"));
    QCOMPARE(json["Name"].toVariant(), QVariant(42.0));
    QCOMPARE(json.size(), 1);
}

void TestQtNoidAppParameter::testParameterToJsonWithDifferentValueTypes()
{
    // Test with integer
    Parameter parInt("IntParam", 100);
    QJsonObject jsonInt = parInt.toJson();
    QVERIFY(jsonInt.contains("IntParam"));
    QCOMPARE(jsonInt["IntParam"].toVariant(), QVariant(100));
    
    // Test with string
    Parameter parString("StringParam", "Hello World");
    QJsonObject jsonString = parString.toJson();
    QVERIFY(jsonString.contains("StringParam"));
    QCOMPARE(jsonString["StringParam"].toVariant(), QVariant("Hello World"));
    
    // Test with boolean
    Parameter parBool("BoolParam", true);
    QJsonObject jsonBool = parBool.toJson();
    QVERIFY(jsonBool.contains("BoolParam"));
    QCOMPARE(jsonBool["BoolParam"].toVariant(), QVariant(true));
    
    // Test with negative double
    Parameter parNegative("NegativeParam", -123.456);
    QJsonObject jsonNegative = parNegative.toJson();
    QVERIFY(jsonNegative.contains("NegativeParam"));
    QCOMPARE(jsonNegative["NegativeParam"].toVariant(), QVariant(-123.456));
}


QTEST_MAIN(TestQtNoidAppParameter)
#include "test_parameter.moc"
