//=============================================================================
// config.cpp
//=============================================================================
#include "QtNoidApp/config.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

namespace QtNoid {
namespace App {


Config::Config(QObject *parent)
    : QObject(parent)
{}


Config::Config(const QString &name, QObject *parent)
    : QObject(parent), m_name(name)
{}


Config::Config(const QJsonObject &schemaConfig, const QJsonObject &valueConfig, QObject *parent)
    : QObject(parent)
{
    // Scan schemaConfig and valueConfig to recreate the config
    QString name = m_name.value();
    if(name.isEmpty() && (schemaConfig.count() == 1)) {
        // Get the unique JSON object in Schema and use it to set the name
        name = schemaConfig.constBegin().key();
        setName(name);
    }
    else if(name.isEmpty() && (valueConfig.count() == 1)) {
        // Get the unique top JSON object in value and use it to set the name
        name = valueConfig.constBegin().key();
        setName(name);
    }
    else {
        // There are no useful data in schema or value
        return;
    }

    // Load parameter lists from schemaConfig and merge with values in valueConfig
    const QJsonObject schemaMain = schemaConfig[name].toObject();
    if(schemaMain.contains("description")) {
        setDescription(schemaMain["description"].toString());
    }
    if(schemaMain.contains("tooltip")) {
        setTooltip(schemaMain["tooltip"].toString());
    }

    // Prepare a QHash map for values so I can get them fast from their name
    // and also skip values that are not objects
    QHash<QString, QJsonObject> valueMap;

    qDebug() << __func__ << valueConfig[name];

    const QJsonObject valueMain = valueConfig[name].toObject();
    const QJsonArray valueArray = valueMain["pages"].toArray();
    for (const QJsonValue& value : valueArray) {
        if (value.isObject()) {
            const QJsonObject valueObj = value.toObject();
            auto valueName = valueObj.begin().key();
            valueMap.insert(valueName, valueObj);
        }
    }

    const QJsonArray schemaArray = schemaMain["pages"].toArray();
    for (const QJsonValue& schema : schemaArray) {
        if (schema.isObject()) {
            const QJsonObject schemaObj = schema.toObject();
            const QString& newPageName = schemaObj.constBegin().key();
            const QJsonObject valueObj = valueMap.value(newPageName, {});
            auto newPage = new ParameterList(schemaObj, valueObj, this);
            bool res = append(newPage);
            if(!res) delete newPage;
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
    for (auto it = m_pagesByIndex.constBegin(); it != m_pagesByIndex.constEnd(); ++it) {
        ParameterList* list = it.value();
        listsArray.append(list->toJsonValues());
    }

    QJsonObject jsonMain({{"pages", listsArray}});
    QJsonObject json({{name, jsonMain}});

    return json;
}

QJsonObject Config::toJsonSchema() const
{
    QString name = m_name;
    if(name.isEmpty()) {
        name = "ConfigName";
    }

    QJsonArray listsArray;
    for (auto it = m_pagesByIndex.constBegin(); it != m_pagesByIndex.constEnd(); ++it) {
        ParameterList* list = it.value();
        listsArray.append(list->toJsonSchema());
    }

    QJsonObject schemaMain;
    schemaMain["description"] = m_description.value();
    schemaMain["tooltip"] = m_tooltip.value();
    schemaMain["pages"] = listsArray;

    QJsonObject schema ({{name, schemaMain}});

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
            ParameterList* page = m_pagesByName.value(valueName, nullptr);
            if(page == nullptr) {
                page = new ParameterList(this);
                page->setName(valueName);
                if(!append(page)) {
                    // not a good object
                    delete page;
                    continue;
                }
            }
            page->valuesFromJson(valueObj);
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

    // Load description and tooltip from schema
    const QJsonObject schemaMain = json[name].toObject();
    if(schemaMain.contains("description")) {
        setDescription(schemaMain["description"].toString());
    }
    if(schemaMain.contains("tooltip")) {
        setTooltip(schemaMain["tooltip"].toString());
    }

    // Load parameter lists from schema and update the object if present
    const QJsonArray schemaArray = schemaMain["pages"].toArray();
    for (const QJsonValue& schema : schemaArray) {
        if (schema.isObject()) {
            const QJsonObject schemaObj = schema.toObject();
            const auto valueName = schemaObj.constBegin().key();
            ParameterList* page = m_pagesByName.value(valueName, nullptr);
            if(page == nullptr) {
                page = new ParameterList(this);
                page->setName(valueName);
                if(!append(page)) {
                    delete page;
                    continue;

                }
            }
            page->schemaFromJson(schemaObj);
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


QString Config::tooltip() const
{
    return m_tooltip.value();
}


void Config::setTooltip(const QString &value)
{
    m_tooltip = value;
}


QBindable<QString> Config::bindableTooltip()
{
    return QBindable<QString>(&m_tooltip);
}


int Config::count() const
{
    return m_pagesByIndex.count();
}


bool Config::append(ParameterList *page)
{
    if (page == nullptr) {
        return false;
    }

    auto pageName = page->name();
    if(pageName.isEmpty()){
        return false;
    }

    if (m_pagesByName.contains(pageName)) {
        return false;
    }

    // Update all indexes
    appendPageAndUpdateIndexs(page);
    return true;
}

bool Config::append(const QJsonObject& schema, const QJsonObject& value)
{
    ParameterList* page = new ParameterList(schema, value, this);
    auto res = append(page);
    if(!res) {
        delete page;
    }
    return res;
}

ParameterList* Config::emplace(const QString& name, const QString& description)
{
    if(name.isEmpty()) {
        return {};
    }
    if (m_pagesByName.contains(name)) {
        return {};
    }

    ParameterList* page = new ParameterList(name, this);
    page->setDescription(description);

    appendPageAndUpdateIndexs(page);

    return page;
}

ParameterList* Config::emplace(const QJsonObject& schema, const QJsonObject& value)
{
    ParameterList* page = new ParameterList(schema, value, this);
    bool res = append(page);
    if(!res) {
        delete page;
        return {};
    }
    return page;
}


void Config::remove(ParameterList *page)
{
    if (page == nullptr) {
        return;
    }

    m_pagesByUniqueId.remove(page->uniqueId());
    m_pagesByName.remove(page->name());

    int idx = m_pageToIndex.value(page, -1);
    m_pageToIndex.remove(page);
    m_pagesByIndex.remove(idx);

    disconnect(page, &QObject::destroyed, this, &Config::onPageDestroyed);
    disconnect(page, &ParameterList::nameChanged, this, nullptr);

    emit pageRemoved(page);
    emit countChanged(m_pagesByIndex.count());
}


void Config::remove(const QString &pageName)
{
    ParameterList* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr) {
        return;
    }
    remove(page);
}


void Config::clear()
{

    if(isEmpty()) return;

    for (auto it = m_pagesByIndex.begin(); it != m_pagesByIndex.end(); ++it) {
        ParameterList* page = it.value();
        m_pagesByUniqueId.remove(page->uniqueId());
        m_pagesByName.remove(page->name());
        m_pageToIndex.remove(page);

        disconnect(page, &QObject::destroyed, this, &Config::onPageDestroyed);
        disconnect(page, &ParameterList::nameChanged, this, nullptr);

        emit pageRemoved(page);
    }
    // After removing all - it's time to clear also last index
    m_pagesByIndex.clear();

    emit countChanged(0);
}


bool Config::isEmpty() const
{
    return m_pagesByIndex.isEmpty();
}


ParameterList *Config::page(int index) const
{
    return m_pagesByIndex.value(index, nullptr);
}


ParameterList *Config::page(const QString &pageName) const
{
    return m_pagesByName.value(pageName, nullptr);
}


int Config::indexOf(ParameterList *page) const
{
    return m_pageToIndex.value(page, -1);
}


int Config::indexOf(const QString &pageName) const
{
    ParameterList *list = m_pagesByName.value(pageName, nullptr);
    return m_pageToIndex.value(list, -1);
}


bool Config::contains(ParameterList *page) const
{
    return m_pageToIndex.contains(page);
}


bool Config::contains(const QString &pageName) const
{
    return m_pagesByName.contains(pageName);
}


QList<ParameterList *> Config::pages() const
{
    QList<ParameterList*> res;
    for(auto it = m_pagesByIndex.cbegin(); it != m_pagesByIndex.cend(); ++it) {
        res << it.value();
    }
    return res;
}


int Config::parametersCount(const QString &pageName) const
{
    ParameterList* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return {};

    return page->count();
}


Parameter* Config::parameter(const QString& paramName, const QString& pageName) const
{
    ParameterList* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return {};

    return page->parameter(paramName);
}


bool Config::saveValuePrivate(const QString &paramName, const QVariant &value, const QString &pageName)
{
    // qDebug() << __func__ << value;
    ParameterList* page = m_pagesByName.value(pageName, nullptr);
    if(page != nullptr) {

        // Update existing page
        if(page->contains(paramName)) {
            // Update the existing value
            return page->setValue(paramName, value);
        }
        // Create the new value
        auto newParam = page->emplace(value, paramName);
        if(newParam == nullptr) {
            return false;
        }
        return true;
    }

    // Create the list and the parameter
    page = emplace(pageName);
    if(page == nullptr) {
        return false;
    }
    auto param = page->emplace(value, paramName);
    if(param == nullptr) {
        return false;
    }

    return true;
}


bool Config::restoreAsBool(const QString &paramName, bool defaultValue, const QString &pageName) const
{
    ParameterList* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return defaultValue;

    return page->value(paramName).toBool();
}


/**
 * @brief Config::setValue: save and create if not exists
 * @param paramName
 * @param value
 * @param listName
 * @return
 */
bool Config::saveValue(const QString &paramName, bool value, const QString &pageName)
{
    return saveValuePrivate(paramName, value, pageName);
}


int Config::restoreAsInt(const QString &paramName, int defaultValue, const QString &pageName) const
{
    ParameterList* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return defaultValue;

    return page->value(paramName).toInt();
}


bool Config::saveValue(const QString &paramName, int value, const QString &pageName)
{
    return saveValuePrivate(paramName, value, pageName);
}


double Config::restoreAsDouble(const QString &paramName, double defaultValue, const QString &pageName) const
{
    ParameterList* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return defaultValue;

    return page->value(paramName).toDouble();
}


bool Config::saveValue(const QString &paramName, double value, const QString &pageName)
{
    return saveValuePrivate(paramName, value, pageName);
}


QString Config::restoreAsString(const QString &paramName, const QString &defaultValue, const QString &pageName) const
{
    ParameterList* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return defaultValue;

    return page->value(paramName).toString();
}


bool Config::saveValue(const QString &paramName, const QString &value, const QString &pageName)
{
    return saveValuePrivate(paramName, value, pageName);
}


QStringList Config::restoreAsStringList(const QString &paramName, const QStringList &defaultValue, const QString &pageName) const
{
    ParameterList* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return defaultValue;

    return page->value(paramName).toStringList();
}


bool Config::saveValue(const QString &paramName, const QStringList &value, const QString &pageName)
{
    return saveValuePrivate(paramName, value, pageName);
}


QVariant Config::restoreAsVariant(const QString& paramName, const QVariant &defaultValue, const QString& pageName) const
{
    ParameterList* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return defaultValue;

    return page->value(paramName);
}


bool Config::saveValue(const QString& paramName, const QVariant& value, const QString& pageName)
{
    return saveValuePrivate(paramName, value, pageName);
}


QByteArray Config::restoreAsByteArray(const QString &paramName, const QByteArray defaultValue, const QString &pageName) const
{
    ParameterList* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return defaultValue;

    auto valueAsString = page->value(paramName).toString();

    return QByteArray::fromBase64(valueAsString.toUtf8());
}


bool Config::saveValue(const QString &paramName, const QByteArray &value, const QString &pageName)
{
    auto valueString = QString::fromUtf8(value.toBase64());
    return saveValuePrivate(paramName, valueString, pageName);
}

QStringList Config::restoreRecentFiles(const QStringList defaultValue, const QString &paramName, const QString &pageName) const
{
    ParameterList* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return defaultValue;

    return page->value(paramName).toStringList();
}

void Config::clearRecentFiles(const QString &paramName, const QString &pageName)
{
    ParameterList* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return;

    if(page->contains(paramName)){
        page->setValue(paramName, QStringList());
    }
}

bool Config::addRecentFile(QString filePath, int max, const QString &paramName, const QString &pageName)
{
    // qDebug() << __func__ << filePath << max << paramName << listName;

    if(filePath.isEmpty())
        return false;

    ParameterList* page = m_pagesByName.value(pageName, nullptr);

    // Create the list and the parameter
    QStringList filePathList;
    if(page == nullptr) {
        // Create a new page and item
        page = emplace(pageName);
        if(page == nullptr) {
            return false;
        }
        filePathList << filePath;
        page->emplace(filePathList, paramName);
        return true;
    }

    // Expand the list or create if not available
    if(!page->contains(paramName)) {
        filePathList << filePath;
        page->emplace(filePathList, paramName);
        return true;
    }

    filePathList = page->value(paramName).toStringList();
    // Remove the file if it already exists to avoid duplicates
    filePathList.removeAll(filePath);
    filePathList.prepend(filePath);
    // Enforce maximum limit
    if(filePathList.size() > max) {
        filePathList.resize(max);
    }

    page->setValue(paramName, filePathList);

    return true;
}



void Config::onPageDestroyed(QObject *obj)
{
    ParameterList *page = qobject_cast<ParameterList*>(obj);

    if(page == nullptr)
        return;

    int idx = m_pageToIndex.value(page, -1);
    if(idx == -1) {
        return;
    }
    m_pageToIndex.remove(page);
    m_pagesByIndex.remove(idx);
    m_pagesByUniqueId.remove(page->uniqueId());
    m_pagesByName.remove(page->name());

    emit pageRemoved(page);
    emit countChanged(m_pagesByIndex.count());
}


void Config::onPageNameEdited(const QString &oldName, const QString &newName)
{
    if(!m_pagesByName.contains(oldName)) {
        emit pageRenameError(oldName, newName);
        return;
    }

    if(m_pagesByName.contains(newName)) {
        emit pageRenameError(oldName, newName);
        return;
    }

    ParameterList* page = m_pagesByName.take(oldName);
    m_pagesByName.insert(newName, page);

    return;
}

void Config::appendPageAndUpdateIndexs(ParameterList *page)
{
    m_pageToIndex.insert(page, m_nextPageIndex);
    m_pagesByIndex.insert(m_nextPageIndex, page);

    // NOW I can increment the pageIndex
    m_nextPageIndex++;
    m_pagesByName.insert(page->name(), page);
    m_pagesByUniqueId.insert(page->uniqueId(), page);

    connect(page, &QObject::destroyed, this, &Config::onPageDestroyed);
    connect(page, &ParameterList::nameEdited, this, &Config::onPageNameEdited);

    emit pageAdded(page);
    emit countChanged(m_pagesByIndex.count());
}


} // namespace App
} // namespace QtNoid
