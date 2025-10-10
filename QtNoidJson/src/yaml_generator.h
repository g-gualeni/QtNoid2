#ifndef QTNOID_YAML_GENERATOR_H
#define QTNOID_YAML_GENERATOR_H

#include "yaml_astnode.h"

#include <QJsonObject>


namespace QtNoid {
namespace Json {
namespace Internal {

class Generator
{
public:
    Generator() = default;

    // Generate QJsonObject from AST
    QJsonObject generate(const std::shared_ptr<ASTNode>& ast);

    // Get last error (if any)
    QString errorString() const { return m_error; }
    bool hasError() const { return !m_error.isEmpty(); }

private:
    // Error tracking
    QString m_error;

    // Conversion methods
    QJsonValue generateValue(const std::shared_ptr<ASTNode>& node);
    QJsonObject generateMap(const MapNode& map);
    QJsonArray generateSeq(const SeqNode& seq);
    QJsonValue generateScalar(const ScalarNode& scalar);

    // Type conversion helpers
    QJsonValue convertScalarToJson(const QString& value, const QString& tag);
    bool isInteger(const QString& value) const;
    bool isFloat(const QString& value) const;
    bool isBoolean(const QString& value) const;
    bool isNull(const QString& value) const;

    // Error handling
    void setError(const QString& message);
};




} // namespace Internal
} // namespace Json
} // namespace QtNoid


#endif // QTNOID_YAML_GENERATOR_H
