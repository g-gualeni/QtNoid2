#include "QtNoidJson/yaml2json.h"
#include "QtNoidCommon/QtNoidCommon"
#include "yaml_generator.h"
#include "yaml_lexer.h"
#include <yaml_parser.h>
#include <QCoreApplication>
#include <QDebug>

namespace QtNoid {
namespace Json {

Yaml2Json::Yaml2Json(const QString yaml, QObject *parent)
    : QObject(parent), m_yaml(yaml)
{
    // Proactive approach: convert immediately in constructor
    convert();
}



void Yaml2Json::setYaml(const QString &newYaml)
{
    if (m_yaml != newYaml) {
        m_yaml = newYaml;
        // Re-convert with new YAML
        convert();
    }
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

    if (m_yaml.isEmpty()) {
        setError("Empty YAML input");
        return;
    }

    // Step 1: LEXER - Tokenize YAML text
    Internal::Lexer lexer(m_yaml);
    QVector<Internal::Token> tokens = lexer.tokenize();

    if (lexer.hasError()) {
        setError(QString("Lexer error: %1").arg(lexer.errorString()));
        return;
    }

    // Step 2: PARSER - Build AST from tokens
    Internal::Parser parser(tokens);
    auto ast = parser.parse();

    if (parser.hasError()) {
        setError(QString("Parser error: %1").arg(parser.errorString()));
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
