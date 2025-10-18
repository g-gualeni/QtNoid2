#include "QtNoidApp/configglobal.h"
#include <QGlobalStatic>

namespace QtNoid {
namespace App {

// Q_GLOBAL_STATIC(Config, globalConfigInstance)
Q_GLOBAL_STATIC_WITH_ARGS(ConfigFile, globalConfigInstance, (""))

ConfigFile *ConfigGlobal::instance()
{
    return globalConfigInstance;
}

} // namespace App
} // namespace QtNoid
