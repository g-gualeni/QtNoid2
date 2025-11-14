#include "QtNoidApp/development.h"
#include "QtNoidApp/settings.h"
#include "QtNoidApp/configglobal.h"
#include "QtNoidCommon/QtNoidCommon"
#include <QApplication>
#include <QFileInfo>
#include <QShortcut>
#include <QClipboard>
#include <QDebug>
#include <QSysInfo>
#include <QWidget>
#include <QMainwindow>
#include <QMenu>
#include <QDir>
#include <QCoreApplication>

namespace QtNoid {
namespace App {


QShortcut *Development::initFullDialogGrabShortcut(QWidget *parent, const QString &keySequence, QString destinationFileOrPath,
                                                bool saveToClipboard)
{
    QShortcut* shortCut = new QShortcut(QKeySequence(keySequence), parent);
    parent->connect(shortCut, &QShortcut::activated, parent, [=](){
        auto screenshot =  Settings::fullDialogGrab(parent);

        QFileInfo FI(destinationFileOrPath);
        QString fileName = destinationFileOrPath;
        // if(!FI.isFile()) {
        if (FI.suffix().isEmpty()) {
            // Gather the class / window name
            if(!fileName.isEmpty()) {
                fileName += "/";
            }
            QString os = QSysInfo::productType();
            auto mainWindow = Settings::mainWindowFromWidget(parent);
            if(mainWindow == nullptr){
                fileName +=  "Screenshot-" + os + ".png";
            }
            else {
                fileName += mainWindow->windowTitle() + "-" + os + ".png";
            }
        }
        bool res = screenshot.save(fileName);
        if(saveToClipboard) {
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setImage(screenshot);
        }
        qDebug() << "saveToClipboard:" << saveToClipboard <<
            "Destination:" << fileName << "Res:" << res;
    } );

    return shortCut;
}



QAction* Development::saveConfigToProject(QMenu *developmentMenu, const QString &projectFolder)
{
    if(developmentMenu == nullptr) {
        return {};
    }

    auto parent = developmentMenu->parent();
    if(parent == nullptr) {
        return {};
    }

    QAction *action = developmentMenu->addAction(parent->tr("Save Config To Project"));

    QFileInfo FI(appConfig->fileName());
    QString configFile = FI.fileName();
    Common::File QtNoidFile;
    auto list = QtNoidFile.listPathRecursively(projectFolder, QStringList(FI.suffix()));
    // qDebug() << __func__ << list.count();

    // Is the file present? => get the full path
    QString projectFile;
    for(auto it = list.constBegin(); it < list.constEnd(); ++it)
    {
        if(it->endsWith(configFile)) {
            projectFile = *it;
            break;
        }
    }
    if(projectFile.isEmpty()) {
        qDebug() << __func__ << "unable to find the project file";
        return {};
    }

    QDir dir(projectFolder);
    auto msg = QString(parent->tr("Copy %1 configuration file to %2")).arg(configFile, dir.relativeFilePath(projectFile));
    action->setStatusTip(msg);

    parent->connect(action, &QAction::triggered, [FI, projectFile](){
        QFile newFile(FI.absoluteFilePath());
        QFile::remove(projectFile);
        auto res = newFile.copy(projectFile);
        qDebug() << "saveConfigToProject" << projectFile << "DONE";
    });

    return action;
}

QAction* Development::initConfigFromResources(QMenu *developmentMenu, const QString &resFileOrPrefix)
{
    if(developmentMenu == nullptr) {
        return {};
    }

    auto parent = developmentMenu->parent();
    if(parent == nullptr) {
        return {};
    }

    QAction *action = developmentMenu->addAction(parent->tr("Init Config From Project"));
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
        QFile::remove(appConfigFile);
        QFile newFile(resConfigFile);
        auto res = newFile.copy(appConfigFile);
        if(!res) {
            qDebug() << "initConfigFromResources Error copying file" << resConfigFile;
            return;
        }
        QFile::setPermissions(appConfigFile, QFile::ReadOwner | QFile::WriteOwner);
        appConfig->load();
        qDebug() << "initConfigFromResources" << resConfigFile << "DONE";
    });

    return action;
}


}   // namespace App
}   // namespace QtNoid
