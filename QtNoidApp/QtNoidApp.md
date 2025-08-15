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

- `QString appExeOrAppBundlePath()`: returns the path of the application executable
file or of the Application Bundle on macOS.

- `QString filePathAsAppSibling(const QString &fileName = {})`: calulates the 
absolute file path for fileName, as an application or bundle sibling. If fileName 
is empty, it will use the applicationName as fileName and .json as the extension.
This is helpful for creating a the configuration file in the same folder as the 
application. 

- `QWidget* mainWindowsFromWidget(QWidget *ref)`: returns the main dialog of the 
current widget. It basically scans all parents untill it finds a MainDialog widget.
You can pass also a `QMainWindow` pointer, since this class inherits
QWidget.

- `QString groupNameFromObjectOrClass(const QObject* ref)`: generates a name for a
group of settings using the object name if present, or the class name. This is 
perfect for storing settings for a pop-up dialog or even for the main dialog.

- `bool updateMainWindowTitle(bool changed, QWidget *ref)`: starting from a widget, 
it updates the main dialog title, adding an * if changed is true, or removing 
the * if false.
- `QPixmap fullDialogGrab(QWidget *ref)`: Starting from a widget, captures the 
full dialog. This is perfect for creating screenshots for a user manuals or release
notes. If you need to capture only the widget content, just use the Qt 
method `grab()`. You can pass also a `QMainWindow` pointer, since this class inherits
QWidget.


### Non Static Methods

&nbsp;

[⬆ Back to top](#qtnoidsapp)

&nbsp;


[← Back to Main Page](./../README.md)

