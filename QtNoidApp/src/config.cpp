//=============================================================================
// config.cpp
//=============================================================================
#include "QtNoidApp/config.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QComboBox>

namespace QtNoid {
namespace App {


Config::Config(QObject *parent)
    : QObject(parent), m_count(0)
{}


Config::Config(const QString &name, QObject *parent)
    : QObject(parent), m_name(name), m_count(0)
{}


Config::Config(const QJsonObject &schemaConfig, const QJsonObject &valueConfig, QObject *parent)
    : QObject(parent), m_count(0)
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

    // Load properties that are only in schema
    const QJsonObject schemaMain = schemaConfig[name].toObject();
    if(schemaMain.contains("description")) {
        setDescription(schemaMain["description"].toString());
    }
    if(schemaMain.contains("tooltip")) {
        setTooltip(schemaMain["tooltip"].toString());
    }

    // Generate objects from schema
    const QJsonArray schemaArray = schemaMain["pages"].toArray();
    for (const QJsonValue& schema : schemaArray) {
        if (schema.isObject()) {
            const QJsonObject schemaObj = schema.toObject();
            const QString& newPageName = schemaObj.constBegin().key();
            auto newPage = new ParametersPage(schemaObj, {}, this);
            bool res = append(newPage);
            if(!res) delete newPage;
        }
    }

    // Generate or update using objects from JSON Values
    // qDebug() << __func__ << valueConfig[name];
    const QJsonObject valueMain = valueConfig[name].toObject();
    const QJsonArray valueArray = valueMain["pages"].toArray();
    for (const QJsonValue& value : valueArray) {
        if (!value.isObject()) {
            continue;
        }
        const QJsonObject valueObj = value.toObject();
        auto valueName = valueObj.begin().key();
        if(contains(valueName)){
            m_pagesByName[valueName]->valuesFromJson(valueObj);
        }
        else {
            auto newPage = new ParametersPage({}, valueObj, this);
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
        ParametersPage* list = it.value();
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
        ParametersPage* list = it.value();
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
    const QJsonObject valueMain = json[name].toObject();
    const QJsonArray valueArray = valueMain["pages"].toArray();
    for (const QJsonValue& value : valueArray) {
        if (value.isObject()) {
            const QJsonObject valueObj = value.toObject();
            const auto valueName = valueObj.constBegin().key();
            ParametersPage* page = m_pagesByName.value(valueName, nullptr);
            if(page == nullptr) {
                page = new ParametersPage(this);
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
            ParametersPage* page = m_pagesByName.value(valueName, nullptr);
            if(page == nullptr) {
                page = new ParametersPage(this);
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
    return m_count.value();
}

QBindable<int> Config::bindableCount()
{
    return QBindable<int>(&m_count);
}

bool Config::append(ParametersPage *page)
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
    ParametersPage* page = new ParametersPage(schema, value, this);
    auto res = append(page);
    if(!res) {
        delete page;
    }
    return res;
}

ParametersPage* Config::emplace(const QString& name, const QString& description)
{
    if(name.isEmpty()) {
        return {};
    }
    if (m_pagesByName.contains(name)) {
        return {};
    }

    ParametersPage* page = new ParametersPage(name, this);
    page->setDescription(description);

    appendPageAndUpdateIndexs(page);

    return page;
}

ParametersPage* Config::emplace(const QJsonObject& schema, const QJsonObject& values)
{
    ParametersPage* page = new ParametersPage(schema, values, this);
    bool res = append(page);
    if(!res) {
        delete page;
        return {};
    }
    return page;
}


void Config::remove(ParametersPage *page)
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
    disconnect(page, &ParametersPage::nameChanged, this, nullptr);

    emit pageRemoved(page);
    m_count = m_pagesByIndex.count();
}


void Config::remove(const QString &pageName)
{
    ParametersPage* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr) {
        return;
    }
    remove(page);
}


void Config::clear()
{

    if(isEmpty()) return;

    for (auto it = m_pagesByIndex.begin(); it != m_pagesByIndex.end(); ++it) {
        ParametersPage* page = it.value();
        m_pagesByUniqueId.remove(page->uniqueId());
        m_pagesByName.remove(page->name());
        m_pageToIndex.remove(page);

        disconnect(page, &QObject::destroyed, this, &Config::onPageDestroyed);
        disconnect(page, &ParametersPage::nameChanged, this, nullptr);

        emit pageRemoved(page);
    }
    // After removing all - it's time to clear also last index
    m_pagesByIndex.clear();

    m_count = 0;
}


bool Config::isEmpty() const
{
    return m_pagesByIndex.isEmpty();
}


ParametersPage *Config::page(int index) const
{
    return m_pagesByIndex.value(index, nullptr);
}


ParametersPage *Config::page(const QString &pageName) const
{
    return m_pagesByName.value(pageName, nullptr);
}


int Config::indexOf(ParametersPage *page) const
{
    return m_pageToIndex.value(page, -1);
}


int Config::indexOf(const QString &pageName) const
{
    ParametersPage *list = m_pagesByName.value(pageName, nullptr);
    return m_pageToIndex.value(list, -1);
}


bool Config::contains(ParametersPage *page) const
{
    return m_pageToIndex.contains(page);
}


bool Config::contains(const QString &pageName) const
{
    return m_pagesByName.contains(pageName);
}


QList<ParametersPage *> Config::pages() const
{
    QList<ParametersPage*> res;
    for(auto it = m_pagesByIndex.cbegin(); it != m_pagesByIndex.cend(); ++it) {
        res << it.value();
    }
    return res;
}


int Config::parametersCount(const QString &pageName) const
{
    ParametersPage* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return {};

    return page->count();
}


Parameter* Config::parameter(const QString& paramName, const QString& pageName) const
{
    ParametersPage* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return {};

    return page->parameter(paramName);
}


bool Config::saveValuePrivate(const QString &paramName, const QVariant &value, const QString &pageName)
{
    // qDebug() << __func__ << value;
    ParametersPage* page = m_pagesByName.value(pageName, nullptr);
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
    ParametersPage* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return defaultValue;

    if(!page->contains(paramName))
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
    ParametersPage* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return defaultValue;

    if(!page->contains(paramName))
        return defaultValue;

    return page->value(paramName).toInt();
}


bool Config::saveValue(const QString &paramName, int value, const QString &pageName)
{
    return saveValuePrivate(paramName, value, pageName);
}


double Config::restoreAsDouble(const QString &paramName, double defaultValue, const QString &pageName) const
{
    ParametersPage* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return defaultValue;

    if(!page->contains(paramName))
        return defaultValue;

    return page->value(paramName).toDouble();
}


bool Config::saveValue(const QString &paramName, double value, const QString &pageName)
{
    return saveValuePrivate(paramName, value, pageName);
}


QString Config::restoreAsString(const QString &paramName, const QString &defaultValue, const QString &pageName) const
{
    ParametersPage* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return defaultValue;

    if(!page->contains(paramName))
        return defaultValue;

    return page->value(paramName).toString();
}


bool Config::saveValue(const QString &paramName, const QString &value, const QString &pageName)
{
    return saveValuePrivate(paramName, value, pageName);
}


QStringList Config::restoreAsStringList(const QString &paramName, const QStringList &defaultValue, const QString &pageName) const
{
    ParametersPage* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return defaultValue;

    if(!page->contains(paramName))
        return defaultValue;

    return page->value(paramName).toStringList();
}


bool Config::saveValue(const QString &paramName, const QStringList &value, const QString &pageName)
{
    return saveValuePrivate(paramName, value, pageName);
}


QVariant Config::restoreAsVariant(const QString& paramName, const QVariant &defaultValue, const QString& pageName) const
{
    ParametersPage* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return defaultValue;

    if(!page->contains(paramName))
        return defaultValue;

    return page->value(paramName);
}


bool Config::saveValue(const QString& paramName, const QVariant& value, const QString& pageName)
{
    return saveValuePrivate(paramName, value, pageName);
}


QByteArray Config::restoreAsByteArray(const QString &paramName, const QByteArray defaultValue, const QString &pageName) const
{
    ParametersPage* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return defaultValue;

    if(!page->contains(paramName))
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
    ParametersPage* page = m_pagesByName.value(pageName, nullptr);
    if(page == nullptr)
        return defaultValue;

    if(!page->contains(paramName))
        return defaultValue;

    return page->value(paramName).toStringList();
}

void Config::clearRecentFiles(const QString &paramName, const QString &pageName)
{
    ParametersPage* page = m_pagesByName.value(pageName, nullptr);
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

    ParametersPage* page = m_pagesByName.value(pageName, nullptr);

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

void Config::restoreComboBoxTextItems(QComboBox *cbo, const QString &paramName, const QStringList &defaultValue, const QString &pageName) const
{
    if(cbo == nullptr)
        return;

    QStringList items = defaultValue;
    QString currentItem;

    do {
        ParametersPage* page = m_pagesByName.value(pageName, nullptr);
        if(page == nullptr)
            break;
        if(!page->contains(paramName))
            break;
        items = page->value(paramName).toStringList();

        QString selectedItemParamName = paramName + QStringLiteral("Current");
        if(!page->contains(selectedItemParamName))
            break;

        currentItem = page->value(selectedItemParamName).toString();

    } while(0);

    cbo->clear();
    cbo->addItems(items);
    cbo->setCurrentText(currentItem);
}

void Config::saveComboBoxTextItems(QComboBox *cbo, const QString &paramName, const QString &pageName)
{
    if(cbo == nullptr)
        return;

    QStringList items;
    for(int ii=0; ii<cbo->count(); ii++) {
        items << cbo->itemText(ii);
    }
    saveValuePrivate(paramName, items, pageName);

    QString currentItem = cbo->currentText();
    QString selectedItemParamName = paramName + QStringLiteral("Current");
    saveValuePrivate(selectedItemParamName, currentItem, pageName);

}



void Config::onPageDestroyed(QObject *obj)
{
    ParametersPage *page = qobject_cast<ParametersPage*>(obj);

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
    m_count = m_pagesByIndex.count();
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

    ParametersPage* page = m_pagesByName.take(oldName);
    m_pagesByName.insert(newName, page);

    return;
}

void Config::appendPageAndUpdateIndexs(ParametersPage *page)
{
    m_pageToIndex.insert(page, m_nextPageIndex);
    m_pagesByIndex.insert(m_nextPageIndex, page);

    // NOW I can increment the pageIndex
    m_nextPageIndex++;
    m_pagesByName.insert(page->name(), page);
    m_pagesByUniqueId.insert(page->uniqueId(), page);

    connect(page, &QObject::destroyed, this, &Config::onPageDestroyed);
    connect(page, &ParametersPage::nameEdited, this, &Config::onPageNameEdited);

    emit pageAdded(page);
    m_count = m_pagesByIndex.count();
}


} // namespace App
} // namespace QtNoid
