
#include "yaml_lexer.h"

namespace QtNoid {
namespace Json {
namespace Internal {

Lexer::Lexer(const QString &source)
    : m_source(source)
{

}

QVector<Token> Lexer::tokenize()
{
    QVector<Token> tokens;

    // Reset state
    m_pos = 0;
    m_line = 1;
    m_column = 1;
    m_error.clear();
    m_atLineStart = true;

    // Emit stream and document start
    tokens.append(makeToken(TokenType::STREAM_START));
    tokens.append(makeToken(TokenType::DOC_START));

    // Initialize indent stack with root level
    m_indentStack.push(0);
    m_contextStack.push(ROOT);

    // Main parsing loop
    while (!isAtEnd() && !hasError()) {
        // Handle newlines
        if (current() == '\n') {
            advance();
            m_atLineStart = true;
            continue;
        }

        // Skip whitespace at line start (will be measured as indent)
        if (m_atLineStart && current() == ' ') {
            handleNewLine(tokens);
            continue;
        }

        // Skip comments
        if (current() == '#') {
            skipComment();
            continue;
        }

        // Skip inline whitespace
        if (current() == ' ' || current() == '\t') {
            skipWhitespace();
            continue;
        }

        // Parse content
        m_atLineStart = false;
        parseContent(tokens);
    }

    // Close all open contexts
    while (m_indentStack.size() > 1) {
        m_indentStack.pop();
        if (!m_contextStack.isEmpty()) {
            Context ctx = m_contextStack.pop();
            if (ctx == MAP) {
                emitMapEnd(tokens);
            } else if (ctx == SEQ) {
                emitSeqEnd(tokens);
            }
        }
    }

    // Emit document and stream end
    tokens.append(makeToken(TokenType::DOC_END));
    tokens.append(makeToken(TokenType::STREAM_END));

    return tokens;

}

QString Lexer::source() const
{
    return m_source;
}

void Lexer::setSource(const QString &newSource)
{
    if(m_source == newSource) {
        return;
    }
}

QChar Lexer::current() const
{
    if (isAtEnd()) return QChar();
    return m_source.at(m_pos);
}

QChar Lexer::peek(int offset) const
{
    int pos = m_pos + offset;
    if (pos >= m_source.length()) return QChar();
    return m_source.at(pos);
}

void Lexer::advance()
{
    if (isAtEnd()) return;

    if (current() == '\n') {
        m_line++;
        m_column = 1;
    } else {
        m_column++;
    }
    m_pos++;
}

bool Lexer::isAtEnd() const
{
    return m_pos >= m_source.length();
}


int Lexer::measureIndent()
{
    int indent = 0;
    int pos = m_pos;

    while (pos < m_source.length() && m_source.at(pos) == ' ') {
        indent++;
        pos++;
    }

    return indent;
}


void Lexer::skipWhitespace()
{
    while (!isAtEnd() && (current() == ' ' || current() == '\t')) {
        advance();
    }
}

void Lexer::skipComment()
{
    // Skip until end of line
    while (!isAtEnd() && current() != '\n') {
        advance();
    }
}


void Lexer::handleNewLine(QVector<Token>& tokens)
{
    int newIndent = measureIndent();
    int currentIndent = m_indentStack.top();

    // Skip the spaces
    for (int i = 0; i < newIndent; i++) {
        advance();
    }
    m_atLineStart = false;

    // Check if line is empty or comment
    if (isAtEnd() || current() == '\n' || current() == '#') {
        return;
    }

    // Handle indent changes
    if (newIndent > currentIndent) {
        emitIndentTokens(tokens, newIndent);
    } else if (newIndent < currentIndent) {
        emitDedentTokens(tokens, newIndent);
    }
}

void Lexer::emitIndentTokens(QVector<Token>& tokens, int newIndent)
{
    m_indentStack.push(newIndent);
    // Context will be determined when we see '-' or ':'
}

void Lexer::emitDedentTokens(QVector<Token>& tokens, int newIndent)
{
    while (m_indentStack.size() > 1 && m_indentStack.top() > newIndent) {
        m_indentStack.pop();

        if (!m_contextStack.isEmpty()) {
            Context ctx = m_contextStack.pop();
            if (ctx == MAP) {
                emitMapEnd(tokens);
            } else if (ctx == SEQ) {
                emitSeqEnd(tokens);
            }
        }
    }
}

void Lexer::emitMapStart(QVector<Token>& tokens)
{
    tokens.append(makeToken(TokenType::MAP_START));
    m_contextStack.push(MAP);
}


void Lexer::emitMapEnd(QVector<Token>& tokens)
{
    tokens.append(makeToken(TokenType::MAP_END));
}

void Lexer::emitSeqStart(QVector<Token>& tokens)
{
    tokens.append(makeToken(TokenType::SEQ_START));
    m_contextStack.push(SEQ);
}

void Lexer::emitSeqEnd(QVector<Token>& tokens)
{
    tokens.append(makeToken(TokenType::SEQ_END));
}

void Lexer::parseContent(QVector<Token>& tokens)
{
    // Check for sequence item
    if (current() == '-' && (peek() == ' ' || peek() == '\n' || peek().isNull())) {
        advance(); // skip '-'
        skipWhitespace();

        // Start sequence if not already in one at this level
        if (m_contextStack.isEmpty() || m_contextStack.top() != SEQ) {
            emitSeqStart(tokens);
        }

        // The value after '-' will be parsed in next iteration
        return;
    }

    // Check for flow sequence [...]
    if (current() == '[') {
        advance(); // skip '['
        emitSeqStart(tokens);

        // Parse items
        while (!isAtEnd() && current() != ']') {
            skipWhitespace();
            if (current() == ',') {
                advance();
                skipWhitespace();
                continue;
            }
            if (current() == ']') break;

            QString value = readScalar();
            QString tag = inferScalarTag(value);
            tokens.append(makeToken(TokenType::SCALAR, value));
        }

        if (current() == ']') {
            advance(); // skip ']'
        }
        emitSeqEnd(tokens);
        return;
    }

    // Read key or value
    QString scalar = readScalar();

    if (scalar.isEmpty()) {
        return;
    }

    skipWhitespace();

    // Check if this is a key (followed by ':')
    if (current() == ':') {
        advance(); // skip ':'
        skipWhitespace();

        // Start map if not already in one at this level
        if (m_contextStack.isEmpty() || m_contextStack.top() != MAP) {
            emitMapStart(tokens);
        }

        // Emit key
        QString tag = inferScalarTag(scalar);
        tokens.append(Token(TokenType::SCALAR, scalar, tag, m_line, m_column));

        // Check if value is on same line
        if (!isAtEnd() && current() != '\n' && current() != '#') {
            // Inline value
            if (current() == '[') {
                // Flow sequence - will be handled in next iteration
                return;
            }

            QString value = readScalar();
            QString valueTag = inferScalarTag(value);
            tokens.append(Token(TokenType::SCALAR, value, valueTag, m_line, m_column));
        }
        // If no value on same line, it will be on next line (nested structure)
    } else {
        // Just a scalar value (in sequence or as value)
        QString tag = inferScalarTag(scalar);
        tokens.append(Token(TokenType::SCALAR, scalar, tag, m_line, m_column));
    }
}


QString Lexer::readScalar()
{
    // Check for quoted string
    if (current() == '"' || current() == '\'') {
        return readQuotedString(current());
    }

    QString result;
    int startColumn = m_column;

    // Read until special character or end of line
    while (!isAtEnd() && isScalarChar(current())) {
        result.append(current());
        advance();
    }

    return result.trimmed();
}

QString Lexer::readQuotedString(QChar quote)
{
    advance(); // skip opening quote
    QString result;

    while (!isAtEnd() && current() != quote) {
        if (current() == '\\' && peek() == quote) {
            advance(); // skip backslash
            result.append(current());
            advance();
        } else {
            result.append(current());
            advance();
        }
    }

    if (current() == quote) {
        advance(); // skip closing quote
    }

    return result;
}


bool Lexer::isScalarChar(QChar c) const
{
    if (c == '\n' || c == '\r') return false;
    if (c == ':') return false;
    if (c == '#') return false;
    if (c == '[' || c == ']') return false;
    if (c == '{' || c == '}') return false;
    if (c == ',') return false;

    return true;
}

QString Lexer::inferScalarTag(const QString& value) const
{
    QString trimmed = value.trimmed();

    // Check for null
    if (trimmed.isEmpty() || trimmed == "null" || trimmed == "~") {
        return "!!null";
    }

    // Check for boolean
    if (trimmed == "true" || trimmed == "false" ||
        trimmed == "True" || trimmed == "False" ||
        trimmed == "TRUE" || trimmed == "FALSE") {
        return "!!bool";
    }

    // Check for integer
    bool isInt = false;
    trimmed.toInt(&isInt);
    if (isInt) {
        return "!!int";
    }

    // Check for float
    bool isFloat = false;
    trimmed.toDouble(&isFloat);
    if (isFloat) {
        return "!!float";
    }

    // Default to string
    return "!!str";
}

void Lexer::setError(const QString& message)
{
    if (m_error.isEmpty()) {
        m_error = QString("Line %1, Col %2: %3").arg(m_line).arg(m_column).arg(message);
    }
}

Token Lexer::makeToken(TokenType type, const QString& value)
{
    return Token(type, value, m_line, m_column);
}

} // namespace Internal
} // namespace Json
} // namespace QtNoid
