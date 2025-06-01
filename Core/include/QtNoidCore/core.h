#ifndef QTNOID_CORE_H
#define QTNOID_CORE_H

#include "global.h"
#include <QObject>
#include <QString>

namespace QtNoid {

class QTNOIDCORE_EXPORT Core
{
public:
    static QString version();
    static QString buildInfo();
};


} // namespace QtNoid

#endif // QTNOID_CORE_H
