# Class: Core
This class contains a set of helper methods to gather information from the application like the path, the windows name or the windows screenshot.
## Static Methods

- `QString appExeOrAppBundleDirPath()`: returns the **folder** of the application executable file or of the Application Bundle on macOS. This is used to manage configuration files or resource files.

- `QString appExeOrAppBundleFilePath()`: returns the **path** of the application executable file or of the Application Bundle on macOS, including the exe file name or the .app folder

- `QString filePathAsAppSibling(const QString &fileName = {})`: calculates the absolute file path for fileName, as an application or bundle sibling. If fileName is empty, it will use the applicationName as fileName and .json as the extension. Usually this method is used to create a configuration file with the same name of the application executable, with a .json extension, in the same folder as the application or of the application bundle for macOS.

- `QWidget* mainWindowsFromWidget(QWidget *ref)`: returns the main dialog of the current widget. It basically scans all parents until it finds a MainDialog widget. It works also with `QMainWindow` pointer, since this class inherits QWidget. This can be used in conjunction with `groupNameFromObjectOrClass()` to dynamically calculate a group name to be used in the configuration file for storing the configuration.

* `QWidget* mainWindowsFromWidget(QWidget &ref)`: this version of `mainWindowsFromWidget()` uses a reference instead of a pointer.

- `QString groupNameFromObjectOrClass(const QObject* ref)`: generates a name for a group of settings using the object name if present, or the class name. This is perfect for storing settings for a pop-up dialog or even for the main dialog. 

* `QString groupNameFromObjectOrClass(const QObject &ref)`: this version of `groupNameFromObjectOrClass()` uses a reference instead of a pointer

- `bool updateMainWindowTitle(bool changed, QWidget *ref)`: starting from a widget, it updates the main dialog title, adding an * character if changed parameter is true, or removing the * if it is false.

- `QImage fullDialogGrab(QWidget *ref)`: Starting from a widget, captures the full dialog as seen in the operating system. This is perfect for creating screenshots for a user manuals or release notes. If you need to capture only the widget content, just use the QWidget method `grab()`. This method captures also the caption and the borders. The parameter 'ref' can be also a `QMainWindow` pointer, since this class inherits QWidget. Since 2.2.0 returns a QImage, because it works better with macOS. With QImage we can apply QColorSpace() and avoid a crash saving as PNG.

**Note**: For an automated screenshot shortcut, see [Development::initFullDialogGrabShortcut](Development.md)




## Non Static Methods
* None

[⬆ Back to QtNoidApp](QtNoidApp.md)
[← Back to README](../../README.md)
