#ifndef QTNOID_TEXT_H
#define QTNOID_TEXT_H

#include "global.h"
#include <QObject>
#include <QString>

namespace QtNoid {
namespace Common {

class QTNOIDCOMMON_EXPORT Text
{
public:
    static bool isValidUrl(const QString &text);

    static QStringList tokenizeSnakeCase(const QString &txt);
    static QStringList tokenizeSnakeCase(const QStringList &list);
    static QStringList tokenizeCamelCase(const QString &txt);
    static QStringList tokenizeCamelCase(const QStringList &list);
    static QStringList tokenizeNumberBlocks(const QString &txt, int minNumberBlockLen = 2);
    static QStringList tokenizeNumberBlocks(const QStringList &list, int minNumberBlockLen = 2);

    static QStringList tokenize(const QString &str, const QString &splittersString, bool splitCamelCase,
                                int minNumBlockLen);

    static QString convertToCamelCase(const QString &text);
    static QString convertToSnakeCase(const QString &text);

private:
};

} // namespace QtCommon
} // namespace QtNoid

#endif // QTNOID_TEXT_H
