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
    void testParameterValueChanged();
    void testParameterValueChangedNoEmitForSameValue();
    void testParameterRange();
    void testParameterName();
    void testParameterDescription();

    // Q_PROPERTY(QVariant value READ value WRITE setValue BINDABLE bindableValue NOTIFY valueChanged FINAL)
    // Q_PROPERTY(QVariant min READ min WRITE setMin BINDABLE bindableMin NOTIFY minChanged FINAL)
    // Q_PROPERTY(QVariant max READ max WRITE setMax BINDABLE bindableMax NOTIFY maxChanged FINAL)
    // Q_PROPERTY(std::pair<QVariant, QVariant> range READ range WRITE setRange NOTIFY rangeChanged FINAL)
    // Q_PROPERTY(QVariantMap presets READ presets WRITE setPresets BINDABLE bindablePresets NOTIFY presetsChanged FINAL)
    // Q_PROPERTY(QString name READ name WRITE setName BINDABLE bindableName NOTIFY nameChanged FINAL)
    // Q_PROPERTY(QString description READ description WRITE setDescription BINDABLE bindableDescription NOTIFY descriptionChanged FINAL)

    // AGGIUNGERE unità di misura per il parameter

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

}

void TestQtNoidAppParameter::testParameterValueChanged()
{
    auto par = Parameter(1000);
    QSignalSpy spy(&par, &Parameter::valueChanged);
    QVERIFY(spy.isValid());

    // Change the value and verify the signal is emitted
    par.setValue(500);
    QCOMPARE(spy.count(), 1);

    // check the value is the correct one
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), 500);

}

void TestQtNoidAppParameter::testParameterValueChangedNoEmitForSameValue()
{
    Parameter par(100);
    QSignalSpy spy(&par, &Parameter::valueChanged);

    // Same value, no activation
    par.setValue(100);
    QCOMPARE(spy.count(), 0);

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


QTEST_MAIN(TestQtNoidAppParameter)
#include "test_parameter.moc"
