#include "QtNoidApp/settings.h"
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMainWindow>
#include <QWidget>
#include <QScreen>
#include <QShortcut>
#include <QClipboard>
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
        // Could be this is already the main window
        return qobject_cast<QMainWindow *>(ref);;
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
    // qDebug() << __func__ << current;
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

    QScreen* screen = mainWindow->screen();
    QRect windowRect = mainWindow->frameGeometry();
    QPoint screenOffset = screen->geometry().topLeft();
    QRect relativeRect = windowRect.translated(-screenOffset);
    QPixmap pixMap = screen->grabWindow(0,
                                        relativeRect.x(),
                                        relativeRect.y(),
                                        relativeRect.width(),
                                        relativeRect.height());


    return pixMap;
}


QShortcut *Settings::initFullDialogGrabShortcut(QWidget *parent, const QString &keySequence, QString destinationPath, bool saveToClipboard)
{
    QShortcut* shortCut = new QShortcut(QKeySequence("Ctrl+Shift+S"), parent);
    parent->connect(shortCut, &QShortcut::activated, parent, [=](){
        QPixmap screenshot =  fullDialogGrab(parent);

        auto mainWindow = mainWindowFromWidget(parent);
        QString fileName = destinationPath;
        if(!fileName.isEmpty()) {
            fileName += "/";
        }
        fileName += mainWindow->windowTitle() + ".png";
        bool res = screenshot.save(fileName);
        if(saveToClipboard) {
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setPixmap(screenshot);
        }
        qDebug() << "saveToClipboard:" << saveToClipboard <<
            "Destination:" << fileName << "Res:" << res;
    } );

    return shortCut;
}




}   // namespace App
}   // namespace QtNoid
