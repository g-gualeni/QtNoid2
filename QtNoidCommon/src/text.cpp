#include "QtNoidCommon/text.h"
#include <QCoreApplication>
#include <QRegularExpression>
#include <QUrl>

namespace QtNoid {
namespace Common {


bool Text::isValidUrl(const QString &text)
{
    if (text.isEmpty()) {
        return false;
    }

    QUrl url(text);
    if(!url.isValid()) {
        return false;
    }
    if(url.scheme().isEmpty()) {
        return false;
    }
    if(url.host().isEmpty()) {
        return false;
    }

    return true;
}

bool Text::isValidEmail(const QString &email)
{
    if (email.isEmpty()) {
        return false;
    }

    // Pattern regex per validazione email base
    static const QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    QRegularExpressionMatch match = emailRegex.match(email);

    return match.hasMatch();
}

QString Text::sanitizeString(const QString &input)
{
    if (input.isEmpty()) {
        return {};
    }

    QString result = input.trimmed();

    // replace multiple space with a single space character
    static const QRegularExpression multipleSpacesRegex("\\s+");
    result.replace(multipleSpacesRegex, " ");

    // Remove control characters
    static const QRegularExpression controlCharsRegex("[\\x00-\\x1F\\x7F]");
    result.remove(controlCharsRegex);

    return result;
}

QStringList Text::tokenizeSnakeCase(const QString &list)
{
    return list.split('_', Qt::SkipEmptyParts);
}

QStringList Text::tokenizeSnakeCase(const QStringList &list)
{
    QStringList out;
    for(const QString &txt : list) {
        out.append(txt.split('_', Qt::SkipEmptyParts));
    }
    return out;
}

QStringList Text::tokenizeCamelCase(const QString &text)
{
    QStringList outTokenList;

    // Scan for start / stop upper case or digit
    int startUpper = -1;
    int readIndex = 0;

    int ii=0;
    QString nextName;
    for(auto c : text){
        if(c.isUpper()) {
            // There is an upper case
            if(startUpper == -1) {
                startUpper = ii;
            }
        }
        else if(startUpper>=0) {
            // No more upper case (after at least one)
            // Before Upper Case (if there is something)
            if(startUpper-readIndex > 0) {
                outTokenList.append(nextName + text.mid(readIndex, startUpper-readIndex));
                nextName.clear();
            }
            QString upperSection = text.mid(startUpper, ii-startUpper);
            if(upperSection.length() == 1) {
                // Just one upper char
                nextName = upperSection;
            }
            else {
                // More Upper chars
                outTokenList.append(upperSection.chopped(1));
                nextName = upperSection.last(1);
            }
            readIndex = ii;
            startUpper = -1;
        }
        ii++;
    }

    // Add tail
    outTokenList.append(nextName + text.mid(readIndex));
    return outTokenList;
}

QStringList Text::tokenizeCamelCase(const QStringList &list)
{
    QStringList out;
    for(const QString &txt : list) {
        out.append(tokenizeCamelCase(txt));
    }
    return out;
}


QStringList Text::tokenizeNumberBlocks(const QString &txt, int numberBlockLen)
{
    QStringList outTokenList;
    int startBlock = -1;
    int readIndex = 0;
    int ii=0;

    // Split this 22Txt and this txt22
    for(auto c : txt){
        if(c.isDigit()) {
            // There is digit
            if(startBlock == -1) {
                startBlock = ii;
            }
        }
        else if(startBlock>=0) {
            // No more numbers case (after at least one)
            if(ii-startBlock >= numberBlockLen) {
                // Append what's before
                if(startBlock-readIndex > 0){
                    outTokenList.append(txt.mid(readIndex, startBlock-readIndex));
                }
                // Append the block
                outTokenList.append(txt.mid(startBlock, ii-startBlock));
                readIndex = ii;
            }
            startBlock = -1;
        }
        ii++;
    }

    // Add tail
    if(startBlock>=0 && (ii-startBlock >= numberBlockLen)){
        // Append what's before
        if(startBlock-readIndex > 0){
            outTokenList.append(txt.mid(readIndex, startBlock-readIndex));
        }
        // Append the block
        outTokenList.append(txt.mid(startBlock, startBlock-ii));
    }
    else {
        // no blocks
        outTokenList.append(txt.mid(readIndex, ii-readIndex));
    }

    return outTokenList;
}

QStringList Text::tokenizeNumberBlocks(const QStringList &list, int numberBlockLen)
{
    QStringList outTokenList;
    for(const QString &txt : list) {
        outTokenList.append(tokenizeNumberBlocks(txt, numberBlockLen));
    }
    return outTokenList;
}

/**
 * @brief tokenize tokenize using a list of splitters and contains instead
 *  of RegExp because this is many times faster even if more limited
 * @param text
 * @param splittersString
 * @param splitCamelCase
 * @param numberBlockLen: min lengh of the number block or 0 to skip number tokenize
 * @return
 */
QStringList Text::tokenize(const QString &text, const QString &splittersString,
                           bool splitCamelCase, int minNumBlockLen)
{
    // Convert all splitter char we see into splitChar. This is much faster than
    //   using regular expression
    QChar splitChar = QChar(0x02);
    QString newStr = text;
    for (QChar splitter : splittersString) {
        newStr = newStr.replace(splitter, splitChar);
    }
    QStringList tokenList;
    tokenList = newStr.split(splitChar, Qt::SkipEmptyParts);

    if(splitCamelCase) {
        tokenList = tokenizeCamelCase(tokenList);
    }

    // Create tokens out of number blocks that are at least splitNumbersFromLetters
    //  consecutive numbers.
    // "2018-08-23 CamelCase333 22Sup"
    if(minNumBlockLen > 0) {
        tokenList = tokenizeNumberBlocks(tokenList, minNumBlockLen);
    }

    return tokenList;
}

QString Text::convertToCamelCase(const QString &text)
{
    QString camelCase;
    auto tokenList = text.split('_', Qt::SkipEmptyParts);

    // Convert first character to UpperCase
    camelCase = std::accumulate(tokenList.begin(), tokenList.end(), QString(),
                                [](const QString &camelCase, const QString &word){
                                    return camelCase + word.at(0).toUpper() + word.mid(1);
                                });
    // -> std::accumulate should be better! At least this is what CPP Check says!
    //    for(const QString &word : snakeCaseString.split('_', Qt::SkipEmptyParts)) {
    //        camelCase += word.at(0).toUpper() + word.mid(1);
    //    }

    return camelCase;
}

/**
 * @brief convertToSnakeCase: separate text blocks using _ character and convert the
 *                            resulting string to lower case
 * @return
 */
QString Text::convertToSnakeCase(const QString &text, int minNumBlockLen)
{
    auto tokenList = tokenizeCamelCase(text);
    if(minNumBlockLen > 0) {
        tokenList = tokenizeNumberBlocks(tokenList, minNumBlockLen);
    }

    return tokenList.join('_').toLower();
}




} // namespace Common
} // namespace QtNoid
