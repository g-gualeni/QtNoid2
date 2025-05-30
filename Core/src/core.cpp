#include "QtNoidCore/core.h"
#include <QCoreApplication>
#include <QDebug>

namespace QtNoid {

Core::Core(QObject *parent)
    : QObject(parent)
{
}

Core::~Core()
{
}

QString Core::version()
{
    return QStringLiteral("1.0.0");
}

QString Core::buildInfo()
{
    return QString("QtNoid::Core %1 - Built with Qt %2 on %3 %4")
           .arg(version())
           .arg(QT_VERSION_STR)
           .arg(__DATE__)
           .arg(__TIME__);
}

} // namespace QtNoid
