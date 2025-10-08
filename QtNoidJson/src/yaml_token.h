#ifndef QTNOID_YAML_TOKEN_H
#define QTNOID_YAML_TOKEN_H

#include <QString>


namespace QtNoid {
namespace Json {
namespace Internal {

// Token types following YAML test suite conventions
enum class TokenType {
    // Stream events
    STREAM_START,    // +STR
    STREAM_END,      // -STR

    // Document events
    DOC_START,       // +DOC
    DOC_END,         // -DOC

    // Collection events
    MAP_START,       // +MAP
    MAP_END,         // -MAP
    SEQ_START,       // +SEQ (sequence/array)
    SEQ_END,         // -SEQ

    // Scalar values
    SCALAR,          // =VAL (actual value)

    // Mapping keys/values
    KEY,             // ? (explicit key)
    VALUE,           // : (explicit value indicator)

    // Aliases and anchors
    ALIAS,           // *anchor_name
    ANCHOR,          // &anchor_name

    // Special
    EOF_TOKEN,       // End of input
    INVALID          // Error token
};

// Token structure
struct Token {
    TokenType type;
    QString value;      // Content for SCALAR, ALIAS, ANCHOR
    QString tag;        // Type tag (!!str, !!int, etc.) - optional
    int line;           // Line number for error reporting
    int column;         // Column number for error reporting

    // Constructors
    Token()
        : type(TokenType::INVALID), line(0), column(0) {}

    Token(TokenType t, int l = 0, int c = 0)
        : type(t), line(l), column(c) {}

    Token(TokenType t, const QString& v, int l = 0, int c = 0)
        : type(t), value(v), line(l), column(c) {}

    Token(TokenType t, const QString& v, const QString& tg, int l = 0, int c = 0)
        : type(t), value(v), tag(tg), line(l), column(c) {}
};


} // namespace Internal
} // namespace Json
} // namespace QtNoid

#endif // QTNOID_YAML_TOKEN_H
