#ifndef QTNOID_APP_CONFIG_H
#define QTNOID_APP_CONFIG_H


#include "global.h"
#include "parameterspage.h"
#include <QObject>
#include <QHash>
#include <QList>
#include <QProperty>


class QComboBox;
namespace QtNoid {
namespace App {

class QTNOIDAPP_EXPORT Config : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName BINDABLE bindableName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString description READ description WRITE setDescription BINDABLE bindableDescription NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(QString tooltip READ tooltip WRITE setTooltip BINDABLE bindableTooltip NOTIFY tooltipChanged FINAL)
    Q_PROPERTY(int count READ count BINDABLE bindableCount NOTIFY countChanged FINAL)

public:

    // ===== ITERATOR CLASSES =====
    class iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = ParametersPage*;
        using difference_type = std::ptrdiff_t;
        using pointer = ParametersPage**;
        using reference = ParametersPage*&;

        iterator() = default;
        explicit iterator(QMap<int, ParametersPage*>::iterator it) : m_it(it) {}

        ParametersPage* operator*() const { return m_it.value(); }
        ParametersPage* operator->() const { return m_it.value(); }

        iterator& operator++() { ++m_it; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++m_it; return tmp; }
        iterator& operator--() { --m_it; return *this; }
        iterator operator--(int) { iterator tmp = *this; --m_it; return tmp; }

        bool operator==(const iterator& other) const { return m_it == other.m_it; }
        bool operator!=(const iterator& other) const { return m_it != other.m_it; }

        int index() const { return m_it.key(); }

    private:
        QMap<int, ParametersPage*>::iterator m_it;
        friend class const_iterator;
        friend class Config;
    };

    class const_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = const ParametersPage*;
        using difference_type = std::ptrdiff_t;
        using pointer = const ParametersPage**;
        using reference = const ParametersPage*&;

        const_iterator() = default;
        explicit const_iterator(QMap<int, ParametersPage*>::const_iterator it) : m_it(it) {}
        const_iterator(const iterator& it) : m_it(it.m_it) {}

        const ParametersPage* operator*() const { return m_it.value(); }
        const ParametersPage* operator->() const { return m_it.value(); }

        const_iterator& operator++() { ++m_it; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++m_it; return tmp; }
        const_iterator& operator--() { --m_it; return *this; }
        const_iterator operator--(int) { const_iterator tmp = *this; --m_it; return tmp; }

        bool operator==(const const_iterator& other) const { return m_it == other.m_it; }
        bool operator!=(const const_iterator& other) const { return m_it != other.m_it; }

        int index() const { return m_it.key(); }

    private:
        QMap<int, ParametersPage*>::const_iterator m_it;
    };

    // Reverse iterators
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // ===== END ITERATOR CLASSES =====

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
    QBindable<int> bindableCount();

    bool append(ParametersPage *page);
    bool append(const QJsonObject& schema, const QJsonObject& value);
    ParametersPage* emplace(const QString& name, const QString& description = {});
    ParametersPage* emplace(const QJsonObject& schema, const QJsonObject& values);
    void remove(ParametersPage* page);
    void remove(const QString& pageName);
    void clear();
    bool isEmpty() const;

    // Access methods
    ParametersPage* page(int index) const;
    ParametersPage* page(const QString& pageName) const;
    int indexOf(ParametersPage* page) const;
    int indexOf(const QString& pageName) const;
    bool contains(ParametersPage* page) const;
    bool contains(const QString& pageName) const;

    // List access
    QList<ParametersPage*> pages() const;

    // Convenience methods for nested access
    int parametersCount(const QString& pageName = "Settings") const;
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

    void restoreComboBoxTextItems(QComboBox* cbo, const QString& paramName, const QStringList& defaultValue, const QString& pageName = "Settings") const;
    void saveComboBoxTextItems(QComboBox* cbo, const QString& paramName, const QString& pageName = "Settings");



    // ===== ITERATOR METHODS =====
    iterator begin() { return iterator(m_pagesByIndex.begin()); }
    iterator end() { return iterator(m_pagesByIndex.end()); }
    const_iterator begin() const { return const_iterator(m_pagesByIndex.constBegin()); }
    const_iterator end() const { return const_iterator(m_pagesByIndex.constEnd()); }
    const_iterator cbegin() const { return const_iterator(m_pagesByIndex.constBegin()); }
    const_iterator cend() const { return const_iterator(m_pagesByIndex.constEnd()); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
    const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }

public:
    Config &operator<<(ParametersPage& page){
        append(&page);
        return *this;
    };
    Config &operator<<(ParametersPage* page){
        if(page) append(page);
        return *this;
    };

signals:
    void nameChanged(const QString& value);
    void descriptionChanged(const QString& value);
    void tooltipChanged(const QString& value);
    void countChanged(int count);
    void pageAdded(const QtNoid::App::ParametersPage* parameterList);
    void pageRemoved(QtNoid::App::ParametersPage* parameterList);
    void pageRenameError(const QString& oldName, const QString& newName);

private slots:
    void onPageDestroyed(QObject *obj);
    void onPageNameEdited(const QString& oldName, const QString& newName);

private:
    Q_OBJECT_BINDABLE_PROPERTY(Config, QString, m_name, &Config::nameChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Config, QString, m_description, &Config::descriptionChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Config, QString, m_tooltip, &Config::tooltipChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Config, int, m_count, &Config::countChanged)
    QHash<int, ParametersPage*> m_pagesByUniqueId;
    QMap<int, ParametersPage*> m_pagesByIndex;
    QHash<ParametersPage*, int> m_pageToIndex; // ParametersPage -> sortIndex
    QHash<QString, ParametersPage*> m_pagesByName;
    int m_nextPageIndex = 0;
    void appendPageAndUpdateIndexs(ParametersPage *page);

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
        for (int ii = 0; ii < config.count(); ++ii) {
            if (ii > 0) debug << ", ";
            QtNoid::App::ParametersPage* page = config.page(ii);
            if (page) {
                debug <<   " {";
                debug << page;
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
