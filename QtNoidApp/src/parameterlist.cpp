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
    for (const Parameter* param : m_parameters) {
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
    for (const Parameter* param : m_parameters) {
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
            addParameter(new Parameter(valueName, valueVal, this));
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

    // Clear existing parameters
    clear();

    // Load parameters from schema
    const QJsonArray schemaArray = json[name].toArray();
    for (const QJsonValue& schema : schemaArray) {
        if (schema.isObject()) {
            const QJsonObject schemaObj = schema.toObject();
            addParameter(new Parameter(schemaObj, {}, this));
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
    return m_parameters.count();
}

void ParameterList::addParameter(Parameter *parameter)
{
    if (!parameter || m_parameters.contains(parameter)) {
        return;
    }
    
    m_parameters.append(parameter);
    connect(parameter, &QObject::destroyed, this, &ParameterList::onParameterDestroyed);
    
    emit parameterAdded(parameter);
    emit countChanged(m_parameters.count());
}

void ParameterList::removeParameter(Parameter *parameter)
{
    if (!parameter || !m_parameters.contains(parameter)) {
        return;
    }
    
    m_parameters.removeOne(parameter);
    disconnect(parameter, &QObject::destroyed, this, &ParameterList::onParameterDestroyed);
    
    emit parameterRemoved(parameter);
    emit countChanged(m_parameters.count());
}

void ParameterList::removeParameter(const QString &name)
{
    Parameter *param = parameter(name);
    if (param) {
        removeParameter(param);
    }
}

void ParameterList::clear()
{
    if(m_parameters.isEmpty())
        return;

    while (!m_parameters.isEmpty()) {
        Parameter *param = m_parameters.takeLast();
        disconnect(param, &QObject::destroyed, this, &ParameterList::onParameterDestroyed);
        emit parameterRemoved(param);
    }
    emit countChanged(0);
}

Parameter *ParameterList::parameter(int index) const
{
    if (index < 0 || index >= m_parameters.count()) {
        return nullptr;
    }
    return m_parameters.at(index);
}

Parameter *ParameterList::parameter(const QString &name) const
{
    for (Parameter *param : m_parameters) {
        if (param && param->name() == name) {
            return param;
        }
    }
    return nullptr;
}

int ParameterList::indexOf(Parameter *parameter) const
{
    return m_parameters.indexOf(parameter);
}

int ParameterList::indexOf(const QString &name) const
{
    for (int i = 0; i < m_parameters.count(); ++i) {
        if (m_parameters.at(i) && m_parameters.at(i)->name() == name) {
            return i;
        }
    }
    return -1;
}

bool ParameterList::contains(Parameter *parameter) const
{
    return m_parameters.contains(parameter);
}

bool ParameterList::contains(const QString &name) const
{
    return parameter(name) != nullptr;
}

QList<Parameter *> ParameterList::parameters() const
{
    return m_parameters;
}

QVariant ParameterList::value(const QString &name) const
{
    Parameter *param = parameter(name);
    return param ? param->value() : QVariant();
}

bool ParameterList::setValue(const QString &name, const QVariant &value)
{
    Parameter *param = parameter(name);
    if (param) {
        param->setValue(value);
        return true;
    }
    return false;
}

void ParameterList::onParameterDestroyed(QObject *parameter)
{
    Parameter *param = static_cast<Parameter*>(parameter);
    if (m_parameters.contains(param)) {
        m_parameters.removeOne(param);
        emit parameterRemoved(param);
        emit countChanged(m_parameters.count());
    }
}


} // namespace App
} // namespace QtNoid
