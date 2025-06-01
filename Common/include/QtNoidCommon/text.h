#ifndef QTNOID_TEXT_H
#define QTNOID_TEXT_H

#include "global.h"
#include <QObject>
#include <QString>

namespace QtNoid {
namespace Common {

class QTNOIDCOMMON_EXPORT Text
{
public:
    static bool isUrl(const QString &text);
};

} // namespace QtCommon
} // namespace QtNoid

#endif // QTNOID_TEXT_H
