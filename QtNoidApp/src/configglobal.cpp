#include "QtNoidApp/configGlobal.h"
#include <QGlobalStatic>

namespace QtNoid {
namespace App {

Q_GLOBAL_STATIC(Config, globalConfigInstance)

Config* ConfigGlobal::instance()
{
    return globalConfigInstance;
}

} // namespace App
} // namespace QtNoid
