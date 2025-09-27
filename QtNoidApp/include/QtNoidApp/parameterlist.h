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

    // ===== ITERATOR CLASSES =====
    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = Parameter*;
        using difference_type = std::ptrdiff_t;
        using pointer = Parameter**;
        using reference = Parameter*&;

        iterator() = default;
        explicit iterator(QMap<int, Parameter*>::iterator it) : m_it(it) {}

        Parameter* operator*() const { return m_it.value(); }
        Parameter* operator->() const { return m_it.value(); }

        iterator& operator++() { ++m_it; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++m_it; return tmp; }
        iterator& operator--() { --m_it; return *this; }
        iterator operator--(int) { iterator tmp = *this; --m_it; return tmp; }

        bool operator==(const iterator& other) const { return m_it == other.m_it; }
        bool operator!=(const iterator& other) const { return m_it != other.m_it; }

        int index() const { return m_it.key(); }

    private:
        QMap<int, Parameter*>::iterator m_it;
        friend class const_iterator;
        friend class ParameterList;
    };

    class const_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = const Parameter*;
        using difference_type = std::ptrdiff_t;
        using pointer = const Parameter**;
        using reference = const Parameter*&;

        const_iterator() = default;
        explicit const_iterator(QMap<int, Parameter*>::const_iterator it) : m_it(it) {}
        const_iterator(const iterator& it) : m_it(it.m_it) {}  // Conversione da non-const

        const Parameter* operator*() const { return m_it.value(); }
        const Parameter* operator->() const { return m_it.value(); }

        const_iterator& operator++() { ++m_it; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++m_it; return tmp; }
        const_iterator& operator--() { --m_it; return *this; }
        const_iterator operator--(int) { const_iterator tmp = *this; --m_it; return tmp; }

        bool operator==(const const_iterator& other) const { return m_it == other.m_it; }
        bool operator!=(const const_iterator& other) const { return m_it != other.m_it; }

        int index() const { return m_it.key(); }

    private:
        QMap<int, Parameter*>::const_iterator m_it;
    };

    // Reverse iterators
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // ===== END ITERATOR CLASSES =====

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
    bool append(Parameter *parameter);
    bool append(const QJsonObject& schema, const QJsonObject& value);
    Parameter* emplace(const QVariant& initialValue, const QString& name, const QString& description = {});
    Parameter* emplace(const QJsonObject& schema, const QJsonObject& value);
    void removeParameter(Parameter* parameter);
    void removeParameter(const QString& name);
    void clear();
    bool isEmpty() const;
    
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
    void applyPreset(const QString& presetName);

    // ===== ITERATOR METHODS =====
    iterator begin() { return iterator(m_parametersByIndex.begin()); }
    iterator end() { return iterator(m_parametersByIndex.end()); }
    const_iterator begin() const { return const_iterator(m_parametersByIndex.constBegin()); }
    const_iterator end() const { return const_iterator(m_parametersByIndex.constEnd()); }
    const_iterator cbegin() const { return const_iterator(m_parametersByIndex.constBegin()); }
    const_iterator cend() const { return const_iterator(m_parametersByIndex.constEnd()); }
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }

public:
    ParameterList &operator<<(Parameter& param){
        append(&param);
        return *this;
    };
    ParameterList &operator<<(Parameter* param){
        if(param) append(param);
        return *this;
    };


signals:
    void nameChanged(const QString& value);
    void descriptionChanged(const QString& value);
    void tooltipChanged(const QString& value);
    void countChanged(int count);
    void parameterAdded(const QtNoid::App::Parameter* parameter);
    void parameterRemoved(QtNoid::App::Parameter* parameter);
    void parameterRenameError(const QString& oldName, const QString& newName);

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

inline QDebug operator<<(QDebug debug, const QtNoid::App::ParameterList &list)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "ParameterList(" << list.name()
                    << ", count=" << list.count();

    if (!list.isEmpty()) {
        debug << ", parameters=[";
        for (int i = 0; i < list.count(); ++i) {
            if (i > 0) debug << ", ";
            QtNoid::App::Parameter* param = list.parameter(i);
            if (param) {
                QString changedFlag = param->isValueChanged()? ", changed" : ", notChanged";
                debug << param->name() << ":{" << param->value() << changedFlag << "}";

            }
        }
        debug << "]";
    }
    debug << ")";
    return debug;
}

inline QDebug operator<<(QDebug debug, const QtNoid::App::ParameterList *list)
{
    if (list) {
        return ::operator<<(debug, *list);
    } else {
        return debug << "ParameterList(nullptr)";
    }
}


#endif // QTNOID_APP_PARAMETERLIST_H
