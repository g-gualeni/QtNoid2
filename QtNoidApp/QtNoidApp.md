# QtNoidApp
This module contains:
- [**Settings**](#class-settings): collection of methods to simplify the management of the application
settings.



&nbsp;

## CMake
```
find_package: QtNoidApp

target_link_libraries: QtNoid::QtNoidApp
```

&nbsp;

## Header

```cpp
#include "QtNoidApp/QtNoidApp"
```

&nbsp;

## Namespace

```cpp
using namespace QtNoid::App;
```

&nbsp;

## Class: Settings
This class contains a set of helper methods to work with the application settings


### Static Methods

- `QString appExeOrAppBundlePath()`: return the path of the application exe file 
or of the Application Bundle for macOS.
- `QString filePathAsAppSibling(const QString &fileName)`: calulate the absolute
file path for fileName, as an application or bundle sibling. This is helpful to
create the configuration file in the same folder of the application.



### Non Static Methods

&nbsp;

[⬆ Back to top](#qtnoidsapp)

&nbsp;


[← Back to Main Page](./../README.md)

