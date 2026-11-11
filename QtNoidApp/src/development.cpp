#include "QtNoidApp/development.h"
#include "QtNoidApp/core.h"
#include "QtNoidApp/configglobal.h"
#include "QtNoidCommon/QtNoidCommon"
#include <QApplication>
#include <QFileInfo>
#include <QShortcut>
#include <QClipboard>
#include <QDebug>
#include <QSysInfo>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QDir>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QUrl>

namespace QtNoid {
namespace App {

QString Development::screenshotFileName(QWidget* parent, const QString& destinationFileOrPath)
{
    QFileInfo FI(destinationFileOrPath);
    if (!FI.suffix().isEmpty()) {
        // destinationFileOrPath is already a full file path.
        return FI.absoluteFilePath();
    }

    // No extension: destinationFileOrPath is a folder (or, if empty, the
    // app's own folder is used), and the file name is built from the
    // window title (or a generic name) plus the OS.
    QString folder = destinationFileOrPath.isEmpty() ? Core::appExeOrAppBundleDirPath()
                                                     : destinationFileOrPath;

    QString osName = QSysInfo::productType();
    auto mainWindow = Core::mainWindowFromWidget(parent);
    QString baseName = (mainWindow == nullptr)
                           ? QStringLiteral("Screenshot")
                           : QtNoid::Common::Text::convertToCamelCase(mainWindow->windowTitle());

    return QDir::cleanPath(folder + "/" + baseName + "-" + osName + ".png");
}

QShortcut* Development::initFullDialogGrabShortcut(QWidget* parent, const QString& keySequence,
                                                   QString destinationFileOrPath,
                                                   bool saveToClipboard, bool openDestinationPath)
{
    if(parent == nullptr){
        return nullptr;
    }

    QShortcut* shortCut = new QShortcut(QKeySequence(keySequence), parent);
    parent->connect(shortCut, &QShortcut::activated, parent, [=](){
        auto screenshot =  Core::fullDialogGrab(parent);

        QString finalFilePath = screenshotFileName(parent, destinationFileOrPath);
        finalFilePath = QtNoid::Common::File::autoNaming(finalFilePath);
        bool res = QDir().mkpath(QFileInfo(finalFilePath).absolutePath());
        if(res) {
            res = screenshot.save(finalFilePath);
        }

        if(saveToClipboard) {
            QClipboard* clipboard = QApplication::clipboard();
            clipboard->setImage(screenshot);
        }
        if(res && openDestinationPath) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(finalFilePath).absolutePath()));
        }
        qDebug() << "saveToClipboard:" << saveToClipboard <<
            "Destination:" << finalFilePath << "->SaveResult:" << res;
    } );

    return shortCut;
}



QAction* Development::createSaveConfigToProjectMenuAction(QMenu* developmentMenu, const QString& projectFolder)
{
    if(developmentMenu == nullptr) {
        return {};
    }
    if(projectFolder.isEmpty()) {
        return {};
    }

    // Look for qt resource file / resource folder
    const QStringList& resourcesList = Common::File::listPathRecursively(projectFolder, {".qrc"});
    if(resourcesList.isEmpty()) {
        return {};
    }
    const QString& resourceFolder = QFileInfo(resourcesList.first()).absolutePath();

    // Look for the file to replace
    QFileInfo FI(appConfig->fileName());
    QString configFile = FI.fileName();
    const QStringList&  list =  Common::File::listPathRecursively(resourceFolder, {("." + FI.suffix())});
    // qDebug() << __func__ << list.count();

    // Is the file present? => get the full path
    QString projectFile;
    for(const QString& currentFile : list){
        if(currentFile.endsWith(configFile)) {
            projectFile = currentFile;
            break;
        }
    }
    if(projectFile.isEmpty()) {
        qDebug() << __func__ << "unable to find the project file";
        return {};
    }

    auto parent = developmentMenu->parent();
    if(parent == nullptr) {
        return {};
    }

    QDir dir(projectFolder);
    auto msg = QString(parent->tr("Copy %1 configuration file to %2")).arg(configFile, dir.relativeFilePath(projectFile));

    QAction* action = developmentMenu->addAction(parent->tr("Copy ConfigFile To Project Resources"));
    action->setStatusTip(msg);
    parent->connect(action, &QAction::triggered, [FI, projectFile](){
        QFile newFile(FI.absoluteFilePath());
        bool removeRes = QFile::remove(projectFile);
        if(!removeRes) {
            qDebug() << "createSaveConfigToProjectMenuAction FAILED to remove file:" << projectFile;
            return;
        }
        bool copyRes = newFile.copy(projectFile);
        if(copyRes) {
            qDebug() << "createSaveConfigToProjectMenuAction - file" << projectFile << "replaced";
        }
        else {
            qDebug() << "createSaveConfigToProjectMenuAction FAILED with file:" << projectFile;
        }
    });

    return action;
}

QAction* Development::createInitConfigFromResourcesMenuAction(QMenu* developmentMenu, const QString& resFileOrPrefix)
{
    if(developmentMenu == nullptr) {
        return {};
    }

    auto parent = developmentMenu->parent();
    if(parent == nullptr) {
        return {};

    }

    QAction* action = developmentMenu->addAction(parent->tr("Init ConfigFile From Resources"));
    QFileInfo FI(appConfig->fileName());

    auto msg = QString(parent->tr("Rewrite %1 configuration file using the model from the resources")).arg(FI.fileName());
    action->setStatusTip(msg);

    QFileInfo resFile(resFileOrPrefix);
    if(resFile.suffix().isEmpty()) {
        // We suppose it is a prefix
        resFile.setFile(resFileOrPrefix + "/" + FI.fileName());
    }

    QString appConfigFile = FI.absoluteFilePath();
    QString resConfigFile = resFile.filePath();

    parent->connect(action, &QAction::triggered, [appConfigFile, resConfigFile](){
        if (!QFile::exists(resConfigFile)) {
            qDebug() << "initConfigFromResources: " << resConfigFile << "not found - check set(CMAKE_AUTORCC ON)";
            return;
        }
        QFile::remove(appConfigFile);
        QFile newFile(resConfigFile);        
        auto res = newFile.copy(appConfigFile);
        if(!res) {
            qDebug() << "initConfigFromResources Error copying file" << resConfigFile << "to: " << appConfigFile;
            return;
        }
        QFile::setPermissions(appConfigFile, QFile::ReadOwner | QFile::WriteOwner);
        res = appConfig->load();
        if(res) {
            qDebug() << "initConfigFromResources" << resConfigFile << "DONE";
        }
        else {
            qDebug() << "initConfigFromResources FAILED with file:" << resConfigFile;
        }
    });

    return action;
}


}   // namespace App
}   // namespace QtNoid
