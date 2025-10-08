#ifndef QTNOID_YAML_LEXER_H
#define QTNOID_YAML_LEXER_H

#include "yaml_token.h"
#include <QString>
#include <QVector>
#include <QStack>

namespace QtNoid {
namespace Json {
namespace Internal {

class Lexer {
public:
    explicit Lexer(const QString& source);

    // Main tokenization method
    QVector<Token> tokenize();

    // Get last error (if any)
    QString errorString() const { return m_error; }
    bool hasError() const { return !m_error.isEmpty(); }

private:
    // Input source
    QString m_source;
    int m_pos = 0;
    int m_line = 1;
    int m_column = 1;

    // Context tracking
    enum Context { ROOT, MAP, SEQ };
    QStack<int> m_indentStack;      // Stack of indentation levels
    QStack<Context> m_contextStack; // Stack of contexts (MAP or SEQ)
    int m_currentIndent = 0;
    bool m_atLineStart = true;

    // Error tracking
    QString m_error;

    // Character navigation
    QChar current() const;
    QChar peek(int offset = 1) const;
    void advance();
    bool isAtEnd() const;

    // Line/indent handling
    int measureIndent();
    void skipWhitespace();
    void skipComment();
    void handleNewLine(QVector<Token>& tokens);

    // Token emission
    void emitIndentTokens(QVector<Token>& tokens, int newIndent);
    void emitDedentTokens(QVector<Token>& tokens, int newIndent);
    void emitMapStart(QVector<Token>& tokens);
    void emitMapEnd(QVector<Token>& tokens);
    void emitSeqStart(QVector<Token>& tokens);
    void emitSeqEnd(QVector<Token>& tokens);

    // Content parsing
    void parseContent(QVector<Token>& tokens);
    QString readScalar();
    QString readQuotedString(QChar quote);
    bool isScalarChar(QChar c) const;

    // Type inference
    QString inferScalarTag(const QString& value) const;

    // Error handling
    void setError(const QString& message);
    Token makeToken(TokenType type, const QString& value = QString());
};

} // namespace Internal
} // namespace Json
} // namespace QtNoid

#endif // QTNOID_YAML_LEXER_H
