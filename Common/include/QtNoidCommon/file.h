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

    static qint64 fileSaveAsTextFile(const QString &absoluteFilePath, const QByteArray &data, bool addExtension = true);
    static qint64 fileSaveAsTextFile(const QString &absoluteFilePath, const QStringList &data, bool addExtension = true);
    static qint64 fileSaveAsTextFile(const QString &absoluteFilePath, const QString &data, bool addExtension = true);

    static QStringList fileLoadAsStringList(const QString &absoluteFilePath);

private:
};

} // namespace QtCommon
} // namespace QtNoid

#endif // QTNOID_FILE_H
