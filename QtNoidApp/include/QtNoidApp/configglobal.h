#ifndef QTNOID_APP_CONFIGGLOBAL_H
#define QTNOID_APP_CONFIGGLOBAL_H

#include "global.h"
#include "configfile.h"

namespace QtNoid {
namespace App {

class QTNOID_APP_EXPORT ConfigGlobal
{
public:
    static ConfigFile* instance();

private:
    ConfigGlobal() = delete;
    ~ConfigGlobal() = delete;
    ConfigGlobal(const ConfigGlobal&) = delete;
    ConfigGlobal& operator=(const ConfigGlobal&) = delete;

};


} // namespace App
} // namespace QtNoid

#define appConfig (QtNoid::App::ConfigGlobal::instance())


#endif // QTNOID_APP_CONFIGGLOBAL_H
