#ifndef QTNOID_FILE_H
#define QTNOID_FILE_H

#include "global.h"
#include <QObject>
#include <QString>

namespace QtNoid {
namespace Common {

class QTNOIDCOMMON_EXPORT File
{
public:
    static QString iniFileAsAppSibling();
    static QString appExeOrAppBundlePath(const QString &appFilePath);
private:
};

} // namespace QtCommon
} // namespace QtNoid

#endif // QTNOID_FILE_H
