
#include "QtNoidCommon/file.h"
#include "QtNoidCommon/text.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QTime>
#include <QUrl>

namespace QtNoid {
namespace Common {



QString File::autoNamingNextName(const QString &currentName)
{
    QFileInfo FI(currentName);
    QString baseName = FI.baseName();

    auto tokens = QtNoid::Common::Text::tokenize(baseName, "", false, 1);

    // Get Last Number
    bool isOk = false;
    int index = 0;
    QString currentCounter;
    QString newFileName;

    for (auto it = tokens.rbegin(); it != tokens.rend(); ++it) {
        auto token = *it;
        index = token.toInt(&isOk);
        if(isOk){
            // We have a counter
            currentCounter = token;
            break;
        }
    }

    // If no counter just add 001
    if(currentCounter.isEmpty()){
        qDebug() << __func__ << FI.path() << FI.dir().filePath("AA");
        newFileName = FI.dir().filePath(QString("%1 001.%2").arg(baseName, FI.completeSuffix()));
        return newFileName;
    }

    // We have a counter
    newFileName = currentName;
    int counterLen = currentCounter.length();
    index++;
    QString newCounter = QString("%1").arg(index, counterLen, 10, QChar('0'));
    auto start = newFileName.lastIndexOf(currentCounter);
    newFileName.replace(start, counterLen, newCounter);

    return newFileName;
}


QString File::fileAutoNaming(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    if(!fileInfo.exists())
        return filePath;

    QString newPath = filePath;
    do {
        newPath = autoNamingNextName(newPath);
    } while(QFile::exists(newPath));

    return newPath;
}


QFileInfo QtNoid::Common::File::fileAutoNaming(const QFileInfo &fileInfo)
{
}




} // namespace Common
} // namespace QtNoid
