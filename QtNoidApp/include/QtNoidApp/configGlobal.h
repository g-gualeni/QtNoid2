#ifndef QTNOID_APP_CONFIGGLOBAL_H
#define QTNOID_APP_CONFIGGLOBAL_H

#include "global.h"
#include "configfile.h"

namespace QtNoid {
namespace App {

class QTNOIDAPP_EXPORT ConfigGlobal
{
public:
    static ConfigFile* instance();

private:
    ConfigGlobal() = delete;
    ~ConfigGlobal() = delete;
    ConfigGlobal(const ConfigGlobal&) = delete;
    ConfigGlobal& operator=(const ConfigGlobal&) = delete;

};

#define appConfig (QtNoid::App::ConfigGlobal::instance())

} // namespace App
} // namespace QtNoid

#endif // QTNOID_APP_CONFIGGLOBAL_H
