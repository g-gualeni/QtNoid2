#ifndef QTNOID_APP_PARAMETER_H
#define QTNOID_APP_PARAMETER_H

#include "global.h"
#include <QObject>
#include <QProperty>
#include <QPropertyNotifier>
#include <QVariant>

namespace QtNoid {
namespace App {


class QTNOIDAPP_EXPORT Parameter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value WRITE setValue BINDABLE bindableValue NOTIFY valueChanged FINAL)
    Q_PROPERTY(QVariant min READ min WRITE setMin BINDABLE bindableMin NOTIFY minChanged FINAL)
    Q_PROPERTY(QVariant max READ max WRITE setMax BINDABLE bindableMax NOTIFY maxChanged FINAL)
    Q_PROPERTY(std::pair<QVariant, QVariant> range READ range WRITE setRange NOTIFY rangeChanged FINAL)
    Q_PROPERTY(QVariantMap presets READ presets WRITE setPresets BINDABLE bindablePresets NOTIFY presetsChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName BINDABLE bindableName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString description READ description WRITE setDescription BINDABLE bindableDescription NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(QString unit READ unit WRITE setUnit BINDABLE bindableUnit NOTIFY unitChanged FINAL)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly BINDABLE bindableReadOnly NOTIFY readOnlyChanged FINAL)

public:
    explicit Parameter(QObject *parent = nullptr);
    explicit Parameter(const QVariant& initialValue, QObject *parent = nullptr);
    explicit Parameter(const QString &name, const QVariant& initialValue, QObject *parent = nullptr);
    explicit Parameter(const QString &name, const QString &description, const QVariant& initialValue, QObject *parent = nullptr);

    // Value management
    QVariant value() const;
    void setValue(const QVariant& val);
    QBindable<QVariant> bindableValue();

    // Range management
    QVariant min() const;
    void setMin(const QVariant& val);
    QBindable<QVariant> bindableMin();

    QVariant max() const;
    void setMax(const QVariant& val);
    QBindable<QVariant> bindableMax();

    std::pair<QVariant, QVariant> range() const;
    void setRange(const QVariant& min, const QVariant& max);
    void setRange(const std::pair<QVariant, QVariant>& newRange);

    // Presets management
    QVariantMap presets() const;
    void setPresets(const QVariantMap& presets);
    void clearPresets();
    QVariant preset(const QString& name) const;
    void setPreset(const QString& name, const QVariant& value);
    void removePreset(const QString& name);
    bool applyPreset(const QString& name);
    QBindable<QVariantMap> bindablePresets();

    // Name
    QString name() const;
    void setName(const QString& value);
    QBindable<QString> bindableName();

    // Description
    QString description() const;
    void setDescription(const QString& value);
    QBindable<QString> bindableDescription();

    // Unit
    QString unit() const;
    void setUnit(const QString& value);
    QBindable<QString> bindableUnit();

    // ReadOnly
    bool readOnly() const;
    void setReadOnly(bool value);
    QBindable<bool> bindableReadOnly();
    

signals:
    void valueChanged(const QVariant &newValue);
    void minChanged(const QVariant &min);
    void maxChanged(const QVariant &max);
    void rangeChanged(const QVariant &min, const QVariant &max);
    void presetsChanged(const QVariantMap &presets);
    void nameChanged(const QString &value);
    void descriptionChanged(const QString &value);
    void unitChanged(const QString &value);
    void readOnlyChanged(bool value);
    void writeAttemptedWhileReadOnly(const QString &parameterName);

private:
    Q_OBJECT_BINDABLE_PROPERTY(Parameter, QVariant, m_value, &Parameter::valueChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Parameter, QVariant, m_min, &Parameter::minChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Parameter, QVariant, m_max, &Parameter::maxChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Parameter, QVariantMap, m_presets, &Parameter::presetsChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Parameter, QString, m_name, &Parameter::nameChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Parameter, QString, m_description, &Parameter::descriptionChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Parameter, QString, m_unit, &Parameter::unitChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Parameter, bool, m_readOnly, &Parameter::readOnlyChanged)

    void enforceRange();
    QVariant clampValue(const QVariant &value) const;
    void connectRangeChanged();
    bool compareVariants(const QVariant &a, const QVariant &b, int comparison) const;
    bool canModify() const; // Modification control
};

} // namespace App
} // namespace QtNoid

#endif // QTNOID_APP_PARAMETER_H
