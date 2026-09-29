# QtNoidApp
This module contains:

- [**Core**](QtNoidAppCore.md): collection of methods to simplify the management of the application settings, or collecting app information like the app or bundle path.

- [**Parameter**](QtNoidAppParameter.md): Generic application parameter class with value storage, range validation, presets, description, tooltip and Qt property binding. This class can be used to manage a single parameter.

- [**ParametersPage**](QtNoidAppParametersPage.md): Container class for managing multiple Parameter instances with binding support and serialization. This class can be used to manage a page of parameters.

- [**Config**](QtNoidAppConfig.md): Container class for managing multiple `ParametersPage` instances with binding support and serialization. This class can be used to manage a bunch of pages of parameters.

- [**ConfigFile**](QtNoidAppConfigFile.md): Wrapper around Config with methods to support the automation of read and write of a configuration file.

- [**ConfigGlobal**](QtNoidAppConfigGlobal.md): this is a singleton class that create a unique instance of ConfigFile, available as **appConfig** that can be used to save and restore application configuration.

- [**Development**](QtNoidAppDevelopment.md): this is a collection of methods that help creating an application and are not intended to be customer facing. API like `saveConfigToProject()` that saves config file into the development project resources, or `initFullDialogGrabShortcut()` that creates a `QShortcut` (Ctrl+Shift+S) to save the application screen in the application or bundle folder.

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

## Examples Using this module

- [AppSettingsBasicUsage:](../../AppSettingsBasicUsage/doc/AppSettingsBasicUsage.md)

- **[AppParameterBasicUsage:](../../Examples/AppParameterBasicUsage/doc/QtNoidAppParameterBasicUsage.md)** use this example to visualize properties of the class [**Parameter**](QtNoidAppParameter.md) and to see how conversion to and from JSON works.

- **[AppParameterListBasicUsage:](doc/AppParameterListBasicUsage.md)**
This example showcase how a [**ParameterList**](QtNoidAppParametersPage.md) class can be use to create an
application configuration page, linking the form content to ParameterList.

- **[AppParameterListBenchmark:](doc/AppParameterListBenchmark.md)**
This example can be used to understand the cost of the creation of a Parameter
object and the cost of linking multiple objects.


[← Back to README](../../README.md)
