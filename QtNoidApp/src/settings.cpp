#include "QtNoidApp/settings.h"
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMainWindow>
#include <QWidget>
#include <QScreen>
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
    // qDebug() << __func__ << cleanFName;
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


QString Settings::groupNameFromObjectOrClass(const QObject *ref)
{
    if (ref == nullptr)
        return QString();

    auto groupName = ref->objectName();
    if(groupName.isEmpty()) {
        return QtNoid::Common::Text::convertToCamelCase(ref->metaObject()->className());
    }
    else {
        return QtNoid::Common::Text::convertToCamelCase(groupName);
    }
}

bool Settings::updateMainWindowTitle(bool changed, QWidget *ref)
{
    auto mainWindow = mainWindowFromWidget(ref);
    if(mainWindow == nullptr)
    {
        return false;
    }

    auto current = mainWindow->windowTitle();
    if(current.last(1) == "*") {
        current = current.removeLast();
    }
    current = current.trimmed();
    qDebug() << __func__ << current;
    if(changed) {
        current += "*";
    }
    mainWindow->setWindowTitle(current);

    return true;
}

QPixmap Settings::fullDialogGrab(QWidget *ref)
{
    auto mainWindow = mainWindowFromWidget(ref);
    if(mainWindow == nullptr)
    {
        return {};
    }

    QRect windowRect = mainWindow->frameGeometry();
    QPixmap pixMap  = mainWindow->screen()->grabWindow(0,
                                                      windowRect.x(),
                                                      windowRect.y(),
                                                      windowRect.width(),
                                                      windowRect.height());

    return pixMap;
}





}   // namespace App
}   // namespace QtNoid
