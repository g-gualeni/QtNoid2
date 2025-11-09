
#ifndef QTNOID_SETTINGS_H
#define QTNOID_SETTINGS_H

#include "global.h"
#include <QObject>
#include <QString>

class QMainWindow;
class QShortcut;


namespace QtNoid {
namespace App {


class QTNOIDAPP_EXPORT Settings
{
public:
    static QString appExeOrAppBundleDirPath();
    static QString appExeOrAppBundleFilePath();
    static QString filePathAsAppSibling(const QString& fileName={});
    static QMainWindow *mainWindowFromWidget(QWidget *ref);
    static QString groupNameFromObjectOrClass(const QObject* ref);
    static bool updateMainWindowTitle(bool changed, QWidget *ref);
    static QImage fullDialogGrab(QWidget *ref);
    static QShortcut *initFullDialogGrabShortcut(QWidget *parent, const QString &keySequence = "Ctrl+Shift+S",
                                                 QString destinationFileOrPath = {}, bool saveToClipboard = true);

private:
};


} // namespace App
} // namespace QtNoid

#endif // QTNOID_SETTINGS_H
