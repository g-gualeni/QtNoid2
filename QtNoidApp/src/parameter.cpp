//=============================================================================
// parameter.cpp
//=============================================================================
#include "QtNoidApp/parameter.h"
#include <QDebug>

namespace QtNoid {
namespace App {

Parameter::Parameter(QObject *parent)
    : QObject(parent)
{
    connectRangeChanged();
}

Parameter::Parameter(const QVariant &initialValue, QObject *parent)
    : QObject(parent), m_value(initialValue)
{
    connectRangeChanged();
}

Parameter::Parameter(const QString &name, const QVariant &initialValue, QObject *parent)
    : QObject(parent), m_name(name), m_value(initialValue)
{
    connectRangeChanged();
}

Parameter::Parameter(const QString &name, const QString &description, const QVariant &initialValue, QObject *parent)
    : QObject(parent), m_name(name), m_description(description), m_value(initialValue)
{
    connectRangeChanged();
}

void Parameter::connectRangeChanged()
{
    connect(this, &Parameter::minChanged, this, [this](const QVariant&) {
        enforceRange();
        emit rangeChanged(m_min.value(), m_max.value());
    });

    connect(this, &Parameter::maxChanged, this, [this](const QVariant&) {
        enforceRange();
        emit rangeChanged(m_min.value(), m_max.value());
    });
}

QVariant Parameter::value() const
{
    return m_value.value();
}

void Parameter::setValue(const QVariant &val)
{
    if (canModify()) {
        // No needs for checking if different or to manually emit value changed
        m_value = clampValue(val);
    }
}

QBindable<QVariant> Parameter::bindableValue()
{
    return QBindable<QVariant>(&m_value);
}


QVariant Parameter::min() const
{
    return m_min.value();
}
void Parameter::setMin(const QVariant &val)
{
    m_min = val;
}
QBindable<QVariant> Parameter::bindableMin()
{
    return QBindable<QVariant>(&m_min);
}


QVariant Parameter::max() const
{
    return m_max.value();
}
void Parameter::setMax(const QVariant &val)
{
    m_max = val;
}
QBindable<QVariant> Parameter::bindableMax()
{
    return QBindable<QVariant>(&m_max);
}


std::pair<QVariant, QVariant> Parameter::range() const
{
    return {m_min.value(), m_max.value()};
}
void Parameter::setRange(const QVariant &min, const QVariant &max)
{
    m_min = min;
    m_max = max;
}
void Parameter::setRange(const std::pair<QVariant, QVariant>& newRange)
{
    m_min = newRange.first;
    m_max = newRange.second;
}


QVariantMap Parameter::presets() const
{
    return m_presets.value();
}
void Parameter::setPresets(const QVariantMap &presets)
{
    m_presets = presets;
}
void Parameter::clearPresets()
{
    m_presets = QVariantMap();
}
QVariant Parameter::preset(const QString &name) const
{
    if(!m_presets.value().contains(name)) {
        return QVariant();
    }
    return m_presets.value()[name];
}
void Parameter::setPreset(const QString &name, const QVariant &value)
{
    QVariantMap current = m_presets.value();
    current[name] = value;
    m_presets = current;
}
void Parameter::removePreset(const QString &name)
{
    if(m_presets.value().contains(name)) {
        QVariantMap currentPresets = m_presets.value();
        currentPresets.remove(name);
        m_presets = currentPresets;
    }
}
bool Parameter::applyPreset(const QString &name)
{
    if(!m_presets.value().contains(name)) {
        return false;
    }
    m_value = m_presets.value()[name];
    return true;
}
QBindable<QVariantMap> Parameter::bindablePresets()
{
    return QBindable<QVariantMap>(&m_presets);
}


QString Parameter::name() const
{
    return m_name.value();
}
void Parameter::setName(const QString &value)
{
    m_name = value;
}
QBindable<QString> Parameter::bindableName()
{
    return QBindable<QString>(&m_name);
}


QString Parameter::description() const
{
    return m_description.value();
}
void Parameter::setDescription(const QString &value)
{
    m_description = value;
}
QBindable<QString> Parameter::bindableDescription()
{
    return QBindable<QString>(&m_description);
}


QString Parameter::unit() const
{
    return m_unit.value();
}
void Parameter::setUnit(const QString &value)
{
    m_unit = value;
}
QBindable<QString> Parameter::bindableUnit()
{
    return QBindable<QString>(&m_unit);
}


bool Parameter::readOnly() const
{
    return m_readOnly.value();
}
void Parameter::setReadOnly(bool value)
{
    m_readOnly = value;
}
QBindable<bool> Parameter::bindableReadOnly()
{
    return QBindable<bool>(&m_readOnly);
}

bool Parameter::canModify() const
{
    if(m_readOnly.value()) {
        const_cast<Parameter*>(this)->emit writeAttemptedWhileReadOnly(m_name.value());
        return false;
    }
    return true;
}


void Parameter::enforceRange()
{
    if(!m_value.value().isValid())
        return;

    m_value = clampValue(m_value);
}

QVariant Parameter::clampValue(const QVariant &value) const
{
    // qDebug() << __func__ << "value:" << value << "range:" << range();
    QVariant result = value;

    // Clamp to min value
    if (m_min.value().isValid() && compareVariants(result, m_min, -1)) {
        result = m_min.value();
    }

    // Clamp to max value
    if (m_max.value().isValid() && compareVariants(result, m_max, 1)) {
        result = m_max.value();
    }

    return result;
}

bool Parameter::compareVariants(const QVariant &a, const QVariant &b, int comparison) const
{
    if (!a.isValid() || !b.isValid())
        return false;

    // Try to convert to numbers
    if (a.canConvert<double>() && b.canConvert<double>()) {
        double da = a.toDouble();
        double db = b.toDouble();
        switch (comparison) {
        case -1: return da < db;
        case 0:  return qFuzzyCompare(da, db);
        case 1:  return da > db;
        }
    }

    // Fallback to string comparison
    QString sa = a.toString();
    QString sb = b.toString();
    switch (comparison) {
    case -1: return sa < sb;
    case 0:  return sa == sb;
    case 1:  return sa > sb;
    }

    return false;
}

} // namespace App
} // namespace QtNoid
