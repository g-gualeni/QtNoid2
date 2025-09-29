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
- **[AppSettingsBasicUsage:](doc/AppSettingsBasicUsage.md)** 
This a rough application that showcases most of the methods of the 
class [**Settings**](#class-settings).

- **[AppParameterBasicUsage:](doc/AppParameterBasicUsage.md)** 
use this example to visualize properties of the class
[**Parameter**](#class-parameter) and to see how conversion to and from JSON works.

- **[AppParameterListBasicUsage:](doc/AppParameterListBasicUsage.md)** 
This example showcase how a [**ParameterList**](#class-parameterlist) class can be use to create an
application configuration page, linking the form content to ParameterList.

- **[AppParameterListBenchmark:](doc/AppParameterListBenchmark.md)** 
This example can be used to understand the cost of the creation of a Parameter 
object and the cost of linking multiple objects.


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

- `QShortcut *initFullDialogGrabShortcut(QWidget *parent, const QString &keySequence, QString destinationPath, bool saveToClipboard)`:
This is an helper method to simplify adding screenshot functionality to an application.
Initializes and returns a keyboard shortcut for capturing full dialog screenshots, with
the lambda to intercept the shortcut and save the screen to a file. 
When triggered, this shortcut captures a screenshot of the entire dialog window and 
saves it to the specified destination path. Optionally, the captured image can also be
copied to the system clipboard for immediate use. 

  Parameters:
   - **parent**: the parent widget that will own the shortcut
   - **keySequence** the keyboard combination that triggers the screenshot;
   if empty the default value "Ctrl+Shift+S" will be used.
   - **destinationPath**: file path where the screenshot will be saved. If empty
   it will be in the application execution folder. The File Name will be the 
   application window title.
   - **saveToClipboard** - If true, copies the captured image to clipboard in addition
   to saving to file.
 
  Returns: A pointer to the configured QShortcut object that handles the screenshot 
  functionality.


&nbsp;

### Non Static Methods


[⬆ Back to top](#qtnoidapp)

&nbsp;
## Class: Parameter
This class is a generic application parameter class with value storage, range 
validation, presets, and Qt property binding. Every properties is bindable and fires 
a signal when changed. The class content can be stored in 2 JSON files: 
 * Value that contains just the name and the value
 * Schema that contains all the other properties that define the object.

A single Parameter has the following
properties:
- **value**: This is a QVariant object that represents the parameter value
- **unit**: This is a QString that represents the unit of measure
- **min**, **max**, **range**: These properties can be used to enforce limitations to 
the range of the value
- **presets**: The parameter can have more than one default value, which are stored in 
a QVariantMap with preset name and value
- **name**: This is a string that represent the parameter name as it is saved in a 
JSON object or as it can be visible in a dialog
- **description**: This is the parameter description
- **tooltip**: This is the parameter tooltip
- **readOnly**: When true, the parameter value (only the value) cannot be modified
- **visible**: This is an additional property that can be used to show or hide the 
parameter in a dialog.

&nbsp;

### Static Methods
There are no static methods

&nbsp;

### Non Static Methods
- `uniqueId()`: return an int that represents the object unique ID
- `isValid()`: true if the object has a name, the value is valid, and if the range is
configured, the value is in the range.
- `isValueChanged()`: true if the object value is different from the internal 
refence value. The reference value is the inizial value of the object or a value 
from a preset application.

&nbsp;
Properties management:
- `value()`:
- `setValue(const QVariant& val)`:
- `bindableValue()`:
- `min()`:
- `setMin(const QVariant& val)`:
- `bindableMin()`:
- `max()`:
- `setMax(const QVariant& val)`:
- `bindableMax()`:
- `range()`:
- `setRange(const QVariant& min, const QVariant& max)`:
- `setRange(const std::pair<QVariant, QVariant>& newRange)`:
- `rangeIsValid()`:
- `presets()`:
- `setPresets(const QVariantMap& presets)`:
- `clearPresets()`:
- `preset(const QString& name)`:
- `setPreset(const QString& name, const QVariant& value)`:
- `removePreset(const QString& name)`:
- `applyPreset(const QString& name)`:
- `bindablePresets()`:
- `name()`:
- `setName(const QString& value)`:
- `bindableName()`:
- `description()`:
- `setDescription(const QString& value)`:
- `bindableDescription()`:
- `unit()`:
- `setUnit(const QString& value)`:
- `bindableUnit()`:
- `tooltip()`:

- `setTooltip(const QString& value)`:
- `bindableTooltip()`:


- `readOnly()`:
- `setReadOnly(bool value)`:
- `bindableReadOnly()`:

- `visible()`:
- `setVisible(bool value)`:
- `bindableVisible()`:


&nbsp;
Serialization methods:
- `toJsonValue()`:
- `toJsonSchema()`:
- `fromJson(const QJsonObject& schema, const QJsonObject& value)`:
- `valueFromJson(const QJsonObject& json)`:
- `schemaFromJson(const QJsonObject& json)`:


&nbsp;
### Signals
- `valueChanged(const QVariant &newValue)`:
- `minChanged(const QVariant &min)`:
- `maxChanged(const QVariant &max)`:
- `rangeChanged(const QVariant &min, const QVariant &max)`:
- `presetsChanged(const QVariantMap &presets)`:
- `nameChanged(const QString &newName)`:
- `nameEdited(const QString &oldName, const QString &newName)`:
- `descriptionChanged(const QString &value)`:
- `unitChanged(const QString &value)`:
- `tooltipChanged(const QString &value)`:
- `readOnlyChanged(bool value)`:
- `visibleChanged(bool value)`:
- `writeAttemptedWhileReadOnly(const QString &parameterName)`:

&nbsp;
### Slots
- `void onValueChanged(const QVariant& newValue)`:

&nbsp;

[⬆ Back to top](#qtnoidapp)

&nbsp;
## Class: ParameterList
This class is a container for managing multiple Parameter instances with support for binding and serialization.

&nbsp;

### Static Methods


&nbsp;

### Non Static Methods

[⬆ Back to top](#qtnoidapp)


&nbsp;

[← Back to Main Page](./../README.md)

