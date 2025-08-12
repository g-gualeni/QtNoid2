#include "QtNoidApp/settings.h"
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMainWindow>
#include <QWidget>
#include <QtNoidCommon/QtNoidCommon>

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

    QString cleanFName;
    if(fileName.isEmpty()) {
        cleanFName = qApp->applicationName() + ".json";
    }
    else {
        QFileInfo FI(fileName);
        cleanFName = FI.fileName();
    }
    qDebug() << __func__ << cleanFName;
    auto res = qApp->applicationDirPath() + "/" +cleanFName;
    return res;
}

QMainWindow *Settings::mainWindowFromWidget(QWidget *ref)
{
    if (ref == nullptr)
        return nullptr;
    QObject *parent = ref->parent();

    if (parent == nullptr) {
        return nullptr;
    }
    // Loop till the QMainWindow
    while (parent->isWidgetType()) {
        if (parent->parent() == nullptr) {
            break;
        }
        parent = parent->parent();
    }
    return qobject_cast<QMainWindow *>(parent);
}


QString Settings::groupNameFromClass(const QObject *ref)
{
    if (ref == nullptr)
        return QString();

    return QtNoid::Common::Text::convertToCamelCase(ref->metaObject()->className());
}


QString Settings::groupNameFromObject(const QObject *ref)
{
return {};
}





}   // namespace App
}   // namespace QtNoid
