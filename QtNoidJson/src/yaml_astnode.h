#ifndef QTNOID_YAML_ASTNODE_H
#define QTNOID_YAML_ASTNODE_H

#include <QString>
#include <QVector>


namespace QtNoid {
namespace Json {
namespace Internal {

// Forward declaration for recursive types
struct ASTNode;


// Scalar node - represents primitive values
struct ScalarNode {
    QString value;
    QString tag;  // Type hint: !!str, !!int, !!bool, !!null, etc.

    ScalarNode() = default;
    explicit ScalarNode(const QString& v, const QString& t = QString())
        : value(v), tag(t) {}
};


// Map node - represents YAML mappings (objects)
struct MapNode {
    // Vector of key-value pairs (both are ASTNodes)
    QVector<QPair<std::shared_ptr<ASTNode>, std::shared_ptr<ASTNode>>> pairs;

    MapNode() = default;
};


// Sequence node - represents YAML sequences (arrays)
struct SeqNode {
    // Vector of child nodes
    QVector<std::shared_ptr<ASTNode>> items;

    SeqNode() = default;
};


// ASTNode is a variant that can hold any of the three node types
struct ASTNode {
    std::variant<ScalarNode, MapNode, SeqNode> data;

    // Constructors for convenience
    ASTNode() : data(ScalarNode()) {}

    explicit ASTNode(const ScalarNode& scalar) : data(scalar) {}
    explicit ASTNode(const MapNode& map) : data(map) {}
    explicit ASTNode(const SeqNode& seq) : data(seq) {}

    // Type checking helpers
    bool isScalar() const { return std::holds_alternative<ScalarNode>(data); }
    bool isMap() const { return std::holds_alternative<MapNode>(data); }
    bool isSeq() const { return std::holds_alternative<SeqNode>(data); }

    // Accessors
    ScalarNode& asScalar() { return std::get<ScalarNode>(data); }
    const ScalarNode& asScalar() const { return std::get<ScalarNode>(data); }

    MapNode& asMap() { return std::get<MapNode>(data); }
    const MapNode& asMap() const { return std::get<MapNode>(data); }

    SeqNode& asSeq() { return std::get<SeqNode>(data); }
    const SeqNode& asSeq() const { return std::get<SeqNode>(data); }
};


} // namespace Internal
} // namespace Json
} // namespace QtNoid

#endif // QTNOID_YAML_ASTNODE_H
