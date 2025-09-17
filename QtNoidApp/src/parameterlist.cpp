//=============================================================================
// parameterlist.cpp
//=============================================================================
#include "QtNoidApp/parameterlist.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

namespace QtNoid {
namespace App {

ParameterList::ParameterList(QObject *parent)
    : QObject(parent)
{
}

ParameterList::ParameterList(const QString &name, QObject *parent)
    : QObject(parent), m_name(name)
{
}

ParameterList::ParameterList(const QJsonObject &schemaList, const QJsonObject &valueList, QObject *parent)
    : QObject(parent)
{
    schemaFromJson(schemaList);
    valuesFromJson(valueList);
}

QJsonObject ParameterList::toJsonValues() const
{
    QString name = m_name;
    if(name.isEmpty()) {
        name = "PageName";
    }

    QJsonArray parametersArray;
    for (const Parameter* param : m_parametersByIndex) {
        if (param) {
            parametersArray.append(param->toJsonValue());
        }
    }
    QJsonObject json;
    json[name] = parametersArray;
    return json;
}

QJsonObject ParameterList::toJsonSchema() const
{
    QString name = m_name;
    if(name.isEmpty()) {
        name = "PageName";
    }

    QJsonArray parametersArray;
    for (const Parameter* param : m_parametersByIndex) {
        if (param) {
            parametersArray.append(param->toJsonSchema());
        }
    }

    QJsonObject schema;
    schema[name] = parametersArray;
    return schema;
}

bool ParameterList::valuesFromJson(const QJsonObject &json)
{
    QString name = m_name.value();
    if(name.isEmpty() && (json.count() == 1)) {
        // Get the unique JSON object and use it to set the name
        name = json.begin().key();
        setName(name);
    }
    else if(!json.contains(name)) {
        // This is not the correct JSON model
        return false;
    }

    // Clear existing parameters
    clear();

    // Load parameters
    const QJsonArray parametersArray = json[name].toArray();
    for (const QJsonValue& value : parametersArray) {
        if (value.isObject()) {
            const QJsonObject valueObj = value.toObject();
            auto valueName = valueObj.begin().key();
            auto valueVal = valueObj.begin().value().toVariant();

            auto newParam = new Parameter(valueName, valueVal, this);
            if(newParam->isValid()) {
                addParameter(newParam);
            }
        }
    }

    return true;
}

bool ParameterList::schemaFromJson(const QJsonObject &json)
{
    QString name = m_name.value();
    if(name.isEmpty() && (json.count() == 1)) {
        // Get the unique JSON object and use it to set the name
        name = json.begin().key();
        setName(name);
    }
    else if(!json.contains(name)) {
        // This is not the correct JSON model
        return false;
    }

    // Check if name already exists
    if(contains(name)){
        return false;
    }

    // Clear existing parameters
    clear();

    // Load parameters from schema
    const QJsonArray schemaArray = json[name].toArray();
    for (const QJsonValue& schema : schemaArray) {
        if (schema.isObject()) {
            const QJsonObject schemaObj = schema.toObject();
            auto newParam = new Parameter(schemaObj, {}, this);
            //no need to check if it is valid since it is just schema
            // if(newParam->isValid()) {} -
            addParameter(newParam);
        }
    }
    return true;
}



QString ParameterList::name() const
{
    return m_name.value();
}

void ParameterList::setName(const QString &value)
{
    m_name = value;
}

QBindable<QString> ParameterList::bindableName()
{
    return QBindable<QString>(&m_name);
}

int ParameterList::count() const
{
    return m_parametersByIndex.count();
}

bool ParameterList::addParameter(Parameter *parameter)
{
    if (parameter == nullptr) {
        return false;
    }
    int paramterId = parameter->uniqueId();
    if (m_parametersByUniqueId.contains(parameter->uniqueId())) {
        return false;
    }

    auto paramName = parameter->name();
    if(paramName.isEmpty()){
        return false;
    }
    if (m_parametersByName.contains(paramName)) {
        return false;
    }
    // Update all indexes
    m_parametersByUniqueId.insert(paramterId, parameter);
    m_parametersByIndex.insert(m_nextParameterIndex, parameter);
    m_nextParameterIndex++;
    m_parametersByName.insert(paramName, parameter);
    connect(parameter, &QObject::destroyed, this, &ParameterList::onParameterDestroyed);

    emit parameterAdded(parameter);
    emit countChanged(m_parametersByIndex.count());
    return true;
}

bool ParameterList::addParameter(const QJsonObject& schema, const QJsonObject& value)
{
    Parameter* parameter = new Parameter(schema, value, this);
    return addParameter(parameter);
}

void ParameterList::removeParameter(Parameter *parameter)
{
    if (parameter == nullptr) {
        return;
    }
    int paramterId = parameter->uniqueId();
    if (m_parametersByUniqueId.remove(paramterId) == false) {
        // ID not found
        return;
    }

    auto paramName = parameter->name();
    if(!paramName.isEmpty()){
        m_parametersByName.remove(paramName);
    }

    int idx = m_parameterToIndex.value(parameter, -1);
    if(idx > 0) {
        m_parameterToIndex.remove(parameter);
        m_parametersByIndex.remove(idx);
    }

    disconnect(parameter, &QObject::destroyed, this, &ParameterList::onParameterDestroyed);
    
    emit parameterRemoved(parameter);
    emit countChanged(m_parametersByIndex.count());
}

void ParameterList::removeParameter(const QString &name)
{
    Parameter* parameter = m_parametersByName.value(name, nullptr);
    if(parameter == nullptr) {
        return;
    }

    m_parametersByName.remove(name);
    m_parametersByUniqueId.remove(parameter->uniqueId());
    int idx = m_parameterToIndex.value(parameter, -1);
    if(idx > 0) {
        m_parameterToIndex.remove(parameter);
        m_parametersByIndex.remove(idx);
    }

    disconnect(parameter, &QObject::destroyed, this, &ParameterList::onParameterDestroyed);
    emit parameterRemoved(parameter);
    emit countChanged(m_parametersByIndex.count());

}

void ParameterList::clear()
{
    for (auto it = m_parametersByUniqueId.begin(); it != m_parametersByUniqueId.end(); ++it) {
        Parameter* param = it.value();
        disconnect(param, &QObject::destroyed, this, &ParameterList::onParameterDestroyed);
        emit parameterRemoved(param);
    }
    m_parametersByUniqueId.clear();
    m_parametersByIndex.clear();
    m_parameterToIndex.clear();
    m_parametersByName.clear();

    emit countChanged(0);
}

Parameter *ParameterList::parameter(int index) const
{
    return m_parametersByIndex.value(index, nullptr);
}

Parameter *ParameterList::parameter(const QString &name) const
{
    return m_parametersByName.value(name, nullptr);
}

int ParameterList::indexOf(Parameter *parameter) const
{
    return m_parameterToIndex.value(parameter, -1);
}

int ParameterList::indexOf(const QString &name) const
{
    Parameter *param = m_parametersByName.value(name, nullptr);

    return m_parameterToIndex.value(param, -1);
}

bool ParameterList::contains(Parameter *parameter) const
{
    return (m_parameterToIndex.value(parameter, -1)>0);
}

bool ParameterList::contains(const QString &name) const
{
    return (m_parametersByName.value(name, nullptr) != nullptr);
}

QList<Parameter *> ParameterList::parameters() const
{
    return m_parameterToIndex.keys();
}

QVariant ParameterList::value(const QString &name) const
{
    Parameter *param = m_parametersByName.value(name, nullptr);
    if(param == nullptr)
        return {};

    return param->value();
}

bool ParameterList::setValue(const QString &name, const QVariant &value)
{
    Parameter *param = m_parametersByName.value(name, nullptr);
    if(param == nullptr) {
        return false;
    }

    param->setValue(value);
    return true;
}

void ParameterList::onParameterDestroyed(QObject *parameter)
{
    Parameter *param = static_cast<Parameter*>(parameter);
    int idx = m_parameterToIndex.value(param, -1);
    if(idx == -1) {
        return;
    }
    m_parameterToIndex.remove(param);
    m_parametersByIndex.remove(idx);
    m_parametersByName.remove(param->name());
    m_parametersByUniqueId.remove(param->uniqueId());

    emit parameterRemoved(param);
    emit countChanged(m_parametersByIndex.count());
}


} // namespace App
} // namespace QtNoid
