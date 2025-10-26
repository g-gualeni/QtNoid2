#ifndef QTNOID_APP_CONFIG_H
#define QTNOID_APP_CONFIG_H


#include "global.h"
#include "parameterlist.h"
#include <QObject>
#include <QHash>
#include <QList>
#include <QProperty>

namespace QtNoid {
namespace App {

class QTNOIDAPP_EXPORT Config : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName BINDABLE bindableName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString description READ description WRITE setDescription BINDABLE bindableDescription NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(QString tooltip READ tooltip WRITE setTooltip BINDABLE bindableTooltip NOTIFY tooltipChanged FINAL)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)

public:
    explicit Config(QObject *parent = nullptr);
    explicit Config(const QString& name, QObject *parent = nullptr);
    explicit Config(const QJsonObject &schemaConfig, const QJsonObject& valueConfig, QObject *parent = nullptr);

    // JSON Serialization / Deserialization
    QJsonObject toJsonValues() const;
    QJsonObject toJsonSchema() const;
    bool valuesFromJson(const QJsonObject& json);
    bool schemaFromJson(const QJsonObject& json);

    // Name
    QString name() const;
    void setName(const QString& value);
    QBindable<QString> bindableName();

    // Description
    QString description() const;
    void setDescription(const QString& value);
    QBindable<QString> bindableDescription();

    // Tooltip
    QString tooltip() const;
    void setTooltip(const QString& value);
    QBindable<QString> bindableTooltip();

    // Container management
    int count() const;
    bool append(ParameterList *page);
    bool append(const QJsonObject& schema, const QJsonObject& value);
    ParameterList* emplace(const QString& name, const QString& description = {});
    ParameterList* emplace(const QJsonObject& schema, const QJsonObject& value);
    void remove(ParameterList* page);
    void remove(const QString& pageName);
    void clear();
    bool isEmpty() const;

    // Access methods
    ParameterList* page(int index) const;
    ParameterList* page(const QString& pageName) const;
    int indexOf(ParameterList* page) const;
    int indexOf(const QString& pageName) const;
    bool contains(ParameterList* page) const;
    bool contains(const QString& pageName) const;

    // List access
    QList<ParameterList*> pages() const;

    // Convenience methods for nested access
    Parameter* parameter(const QString& paramName, const QString& pageName = "Settings") const;

    bool restoreAsBool(const QString& paramName, bool defaultValue, const QString& pageName = "Settings") const;
    bool saveValue(const QString& paramName, bool value, const QString& pageName = "Settings");

    int restoreAsInt(const QString& paramName, int defaultValue, const QString& pageName = "Settings") const;
    bool saveValue(const QString& paramName, int value, const QString& pageName = "Settings");

    double restoreAsDouble(const QString& paramName, double defaultValue, const QString& pageName = "Settings") const;
    bool saveValue(const QString& paramName, double value, const QString& pageName = "Settings");

    QString restoreAsString(const QString& paramName, const QString& defaultValue, const QString& pageName = "Settings") const;
    bool saveValue(const QString& paramName, const QString& value, const QString& pageName = "Settings");

    QStringList restoreAsStringList(const QString& paramName, const QStringList& defaultValue, const QString& pageName = "Settings") const;
    bool saveValue(const QString& paramName, const QStringList& value, const QString& pageName = "Settings");

    QVariant restoreAsVariant(const QString &paramName, const QVariant &defaultValue, const QString &pageName = "Settings") const;
    bool saveValue(const QString& paramName, const QVariant& value, const QString& pageName = "Settings");

    QByteArray restoreAsByteArray(const QString& paramName, const QByteArray defaultValue, const QString& pageName = "Settings") const;
    bool saveValue(const QString& paramName, const QByteArray& value, const QString& pageName = "Settings");

    QStringList restoreRecentFiles(const QStringList defaultValue = {}, const QString& paramName="RecentFiles", const QString& pageName = "Settings") const;
    void clearRecentFiles(const QString& paramName="RecentFiles", const QString& pageName = "Settings");
    bool addRecentFile(QString filePath, int max = 10, const QString& paramName = "RecentFiles", const QString& pageName = "Settings");

public:
    Config &operator<<(ParameterList& page){
        append(&page);
        return *this;
    };
    Config &operator<<(ParameterList* page){
        if(page) append(page);
        return *this;
    };

signals:
    void nameChanged(const QString& value);
    void descriptionChanged(const QString& value);
    void tooltipChanged(const QString& value);
    void countChanged(int count);
    void pageAdded(const QtNoid::App::ParameterList* parameterList);
    void pageRemoved(QtNoid::App::ParameterList* parameterList);
    void pageRenameError(const QString& oldName, const QString& newName);

private slots:
    void onPageDestroyed(QObject* parameterList);
    void onPageNameEdited(const QString& oldName, const QString& newName);

private:
    Q_OBJECT_BINDABLE_PROPERTY(Config, QString, m_name, &Config::nameChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Config, QString, m_description, &Config::descriptionChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Config, QString, m_tooltip, &Config::tooltipChanged)
    QHash<int, ParameterList*> m_pagesByUniqueId;
    QMap<int, ParameterList*> m_pagesByIndex;
    QHash<ParameterList*, int> m_pageToIndex; // ParameterList -> sortIndex
    QHash<QString, ParameterList*> m_pagesByName;
    int m_nextPageIndex = 0;
    void appendPageAndUpdateIndexs(ParameterList *page);

    bool saveValuePrivate(const QString &paramName, const QVariant &value, const QString &pageName);

};

} // namespace App
} // namespace QtNoid

inline QDebug operator<<(QDebug debug, const QtNoid::App::Config &config)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "Config(" << config.name()
                    << ", count: " << config.count();

    if (!config.description().isEmpty()) {
        debug << ", description: " << config.description();
    }
    if (!config.tooltip().isEmpty()) {
        debug << ", tooltip: " << config.tooltip();
    }

    if (!config.isEmpty()) {
        debug << ", pages: [";
        for (int i = 0; i < config.count(); ++i) {
            if (i > 0) debug << ", ";
            QtNoid::App::ParameterList* list = config.page(i);
            if (list) {
                debug <<   " {";
                debug << list;
                debug <<  "}";
            }
        }
        debug << "]";
    }
    debug << ")";
    return debug;
}

inline QDebug operator<<(QDebug debug, const QtNoid::App::Config *config)
{
    if (config) {
        return ::operator<<(debug, *config);
    } else {
        return debug << "Config(nullptr)";
    }
}


#endif // QTNOID_APP_CONFIG_H
