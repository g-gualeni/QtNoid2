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

    static QStringList tokenizeSnakeCase(const QString &text);
    static QStringList tokenizeSnakeCase(const QStringList &list);

    static QStringList tokenizeCamelCase(const QString &text);
    static QStringList tokenizeCamelCase(const QStringList &list);

    static QStringList tokenizeNumberBlocks(const QString &text,
                                            int minNumberBlockLen=2);
    static QStringList tokenizeNumberBlocks(const QStringList &list,
                                            int minNumberBlockLen=2);

    static QStringList tokenize(const QString &text, const QString &splittersString,
                                bool splitCamelCase=true, int minNumBlockLen=2);

    static QString convertToCamelCase(const QString &text);

    static QString convertToSnakeCase(const QString &text, int minNumBlockLen=2);

private:
};

} // namespace QtCommon
} // namespace QtNoid

#endif // QTNOID_TEXT_H
