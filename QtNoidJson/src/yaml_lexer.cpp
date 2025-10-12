
#include "yaml_lexer.h"

namespace QtNoid {
namespace Json {
namespace Internal {

Lexer::Lexer()
{
}

Lexer::Lexer(const QString &source)
    : m_yaml(source)
{

}

QVector<Token> Lexer::tokenize()
{
    // Reset state
    m_pos = 0;
    m_line = 1;
    m_column = 1;
    m_error.clear();
    m_atLineStart = true;
    m_tokens.clear();

    // Emit stream and document start
    m_tokens.append(makeToken(TokenType::STREAM_START));
    m_tokens.append(makeToken(TokenType::DOC_START));

    // Initialize indent stack with root level
    m_indentStack.clear();
    m_contextStack.clear();
    m_indentStack.push(-1);

    // Main parsing loop
    while (!isAtEnd() && !hasError()) {
        // Handle newlines
        if (current() == '\n') {
            advance();
            m_atLineStart = true;
            continue;
        }

        if (current() == '#') {
            skipComment();
            continue;
        }

        // Handle indentation at line start
        if (m_atLineStart && (current() == ' ' || !current().isNull())) {
            int indent = measureIndent();

            // Skip whitespace
            while (!isAtEnd() && current() == ' ') {
                advance();
            }

            // Check if line is empty or comment after spaces
            if (isAtEnd() || current() == '\n' || current() == '#') {
                if (current() == '#') {
                    skipComment();
                }
                continue;
            }

            m_atLineStart = false;

            // Handle dedent (closing structures)
            int currentIndent = m_indentStack.top();
            if (indent < currentIndent) {
                emitDedentTokens(indent);
            }

            // Parse content at this indent level
            parseContent(indent);

        } else {
            m_atLineStart = false;
            skipWhitespace();
        }
    }

    // Close all open contexts
    while (!m_contextStack.isEmpty()) {
        if(!m_indentStack.isEmpty()) {
            m_indentStack.pop();
        }
        Context ctx = m_contextStack.pop();
        if (ctx == MAP) {
            emitMapEnd();
        } else if (ctx == SEQ) {
            emitSeqEnd();
        }
    }

    // Verify stacks are properly synchronized (only sentinel should remain)
    if (m_indentStack.size() > 1) {
        setError("Stack synchronization error at end: unclosed indent levels");
    }

    // Emit document and stream end
    m_tokens.append(makeToken(TokenType::DOC_END));
    m_tokens.append(makeToken(TokenType::STREAM_END));

    return m_tokens;

}

QString Lexer::yaml() const
{
    return m_yaml;
}

void Lexer::setYaml(const QString &newYaml)
{
    if(m_yaml == newYaml) {
        return;
    }
}

QVector<Token> Lexer::tokens() const
{
    return m_tokens;
}

QStringList Lexer::tokensAsStringList() const
{
    QStringList res;

    for (const auto& token : m_tokens) {
        QString tokenStr;

        // Convert token type to string
        switch (token.type) {
        case Internal::TokenType::STREAM_START:
            tokenStr = "+STR";
            break;
        case Internal::TokenType::STREAM_END:
            tokenStr = "-STR";
            break;
        case Internal::TokenType::DOC_START:
            tokenStr = "+DOC";
            break;
        case Internal::TokenType::DOC_END:
            tokenStr = "-DOC";
            break;
        case Internal::TokenType::MAP_START:
            tokenStr = "+MAP";
            break;
        case Internal::TokenType::MAP_END:
            tokenStr = "-MAP";
            break;
        case Internal::TokenType::SEQ_START:
            tokenStr = "+SEQ";
            break;
        case Internal::TokenType::SEQ_END:
            tokenStr = "-SEQ";
            break;
        case Internal::TokenType::SCALAR:
            tokenStr = QString("=VAL ");
            if (!token.tag.isEmpty()) {
                tokenStr += token.tag + " ";
            }
            tokenStr += ":" + token.value;
            break;
        case Internal::TokenType::KEY:
            tokenStr = "?";
            break;
        case Internal::TokenType::VALUE:
            tokenStr = ":";
            break;
        case Internal::TokenType::ALIAS:
            tokenStr = "*" + token.value;
            break;
        case Internal::TokenType::ANCHOR:
            tokenStr = "&" + token.value;
            break;
        case Internal::TokenType::EOF_TOKEN:
            tokenStr = "EOF";
            break;
        case Internal::TokenType::INVALID:
            tokenStr = "INVALID";
            break;
        }

        res.append(tokenStr);
    }

    return res;
}

QChar Lexer::current() const
{
    if (isAtEnd()) return QChar();
    return m_yaml.at(m_pos);
}

QChar Lexer::peek(int offset) const
{
    int pos = m_pos + offset;
    if (pos >= m_yaml.length()) return QChar();
    return m_yaml.at(pos);
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
    return m_pos >= m_yaml.length();
}


int Lexer::measureIndent()
{
    int indent = 0;
    int pos = m_pos;

    while (pos < m_yaml.length() && m_yaml.at(pos) == ' ') {
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
        emitIndentTokens(newIndent);
    } else if (newIndent < currentIndent) {
        emitDedentTokens(newIndent);
    }
}

void Lexer::emitIndentTokens(int newIndent)
{
    // NOTE: This method is not used in the current implementation
    // Indent handling is done directly in parseContent()
    // Kept for API compatibility but does nothing to avoid stack desynchronization
    // m_indentStack.push(newIndent);
}

void Lexer::emitDedentTokens(int newIndent)
{
    while (!m_indentStack.isEmpty() && m_indentStack.top() > newIndent) {
        m_indentStack.pop();

        // CRITICAL: Always pop context when we pop indent to keep them synchronized
        if (!m_contextStack.isEmpty()) {
            Context ctx = m_contextStack.pop();
            if (ctx == MAP) {
                emitMapEnd();
            } else if (ctx == SEQ) {
                emitSeqEnd();
            }
        }
        else {
            // This should never happen if stacks are synchronized
            setError("Stack synchronization error: indent stack larger than context stack");
            break;
        }
    }
}

void Lexer::emitMapStart()
{
    m_tokens.append(makeToken(TokenType::MAP_START));
    m_contextStack.push(MAP);
}


void Lexer::emitMapEnd()
{
    m_tokens.append(makeToken(TokenType::MAP_END));
}

void Lexer::emitSeqStart()
{
    m_tokens.append(makeToken(TokenType::SEQ_START));
    m_contextStack.push(SEQ);
}

void Lexer::emitSeqEnd()
{
    m_tokens.append(makeToken(TokenType::SEQ_END));
}

void Lexer::parseContent(int currentIndent)
{
    // Check for sequence item (dash)
    if (current() == '-' && (peek() == ' ' || peek() == '\n' || peek().isNull())) {
        advance(); // skip '-'
        skipWhitespace();

        // If not in a sequence at this level, start one
        if (m_contextStack.isEmpty() || m_indentStack.top() < currentIndent) {
            emitSeqStart();  // Pushes to contextStack
            m_indentStack.push(currentIndent);  // Push to indentStack - now synchronized

            // Verify synchronization (contextStack size should equal indentStack size - 1 due to sentinel)
            if (m_contextStack.size() != m_indentStack.size() - 1) {
                setError("Stack synchronization error in sequence start");
                return;
            }
        }

        // Parse the item value (could be on same line or next)
        if (!isAtEnd() && current() != '\n') {
            parseContent(currentIndent + 2);  // Item content indented
        }

        // The value after '-' will be parsed in next iteration
        return;
    }

    // Check for flow sequence [...]
    if (current() == '[') {
        advance(); // skip '['
        emitSeqStart();

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
            m_tokens.append(Token(TokenType::SCALAR, value, tag, m_line, m_column));
        }

        if (current() == ']') {
            advance(); // skip ']'
        }
        emitSeqEnd();
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
        if (m_contextStack.isEmpty() || m_indentStack.top() < currentIndent) {
            emitMapStart();  // Pushes to contextStack
            m_indentStack.push(currentIndent);  // Push to indentStack - now synchronized

            // Verify synchronization
            if (m_contextStack.size() != m_indentStack.size() - 1) {
                setError("Stack synchronization error in map start");
                return;
            }
        }

        // Emit key
        QString tag = inferScalarTag(scalar);
        m_tokens.append(Token(TokenType::SCALAR, scalar, tag, m_line, m_column));

        // Check if value is on same line
        if (!isAtEnd() && current() != '\n' && current() != '#') {
            // Inline value
            if (current() == '[') {
                // Flow sequence - will be handled in next iteration
                parseContent(currentIndent);
            }
            else {
                // Scalar value
                QString value = readScalar();
                QString valueTag = inferScalarTag(value);
                m_tokens.append(Token(TokenType::SCALAR, value, valueTag, m_line, m_column));
            }
        }
        // If no value on same line, it will be on next line (nested structure)
    }
    else {
        // Just a scalar value (in sequence or as value)
        QString tag = inferScalarTag(scalar);
        m_tokens.append(Token(TokenType::SCALAR, scalar, tag, m_line, m_column));
    }
}


QString Lexer::readScalar()
{
    // Check for quoted string
    if (current() == '"' || current() == '\'') {
        return readQuotedString(current());
    }

    QString result;

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
