#include "QtNoidApp/parameter.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>



namespace QtNoid {
namespace App {

QAtomicInt Parameter::s_nextUniqueId(1000);  // Inizializzazione thread-safe


QAtomicInt Parameter::getNextUniqueId()
{
    if (s_nextUniqueId == INT_MAX) {
        s_nextUniqueId = 0;  // Reset if overflow
    }
    return s_nextUniqueId++;
}


Parameter::Parameter(QObject *parent)
    : QObject(parent), m_uniqueId(getNextUniqueId()), m_visible(true)
{
    connectRangeChanged();
}

Parameter::Parameter(const QVariant &initialValue, QObject *parent)
    : QObject(parent), m_uniqueId(getNextUniqueId()), m_value(initialValue), m_visible(true)
{
    connectRangeChanged();
}

Parameter::Parameter(const QVariant &initialValue, const QString &name, QObject *parent)
    : QObject(parent), m_uniqueId(getNextUniqueId()), m_name(name), m_value(initialValue), m_visible(true)
{
    connectRangeChanged();
}

Parameter::Parameter(const QVariant &initialValue, const QString &name, const QString &description, QObject *parent)
    : QObject(parent), m_uniqueId(getNextUniqueId()), m_name(name), m_description(description), m_value(initialValue), m_visible(true)
{
    connectRangeChanged();
}

Parameter::Parameter(const QJsonObject &schema, const QJsonObject &value, QObject *parent)
    : QObject(parent), m_uniqueId(getNextUniqueId()), m_visible(true)
{
    // Extract parameter name from schema (first key) or value (first key)
    QString paramName;
    if (!schema.isEmpty()) {
        paramName = schema.begin().key();
    } else if (!value.isEmpty()) {
        paramName = value.begin().key();
    }
    
    if (!paramName.isEmpty()) {
        m_name = paramName;
        
        // Initialize from schema using paramName as key
        if (schema.contains(paramName)) {
            QJsonObject schemaData = schema[paramName].toObject();
            
            if (schemaData.contains("description")) {
                m_description = schemaData["description"].toString();
            }
            if (schemaData.contains("unit")) {
                m_unit = schemaData["unit"].toString();
            }
            if (schemaData.contains("tooltip")) {
                m_tooltip = schemaData["tooltip"].toString();
            }
            if (schemaData.contains("readOnly")) {
                m_readOnly = schemaData["readOnly"].toBool();
            }
            if (schemaData.contains("visible")) {
                m_visible = schemaData["visible"].toBool();
            }
            if (schemaData.contains("min")) {
                m_min = schemaData["min"].toVariant();
            }
            if (schemaData.contains("max")) {
                m_max = schemaData["max"].toVariant();
            }
        }
        
        // Initialize value if paramName is correct
        if (value.contains(paramName)) {
            m_value = value[paramName].toVariant();
        }
    }
    
    connectRangeChanged();
}

QJsonObject Parameter::toJsonValue() const
{
    QJsonObject res;
    QString name = m_name;
    if(name.isEmpty()) {
        name = "Name";
    }
    res[name] = QJsonValue::fromVariant(value());
    return res;
}

QJsonObject Parameter::toJsonSchema() const
{
    QJsonObject schema;
    schema["description"] = m_description.value();
    schema["unit"] = m_unit.value();
    schema["tooltip"] = m_tooltip.value();
    schema["readOnly"] = m_readOnly.value();
    schema["visible"] = m_visible.value();
    schema["min"] = QJsonValue::fromVariant(m_min.value());
    schema["max"] = QJsonValue::fromVariant(m_max.value());


    if(m_presets.value().count() > 0) {
        QVariantMap presetsMap = m_presets;
        QJsonArray presetsArray;
        for (auto it = presetsMap.constBegin(); it != presetsMap.constEnd(); ++it) {
            QJsonObject item;
            item[it.key()] = it.value().toJsonValue();
            presetsArray.append(item);
        }
        schema["presets"] = presetsArray;
    }

    QString name = m_name;
    if(name.isEmpty()) {
        name = "Name";
    }

    QJsonObject res;
    res[name] = schema;
    return res;
}

bool Parameter::fromJson(const QJsonObject &schema, const QJsonObject &value)
{
    // Override the parameter name using schema
    if(schema.isEmpty())    return false;
    QString paramName = schema.begin().key();
    if(paramName.isEmpty()) return false;
    m_name = paramName;

    if(!schemaFromJson(schema))         return false;
    if(!valueFromJson(value))         return false;

    return true;
}

bool Parameter::valueFromJson(const QJsonObject& json)
{
    //
    QString name = m_name.value();
    if(name.isEmpty() && (json.count() == 1)) {
        // Get the unique JSON object and use it to set the name
        name = json.begin().key();
        setName(name);
        QVariant value = json.value(name).toVariant();
        setValue(value);
        return true;
    }

    if(json.contains(name)) {
        QVariant value = json[name].toVariant();
        setValue(value);
        return true;
    }
    return false;
}

bool Parameter::schemaFromJson(const QJsonObject &json)
{
    // Sanity check
    if(json.isEmpty())
        return false;

    // Get parameter name - use existing name or first key in JSON
    QString paramName = m_name.value();
    if (paramName.isEmpty()) {
        paramName = json.begin().key();
        setName(paramName);
    }
    
    if (!json.contains(paramName)) {
        return false;
    }
    
    QJsonObject schemaData = json[paramName].toObject();
    
    // Load schema properties
    if (schemaData.contains("description")) {
        setDescription(schemaData["description"].toString());
    }
    else{
        setDescription(QString());
    }
    if (schemaData.contains("unit")) {
        setUnit(schemaData["unit"].toString());
    }
    else {
        setUnit(QString());
    }
    if (schemaData.contains("tooltip")) {
        setTooltip(schemaData["tooltip"].toString());
    }
    else {
        setTooltip(QString());
    }
    if (schemaData.contains("readOnly")) {
        setReadOnly(schemaData["readOnly"].toBool());
    }
    else {
        setReadOnly(false);
    }
    if (schemaData.contains("visible")) {
        setVisible(schemaData["visible"].toBool());
    }
    else {
        setVisible(true);
    }
    QVariant min;
    QVariant max;
    if (schemaData.contains("min")) {
        min = schemaData["min"].toVariant();
    }
    if (schemaData.contains("max")) {
        max=schemaData["max"].toVariant();
    }
    setRange(min, max);

    if (schemaData.contains("presets")) {
        const QJsonArray presetList = schemaData["presets"].toArray();
        for(auto it = presetList.constBegin(); it != presetList.constEnd(); ++it) {
            if(it->isObject()) {
                QJsonObject preset = it->toObject();
                QString name = preset.begin().key();
                QVariant val = preset.begin().value().toVariant();
                setPreset(name, val);
            }
        }
    }
    else {
        setPresets({});
    }

    return true;
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
    if(min.isValid() && max.isValid()) {
        if(compareVariants(min, max, -1)) {
            m_min = min;
            m_max = max;
        }
        else {
            m_min = max;
            m_max = min;
        }
        return;
    }

    // If one of the two is invalid, I have no compare terms
    m_min = min;
    m_max = max;
    return;
}
void Parameter::setRange(const std::pair<QVariant, QVariant>& newRange)
{
    setRange(newRange.first, newRange.second);
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

    auto val = m_presets.value()[name];
    if(val == m_value) {
        // No needs to trigger errors if the variable is locked
        return true;
    }
    if (!canModify()) {
        // We have a different value but we cannot change
        return false;
    }

    m_value = clampValue(val);
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
void Parameter::setName(const QString &newName)
{
    QString oldName = m_name;
    if(oldName != newName) {
        m_name = newName;
        emit nameEdited(oldName, newName);
    }
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


QString Parameter::tooltip() const
{
    return m_tooltip.value();
}
void Parameter::setTooltip(const QString &value)
{
    m_tooltip = value;
}
QBindable<QString> Parameter::bindableTooltip()
{
    return QBindable<QString>(&m_tooltip);
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

bool Parameter::visible() const
{
    return m_visible.value();
}
void Parameter::setVisible(bool value)
{
    m_visible = value;
}
QBindable<bool> Parameter::bindableVisible()
{
    return QBindable<bool>(&m_visible);
}

bool Parameter::canModify() const
{
    if(m_readOnly.value()) {
        emit const_cast<Parameter*>(this)->writeAttemptedWhileReadOnly(m_name.value());
        return false;
    }
    return true;
}


bool Parameter::isValid() const
{
    if (!m_value.value().isValid()) {
        return false;
    }

    // Parameter must have a non-empty name
    if (m_name.value().isEmpty()) {
        return false;
    }

    // If min/max are set, they must be valid
    if (m_min.value().isValid() && m_max.value().isValid()) {
        if (compareVariants(m_min.value(), m_max.value(), 1)) {
            return false;
        }
    }

    // If value is set and range is defined, value must be within range
    if (m_value.value().isValid()) {
        if (m_min.value().isValid() && compareVariants(m_value.value(), m_min.value(), -1)) {
            return false;
        }
        if (m_max.value().isValid() && compareVariants(m_value.value(), m_max.value(), 1)) {
            return false;
        }
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


/**
 * @brief Parameter::compareVariants
 * @param a
 * @param b
 * @param comparison -1 sa < sb, 0 sa == sb, +1 sa > sb
 * @return
 */
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
