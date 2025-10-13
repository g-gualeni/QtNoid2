#ifndef QTNOID_YAML_PARSER_H
#define QTNOID_YAML_PARSER_H

#include "yaml_astnode.h"
#include "yaml_token.h"



namespace QtNoid {
namespace Json {
namespace Internal {

class Parser {

public:
    explicit Parser();
    explicit Parser(const QVector<Token> &tokens);

    // Parse tokens into AST
    std::shared_ptr<ASTNode> parse();

    // Get last error (if any)
    QString errorString() const { return m_error; }
    bool hasError() const { return !m_error.isEmpty(); }

    QVector<Token> tokens() const;
    void setTokens(const QVector<Token> &newTokens);

private:
    // Token stream
    QVector<Token> m_tokens;
    int m_pos = 0;

    // Error tracking
    QString m_error;

    // Token navigation
    const Token& current() const;
    const Token& peek(int offset = 1) const;
    void advance();
    bool isAtEnd() const;
    bool check(TokenType type) const;
    bool match(TokenType type);

    // Parsing methods
    // std::shared_ptr<ASTNode> parseDocument();
    std::shared_ptr<ASTNode> parseValue();
    std::shared_ptr<ASTNode> parseMap();
    std::shared_ptr<ASTNode> parseSeq();
    std::shared_ptr<ASTNode> parseScalar();

    // Helper methods
    QString consumeScalar();
    void skipStreamAndDocTokens();

    // Error handling
    void setError(const QString& message, const Token& tok);
    Token makeInvalidToken() const;
};



} // namespace Internal
} // namespace Json
} // namespace QtNoid

#endif // QTNOID_YAML_PARSER_H
