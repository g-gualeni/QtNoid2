#ifndef QTNOID_YAML2JSON_H
#define QTNOID_YAML2JSON_H

#include "global.h"
#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


// YAML Text → [LEXER] → Tokens → [PARSER] → AST → [GENERATOR] → QJsonObject

class TestQtNoidJsonYaml2Json;

namespace QtNoid {
namespace Json {


class QTNOIDJSON_EXPORT Yaml2Json :  public QObject
{
    Q_OBJECT

    friend class ::TestQtNoidJsonYaml2Json;

public:
    explicit Yaml2Json(QObject *parent = nullptr) : QObject(parent){}
    explicit Yaml2Json(const QString yaml, QObject *parent = nullptr);

    bool isValid() const;

    QString yaml() const;
    void setYaml(const QString &newYaml);

    QJsonObject json() const;


private:
    QString m_yaml;
    QJsonObject m_json;
    bool m_isValid = true;
    QString m_error;

    // QVector<Token> tokenize(const QString& yaml);
    // ASTNode parse(const QVector<Token>& tokens);
    // QJsonObject generate(const ASTNode& ast);

};

} // namespace Json
} // namespace QtNoid

#endif // QTNOID_YAML2JSON_H
