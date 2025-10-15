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

    // Container management
    int count() const;
    bool append(ParameterList *parameterList);
    bool append(const QJsonObject& schema, const QJsonObject& value);
    ParameterList* emplace(const QString& name, const QString& description = {});
    ParameterList* emplace(const QJsonObject& schema, const QJsonObject& value);
    void removeParameterList(ParameterList* parameterList);
    void removeParameterList(const QString& name);
    void clear();
    bool isEmpty() const;

    // Access methods
    ParameterList* parameterList(int index) const;
    ParameterList* parameterList(const QString& name) const;
    int indexOf(ParameterList* parameterList) const;
    int indexOf(const QString& name) const;
    bool contains(ParameterList* parameterList) const;
    bool contains(const QString& name) const;

    // List access
    QList<ParameterList*> parameterLists() const;

    // Convenience methods for nested access
    Parameter* parameter(const QString& listName, const QString& paramName) const;
    QVariant value(const QString& listName, const QString& paramName) const;
    bool setValue(const QString& listName, const QString& paramName, const QVariant& value);

public:
    Config &operator<<(ParameterList& paramList){
        append(&paramList);
        return *this;
    };
    Config &operator<<(ParameterList* paramList){
        if(paramList) append(paramList);
        return *this;
    };

signals:
    void nameChanged(const QString& value);
    void descriptionChanged(const QString& value);
    void countChanged(int count);
    void parameterListAdded(const QtNoid::App::ParameterList* parameterList);
    void parameterListRemoved(QtNoid::App::ParameterList* parameterList);
    void parameterListRenameError(const QString& oldName, const QString& newName);

private slots:
    void onParameterListDestroyed(QObject* parameterList);
    void onParameterListNameEdited(const QString& oldName, const QString& newName);

private:
    Q_OBJECT_BINDABLE_PROPERTY(Config, QString, m_name, &Config::nameChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Config, QString, m_description, &Config::descriptionChanged)
    QHash<int, ParameterList*> m_parameterListsByUniqueId;
    QMap<int, ParameterList*> m_parameterListsByIndex;
    QHash<ParameterList*, int> m_parameterListToIndex; // ParameterList -> sortIndex
    QHash<QString, ParameterList*> m_parameterListsByName;
    int m_nextParameterListIndex = 0;
    void appendParameterListAndUpdateIndexs(ParameterList *parameterList);

    // Static counter for unique IDs
    static int s_nextUniqueId;
    int generateUniqueId();
};

} // namespace App
} // namespace QtNoid

inline QDebug operator<<(QDebug debug, const QtNoid::App::Config &config)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "Config(" << config.name()
                    << ", count=" << config.count();

    if (!config.isEmpty()) {
        debug << ", parameterLists=[";
        for (int i = 0; i < config.count(); ++i) {
            if (i > 0) debug << ", ";
            QtNoid::App::ParameterList* list = config.parameterList(i);
            if (list) {
                debug << list->name() << ":{count=" << list->count() << "}";
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
