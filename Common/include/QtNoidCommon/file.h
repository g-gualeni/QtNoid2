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

    static QString fileAutoNaming(const QString &filePath);
    static QFileInfo fileAutoNaming(const QFileInfo &fileInfo);


    static bool fileCompareIfEqual(const QString &path, const QString &newPath);
    static bool fileCompareIfEqual(const QFileInfo &afileInfo, const QFileInfo &newFileInfo);

    static bool fileIsTextFile(const QString &absoluteFilePath);
    static bool fileIsTextFile(const QFileInfo &fileInfo);

    static qint64 fileSaveAsTextFile(const QString &absoluteFilePath, const QByteArray &data, bool addExtension = true);
    static qint64 fileSaveAsTextFile(const QString &absoluteFilePath, const QStringList &data, bool addExtension = true);
    static qint64 fileSaveAsTextFile(const QString &absoluteFilePath, const QString &data, bool addExtension = true);

    static QStringList fileLoadAsStringList(const QString &absoluteFilePath);

private:
};

} // namespace QtCommon
} // namespace QtNoid

#endif // QTNOID_FILE_H
