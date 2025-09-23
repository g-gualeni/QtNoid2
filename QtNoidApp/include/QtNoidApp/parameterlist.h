#ifndef QTNOID_APP_PARAMETERLIST_H
#define QTNOID_APP_PARAMETERLIST_H


#include "global.h"
#include "parameter.h"
#include <QObject>
#include <QList>
#include <QProperty>

namespace QtNoid {
namespace App {

class QTNOIDAPP_EXPORT ParameterList : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName BINDABLE bindableName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString description READ description WRITE setDescription BINDABLE bindableDescription NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(QString tooltip READ tooltip WRITE setTooltip BINDABLE bindableTooltip NOTIFY tooltipChanged FINAL)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)

public:
    explicit ParameterList(QObject *parent = nullptr);
    explicit ParameterList(const QString& name, QObject *parent = nullptr);
    explicit ParameterList(const QJsonObject &schemaList, const QJsonObject& valueList, QObject *parent = nullptr);

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
    
    // List management
    int count() const;
    bool append(Parameter* parameter);
    bool append(const QJsonObject& schema, const QJsonObject& value);
    Parameter* emplace(const QString& name, const QString& description = QString(), const QVariant& initialValue = QVariant());
    Parameter* emplace(const QJsonObject& schema, const QJsonObject& value);
    void removeParameter(Parameter* parameter);
    void removeParameter(const QString& name);
    void clear();
    bool isEmpty();
    
    // Access methods
    Parameter* parameter(int index) const;
    Parameter* parameter(const QString& name) const;
    int indexOf(Parameter* parameter) const;
    int indexOf(const QString& name) const;
    bool contains(Parameter* parameter) const;
    bool contains(const QString& name) const;
    
    // List access
    QList<Parameter*> parameters() const;
    
    // Convenience methods
    QVariant value(const QString& name) const;
    bool setValue(const QString& name, const QVariant& value);
    
signals:
    void nameChanged(const QString& value);
    void descriptionChanged(const QString& value);
    void tooltipChanged(const QString& value);
    void countChanged(int count);
    void parameterAdded(QtNoid::App::Parameter* parameter);
    void parameterRemoved(QtNoid::App::Parameter* parameter);
    void parameterRenameError(QtNoid::App::Parameter* parameter, const QString& oldName, const QString& newName);


private slots:
    void onParameterDestroyed(QObject* parameter);
    void onParameterNameEdited(const QString& oldName, const QString& newName);


private:
    Q_OBJECT_BINDABLE_PROPERTY(ParameterList, QString, m_name, &ParameterList::nameChanged)
    Q_OBJECT_BINDABLE_PROPERTY(ParameterList, QString, m_description, &ParameterList::descriptionChanged)
    Q_OBJECT_BINDABLE_PROPERTY(ParameterList, QString, m_tooltip, &ParameterList::tooltipChanged)
    QHash<int, Parameter*> m_parametersByUniqueId;
    QMap<int, Parameter*> m_parametersByIndex;
    QHash<Parameter*, int> m_parameterToIndex; // Parameter -> sortIndex
    QHash<QString, Parameter*> m_parametersByName;
    int m_nextParameterIndex = 0;
    void appendParameterAndUpdateIndexs(Parameter *parameter);
};

} // namespace App
} // namespace QtNoid

#endif // QTNOID_APP_PARAMETERLIST_H
