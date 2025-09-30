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
validation, presets, and Qt property binding. Every property is bindable and fires
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
- **name**: This is a string that represents the parameter name as it is saved in a
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

### Constructors
- `Parameter(QObject *parent = nullptr)`: Creates an empty parameter with default 
values and no initial configuration
- `Parameter(const QVariant& initialValue, QObject *parent = nullptr)`: Creates a 
parameter with a specified initial value
- `Parameter(const QVariant& initialValue, const QString &name, 
QObject *parent = nullptr)`: Creates a parameter with initial value and name
- `Parameter(const QVariant& initialValue, const QString &name, 
const QString &description, QObject *parent = nullptr)`: Creates a parameter with 
initial value, name, and description
- `Parameter(const QJsonObject& schema, const QJsonObject& value, 
QObject *parent = nullptr)`: Creates a parameter by loading configuration from JSON 
schema and value objects

&nbsp;

### Support methods
- `uniqueId()`: Returns an int that represents the object unique ID
- `isValid()`: Returns true if the object meets all of the following conditions: 
has a name, the value is valid, and if a range is configured, the value falls
within that range.
- `isValueChanged()`: Returns true if the object's current value is different from 
the internal reference value. The reference value is either: the initial value of the 
object or a value from a preset application.

&nbsp;
### Properties management methods
- `value()`: Returns the current value of the parameter as a QVariant
- `setValue(const QVariant& val)`: Sets the parameter value, with range validation 
if configured
- `bindableValue()`: Returns a bindable property for the value, enabling Qt's 
property binding system

- `min()`: Returns the minimum allowed value for the parameter
- `setMin(const QVariant& val)`: Sets the minimum allowed value for range validation
- `bindableMin()`: Returns a bindable property for the minimum value
- `max()`: Returns the maximum allowed value for the parameter
- `setMax(const QVariant& val)`: Sets the maximum allowed value for range validation
- `bindableMax()`: Returns a bindable property for the maximum value
- `range()`: Returns a pair containing the current min and max values
- `setRange(const QVariant& min, const QVariant& max)`: Sets both minimum and maximum 
values for range validation
- `setRange(const std::pair<QVariant, QVariant>& newRange)`: Sets the range using a 
pair of min/max values
- `rangeIsValid()`: Returns true if the current range configuration is valid 
(min <= max)
- `presets()`: Returns the map of all available preset values (name -> value pairs)
- `setPresets(const QVariantMap& presets)`: Sets the complete map of preset values
- `clearPresets()`: Removes all preset values from the parameter
- `preset(const QString& name)`: Returns the value of a specific preset by name
- `setPreset(const QString& name, const QVariant& value)`: Adds or updates a single 
preset
- `removePreset(const QString& name)`: Removes a specific preset by name
- `applyPreset(const QString& name)`: Sets the parameter value to the specified 
preset value, if the preset name exists.
- `bindablePresets()`: Returns a bindable property for the presets map
- `name()`: Returns the parameter name as displayed in dialogs and used in 
JSON serialization
- `setName(const QString& value)`: Sets the parameter name
- `bindableName()`: Returns a bindable property for the parameter name
- `description()`: Returns the parameter description text
- `setDescription(const QString& value)`: Sets the parameter description
- `bindableDescription()`: Returns a bindable property for the description
- `unit()`: Returns the unit of measure string for the parameter
- `setUnit(const QString& value)`: Sets the unit of measure
- `bindableUnit()`: Returns a bindable property for the unit
- `tooltip()`: Returns the tooltip text shown in UI elements
- `setTooltip(const QString& value)`: Sets the tooltip text
- `bindableTooltip()`: Returns a bindable property for the tooltip
- `readOnly()`: Returns true if the parameter value cannot be modified
- `setReadOnly(bool value)`: Sets the read-only state of the parameter
- `bindableReadOnly()`: Returns a bindable property for the read-only state
- `visible()`: Returns true if the parameter should be visible in dialogs
- `setVisible(bool value)`: Sets the visibility state of the parameter
- `bindableVisible()`: Returns a bindable property for the visibility state


&nbsp;
### Serialization methods
- `toJsonValue()`: Returns a QJsonObject containing only the parameter name and 
current value. This is used to save the configuration in a configuration file.
- `toJsonSchema()`: Returns a QJsonObject containing all parameter properties 
(min, max, presets, description, etc.) except the current value and this is the 
definition of the object.
- `fromJson(const QJsonObject& schema, const QJsonObject& value)`: Restores the 
parameter from both schema and value JSON objects, reconstructing the complete 
parameter state
- `valueFromJson(const QJsonObject& json)`: Loads only the parameter value from a 
JSON object, leaving other properties unchanged
- `schemaFromJson(const QJsonObject& json)`: Update the configuration of current 
Parameter object, from the JSON schema object.


&nbsp;
### Signals
- `valueChanged(const QVariant &newValue)`: Emitted when the parameter value changes
- `minChanged(const QVariant &min)`: Emitted when the minimum value constraint is 
modified
- `maxChanged(const QVariant &max)`: Emitted when the maximum value constraint is 
modified
- `rangeChanged(const QVariant &min, const QVariant &max)`: Emitted when either 
minimum or maximum values change
- `presetsChanged(const QVariantMap &presets)`: Emitted when the presets map is 
modified (added, removed, or cleared)
- `nameChanged(const QString &newName)`: Emitted when the parameter name is changed
- `nameEdited(const QString &oldName, const QString &newName)`: Emitted during name 
editing, providing both old and new names
- `descriptionChanged(const QString &value)`: Emitted when the parameter description 
is modified
- `unitChanged(const QString &value)`: Emitted when the unit of measure is changed
- `tooltipChanged(const QString &value)`: Emitted when the tooltip text is modified
- `readOnlyChanged(bool value)`: Emitted when the read-only state is toggled
- `visibleChanged(bool value)`: Emitted when the visibility state is changed
- `writeAttemptedWhileReadOnly(const QString &parameterName)`: Emitted when an 
attempt is made to modify a read-only parameter

&nbsp;
### Slots
- `onValueChanged(const QVariant& newValue)`: Slot that can be connected to external 
signals to update the parameter value. It is basically a duplicate of setValue I created
for simplify tracking of the activation.

&nbsp;

[⬆ Back to top](#qtnoidapp)

&nbsp;
## Class: ParameterList
This class is a container for managing multiple Parameter instances with support for 
binding and serialization. It provides a comprehensive API for adding, removing, 
accessing, and managing Parameter objects, along with JSON serialization capabilities 
for both values and schema definitions. The class supports Qt's property
binding system and provides STL-compatible iterators for efficient traversal.

A ParameterList has the following properties:
- **name**: String identifier for the parameter list
- **description**: Descriptive text explaining the purpose of the parameter list
- **tooltip**: Tooltip text for UI elements
- **count**: Read-only property indicating the number of parameters in the list

&nbsp;

### Static Methods
There are no static methods

&nbsp;

### Constructors
- `ParameterList(QObject *parent = nullptr)`: Creates an empty parameter list with 
default values
- `ParameterList(const QString& name, QObject *parent = nullptr)`: Creates a parameter
list with a specified name
- `ParameterList(const QJsonObject &schemaList, const QJsonObject& valueList, 
QObject *parent = nullptr)`: Creates a parameter list by loading configuration from 
JSON schema and values objects

&nbsp;

### Properties management methods
- `name()`: Returns the name of the parameter list
- `setName(const QString& value)`: Sets the parameter list name
- `bindableName()`: Returns a bindable property for the name
- `description()`: Returns the description text of the parameter list
- `setDescription(const QString& value)`: Sets the parameter list description
- `bindableDescription()`: Returns a bindable property for the description
- `tooltip()`: Returns the tooltip text for the parameter list
- `setTooltip(const QString& value)`: Sets the tooltip text
- `bindableTooltip()`: Returns a bindable property for the tooltip
- `count()`: Returns the number of parameters in the list

&nbsp;

### List management methods
- `append(Parameter *parameter)`: Adds an existing Parameter to the list, returns 
true on success
- `append(const QJsonObject& schema, const QJsonObject& value)`: Creates and adds a 
Parameter from JSON objects, returns true on success
- `emplace(const QVariant& initialValue, const QString& name, 
const QString& description = {})`: Creates a new Parameter with the given properties 
and adds it to the list, returns pointer to created Parameter
- `emplace(const QJsonObject& schema, const QJsonObject& value)`: Creates a new 
Parameter from JSON objects and adds it to the list, returns pointer to created 
Parameter
- `removeParameter(Parameter* parameter)`: Removes the specified Parameter from the 
list
- `removeParameter(const QString& name)`: Removes the Parameter with the given name 
from the list
- `clear()`: Removes all Parameters from the list
- `isEmpty()`: Returns true if the list contains no Parameters

&nbsp;

### Access methods
- `parameter(int index)`: Returns the Parameter at the specified index, or nullptr 
if index is invalid
- `parameter(const QString& name)`: Returns the Parameter with the specified name, 
or nullptr if not found
- `indexOf(Parameter* parameter)`: Returns the index of the specified Parameter, 
or -1 if not found
- `indexOf(const QString& name)`: Returns the index of the Parameter with the given 
name, or -1 if not found
- `contains(Parameter* parameter)`: Returns true if the list contains the specified 
Parameter
- `contains(const QString& name)`: Returns true if the list contains a Parameter 
with the given name
- `parameters()`: Returns a QList containing all Parameter pointers in the list

&nbsp;

### Convenience methods
- `value(const QString& name)`: Returns the value of the Parameter with the given 
name, or invalid QVariant if not found
- `setValue(const QString& name, const QVariant& value)`: Sets the value of the 
Parameter with the given name, returns true on success
- `applyPreset(const QString& presetName)`: Applies the specified preset to all 
Parameters that have it defined

&nbsp;

### Iterator methods
- `begin()`: Returns an iterator to the beginning of the parameter list
- `end()`: Returns an iterator to the end of the parameter list
- `cbegin()`: Returns a const iterator to the beginning of the parameter list
- `cend()`: Returns a const iterator to the end of the parameter list
- `rbegin()`: Returns a reverse iterator to the beginning (end) of the parameter list
- `rend()`: Returns a reverse iterator to the end (beginning) of the parameter list
- `crbegin()`: Returns a const reverse iterator to the beginning (end) of the parameter list
- `crend()`: Returns a const reverse iterator to the end (beginning) of the parameter list

&nbsp;

### Serialization methods
- `toJsonValues()`: Returns a QJsonObject containing all parameter names and their 
current values for configuration storage
- `toJsonSchema()`: Returns a QJsonObject containing the schema definitions of all 
parameters (properties, ranges, presets, etc.)
- `valuesFromJson(const QJsonObject& json)`: Loads parameter values from a JSON object,
returns true on success
- `schemaFromJson(const QJsonObject& json)`: Updates parameter configurations from a 
JSON schema object, returns true on success

&nbsp;

### Operators
- `operator<<(Parameter& param)`: Stream insertion operator for adding a Parameter 
reference to the list
- `operator<<(Parameter* param)`: Stream insertion operator for adding a Parameter 
pointer to the list

&nbsp;

### Signals
- `nameChanged(const QString& value)`: Emitted when the parameter list name is changed
- `descriptionChanged(const QString& value)`: Emitted when the parameter list 
description is modified
- `tooltipChanged(const QString& value)`: Emitted when the parameter list tooltip is 
changed
- `countChanged(int count)`: Emitted when the number of parameters in the list changes
- `parameterAdded(const QtNoid::App::Parameter* parameter)`: Emitted when a parameter 
is added to the list
- `parameterRemoved(QtNoid::App::Parameter* parameter)`: Emitted when a parameter is 
removed from the list
- `parameterRenameError(const QString& oldName, const QString& newName)`: Emitted when 
a parameter rename operation fails due to name conflicts


&nbsp;


[⬆ Back to top](#qtnoidapp)


&nbsp;

[← Back to Main Page](./../README.md)



