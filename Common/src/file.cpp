
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
        newFileName = QString("%1 001.%2").arg(baseName, FI.completeSuffix());
        if(FI.filePath() != FI.fileName()) {
            // There is also a path
            newFileName = FI.path() + "/" + newFileName;
        }
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


QString File::autoNaming(const QString &filePath)
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


QFileInfo QtNoid::Common::File::autoNaming(const QFileInfo &fileInfo)
{
    if(!fileInfo.exists())
        return fileInfo;

    QString newFileName = fileInfo.fileName();

    do {
        newFileName = autoNamingNextName(newFileName);
    } while(QFile::exists(newFileName));

    return QFileInfo(newFileName);
}

bool File::compareIfEqual(const QString &filePath1, const QString &filePath2)
{
    return compareIfEqual(QFileInfo(filePath1), QFileInfo(filePath2));
}


bool File::compareIfEqual(const QFileInfo &fileInfo1, const QFileInfo &fileInfo2)
{
    // Non existing files are equal
    if (!fileInfo1.exists() && !fileInfo2.exists()) {
        return true;
    }
    // If one is not existing then they are different
    if (!fileInfo1.exists() || !fileInfo2.exists()) {
        return false; // Solo uno inesistente = diversi
    }
    // Size check
    qint64 size = fileInfo1.size();
    if (size != fileInfo2.size()) {
        return false;
    }

    // is the same fisical path?
    if (fileInfo1.canonicalFilePath() == fileInfo2.canonicalFilePath()) {
        return true;
    }


    QFile f1(fileInfo1.absoluteFilePath());
    QFile f2(fileInfo2.absoluteFilePath());

    if (!f1.open(QIODevice::ReadOnly)) {
        return false;
    }
    if (!f2.open(QIODevice::ReadOnly)) {
        return false;
    }

    uchar* f1Buff = f1.map(0, size);
    uchar* f2Buff = f2.map(0, size);

    int res = memcmp(f1Buff, f2Buff, static_cast<unsigned int>(size));

    f1.unmap(f1Buff);
    f2.unmap(f2Buff);

    if(res ==0) {
        // the two files are Equal
        return true;
    }


    return false;
}

bool File::isTextFile(const QString &filePath)
{
    QFileInfo FI(filePath);
    return isTextFile(FI);
}

bool File::isTextFile(const QFileInfo &fileInfo)
{
    // Sanity check
    if (!fileInfo.exists()) {
        return false;
    }

    if(!fileInfo.isFile()) {
        return false;
    }


    // Read filesize or max 50.000 char and look for \0
    qint64 size = qMin(static_cast<qint64>(50000), fileInfo.size());
    QFile file(fileInfo.absoluteFilePath());

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QTextStream stream(&file);
    stream.setAutoDetectUnicode(true);
    QString content = stream.read(size);
    qDebug() << "Codec rilevato:" << stream.encoding();

    // Controllo se contiene caratteri non stampabili?
    //  - dipende, perchè se fosse UNICODE ne avrebbe molti
    // uso il reader di Qt e cosa succede?

    // QTextStream bool QTextStream::autoDetectUnicode() const

    // e lo converto in unicode

    //  qDebug() << "Codec rilevato:" << stream.codec()->name();


    // Attenzione che devo gestire sia il testo UCHAR che il testo UTF8

    return false;
}







} // namespace Common
} // namespace QtNoid
