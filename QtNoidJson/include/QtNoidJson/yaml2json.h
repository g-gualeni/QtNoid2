#ifndef QTNOID_YAML2JSON_H
#define QTNOID_YAML2JSON_H

#include "global.h"
#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// YAML Text → [rapidyaml] → JSON text → [QJsonDocument] → QJsonObject / QJsonArray

namespace QtNoid {
namespace Json {

class QTNOIDJSON_EXPORT Yaml2Json : public QObject
{
    Q_OBJECT

public:
    explicit Yaml2Json(QObject* parent = nullptr);
    explicit Yaml2Json(const QString& yaml, QObject* parent = nullptr);

    bool isValid() const { return m_isValid; }
    QString errorString() const { return m_error; }

    QString yaml() const { return m_yaml; }
    void setYaml(const QString& newYaml);

    QJsonDocument document() const { return m_doc; }
    QJsonObject json() const { return m_doc.object(); }
    QJsonArray jsonArray() const { return m_doc.array(); }
    QString jsonAsString(QJsonDocument::JsonFormat format = QJsonDocument::Indented) const;

    static QJsonDocument convert(const QString& yaml, QString* error = nullptr);

signals:
    void errorOccurred(const QString& error);

private:
    QString m_yaml;
    QJsonDocument m_doc;
    bool m_isValid = true;
    QString m_error;

    void update();
};

} // namespace Json
} // namespace QtNoid

#endif // QTNOID_YAML2JSON_H
