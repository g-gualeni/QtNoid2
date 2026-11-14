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
#include <QSaveFile>
#include <QJsonDocument>
#include <QJsonObject>

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



QAction* Development::createSaveCurrentConfigToProjectMenuAction(QMenu* developmentMenu, const QString& projectFolder)
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
            if(QFileInfo(currentFile).fileName() == configFile) {
                projectFile = currentFile;
                break;
            }
        }
    }
    if(projectFile.isEmpty()) {
        qDebug() << __func__ << "unable to find the project file";
        return {};
    }

    QDir dir(projectFolder);
    auto msg = QString(tr("Copy Current App Configuration file to %2")).arg(dir.relativeFilePath(projectFile));

    QAction* action = developmentMenu->addAction(tr("Save appConfig To Project Resources"));
    action->setStatusTip(msg);

    QObject::connect(action, &QAction::triggered, [projectFile](){
        const QByteArray newContent = QJsonDocument(appConfig->toJsonValues()).toJson();
        QSaveFile out(projectFile);
        if (!out.open(QIODevice::WriteOnly) || out.write(newContent) != newContent.size() || !out.commit()) {
            qDebug() << "createSaveCurrentConfigToProjectMenuAction FAILED and cannot write:" << projectFile << out.errorString();
            return;
        }
        else {
            qDebug() << "createSaveCurrentConfigToProjectMenuAction - file" << projectFile << "replaced";
        }
    });

    return action;
}

QAction* Development::createInitConfigFromResourcesMenuAction(QMenu* developmentMenu, const QString& resFileOrPrefix)
{
    if(developmentMenu == nullptr) {
        return {};
    }

    QAction* action = developmentMenu->addAction(tr("Init ConfigFile From Resources"));
    QFileInfo FI(appConfig->fileName());

    auto msg = QString(tr("Rewrite %1 configuration file using the model from the resources")).arg(FI.fileName());
    action->setStatusTip(msg);

    QString resConfigFile = resFileOrPrefix;
    if (QFileInfo(resFileOrPrefix).isDir()) {
        resConfigFile = QDir(resFileOrPrefix).filePath(FI.fileName());
    }

    QObject::connect(action, &QAction::triggered, [resConfigFile](){
        if (!QFile::exists(resConfigFile)) {
            qDebug() << "createInitConfigFromResourcesMenuAction: " << resConfigFile << "not found - check set(CMAKE_AUTORCC ON)";
            return;
        }

        // Read new content
        QFile resFile(resConfigFile);
        if (!resFile.open(QIODevice::ReadOnly)) {
            qDebug() << "createInitConfigFromResourcesMenuAction: " << resConfigFile
                     << "Cannot open"  << resFile.errorString();;
            return;
        }
        const QByteArray newContent = resFile.readAll();

        // Write New Content
        QString appConfigFile = appConfig->fileName();
        QSaveFile out(appConfigFile);
        if (!out.open(QIODevice::WriteOnly) || out.write(newContent) != newContent.size() || !out.commit()) {
            qDebug() << "createInitConfigFromResourcesMenuAction: cannot write" << appConfigFile << out.errorString();
            return;
        }

        if(appConfig->load()) {
            qDebug() << "createInitConfigFromResourcesMenuAction" << resConfigFile << "DONE";
        }
        else {
            qDebug() << "createInitConfigFromResourcesMenuAction FAILED with file:" << resConfigFile
                << "- the JSON root must be:" << appConfig->name();
        }
    });

    return action;
}


}   // namespace App
}   // namespace QtNoid
