#include "QtNoidApp/development.h"
#include "QtNoidApp/settings.h"
#include <QApplication>
#include <QFileInfo>
#include <QShortcut>
#include <QClipboard>
#include <QDebug>
#include <QSysInfo>
#include <QWidget>
#include <QMainwindow>

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


}   // namespace App
}   // namespace QtNoid
