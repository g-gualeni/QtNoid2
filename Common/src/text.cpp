#include "QtNoidCommon/text.h"
#include <QCoreApplication>
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

QStringList Text::tokenizeSnakeCase(const QString &txt)
{
    return txt.split("_");
}

QStringList Text::tokenizeSnakeCase(const QStringList &list)
{
    QStringList out;
    for(const QString &txt : list) {
        out.append(tokenizeSnakeCase(txt));
    }
    return out;
}

QStringList Text::tokenizeCamelCase(const QString &txt)
{
    QStringList outTokenList;

    // Scan for start / stop upper case or digit
    int startUpper = -1;
    int readIndex = 0;

    int ii=0;
    QString nextName;
    for(auto c : txt){
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
                outTokenList.append(nextName + txt.mid(readIndex, startUpper-readIndex));
                nextName.clear();
            }
            QString upperSection = txt.mid(startUpper, ii-startUpper);
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
    outTokenList.append(nextName + txt.mid(readIndex));
    return outTokenList;
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

QString Text::convertToCamelCase(const QString &text)
{

}


/**
 * @brief convertToSnakeCase: separate text blocks using _ character and convert the
 *                            resulting string to lower case
 * @return
 */
QString Text::convertToSnakeCase(const QString &text)
{
    return tokenizeCamelCase(text).join('_').toLower();
}




QStringList Text::tokenizeCamelCase(const QStringList &list)
{
    QStringList out;

    for(const QString &txt : list) {
        out.append(tokenizeCamelCase(txt));
    }
    return out;
}

QStringList Text::tokenize(const QString &str, const QString &splittersString, bool splitCamelCase, int minNumBlockLen)
{

}




} // namespace Common
} // namespace QtNoid
