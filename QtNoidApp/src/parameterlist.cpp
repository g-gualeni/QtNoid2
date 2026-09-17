//=============================================================================
// parameterlist.cpp
//=============================================================================
#include "QtNoidApp/parameterlist.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

namespace QtNoid {
namespace App {

QMutex ParameterList::s_uniqueIdMutex;
int ParameterList::s_nextUniqueId(10);  // thread-safe


int ParameterList::getNextUniqueId()
{
    QMutexLocker locker(&s_uniqueIdMutex);
    if (s_nextUniqueId == INT_MAX) {
        s_nextUniqueId = 0;  // Reset if overflow
    }
    return s_nextUniqueId++;
}


ParameterList::ParameterList(QObject *parent)
    : QObject(parent), m_uniqueId(getNextUniqueId()), m_count(0), m_visible(true)
{
}

ParameterList::ParameterList(const QString &name, QObject *parent)
    : QObject(parent), m_name(name), m_uniqueId(getNextUniqueId()), m_count(0), m_visible(true)
{
}

ParameterList::ParameterList(const QJsonObject &schemaList, const QJsonObject &valueList, QObject *parent)
    : QObject(parent), m_uniqueId(getNextUniqueId()), m_count(0), m_visible(true)
{
    // Scan schemaList and valueList to recreate the page
    // Get the name from schema or from value
    if (schemaList.count() == 1) {
        setName(schemaList.constBegin().key());
    } else if (valueList.count() == 1) {
        setName(valueList.constBegin().key());
    }

    // Get Value first to avoid ReadOnly blocking the update
    valuesFromJson(valueList);
    schemaFromJson(schemaList);
}


QJsonObject ParameterList::toJsonValues() const
{
    QString name = m_name;
    if(name.isEmpty()) {
        name = "pageName";
    }

    QJsonArray parametersArray;
    for (auto it = m_parametersByIndex.constBegin(); it != m_parametersByIndex.constEnd(); ++it) {
        Parameter* param = it.value();
        parametersArray.append(param->toJsonValue());
    }

    QJsonObject main({{"parameters", parametersArray}});
    QJsonObject json({{name, main}});
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

    QJsonObject schemaObject;
    schemaObject["description"] = m_description.value();
    schemaObject["tooltip"] = m_tooltip.value();
    schemaObject["parameters"] = parametersArray;
    schemaObject["visible"] = m_visible.value();
    schemaObject["label"] = m_label.value();
    schemaObject["readOnly"] = m_readOnly.value();


    QJsonObject schema;
    schema[name] = schemaObject;

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
    const QJsonObject jsonMain = json[name].toObject();
    const QJsonArray parametersArray = jsonMain["parameters"].toArray();
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

    const QJsonObject schemaMain = json[name].toObject();

    if(schemaMain.contains("visible")) {
        setVisible(schemaMain["visible"].toBool());
    }
    if(schemaMain.contains("description")) {
        setDescription(schemaMain["description"].toString());
    }
    if(schemaMain.contains("tooltip")) {
        setTooltip(schemaMain["tooltip"].toString());
    }
    if(schemaMain.contains("label")) {
        setLabel(schemaMain["label"].toString());
    }
    if(schemaMain.contains("readOnly")) {
        setReadOnly(schemaMain["readOnly"].toBool());
    }

    // Load parameters from schema and update the object if present
    const QJsonArray schemaArray = schemaMain["parameters"].toArray();
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


void ParameterList::setName(const QString &newName)
{
    if(m_name == newName)
        return;

    QString oldName = m_name;
    m_name = newName;
    emit nameEdited(oldName, newName);
}


QBindable<QString> ParameterList::bindableName()
{
    return QBindable<QString>(&m_name);
}

QString ParameterList::label() const
{
    return m_label.value();
}

void ParameterList::setLabel(const QString &newLabel)
{
    m_label = newLabel;
}

QBindable<QString> ParameterList::bindableLabel()
{
    return QBindable<QString>(&m_label);
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

bool ParameterList::readOnly() const
{
    return m_readOnly.value();
}

void ParameterList::setReadOnly(bool value)
{
    m_readOnly = value;
}

QBindable<bool> ParameterList::bindableReadOnly()
{
    return QBindable<bool>(&m_readOnly);
}

bool ParameterList::visible() const
{
    return m_visible.value();
}

void ParameterList::setVisible(bool value)
{
    m_visible = value;
}

QBindable<bool> ParameterList::bindableVisible()
{
    return QBindable<bool>(&m_visible);
}

bool ParameterList::isValueChanged()
{
    // qDebug() << Q_FUNC_INFO  << "m_valueChangedCounter"  << m_valueChangedCounter;
    return m_isValueChanged.value();
}

QBindable<bool> ParameterList::bindableIsValueChanged()
{
    return QBindable<bool>(&m_isValueChanged);
}

int ParameterList::count() const
{
    return m_count.value();
}

QBindable<int> ParameterList::bindableCount()
{
    return QBindable<int>(&m_count);
}

bool ParameterList::append(Parameter *parameter)
{
    if (parameter == nullptr) {
        return false;
    }
    // int paramterId = parameter->uniqueId();
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

void ParameterList::removeParameterInternal(Parameter *parameter)
{
    m_parametersByUniqueId.remove(parameter->uniqueId());
    m_parametersByName.remove(parameter->name());

    int idx = m_parameterToIndex.value(parameter, -1);
    if (idx != -1) {
        m_parameterToIndex.remove(parameter);
        m_parametersByIndex.remove(idx);
    }

    if (parameter->isValueChanged()) {
        onParameterIsValueChangedChanged(false);
    }

    disconnect(parameter, &Parameter::aboutToBeDestroyed, this, &ParameterList::onParameterAboutToBeDestroyed);
    disconnect(parameter, &Parameter::nameEdited, this, &ParameterList::onParameterNameEdited);
    disconnect(parameter, &Parameter::isValueChangedChanged, this, &ParameterList::onParameterIsValueChangedChanged);

    emit parameterRemoved(parameter);
    m_count = m_parametersByIndex.count();

    if (m_parametersByIndex.isEmpty()) {
        m_nextParameterIndex = 0;
    }

}


void ParameterList::removeParameter(Parameter *parameter)
{
    if (parameter == nullptr) {
        return;
    }
    if (!m_parametersByUniqueId.contains(parameter->uniqueId())) {
        // Not part of this list
        return;
    }
    removeParameterInternal(parameter);
}

void ParameterList::removeParameter(const QString &name)
{
    Parameter* parameter = m_parametersByName.value(name, nullptr);
    if (parameter == nullptr) {
        return;
    }
    removeParameterInternal(parameter);
}

void ParameterList::clear()
{
    if(isEmpty()) return;

    for (auto it = m_parametersByIndex.begin(); it != m_parametersByIndex.end(); ++it) {
        Parameter* param = it.value();
        disconnect(param, &Parameter::aboutToBeDestroyed, this, &ParameterList::onParameterAboutToBeDestroyed);
        disconnect(param, &Parameter::nameEdited, this, &ParameterList::onParameterNameEdited);
        disconnect(param, &Parameter::isValueChangedChanged, this, &ParameterList::onParameterIsValueChangedChanged);
        emit parameterRemoved(param);
    }

    m_valueChangedCounter = 0;
    m_isValueChanged = false;

    m_parametersByUniqueId.clear();
    m_parametersByIndex.clear();
    m_parameterToIndex.clear();
    m_parametersByName.clear();

    m_nextParameterIndex = 0;
    m_count = 0;
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


void ParameterList::onParameterAboutToBeDestroyed(Parameter *parameter, int uniqueId, bool wasChanged)
{
    int idx = m_parameterToIndex.value(parameter, -1);
    if(idx == -1) {
        return;
    }
    m_parameterToIndex.remove(parameter);
    m_parametersByUniqueId.remove(uniqueId);
    m_parametersByIndex.remove(idx);

    // Remove from m_parametersByName using param
    // because param->name() could be modified
    QString key = m_parametersByName.key(parameter);
    m_parametersByName.remove(key);

    if (wasChanged) {
        onParameterIsValueChangedChanged(false);
    }

    emit parameterRemoved(parameter);
    m_count = m_parametersByIndex.count();
    if (isEmpty()) {
        m_nextParameterIndex = 0;
    }
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

void ParameterList::onParameterIsValueChangedChanged(bool changed)
{
    if(changed) {
        ++m_valueChangedCounter;
    }
    else {
        --m_valueChangedCounter;
    }
    m_isValueChanged = (m_valueChangedCounter >0);
}

void ParameterList::appendParameterAndUpdateIndexs(Parameter *parameter)
{
    m_parametersByUniqueId.insert(parameter->uniqueId(), parameter);
    m_parameterToIndex.insert(parameter, m_nextParameterIndex);
    m_parametersByIndex.insert(m_nextParameterIndex, parameter);
    m_nextParameterIndex++;
    m_parametersByName.insert(parameter->name(), parameter);
    connect(parameter, &Parameter::aboutToBeDestroyed, this, &ParameterList::onParameterAboutToBeDestroyed);
    connect(parameter, &Parameter::nameEdited, this, &ParameterList::onParameterNameEdited);
    connect(parameter, &Parameter::isValueChangedChanged, this, &ParameterList::onParameterIsValueChangedChanged);
    if(parameter->isValueChanged()) {
        onParameterIsValueChangedChanged(true);
    }

    emit parameterAdded(parameter);
    m_count = m_parametersByIndex.count();
}




} // namespace App
} // namespace QtNoid
