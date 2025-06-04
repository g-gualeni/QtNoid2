
#include "QtNoidCommon/file.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QTime>
#include <QUrl>

namespace QtNoid {
namespace Common {


/**
 * @brief fileIniAsAppSibling
 * Split the application file path and remove the app boundle
 * @return
 */

QString File::iniFileAsAppSibling()
{
    auto appFilePath = qApp->applicationFilePath();
    QString appPath = appExeOrAppBundlePath(appFilePath);
    if(appPath.at(appPath.length() - 4) == '.') {
        appPath.chop(4);
    }
    appPath.append(".ini");
    return appPath;

}

QString File::appExeOrAppBundlePath(const QString &appFilePath)
{
    if(appFilePath.contains(".app/Contents/MacOS")){
        // This is a bundle for macOS
        QFileInfo FI(appFilePath);
        QDir dir;
        FI.setFile(dir.cleanPath(FI.filePath() + "/../../../"));
        return FI.filePath();
    }
    else {
        return appFilePath;
    }
}

} // namespace Common
} // namespace QtNoid
