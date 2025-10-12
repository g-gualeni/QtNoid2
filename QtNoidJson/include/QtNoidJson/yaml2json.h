#ifndef QTNOID_YAML2JSON_H
#define QTNOID_YAML2JSON_H

#include "global.h"
#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSharedPointer>


// YAML Text → [LEXER] → Tokens → [PARSER] → AST → [GENERATOR] → QJsonObject

class TestQtNoidJsonYaml2Json;

namespace QtNoid {
namespace Json {

namespace Internal {
    class Lexer;
}



class QTNOIDJSON_EXPORT Yaml2Json :  public QObject
{
    Q_OBJECT

    friend class ::TestQtNoidJsonYaml2Json;

public:
    explicit Yaml2Json(QObject *parent = nullptr);
    explicit Yaml2Json(const QString yaml, QObject *parent = nullptr);

    bool isValid() const {return m_isValid;}
    QString errorString() const { return m_error; }

    QString yaml() const;
    void setYaml(const QString &newYaml);

    QJsonObject json() const { return m_json; }

    QStringList tokens();

signals:
    void errorOccurred(const QString& error);

private:
    QString m_yaml;
    QJsonObject m_json;
    bool m_isValid = true;
    QString m_error;
    QSharedPointer<Internal::Lexer> m_lexer;

    QSharedPointer<Internal::Lexer> lexer() const;
    void setError(const QString& error);
    void convert();

};

} // namespace Json
} // namespace QtNoid

#endif // QTNOID_YAML2JSON_H
