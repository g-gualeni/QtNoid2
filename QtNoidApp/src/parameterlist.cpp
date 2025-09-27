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
    // Scan schemaList and valueList to recreate the page
    QString name = m_name.value();
    if(name.isEmpty() && (schemaList.count() == 1)) {
        // Get the unique JSON object and use it to set the name
        name = schemaList.constBegin().key();
        setName(name);
    }
    else if(name.isEmpty() && (valueList.count() == 1)) {
        // Get the unique JSON object and use it to set the name
        name = valueList.constBegin().key();
        setName(name);
    }

    // Prepare a QHash map for values so I can get them fast from their name
    QHash<QString, QJsonObject> valueMap;
    const QJsonArray valueArray = valueList[name].toArray();
    for (const QJsonValue& value : valueArray) {
        if (value.isObject()) {
            const QJsonObject valueObj = value.toObject();
            auto valueName = valueObj.begin().key();
            valueMap.insert(valueName, valueObj);
        }
    }

    // Load parameters from schemaList and merge with values in valueList
    const QJsonArray schemaArray = schemaList[name].toArray();
    for (const QJsonValue& schema : schemaArray) {
        if (schema.isObject()) {
            const QJsonObject schemaObj = schema.toObject();
            const QString& newParamName = schemaObj.constBegin().key();
            const QJsonObject valueObj = valueMap.value(newParamName, {});
            auto newParam = new Parameter(schemaObj, valueObj, this);
            bool res = append(newParam);
            if(!res) delete newParam;
        }
    }
}

QJsonObject ParameterList::toJsonValues() const
{
    QString name = m_name;
    if(name.isEmpty()) {
        name = "PageName";
    }

    QJsonArray parametersArray;
    for (auto it = m_parametersByIndex.constBegin(); it != m_parametersByIndex.constEnd(); ++it) {
        Parameter* param = it.value();
        parametersArray.append(param->toJsonValue());
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
    for (auto it = m_parametersByIndex.constBegin(); it != m_parametersByIndex.constEnd(); ++it) {
        Parameter* param = it.value();
        parametersArray.append(param->toJsonSchema());
    }

    QJsonObject schema;
    schema[name] = parametersArray;
    return schema;
}

/**
 * @brief ParameterList::valuesFromJson load or update values using an JSON file
 * @param json
 * @return
 */
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

    // Load parameters
    const QJsonArray parametersArray = json[name].toArray();

    // qDebug() << __func__ << parametersArray;

    for (const QJsonValue& value : parametersArray) {
        if (value.isObject()) {
            const QJsonObject valueObj = value.toObject();
            const auto valueName = valueObj.constBegin().key();
            const auto valueVal = valueObj.constBegin().value().toVariant();
            Parameter* par = m_parametersByName.value(valueName, nullptr);
            if(par == nullptr) {
                auto newParam = new Parameter(valueVal, valueName, this);
                if(!append(newParam)) {
                    delete newParam;
                }
            }
            else {
                par->setValue(valueVal);
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

    // Load parameters from schema and update the object if present
    const QJsonArray schemaArray = json[name].toArray();
    for (const QJsonValue& schema : schemaArray) {
        if (schema.isObject()) {
            const QJsonObject schemaObj = schema.toObject();
            const auto valueName = schemaObj.constBegin().key();
            Parameter* par = m_parametersByName.value(valueName, nullptr);
            if(par == nullptr) {
                auto newParam = new Parameter(schemaObj, {}, this);
                if(!append(newParam)) {
                    delete newParam;
                }
            }
            else {
                par->schemaFromJson(schemaObj);
            }
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

QString ParameterList::description() const
{
    return m_description.value();
}

void ParameterList::setDescription(const QString &value)
{
    m_description = value;
}

QBindable<QString> ParameterList::bindableDescription()
{
    return QBindable<QString>(&m_description);
}

QString ParameterList::tooltip() const
{
    return m_tooltip.value();
}

void ParameterList::setTooltip(const QString &value)
{
    m_tooltip = value;
}

QBindable<QString> ParameterList::bindableTooltip()
{
    return QBindable<QString>(&m_tooltip);
}

int ParameterList::count() const
{
    return m_parametersByIndex.count();
}

bool ParameterList::append(Parameter *parameter)
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
    appendParameterAndUpdateIndexs(parameter);
    return true;
}

bool ParameterList::append(const QJsonObject& schema, const QJsonObject& value)
{
    Parameter* parameter = new Parameter(schema, value, this);
    auto res = append(parameter);
    if(!res) {
        delete parameter;
    }
    return res;
}

Parameter* ParameterList::emplace(const QVariant& initialValue, const QString& name, const QString& description)
{
    if(name.isEmpty()) {
        return {};
    }
    if (m_parametersByName.contains(name)) {
        return {};
    }

    Parameter* parameter = new Parameter(initialValue, name, description, this);
    appendParameterAndUpdateIndexs(parameter);

    return parameter;
}

Parameter* ParameterList::emplace(const QJsonObject& schema, const QJsonObject& value)
{
    Parameter* parameter = new Parameter(schema, value, this);
    bool res = append(parameter);
    if(!res) {
        delete parameter;
        return nullptr;
    }
    return parameter;
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

    m_parametersByName.remove(parameter->name());

    int idx = m_parameterToIndex.value(parameter, -1);
    if(idx != -1) {
        m_parameterToIndex.remove(parameter);
        m_parametersByIndex.remove(idx);
    }

    disconnect(parameter, &QObject::destroyed, this, &ParameterList::onParameterDestroyed);
    disconnect(parameter, &Parameter::nameEdited, this, &ParameterList::onParameterNameEdited);

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
    disconnect(parameter, &Parameter::nameEdited, this, &ParameterList::onParameterNameEdited);
    emit parameterRemoved(parameter);
    emit countChanged(m_parametersByIndex.count());
}

void ParameterList::clear()
{
    if(isEmpty()) return;

    for (auto it = m_parametersByIndex.begin(); it != m_parametersByIndex.end(); ++it) {
        Parameter* param = it.value();
        disconnect(param, &QObject::destroyed, this, &ParameterList::onParameterDestroyed);
        disconnect(param, &Parameter::nameEdited, this, &ParameterList::onParameterNameEdited);
        emit parameterRemoved(param);
    }
    m_parametersByUniqueId.clear();
    m_parametersByIndex.clear();
    m_parameterToIndex.clear();
    m_parametersByName.clear();

    emit countChanged(0);
}

bool ParameterList::isEmpty() const
{
    return m_parametersByIndex.isEmpty();
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
    return m_parameterToIndex.contains(parameter);
}

bool ParameterList::contains(const QString &name) const
{
    return m_parametersByName.contains(name);
}

QList<Parameter *> ParameterList::parameters() const
{
    // return m_parameterToIndex.keys(); -> this doesn't guarantee the sorting
    QList<Parameter*> res;
    for(auto it = m_parametersByIndex.cbegin(); it != m_parametersByIndex.cend(); ++it) {
        res << it.value();
    }
    return res;
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

void ParameterList::applyPreset(const QString &presetName)
{
    for (auto it = m_parametersByIndex.constBegin(); it != m_parametersByIndex.constEnd(); ++it) {
        Parameter* param = it.value();
        param->applyPreset(presetName);
    }
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
    m_parametersByUniqueId.remove(param->uniqueId());

    // Remove from m_parametersByName using param
    // because param->name() could be modified
    QString key = m_parametersByName.key(param);
    m_parametersByName.remove(key);

    emit parameterRemoved(param);
    emit countChanged(m_parametersByIndex.count());
}

void ParameterList::onParameterNameEdited(const QString &oldName, const QString &newName)
{
    if(!m_parametersByName.contains(oldName)) {
        emit parameterRenameError(oldName, newName);
        return;
    }

    if(m_parametersByName.contains(newName)) {
        emit parameterRenameError(oldName, newName);
        return;
    }

    Parameter* currentParameter = m_parametersByName.take(oldName);
    m_parametersByName.insert(newName, currentParameter);

    return;
}

void ParameterList::appendParameterAndUpdateIndexs(Parameter *parameter)
{
    m_parametersByUniqueId.insert(parameter->uniqueId(), parameter);
    m_parameterToIndex.insert(parameter, m_nextParameterIndex);
    m_parametersByIndex.insert(m_nextParameterIndex, parameter);
    m_nextParameterIndex++;
    m_parametersByName.insert(parameter->name(), parameter);
    connect(parameter, &QObject::destroyed, this, &ParameterList::onParameterDestroyed);
    connect(parameter, &Parameter::nameEdited, this, &ParameterList::onParameterNameEdited);

    emit parameterAdded(parameter);
    emit countChanged(m_parametersByIndex.count());
}


} // namespace App
} // namespace QtNoid
