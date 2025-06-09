
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
    if(size == 0) {
        return false;
    }

    QFile file(fileInfo.absoluteFilePath());

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QTextStream stream(&file);
    stream.setAutoDetectUnicode(true);
    QString content = stream.read(size);
    Q_UNUSED(content)

    // This test cover files with the BOM. Detection in this case
    // is quite reliable using QTextStream::encoding().
    if(stream.encoding() >= QStringConverter::Utf16) {
        // this is UNICODE text for sure
        return true;
    }

    // If codec is 0 then I can check if there are null characters
    // Count the number of control characters
    file.seek(0);
    QByteArray rawData = file.read(size);
    int controlChars = 0;
    for (size_t ii=0; ii< rawData.size(); ii++) {
        unsigned char c = static_cast<unsigned char>(rawData[ii]);
        if (c == 0 || (c < 32 && c != 9 && c != 10 && c != 13)) {
            controlChars++;
        }
    }
    if(controlChars == 0) {
        // No control chars, means it is a 8 bit char text.
        return true;
    }

    // All the rest is not text
    return false;
}


QFileInfo File::saveAsTextFileCreatePath(const QString &filePath, const QString &basePath, const QString &fileSuffix)
{
    QString newPath = basePath + "/" + filePath;
    QFileInfo fileInfo(newPath);

    QDir dir;

    if(!dir.mkpath(fileInfo.absolutePath())){
        return {};
    };

    if(fileSuffix != "*") {
        QString newFileName = fileSuffix.trimmed();
        if(newFileName.isEmpty()){
        }
        else if(newFileName.startsWith(".")) {
        }
        else {
            newFileName = "." + newFileName;
        }
        newFileName = fileInfo.completeBaseName() + newFileName;
        fileInfo.setFile(fileInfo.absoluteDir(), newFileName);
    }

    return fileInfo;
}


QString File::saveAsTextFile(const QString &data, const QString &filePath, const QString &basePath, const QString &fileSuffix)
{
    auto path = saveAsTextFileCreatePath(filePath, basePath, fileSuffix).absoluteFilePath();

    auto file = QFile(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return {};
    }

    auto stream = QTextStream(&file);
    stream << data;

    return path;
}


QString File::saveAsTextFile(const QStringList &data, const QString &filePath, const QString &basePath, const QString &fileSuffix)
{
    auto path = saveAsTextFileCreatePath(filePath, basePath, fileSuffix).absoluteFilePath();

    auto file = QFile(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return {};
    }

    auto stream = QTextStream(&file);
    stream << data.join("\n");

    return path;
}

QStringList File::readAsStringList(const QString &absoluteFilePath)
{
    QFile f(absoluteFilePath);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return {};
    }
    QTextStream stream(&f);

    QStringList lines;
    while (!stream.atEnd()) {
        lines << stream.readLine();
    }

    f.close();

    return lines;
}


QStringList File::listPathRecursively(const QString &path, const QStringList &nameFilters)
{
    QStringList list;
    QFileInfo pathFI(path);
    QDir dir;

    // If we have a path then we look for siblings and all
    // sub-folders
    if(pathFI.exists() && pathFI.isFile()) {
        dir.setPath(pathFI.absolutePath());
    }
    else {
        dir.setPath(path);
    }

    // We have a folder, scan it!
    // Use System to list also broken .lnk files
    QDir::Filters filters = QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::System;

    // entryInfoList(nameFilters, ...) skip filter also folders name
    const QFileInfoList fileList = dir.entryInfoList(filters);
    for(const QFileInfo& FI : fileList) {
        if(FI.isDir()) {
            list.append(listPathRecursively(FI.filePath(), nameFilters));
        }
        else {
            QString filePath = FI.filePath();
            if(nameFilters.isEmpty()) {
                list << filePath;
            }
            // else not needed
            for(const QString &ext : nameFilters) {
                if(filePath.endsWith(ext, Qt::CaseInsensitive)) {
                    list << filePath;
                    break;
                }
            }
        }
    }

    return list;
}











} // namespace Common
} // namespace QtNoid
