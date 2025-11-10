# Class: Settings
This class contains a set of helper methods to work with the application settings


## Static Methods

- `QString appExeOrAppBundleDirPath()`: returns the folder of the application executable
file or of the Application Bundle on macOS. This is used to manage configuration files
or resource files.

- `QString appExeOrAppBundleFilePath()`: returns the path of the application executable
file or of the Application Bundle on macOS, including the exe file name or the .app
folder

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

- `QImage fullDialogGrab(QWidget *ref)`: Starting from a widget, captures the
full dialog as seen in the operating system. This is perfect for creating screenshots
for a user manuals or release notes. If you need to capture only the widget content,
just use the QWidget method `grab()`. This method captures also the caption and
the borders. The parameter 'ref' can be even a `QMainWindow` pointer, since this
class inherits QWidget. Since 2.2.0 returns a QImage, becuase of a problem with macOs.
With QImage we can apply QColorSpace() and avoid a crash saving as PNG.

**Note**: For an automated screenshot shortcut, see [Development::initFullDialogGrabShortcut](Development.md).


&nbsp;

## Non Static Methods


[⬆ Back to QtNoidApp](../QtNoidApp.md)

&nbsp;

[← Back to Main Page](../../README.md)
