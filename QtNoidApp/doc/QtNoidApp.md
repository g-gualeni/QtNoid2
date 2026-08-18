# QtNoidApp
This module contains:

- [**Settings**](doc/Settings.md): collection of methods to simplify the management of the application settings, or collecting app information like the app or bundle path.

- [**Parameter**](doc/Parameter.md): Generic application parameter class with value storage, range validation, presets, description, tooltip and Qt property binding. This class can be used to manage a single parameter.

- [**ParameterList**](doc/ParameterList.md): Container class for managing multiple Parameter instances with binding support and serialization. This class can be used to manage a page of parameters.

- [**Config**](doc/Config.md): Container class for managing multiple ParameterList instances with binding support and serialization. This class can be used to manage a bunch of pages of parameters.

- [**ConfigFile**](doc/ConfigFile.md): Wrapper around Config with methods to support the automation of read and write of a configuration file.

- [**ConfigGlobal**](doc/ConfigGlobal.md): this is a singleton class that create the unique instance for appConfig that can be used to save and restore application configuration.

- [**Development**](doc/Development.md): this is a collection of methods that help creating an application and are not intended to be customer facing. API like saveConfigToProject() that saves config file into the development project resources, or initFullDialogGrabShortcut() that create a shortcut to print the application screen and save it in the application or bundle folder.

## CMake
```
find_package: QtNoidApp

target_link_libraries: QtNoid::QtNoidApp
```
## Header

```cpp
#include "QtNoidApp/QtNoidApp"
```

## Namespace

```cpp
using namespace QtNoid::App;
```

## Examples

- **[AppSettingsBasicUsage:](doc/AppSettingsBasicUsage.md)**
This a rough application that showcases most of the methods of the
class [**Settings**](doc/Settings.md).

- **[AppParameterBasicUsage:](doc/AppParameterBasicUsage.md)**
use this example to visualize properties of the class
[**Parameter**](doc/Parameter.md) and to see how conversion to and from JSON works.

- **[AppParameterListBasicUsage:](doc/AppParameterListBasicUsage.md)**
This example showcase how a [**ParameterList**](doc/ParameterList.md) class can be use to create an
application configuration page, linking the form content to ParameterList.

- **[AppParameterListBenchmark:](doc/AppParameterListBenchmark.md)**
This example can be used to understand the cost of the creation of a Parameter
object and the cost of linking multiple objects.


[← Back to README](../../README.md)
