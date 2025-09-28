# QtNoidApp
This module contains:
- [**Settings**](#class-settings): collection of methods to simplify the management of 
the application settings.
- [**Parameter**](#class-parameter): Generic application parameter class with 
value storage, range validation, presets, and Qt property binding.
- [**ParameterList**](#class-parameterlist): Container class for managing multiple 
Parameter instances with binding support and serialization.



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

## Examples
- **AppSettingsBasicUsage:** This a rough application that showcases most of the methods
of the class App::Settings.
- **AppParameterBasicUsage:** use this example to visualize properties of the class
Parameter and to see how conversion to and from JSON works.


&nbsp;


## Class: Settings
This class contains a set of helper methods to work with the application settings


### Static Methods

- `QString appExeOrAppBundlePath()`: returns the path of the application executable
file or of the Application Bundle on macOS.

- `QString filePathAsAppSibling(const QString &fileName = {})`: calulates the 
absolute file path for fileName, as an application or bundle sibling. If fileName 
is empty, it will use the applicationName as fileName and .json as the extension.
By default this method creates a the configuration file with the same name of the 
application executable, with a .json extension, in the same folder as the 
application or of the application bundle for macOS. 

- `QWidget* mainWindowsFromWidget(QWidget *ref)`: returns the main dialog of the 
current widget. It basically scans all parents untill it finds a MainDialog widget.
It works also with `QMainWindow` pointer, since this class inherits QWidget. This
can be used in conjunction with __**groupNameFromObjectOrClass()**__ to dynamically 
calculate a group name to be used in the configuration file for storing the configuration.

- `QString groupNameFromObjectOrClass(const QObject* ref)`: generates a name for a
group of settings using the object name if present, or the class name. This is 
perfect for storing settings for a pop-up dialog or even for the main dialog.

- `bool updateMainWindowTitle(bool changed, QWidget *ref)`: starting from a widget, 
it updates the main dialog title, adding an * if changed is true, or removing 
the * if false.

- `QPixmap fullDialogGrab(QWidget *ref)`: Starting from a widget, captures the 
full dialog as seen in the operating system. This is perfect for creating screenshots 
for a user manuals or release notes. If you need to capture only the widget content, 
just use the QWidget method `grab()`. It works also a `QMainWindow` pointer, since this 
class inherits QWidget.

&nbsp;

### Non Static Methods


[⬆ Back to top](#qtnoidsapp)

&nbsp;
## Class: Parameter
This class is a generic application parameter class with value storage, range 
validation, presets, and Qt property binding.

&nbsp;

### Static Methods


&nbsp;

### Non Static Methods

[⬆ Back to top](#qtnoidsapp)

&nbsp;
## Class: ParameterList
This class is a container for managing multiple Parameter instances with support for binding and serialization.

&nbsp;

### Static Methods


&nbsp;

### Non Static Methods

[⬆ Back to top](#qtnoidsapp)


&nbsp;

[← Back to Main Page](./../README.md)

