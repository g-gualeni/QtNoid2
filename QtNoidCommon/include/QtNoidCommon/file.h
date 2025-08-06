#ifndef QTNOID_FILE_H
#define QTNOID_FILE_H

#include "global.h"
#include <QFileInfo>
#include <QObject>
#include <QString>

namespace QtNoid {
namespace Common {

class QTNOIDCOMMON_EXPORT File
{
public:

    static QString autoNamingNextName(const QString &currentName);

    static QString autoNaming(const QString &filePath);
    static QFileInfo autoNaming(const QFileInfo &fileInfo);

    static bool compareIfEqual(const QString &filePath1, const QString &filePath2);
    static bool compareIfEqual(const QFileInfo &fileInfo1, const QFileInfo &fileInfo2);

    static bool isTextFile(const QString &filePath);
    static bool isTextFile(const QFileInfo &fileInfo);

    static QString saveAsTextFile(const QString &data, const QString &filePath, const QString &basePath="", const QString &fileSuffix="*");
    static QString saveAsTextFile(const QStringList &data, const QString &filePath, const QString &basePath="", const QString &fileSuffix="*");

    static QStringList readAsStringList(const QString &absoluteFilePath);

    QStringList listPathRecursively(const QString &path, const QStringList &nameFilters={});

private:
    static QFileInfo saveAsTextFileCreatePath(const QString &filePath, const QString &basePath, const QString &fileSuffix);

};


} // namespace QtCommon
} // namespace QtNoid

#endif // QTNOID_FILE_H
