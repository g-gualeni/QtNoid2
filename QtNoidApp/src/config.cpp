//=============================================================================
// config.cpp
//=============================================================================
#include "QtNoidApp/config.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

namespace QtNoid {
namespace App {

// Initialize static counter
int Config::s_nextUniqueId = 0;

Config::Config(QObject *parent)
    : QObject(parent)
{
}

Config::Config(const QString &name, QObject *parent)
    : QObject(parent), m_name(name)
{
}

Config::Config(const QJsonObject &schemaConfig, const QJsonObject &valueConfig, QObject *parent)
    : QObject(parent)
{
    // Scan schemaConfig and valueConfig to recreate the config
    QString name = m_name.value();
    if(name.isEmpty() && (schemaConfig.count() == 1)) {
        // Get the unique JSON object and use it to set the name
        name = schemaConfig.constBegin().key();
        setName(name);
    }
    else if(name.isEmpty() && (valueConfig.count() == 1)) {
        // Get the unique JSON object and use it to set the name
        name = valueConfig.constBegin().key();
        setName(name);
    }

    // Prepare a QHash map for values so I can get them fast from their name
    QHash<QString, QJsonObject> valueMap;
    const QJsonArray valueArray = valueConfig[name].toArray();
    for (const QJsonValue& value : valueArray) {
        if (value.isObject()) {
            const QJsonObject valueObj = value.toObject();
            auto valueName = valueObj.begin().key();
            valueMap.insert(valueName, valueObj);
        }
    }

    // Load parameter lists from schemaConfig and merge with values in valueConfig
    const QJsonArray schemaArray = schemaConfig[name].toArray();
    for (const QJsonValue& schema : schemaArray) {
        if (schema.isObject()) {
            const QJsonObject schemaObj = schema.toObject();
            const QString& newListName = schemaObj.constBegin().key();
            const QJsonObject valueObj = valueMap.value(newListName, {});
            auto newList = new ParameterList(schemaObj, valueObj, this);
            bool res = append(newList);
            if(!res) delete newList;
        }
    }
}

QJsonObject Config::toJsonValues() const
{
    QString name = m_name;
    if(name.isEmpty()) {
        name = "ConfigName";
    }

    QJsonArray listsArray;
    for (auto it = m_parameterListsByIndex.constBegin(); it != m_parameterListsByIndex.constEnd(); ++it) {
        ParameterList* list = it.value();
        listsArray.append(list->toJsonValues());
    }

    QJsonObject json;
    json[name] = listsArray;
    return json;
}

QJsonObject Config::toJsonSchema() const
{
    QString name = m_name;
    if(name.isEmpty()) {
        name = "ConfigName";
    }

    QJsonArray listsArray;
    for (auto it = m_parameterListsByIndex.constBegin(); it != m_parameterListsByIndex.constEnd(); ++it) {
        ParameterList* list = it.value();
        listsArray.append(list->toJsonSchema());
    }

    QJsonObject schema;
    schema[name] = listsArray;
    return schema;
}

/**
 * @brief Config::valuesFromJson load or update values using a JSON file
 * @param json
 * @return
 */
bool Config::valuesFromJson(const QJsonObject &json)
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

    // Load parameter lists
    const QJsonArray listsArray = json[name].toArray();

    for (const QJsonValue& value : listsArray) {
        if (value.isObject()) {
            const QJsonObject valueObj = value.toObject();
            const auto valueName = valueObj.constBegin().key();
            ParameterList* list = m_parameterListsByName.value(valueName, nullptr);
            if(list == nullptr) {
                auto newList = new ParameterList(this);
                newList->setName(valueName);
                if(!append(newList)) {
                    delete newList;
                } else {
                    newList->valuesFromJson(valueObj);
                }
            }
            else {
                list->valuesFromJson(valueObj);
            }
        }
    }
    return true;
}

bool Config::schemaFromJson(const QJsonObject &json)
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

    // Load parameter lists from schema and update the object if present
    const QJsonArray schemaArray = json[name].toArray();
    for (const QJsonValue& schema : schemaArray) {
        if (schema.isObject()) {
            const QJsonObject schemaObj = schema.toObject();
            const auto valueName = schemaObj.constBegin().key();
            ParameterList* list = m_parameterListsByName.value(valueName, nullptr);
            if(list == nullptr) {
                auto newList = new ParameterList(this);
                newList->setName(valueName);
                if(!append(newList)) {
                    delete newList;
                } else {
                    newList->schemaFromJson(schemaObj);
                }
            }
            else {
                list->schemaFromJson(schemaObj);
            }
        }
    }
    return true;
}

QString Config::name() const
{
    return m_name.value();
}

void Config::setName(const QString &value)
{
    m_name = value;
}

QBindable<QString> Config::bindableName()
{
    return QBindable<QString>(&m_name);
}

QString Config::description() const
{
    return m_description.value();
}

void Config::setDescription(const QString &value)
{
    m_description = value;
}

QBindable<QString> Config::bindableDescription()
{
    return QBindable<QString>(&m_description);
}

int Config::count() const
{
    return m_parameterListsByIndex.count();
}

bool Config::append(ParameterList *parameterList)
{
    if (parameterList == nullptr) {
        return false;
    }

    auto listName = parameterList->name();
    if(listName.isEmpty()){
        return false;
    }

    if (m_parameterListsByName.contains(listName)) {
        return false;
    }

    // Generate a unique ID for this list
    int uniqueId = generateUniqueId();
    if (m_parameterListsByUniqueId.contains(uniqueId)) {
        return false;
    }

    // Update all indexes
    appendParameterListAndUpdateIndexs(parameterList);
    m_parameterListsByUniqueId.insert(uniqueId, parameterList);
    return true;
}

bool Config::append(const QJsonObject& schema, const QJsonObject& value)
{
    ParameterList* parameterList = new ParameterList(schema, value, this);
    auto res = append(parameterList);
    if(!res) {
        delete parameterList;
    }
    return res;
}

ParameterList* Config::emplace(const QString& name, const QString& description)
{
    if(name.isEmpty()) {
        return {};
    }
    if (m_parameterListsByName.contains(name)) {
        return {};
    }

    ParameterList* parameterList = new ParameterList(name, this);
    parameterList->setDescription(description);
    int uniqueId = generateUniqueId();
    m_parameterListsByUniqueId.insert(uniqueId, parameterList);
    appendParameterListAndUpdateIndexs(parameterList);

    return parameterList;
}

ParameterList* Config::emplace(const QJsonObject& schema, const QJsonObject& value)
{
    ParameterList* parameterList = new ParameterList(schema, value, this);
    bool res = append(parameterList);
    if(!res) {
        delete parameterList;
        return nullptr;
    }
    return parameterList;
}

void Config::removeParameterList(ParameterList *parameterList)
{
    if (parameterList == nullptr) {
        return;
    }

    // Find the unique ID by iterating through the map
    int uniqueId = -1;
    for (auto it = m_parameterListsByUniqueId.constBegin(); it != m_parameterListsByUniqueId.constEnd(); ++it) {
        if (it.value() == parameterList) {
            uniqueId = it.key();
            break;
        }
    }

    if (uniqueId == -1) {
        // List not found
        return;
    }

    m_parameterListsByUniqueId.remove(uniqueId);
    m_parameterListsByName.remove(parameterList->name());

    int idx = m_parameterListToIndex.value(parameterList, -1);
    if(idx != -1) {
        m_parameterListToIndex.remove(parameterList);
        m_parameterListsByIndex.remove(idx);
    }

    disconnect(parameterList, &QObject::destroyed, this, &Config::onParameterListDestroyed);
    disconnect(parameterList, &ParameterList::nameChanged, this, nullptr);

    emit parameterListRemoved(parameterList);
    emit countChanged(m_parameterListsByIndex.count());
}

void Config::removeParameterList(const QString &name)
{
    ParameterList* parameterList = m_parameterListsByName.value(name, nullptr);
    if(parameterList == nullptr) {
        return;
    }

    m_parameterListsByName.remove(name);

    // Find and remove by unique ID
    int uniqueId = -1;
    for (auto it = m_parameterListsByUniqueId.constBegin(); it != m_parameterListsByUniqueId.constEnd(); ++it) {
        if (it.value() == parameterList) {
            uniqueId = it.key();
            break;
        }
    }
    if (uniqueId != -1) {
        m_parameterListsByUniqueId.remove(uniqueId);
    }

    int idx = m_parameterListToIndex.value(parameterList, -1);
    if(idx > 0) {
        m_parameterListToIndex.remove(parameterList);
        m_parameterListsByIndex.remove(idx);
    }

    disconnect(parameterList, &QObject::destroyed, this, &Config::onParameterListDestroyed);
    disconnect(parameterList, &ParameterList::nameChanged, this, nullptr);
    emit parameterListRemoved(parameterList);
    emit countChanged(m_parameterListsByIndex.count());
}

void Config::clear()
{
    if(isEmpty()) return;

    for (auto it = m_parameterListsByIndex.begin(); it != m_parameterListsByIndex.end(); ++it) {
        ParameterList* list = it.value();
        disconnect(list, &QObject::destroyed, this, &Config::onParameterListDestroyed);
        disconnect(list, &ParameterList::nameChanged, this, nullptr);
        emit parameterListRemoved(list);
    }
    m_parameterListsByUniqueId.clear();
    m_parameterListsByIndex.clear();
    m_parameterListToIndex.clear();
    m_parameterListsByName.clear();

    emit countChanged(0);
}

bool Config::isEmpty() const
{
    return m_parameterListsByIndex.isEmpty();
}

ParameterList *Config::parameterList(int index) const
{
    return m_parameterListsByIndex.value(index, nullptr);
}

ParameterList *Config::parameterList(const QString &name) const
{
    return m_parameterListsByName.value(name, nullptr);
}

int Config::indexOf(ParameterList *parameterList) const
{
    return m_parameterListToIndex.value(parameterList, -1);
}

int Config::indexOf(const QString &name) const
{
    ParameterList *list = m_parameterListsByName.value(name, nullptr);
    return m_parameterListToIndex.value(list, -1);
}

bool Config::contains(ParameterList *parameterList) const
{
    return m_parameterListToIndex.contains(parameterList);
}

bool Config::contains(const QString &name) const
{
    return m_parameterListsByName.contains(name);
}

QList<ParameterList *> Config::parameterLists() const
{
    QList<ParameterList*> res;
    for(auto it = m_parameterListsByIndex.cbegin(); it != m_parameterListsByIndex.cend(); ++it) {
        res << it.value();
    }
    return res;
}

Parameter* Config::parameter(const QString& listName, const QString& paramName) const
{
    ParameterList* list = m_parameterListsByName.value(listName, nullptr);
    if(list == nullptr)
        return nullptr;

    return list->parameter(paramName);
}

QVariant Config::value(const QString& listName, const QString& paramName) const
{
    ParameterList* list = m_parameterListsByName.value(listName, nullptr);
    if(list == nullptr)
        return {};

    return list->value(paramName);
}

bool Config::setValue(const QString& listName, const QString& paramName, const QVariant& value)
{
    ParameterList* list = m_parameterListsByName.value(listName, nullptr);
    if(list == nullptr) {
        return false;
    }

    return list->setValue(paramName, value);
}

void Config::onParameterListDestroyed(QObject *parameterList)
{
    ParameterList *list = static_cast<ParameterList*>(parameterList);
    int idx = m_parameterListToIndex.value(list, -1);
    if(idx == -1) {
        return;
    }
    m_parameterListToIndex.remove(list);
    m_parameterListsByIndex.remove(idx);

    // Find and remove by unique ID
    int uniqueId = -1;
    for (auto it = m_parameterListsByUniqueId.constBegin(); it != m_parameterListsByUniqueId.constEnd(); ++it) {
        if (it.value() == list) {
            uniqueId = it.key();
            break;
        }
    }
    if (uniqueId != -1) {
        m_parameterListsByUniqueId.remove(uniqueId);
    }

    // Remove from m_parameterListsByName using list
    // because list->name() could be modified
    QString key = m_parameterListsByName.key(list);
    m_parameterListsByName.remove(key);

    emit parameterListRemoved(list);
    emit countChanged(m_parameterListsByIndex.count());
}

void Config::onParameterListNameEdited(const QString &oldName, const QString &newName)
{
    if(!m_parameterListsByName.contains(oldName)) {
        emit parameterListRenameError(oldName, newName);
        return;
    }

    if(m_parameterListsByName.contains(newName)) {
        emit parameterListRenameError(oldName, newName);
        return;
    }

    ParameterList* currentList = m_parameterListsByName.take(oldName);
    m_parameterListsByName.insert(newName, currentList);

    return;
}

void Config::appendParameterListAndUpdateIndexs(ParameterList *parameterList)
{
    m_parameterListToIndex.insert(parameterList, m_nextParameterListIndex);
    m_parameterListsByIndex.insert(m_nextParameterListIndex, parameterList);
    m_nextParameterListIndex++;
    m_parameterListsByName.insert(parameterList->name(), parameterList);
    // connect(parameterList, &QObject::destroyed, this, &Config::onParameterListDestroyed);
    // connect(parameterList, &ParameterList::nameChanged, this, &Config::onParameterListNameEdited);

    emit parameterListAdded(parameterList);
    emit countChanged(m_parameterListsByIndex.count());
}

int Config::generateUniqueId()
{
    return s_nextUniqueId++;
}


} // namespace App
} // namespace QtNoid
