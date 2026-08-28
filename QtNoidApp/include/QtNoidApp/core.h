
#ifndef QTNOID_CORE_H
#define QTNOID_CORE_H

#include "global.h"
#include <QObject>
#include <QString>

class QMainWindow;
class QShortcut;


namespace QtNoid {
namespace App {


class QTNOIDAPP_EXPORT Core
{
public:
    static QString appExeOrAppBundleDirPath();
    static QString appExeOrAppBundleFilePath();
    static QString filePathAsAppSibling(const QString &fileName={});
    static QMainWindow *mainWindowFromWidget(QWidget &ref);
    static QMainWindow *mainWindowFromWidget(QWidget *ref);
    static QString groupNameFromObjectOrClass(const QObject &ref);
    static QString groupNameFromObjectOrClass(const QObject *ref);
    static bool updateMainWindowTitle(bool changed, QWidget *ref);
    static QImage fullDialogGrab(QWidget *ref);

private:
};


} // namespace App
} // namespace QtNoid

#endif // QTNOID_CORE_H
