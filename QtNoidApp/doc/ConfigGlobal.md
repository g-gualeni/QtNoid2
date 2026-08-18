# Class: ConfigGlobal
This is a singleton class that create a unique instance of the class [**ConfigFile**](ConfigFile.md), available as a global variable called **appConfig** that can be used to save and restore automatically the application last configuration.
The configuration is stored in a file called as the application executable with suffix .json
The configuration is saved automatically when the application is terminated and it is restored when the application start.
This is the macro that map appConfig, in the configglobal.h file:

`#define appConfig (QtNoid::App::ConfigGlobal::instance())`

## CMake
```
find_package: QtNoidApp

target_link_libraries: QtNoid::QtNoidApp
```
## Header

```cpp
#include "QtNoidApp/QtNoidApp"
```


ConfigFile
- in a JSON filed with the same name of the application
- #define appConfig (QtNoid::App::ConfigGlobal::instance())

namespace QtNoid {
namespace App {


[⬆ Back to QtNoidApp](QtNoidApp.md)

&nbsp;

[← Back to README](../../README.md)
