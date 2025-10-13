#include "QtNoidJson/yaml2json.h"
#include "QtNoidCommon/QtNoidCommon"
#include "yaml_generator.h"
#include "yaml_lexer.h"
#include <yaml_parser.h>
#include <QCoreApplication>
#include <QDebug>

namespace QtNoid {
namespace Json {

Yaml2Json::Yaml2Json(QObject *parent)
    : QObject(parent)
{
    m_lexer = QSharedPointer<Internal::Lexer>::create();
    m_parser = QSharedPointer<Internal::Parser>::create();
}

Yaml2Json::Yaml2Json(const QString yaml, QObject *parent)
    : QObject(parent)
{
    m_lexer = QSharedPointer<Internal::Lexer>::create(yaml);
    m_parser = QSharedPointer<Internal::Parser>::create();
    // Proactive approach: convert immediately in constructor
    convert();
}

QString Yaml2Json::yaml() const
{
    return m_lexer->yaml();
}



void Yaml2Json::setYaml(const QString &newYaml)
{
    if (m_lexer->yaml() != newYaml) {
        m_lexer->setYaml(newYaml);
        convert();
    }
}

QStringList Yaml2Json::tokens()
{
    return m_lexer->tokensAsStringList();
}

QSharedPointer<Internal::Lexer> Yaml2Json::lexer() const
{
    return m_lexer;
}

void Yaml2Json::setError(const QString &error)
{
    m_isValid = false;
    m_error = error;
    m_json = QJsonObject(); // Clear any partial result

    // Emit signal for Qt integration
    emit errorOccurred(error);
}

void Yaml2Json::convert()
{
    // Reset state
    m_isValid = true;
    m_error.clear();
    m_json = QJsonObject();

    m_lexer->tokenize();
    if (m_lexer->hasError()) {
        setError(QString("Lexer error: %1").arg(m_lexer->errorString()));
        return;
    }

    // Step 2: PARSER - Build AST from tokens
    // qDebug() << tokens();
    m_parser->setTokens(m_lexer->tokens());
    auto ast = m_parser->parse();

    if (m_parser->hasError()) {
        setError(QString("Parser error: %1").arg(m_parser->errorString()));
        return;
    }

    if (!ast) {
        setError("Parser returned null AST");
        return;
    }

    // Step 3: GENERATOR - Generate QJsonObject from AST
    Internal::Generator generator;
    m_json = generator.generate(ast);

    if (generator.hasError()) {
        setError(QString("Generator error: %1").arg(generator.errorString()));
        return;
    }

    // Success!
    m_isValid = true;
}






} // namespace Json
} // namespace QtNoid
