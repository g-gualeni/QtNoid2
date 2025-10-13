#include "yaml_parser.h"

namespace QtNoid {
namespace Json {
namespace Internal {

Parser::Parser()
{
}

Parser::Parser(const QVector<Token> &tokens)
    : m_tokens(tokens)
{
}

std::shared_ptr<ASTNode> QtNoid::Json::Internal::Parser::parse()
{
    // Reset state
    m_pos = 0;
    m_error.clear();

    // Skip STREAM_START and DOC_START
    skipStreamAndDocTokens();

    if (isAtEnd()) {
        // Empty document
        return std::make_shared<ASTNode>(ScalarNode("", "!!null"));
    }

    // Parse the main document
    return parseValue();
}

QVector<Token> Parser::tokens() const
{
    return m_tokens;
}

void Parser::setTokens(const QVector<Token> &newTokens)
{
    m_tokens = newTokens;
}

const Token &Parser::current() const
{
    if(m_pos >= m_tokens.size()) {
        static Token eofToken(TokenType::EOF_TOKEN);
        return eofToken;
    }
    return m_tokens.at(m_pos);
}

const Token &Parser::peek(int offset) const
{
    int pos = m_pos + offset;
    if (pos >= m_tokens.size()) {
        static Token eofToken(TokenType::EOF_TOKEN);
        return eofToken;
    }
    return m_tokens.at(pos);
}

void Parser::advance()
{
    if (!isAtEnd()) {
        m_pos++;
    }
}

bool Parser::isAtEnd() const
{
    return m_pos >= m_tokens.size() ||
           current().type == TokenType::EOF_TOKEN ||
           current().type == TokenType::DOC_END ||
           current().type == TokenType::STREAM_END;
}

bool Parser::check(TokenType type) const
{
    if (isAtEnd()) return false;
    return current().type == type;
}

bool Parser::match(TokenType type)
{
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

// std::shared_ptr<ASTNode> Parser::parseDocument()
// {
//     return parseValue();
// }

std::shared_ptr<ASTNode> Parser::parseValue()
{
    if (isAtEnd()) {
        return std::make_shared<ASTNode>(ScalarNode("", "!!null"));
    }

    TokenType type = current().type;

    switch (type) {
    case TokenType::MAP_START:
        return parseMap();

    case TokenType::SEQ_START:
        return parseSeq();

    case TokenType::SCALAR:
        return parseScalar();

    case TokenType::MAP_END:
    case TokenType::SEQ_END:
    case TokenType::DOC_END:
    case TokenType::STREAM_END:
        // End markers - return null
        return std::make_shared<ASTNode>(ScalarNode("", "!!null"));
        advance();

    default:
        setError("Unexpected token type", current());
        return std::make_shared<ASTNode>(ScalarNode("", "!!null"));
    }

}

std::shared_ptr<ASTNode> Parser::parseMap()
{
    if (!match(TokenType::MAP_START)) {
        setError("Expected MAP_START", current());
        return std::make_shared<ASTNode>(MapNode());
    }

    MapNode map;

    // Parse key-value pairs
    while (!isAtEnd() && !check(TokenType::MAP_END)) {
        // Parse key
        if (!check(TokenType::SCALAR)) {
            // Nested structure or end
            if (check(TokenType::MAP_END) || check(TokenType::SEQ_END)) {
                break;
            }
            setError("Expected scalar key in map", current());
            break;
        }

        auto key = parseScalar();

        // Parse value
        auto value = parseValue();

        // Add pair to map
        map.pairs.append(qMakePair(key, value));
    }

    // Consume MAP_END
    match(TokenType::MAP_END);

    return std::make_shared<ASTNode>(map);
}

std::shared_ptr<ASTNode> Parser::parseSeq()
{
    if (!match(TokenType::SEQ_START)) {
        setError("Expected SEQ_START", current());
        return std::make_shared<ASTNode>(SeqNode());
    }

    SeqNode seq;

    // Parse sequence items
    while (!isAtEnd() && !check(TokenType::SEQ_END)) {        
        auto item = parseValue();
        seq.items.append(item);
        if(!m_error.isEmpty()) {
            break;
        }
    }

    // Consume SEQ_END
    match(TokenType::SEQ_END);

    return std::make_shared<ASTNode>(seq);
}

std::shared_ptr<ASTNode> Parser::parseScalar()
{
    if (!check(TokenType::SCALAR)) {
        setError("Expected SCALAR", current());
        return std::make_shared<ASTNode>(ScalarNode("", "!!null"));
    }

    Token tok = current();
    advance();

    return std::make_shared<ASTNode>(ScalarNode(tok.value, tok.tag));
}

QString Parser::consumeScalar()
{
    if (!check(TokenType::SCALAR)) {
        setError("Expected scalar value", current());
        return QString();
    }

    QString value = current().value;
    advance();
    return value;
}

void Parser::skipStreamAndDocTokens()
{
    while (!isAtEnd() &&
           (current().type == TokenType::STREAM_START ||
            current().type == TokenType::DOC_START)) {
        advance();
    }
}

void Parser::setError(const QString &message, const Token &tok)
{
    if (m_error.isEmpty()) {
        m_error = QString("Parse error at line %1, col %2: %3")
        .arg(tok.line)
            .arg(tok.column)
            .arg(message);
    }
}

Token Parser::makeInvalidToken() const
{
    return Token(TokenType::INVALID, 0, 0);
}


} // namespace Internal
} // namespace Json
} // namespace QtNoid
