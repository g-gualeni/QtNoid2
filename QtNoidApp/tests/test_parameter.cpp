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
    void testParameterApplyPresetReadOnly();

    void testParameterRange();
    void testParameterName();
    void testParameterDescription();
    void testParameterUnit();
    void testParameterReadOnly();

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

void TestQtNoidAppParameter::testParameterApplyPresetReadOnly()
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


QTEST_MAIN(TestQtNoidAppParameter)
#include "test_parameter.moc"
