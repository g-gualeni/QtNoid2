#include "yaml_generator.h"
#include <QJsonArray>

namespace QtNoid {
namespace Json {
namespace Internal {

QJsonObject Generator::generate(const std::shared_ptr<ASTNode> &ast)
{
    // Reset state
    m_error.clear();

    if (!ast) {
        setError("Null AST node");
        return QJsonObject();
    }

    // Generate value from AST
    QJsonValue value = generateValue(ast);

    // If root is an object, return it
    if (value.isObject()) {
        return value.toObject();
    }

    // Otherwise wrap in an object with "value" key
    QJsonObject result;
    result["value"] = value;
    return result;
}

QJsonValue Generator::generateValue(const std::shared_ptr<ASTNode> &node)
{
    if (!node) {
        return QJsonValue();
    }

    // Check node type and generate appropriate JSON
    if (node->isScalar()) {
        return generateScalar(node->asScalar());
    } else if (node->isMap()) {
        return generateMap(node->asMap());
    } else if (node->isSeq()) {
        return generateSeq(node->asSeq());
    }

    setError("Unknown AST node type");
    return QJsonValue();
}

QJsonObject Generator::generateMap(const MapNode &map)
{
    QJsonObject result;

    for (const auto& pair : map.pairs) {
        // Get key (should be a scalar)
        QString key;
        if (pair.first && pair.first->isScalar()) {
            key = pair.first->asScalar().value;
        } else {
            setError("Map key must be a scalar");
            continue;
        }

        // Get value (can be any type)
        QJsonValue value = generateValue(pair.second);

        result[key] = value;
    }

    return result;
}

QJsonArray Generator::generateSeq(const SeqNode &seq)
{
    QJsonArray result;

    for (const auto& item : seq.items) {
        QJsonValue value = generateValue(item);
        result.append(value);
    }

    return result;
}

QJsonValue Generator::generateScalar(const ScalarNode &scalar)
{
    return convertScalarToJson(scalar.value, scalar.tag);
}

QJsonValue Generator::convertScalarToJson(const QString &value, const QString &tag)
{
    QString trimmed = value.trimmed();

    // Use tag if available
    if (!tag.isEmpty()) {
        if (tag == "!!null") {
            return QJsonValue(QJsonValue::Null);
        } else if (tag == "!!bool") {
            return QJsonValue(isBoolean(trimmed) &&
                              (trimmed.toLower() == "true" ||
                               trimmed == "True" ||
                               trimmed == "TRUE"));
        } else if (tag == "!!int") {
            bool ok = false;
            int intValue = trimmed.toInt(&ok);
            if (ok) return QJsonValue(intValue);
            // Fallback to double for large integers
            double doubleValue = trimmed.toDouble(&ok);
            if (ok) return QJsonValue(doubleValue);
        } else if (tag == "!!float") {
            bool ok = false;
            double doubleValue = trimmed.toDouble(&ok);
            if (ok) return QJsonValue(doubleValue);
        } else if (tag == "!!str") {
            return QJsonValue(value);
        }
    }

    // Auto-detect type if no tag or tag conversion failed
    if (isNull(trimmed)) {
        return QJsonValue(QJsonValue::Null);
    }

    if (isBoolean(trimmed)) {
        return QJsonValue(trimmed.toLower() == "true" ||
                          trimmed == "True" ||
                          trimmed == "TRUE");
    }

    if (isInteger(trimmed)) {
        bool ok = false;
        int intValue = trimmed.toInt(&ok);
        if (ok) return QJsonValue(intValue);
        // Fallback to double for large integers
        double doubleValue = trimmed.toDouble(&ok);
        if (ok) return QJsonValue(doubleValue);
    }

    if (isFloat(trimmed)) {
        bool ok = false;
        double doubleValue = trimmed.toDouble(&ok);
        if (ok) return QJsonValue(doubleValue);
    }

    // Default to string
    return QJsonValue(value);
}

bool Generator::isInteger(const QString &value) const
{
    QString trimmed = value.trimmed();
    if (trimmed.isEmpty()) return false;

    bool ok = false;
    trimmed.toInt(&ok);
    return ok;
}

bool Generator::isFloat(const QString &value) const
{
    QString trimmed = value.trimmed();
    if (trimmed.isEmpty()) return false;

    // Must contain '.' or 'e'/'E' to be considered float
    if (!trimmed.contains('.') &&
        !trimmed.contains('e') &&
        !trimmed.contains('E')) {
        return false;
    }

    bool ok = false;
    trimmed.toDouble(&ok);
    return ok;
}

bool Generator::isBoolean(const QString &value) const
{
    QString lower = value.trimmed().toLower();
    return lower == "true" || lower == "false" ||
           value == "True" || value == "False" ||
           value == "TRUE" || value == "FALSE";
}

bool Generator::isNull(const QString &value) const
{
    QString trimmed = value.trimmed();
    return trimmed.isEmpty() ||
           trimmed == "null" ||
           trimmed == "Null" ||
           trimmed == "NULL" ||
           trimmed == "~";
}

void Generator::setError(const QString &message)
{
    if (m_error.isEmpty()) {
        m_error = QString("Generation error: %1").arg(message);
    }
}


} // namespace Internal
} // namespace Json
} // namespace QtNoid
