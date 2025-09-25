#include "QtNoidCore/core.h"
#include "QtNoidCore/QtNoidCoreVersion.h"
#include <QCoreApplication>
#include <QDebug>

namespace QtNoid {

QString Core::version()
{
    return QStringLiteral(PROJECT_VERSION);
}

QString Core::buildInfo()
{
    return QString(PROJECT_NAME " %1 - Built with Qt %2 on %3 %4 - " PROJECT_COPYRIGHT)
           .arg(version(), QT_VERSION_STR, __DATE__, __TIME__);
}

} // namespace QtNoid
