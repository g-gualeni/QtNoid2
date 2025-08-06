#include "QtNoidApp/settings.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>

namespace QtNoid {
namespace App {

QString Settings::appExeOrAppBundlePath()
{
    auto appFilePath = qApp->applicationFilePath();
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

QString Settings::filePathAsAppSibling(const QString &fileName)
{
    QFileInfo FI(fileName);
    auto cleanFName = FI.fileName();
    auto res = qApp->applicationDirPath() + "/" +cleanFName;
    return res;
}

}
}
