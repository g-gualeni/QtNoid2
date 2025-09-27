
#ifndef QTNOID_SETTINGS_H
#define QTNOID_SETTINGS_H

#include "global.h"
#include <QObject>
#include <QString>

class QMainWindow;

namespace QtNoid {
namespace App {


class QTNOIDAPP_EXPORT Settings
{
public:
    static QString appExeOrAppBundlePath();
    static QString filePathAsAppSibling(const QString& fileName={});
    static QMainWindow *mainWindowFromWidget(QWidget *ref);
    static QString groupNameFromObjectOrClass(const QObject* ref);
    static bool updateMainWindowTitle(bool changed, QWidget *ref);
    static QPixmap fullDialogGrab(QWidget *ref);

private:
};


} // namespace App
} // namespace QtNoid

#endif // QTNOID_SETTINGS_H
