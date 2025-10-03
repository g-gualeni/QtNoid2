#include "text2json.h"
#include <QJsonDocument>

// ============================================================================
// LEXER IMPLEMENTATION
// ============================================================================

Lexer::Lexer(const QString& source)
    : m_source(source), m_pos(0), m_line(1), m_column(1), m_atLineStart(true)
{
    m_indentStack.push(0); // Livello base
}

QChar Lexer::current() const {
    return m_pos < m_source.length() ? m_source[m_pos] : QChar('\0');
}

QChar Lexer::peek(int offset) const {
    int pos = m_pos + offset;
    return pos < m_source.length() ? m_source[pos] : QChar('\0');
}

void Lexer::advance() {
    if (m_pos < m_source.length()) {
        if (m_source[m_pos] == '\n') {
            m_line++;
            m_column = 1;
            m_atLineStart = true;
        } else {
            m_column++;
        }
        m_pos++;
    }
}

void Lexer::skipWhitespace(bool preserveNewlines) {
    while (current().isSpace() && !(preserveNewlines && current() == '\n')) {
        if (current() == '\n') {
            m_atLineStart = true;
        } else if (current() != ' ' && current() != '\t') {
            // Altri spazi bianchi non newline
        }
        advance();
    }
}

void Lexer::skipComment() {
    if (current() == '/' && peek() == '/') {
        while (current() != '\n' && current() != '\0') {
            advance();
        }
    } else if (current() == '#') {
        while (current() != '\n' && current() != '\0') {
            advance();
        }
    }
}

int Lexer::measureIndentation() {
    int indent = 0;
    int tempPos = m_pos;
    
    while (tempPos < m_source.length() && 
           (m_source[tempPos] == ' ' || m_source[tempPos] == '\t')) {
        if (m_source[tempPos] == '\t') {
            indent += 4; // Tab = 4 spazi
        } else {
            indent++;
        }
        tempPos++;
    }
    
    return indent;
}

void Lexer::handleIndentation(int currentIndent) {
    int previousIndent = m_indentStack.top();
    
    if (currentIndent > previousIndent) {
        // Aumento indentazione
        m_indentStack.push(currentIndent);
        m_pendingTokens.append(Token(TokenType::Indent, "", m_line, 1));
    } else if (currentIndent < previousIndent) {
        // Riduzione indentazione - può generare più DEDENT
        while (!m_indentStack.isEmpty() && m_indentStack.top() > currentIndent) {
            m_indentStack.pop();
            m_pendingTokens.append(Token(TokenType::Dedent, "", m_line, 1));
        }
        
        // Errore: indentazione non consistente
        if (m_indentStack.isEmpty() || m_indentStack.top() != currentIndent) {
            m_pendingTokens.clear();
            m_pendingTokens.append(Token(TokenType::Invalid, 
                "Inconsistent indentation", m_line, 1));
        }
    }
}

Token Lexer::readString() {
    int startLine = m_line;
    int startColumn = m_column;
    QString value;
    
    advance(); // Skip opening quote
    
    while (current() != '"' && current() != '\0') {
        if (current() == '\\') {
            advance();
            switch (current().toLatin1()) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case 'r': value += '\r'; break;
                case '\\': value += '\\'; break;
                case '"': value += '"'; break;
                default: value += current(); break;
            }
        } else {
            value += current();
        }
        advance();
    }
    
    if (current() == '"') {
        advance(); // Skip closing quote
        return Token(TokenType::String, value, startLine, startColumn);
    }
    
    return Token(TokenType::Invalid, "Unterminated string", startLine, startColumn);
}

Token Lexer::readNumber() {
    int startLine = m_line;
    int startColumn = m_column;
    QString value;
    
    if (current() == '-') {
        value += current();
        advance();
    }
    
    while (current().isDigit()) {
        value += current();
        advance();
    }
    
    if (current() == '.') {
        value += current();
        advance();
        while (current().isDigit()) {
            value += current();
            advance();
        }
    }
    
    return Token(TokenType::Number, value, startLine, startColumn);
}

Token Lexer::readIdentifierOrString() {
    int startLine = m_line;
    int startColumn = m_column;
    QString value;
    
    // Legge fino a trovare: spazio, newline, :, ,, {, }, [, ], o fine file
    while (current() != '\0' && 
           !current().isSpace() &&
           current() != ':' && 
           current() != ',' &&
           current() != '{' && 
           current() != '}' &&
           current() != '[' && 
           current() != ']' &&
           current() != '-') {
        value += current();
        advance();
    }
    
    // Check for keywords
    if (value == "true" || value == "false") {
        return Token(TokenType::Boolean, value, startLine, startColumn);
    }
    if (value == "null") {
        return Token(TokenType::Null, value, startLine, startColumn);
    }
    
    // Altrimenti è un identificatore/stringa senza virgolette
    return Token(TokenType::Identifier, value, startLine, startColumn);
}

Token Lexer::nextToken() {
    // Ritorna token in coda (DEDENT multipli)
    if (!m_pendingTokens.isEmpty()) {
        return m_pendingTokens.takeFirst();
    }
    
    // Gestione indentazione a inizio riga
    if (m_atLineStart) {
        // Salta righe vuote e commenti
        while (current().isSpace() || current() == '#' || (current() == '/' && peek() == '/')) {
            if (current() == '#' || (current() == '/' && peek() == '/')) {
                skipComment();
            }
            if (current() == '\n') {
                advance();
                continue;
            }
            if (current().isSpace() && current() != '\n') {
                break;
            }
        }
        
        if (current() == '\0') {
            // Fine file - genera tutti i DEDENT necessari
            while (m_indentStack.size() > 1) {
                m_indentStack.pop();
                m_pendingTokens.append(Token(TokenType::Dedent, "", m_line, 1));
            }
            if (!m_pendingTokens.isEmpty()) {
                return m_pendingTokens.takeFirst();
            }
            return Token(TokenType::EndOfFile, "", m_line, m_column);
        }
        
        // Misura indentazione solo se non siamo su una riga vuota
        if (current() != '\n') {
            int indent = measureIndentation();
            handleIndentation(indent);
            
            // Salta gli spazi dell'indentazione
            while (current() == ' ' || current() == '\t') {
                advance();
            }
            
            m_atLineStart = false;
            
            if (!m_pendingTokens.isEmpty()) {
                return m_pendingTokens.takeFirst();
            }
        }
    }
    
    skipWhitespace(true); // Preserva newline
    skipComment();
    
    if (current() == '\n') {
        advance();
        m_atLineStart = true;
        return Token(TokenType::Newline, "", m_line - 1, m_column);
    }
    
    if (m_pos >= m_source.length()) {
        // Fine file - genera tutti i DEDENT necessari
        while (m_indentStack.size() > 1) {
            m_indentStack.pop();
            m_pendingTokens.append(Token(TokenType::Dedent, "", m_line, 1));
        }
        if (!m_pendingTokens.isEmpty()) {
            return m_pendingTokens.takeFirst();
        }
        return Token(TokenType::EndOfFile, "", m_line, m_column);
    }
    
    int line = m_line;
    int col = m_column;
    QChar ch = current();
    
    switch (ch.toLatin1()) {
        case '{':
            advance();
            return Token(TokenType::LBrace, "{", line, col);
        case '}':
            advance();
            return Token(TokenType::RBrace, "}", line, col);
        case '[':
            advance();
            return Token(TokenType::LBracket, "[", line, col);
        case ']':
            advance();
            return Token(TokenType::RBracket, "]", line, col);
        case ':':
            advance();
            return Token(TokenType::Colon, ":", line, col);
        case ',':
            advance();
            return Token(TokenType::Comma, ",", line, col);
        case '-':
            // Potrebbe essere un numero negativo o un dash per liste
            if (peek().isDigit()) {
                return readNumber();
            } else {
                advance();
                return Token(TokenType::Dash, "-", line, col);
            }
        case '"':
            return readString();
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return readNumber();
        default:
            if (ch.isLetter() || ch == '_') {
                return readIdentifierOrString();
            }
            break;
    }
    
    advance();
    return Token(TokenType::Invalid, QString("Unexpected character: ") + ch, line, col);
}

bool Lexer::hasNext() const {
    return m_pos < m_source.length() || !m_pendingTokens.isEmpty();
}

// ============================================================================
// PARSER IMPLEMENTATION
// ============================================================================

Parser::Parser(const QString& source)
    : m_lexer(source)
{
    advance();
}

void Parser::advance() {
    m_current = m_lexer.nextToken();
}

bool Parser::match(TokenType type) {
    if (m_current.type == type) {
        advance();
        return true;
    }
    return false;
}

bool Parser::expect(TokenType type, const QString& message) {
    if (m_current.type != type) {
        setError(message);
        return false;
    }
    advance();
    return true;
}

void Parser::skipNewlines() {
    while (m_current.type == TokenType::Newline) {
        advance();
    }
}

void Parser::setError(const QString& message) {
    if (m_error.isEmpty()) {
        m_error = QString("Line %1, Column %2: %3")
                     .arg(m_current.line)
                     .arg(m_current.column)
                     .arg(message);
    }
}

bool Parser::isValueStart() const {
    return m_current.type == TokenType::String ||
           m_current.type == TokenType::Number ||
           m_current.type == TokenType::Boolean ||
           m_current.type == TokenType::Null ||
           m_current.type == TokenType::Identifier ||
           m_current.type == TokenType::LBrace ||
           m_current.type == TokenType::LBracket ||
           m_current.type == TokenType::Dash;
}

QJsonValue Parser::parseValue() {
    skipNewlines();
    
    switch (m_current.type) {
        case TokenType::String:
        case TokenType::Identifier:
            {
                QString val = m_current.value;
                advance();
                return QJsonValue(val);
            }
        case TokenType::Number:
            {
                QString val = m_current.value;
                advance();
                if (val.contains('.')) {
                    return QJsonValue(val.toDouble());
                } else {
                    return QJsonValue(val.toLongLong());
                }
            }
        case TokenType::Boolean:
            {
                bool val = (m_current.value == "true");
                advance();
                return QJsonValue(val);
            }
        case TokenType::Null:
            advance();
            return QJsonValue(QJsonValue::Null);
        case TokenType::LBrace:
            return QJsonValue(parseBracedObject());
        case TokenType::LBracket:
            return QJsonValue(parseBracketArray());
        case TokenType::Dash:
            return QJsonValue(parseListArray());
        default:
            setError("Expected value");
            return QJsonValue();
    }
}

QJsonObject Parser::parseObject() {
    skipNewlines();
    
    // Decide se usare graffe o indentazione
    if (m_current.type == TokenType::LBrace) {
        return parseBracedObject();
    } else if (m_current.type == TokenType::Indent) {
        return parseIndentedObject();
    }
    
    setError("Expected object");
    return QJsonObject();
}

QJsonObject Parser::parseBracedObject() {
    QJsonObject obj;
    
    if (!expect(TokenType::LBrace, "Expected '{'")) {
        return obj;
    }
    
    skipNewlines();
    
    while (m_current.type != TokenType::RBrace && m_current.type != TokenType::EndOfFile) {
        skipNewlines();
        
        if (m_current.type == TokenType::RBrace) {
            break;
        }
        
        QString key;
        
        if (m_current.type == TokenType::Identifier || m_current.type == TokenType::String) {
            key = m_current.value;
            advance();
        } else {
            setError("Expected property name");
            return obj;
        }
        
        skipNewlines();
        
        if (!expect(TokenType::Colon, "Expected ':'")) {
            return obj;
        }
        
        skipNewlines();
        obj[key] = parseValue();
        
        skipNewlines();
        
        if (m_current.type == TokenType::Comma) {
            advance();
            skipNewlines();
        }
    }
    
    expect(TokenType::RBrace, "Expected '}'");
    return obj;
}

QJsonObject Parser::parseIndentedObject() {
    QJsonObject obj;
    
    if (!expect(TokenType::Indent, "Expected indentation")) {
        return obj;
    }
    
    skipNewlines();
    
    while (m_current.type != TokenType::Dedent && m_current.type != TokenType::EndOfFile) {
        skipNewlines();
        
        if (m_current.type == TokenType::Dedent) {
            break;
        }
        
        QString key;
        
        if (m_current.type == TokenType::Identifier || m_current.type == TokenType::String) {
            key = m_current.value;
            advance();
        } else {
            setError("Expected property name");
            return obj;
        }
        
        if (!expect(TokenType::Colon, "Expected ':'")) {
            return obj;
        }
        
        skipNewlines();
        
        // Controlla se il valore è un oggetto indentato
        if (m_current.type == TokenType::Indent) {
            obj[key] = parseIndentedObject();
        } else if (m_current.type == TokenType::LBrace) {
            obj[key] = parseBracedObject();
        } else if (m_current.type == TokenType::Dash) {
            obj[key] = parseListArray();
        } else {
            obj[key] = parseValue();
        }
        
        skipNewlines();
    }
    
    if (m_current.type == TokenType::Dedent) {
        advance();
    }
    
    return obj;
}

QJsonArray Parser::parseArray() {
    if (m_current.type == TokenType::LBracket) {
        return parseBracketArray();
    } else if (m_current.type == TokenType::Dash) {
        return parseListArray();
    }
    
    setError("Expected array");
    return QJsonArray();
}

QJsonArray Parser::parseBracketArray() {
    QJsonArray arr;
    
    if (!expect(TokenType::LBracket, "Expected '['")) {
        return arr;
    }
    
    skipNewlines();
    
    while (m_current.type != TokenType::RBracket && m_current.type != TokenType::EndOfFile) {
        skipNewlines();
        
        if (m_current.type == TokenType::RBracket) {
            break;
        }
        
        arr.append(parseValue());
        
        skipNewlines();
        
        if (m_current.type == TokenType::Comma) {
            advance();
            skipNewlines();
        } else if (m_current.type != TokenType::RBracket) {
            break;
        }
    }
    
    expect(TokenType::RBracket, "Expected ']'");
    return arr;
}

QJsonArray Parser::parseListArray() {
    QJsonArray arr;
    
    // Array stile lista con '-'
    while (m_current.type == TokenType::Dash) {
        advance(); // Skip '-'
        skipNewlines();
        
        // Controlla se l'elemento è un oggetto indentato
        if (m_current.type == TokenType::Indent) {
            arr.append(parseIndentedObject());
        } else if (m_current.type == TokenType::LBrace) {
            arr.append(parseBracedObject());
        } else if (m_current.type == TokenType::Identifier || 
                   m_current.type == TokenType::String) {
            // Potrebbe essere l'inizio di un oggetto inline
            QString firstKey = m_current.value;
            advance();
            
            if (m_current.type == TokenType::Colon) {
                // È un oggetto inline
                QJsonObject obj;
                obj[firstKey] = parseValue();
                
                skipNewlines();
                
                // Continua a leggere coppie chiave:valore sulla stessa riga o indentate
                while ((m_current.type == TokenType::Identifier || 
                        m_current.type == TokenType::String) &&
                       m_current.type != TokenType::Dash) {
                    QString key = m_current.value;
                    advance();
                    
                    if (m_current.type != TokenType::Colon) {
                        break;
                    }
                    advance();
                    skipNewlines();
                    
                    obj[key] = parseValue();
                    skipNewlines();
                }
                
                arr.append(obj);
            } else {
                // È solo un valore semplice
                arr.append(QJsonValue(firstKey));
            }
        } else {
            arr.append(parseValue());
        }
        
        skipNewlines();
    }
    
    return arr;
}

QJsonDocument Parser::parse() {
    m_error.clear();
    skipNewlines();
    
    QJsonValue root = parseValue();
    
    if (hasError()) {
        return QJsonDocument();
    }
    
    if (root.isObject()) {
        return QJsonDocument(root.toObject());
    } else if (root.isArray()) {
        return QJsonDocument(root.toArray());
    }
    
    setError("Root must be object or array");
    return QJsonDocument();
}

// ============================================================================
// TEXT2JSON IMPLEMENTATION
// ============================================================================

Text2Json::Text2Json() {
}

QJsonDocument Text2Json::compile(const QString& dslText) {
    m_lastError.clear();
    
    Parser parser(dslText);
    QJsonDocument doc = parser.parse();
    
    if (parser.hasError()) {
        m_lastError = parser.lastError();
        return QJsonDocument();
    }
    
    return doc;
}

QString Text2Json::compileToString(const QString& dslText, QJsonDocument::JsonFormat format) {
    QJsonDocument doc = compile(dslText);
    if (hasError()) {
        return QString();
    }
    return QString::fromUtf8(doc.toJson(format));
}

QString Text2Json::valueToString(const QJsonValue& value, int indent) {
    QString indentStr = QString("    ").repeated(indent);
    
    switch (value.type()) {
        case QJsonValue::String: {
            QString str = value.toString();
            // Usa virgolette solo se contiene spazi
            if (str.contains(' ') || str.contains('\t') || str.contains('\n')) {
                return QString("\"%1\"").arg(str);
            }
            return str;
        }
        case QJsonValue::Double:
            return QString::number(value.toDouble());
        case QJsonValue::Bool:
            return value.toBool() ? "true" : "false";
        case QJsonValue::Null:
            return "null";
        case QJsonValue::Object:
            return objectToDsl(value.toObject(), indent);
        case QJsonValue::Array:
            return arrayToDsl(value.toArray(), indent);
        default:
            return "";
    }
}

QString Text2Json::objectToDsl(const QJsonObject& obj, int indent) {
    QString result;
    QString indentStr = QString("    ").repeated(indent);
    
    QStringList keys = obj.keys();
    for (int i = 0; i < keys.size(); ++i) {
        const QString& key = keys[i];
        QJsonValue val = obj[key];
        
        if (i > 0) {
            result += "\n";
        }
        
        result += indentStr + key + ":";
        
        if (val.isObject() || val.isArray()) {
            result += "\n" + valueToString(val, indent + 1);
        } else {
            result += " " + valueToString(val, indent);
        }
    }
    
    return result;
}

QString Text2Json::arrayToDsl(const QJsonArray& arr, int indent) {
    // Controlla se è un array di oggetti (usa sintassi lista)
    bool hasObjects = false;
    for (const QJsonValue& val : arr) {
        if (val.isObject()) {
            hasObjects = true;
            break;
        }
    }
    
    if (hasObjects) {
        // Sintassi lista con '-'
        QString result;
        QString indentStr = QString("    ").repeated(indent);
        
        for (int i = 0; i < arr.size(); ++i) {
            if (i > 0) {
                result += "\n";
            }
            result += indentStr + "- ";
            
            if (arr[i].isObject()) {
                QJsonObject obj = arr[i].toObject();
                QStringList keys = obj.keys();
                for (int j = 0; j < keys.size(); ++j) {
                    if (j > 0) {
                        result += "\n" + QString("    ").repeated(indent + 1);
                    }
                    result += keys[j] + ": " + valueToString(obj[keys[j]], indent + 1);
                }
            } else {
                result += valueToString(arr[i], indent);
            }
        }
        
        return result;
    } else {
        // Sintassi bracket
        QString result = "[";
        
        for (int i = 0; i < arr.size(); ++i) {
            result += valueToString(arr[i], indent);
            if (i < arr.size() - 1) {
                result += ", ";
            }
        }
        
        result += "]";
        return result;
    }
}

QString Text2Json::jsonToDsl(const QJsonDocument& json) {
    if (json.isObject()) {
        return objectToDsl(json.object(), 0);
    } else if (json.isArray()) {
        return arrayToDsl(json.array(), 0);
    }
    return "";
}

QString Text2Json::jsonToDsl(const QString& jsonString) {
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (doc.isNull()) {
        m_lastError = "Invalid JSON string";
        return "";
    }
    return jsonToDsl(doc);
}