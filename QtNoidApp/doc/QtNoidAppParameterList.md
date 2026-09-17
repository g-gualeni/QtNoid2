# Class: ParameterList
This class is a container for managing multiple Parameter instances with support for binding and serialization. It provides a comprehensive API for adding, removing, accessing, and managing Parameter objects, along with JSON serialization capabilities for both values and schema definitions. The class supports Qt's property binding system and provides STL-compatible iterators for efficient traversal. 
A ParameterList has the following properties:
- **name**: String identifier for the parameter list.
- **label:** this can be an alias for the name or its translation in local language. Since it is not tied to the parameter name, it is the preferred one for the user interface.
- **description**: Descriptive text explaining the purpose of the parameter list.
- **tooltip**: Tooltip text for UI elements.
- **readOnly**: Indicates that the values in this parameter list should not be modified.  Trying to change it fires the signal writeAttemptedWhileReadOnly().
- **visible:** use this flag to show or hide the parameterList in a dialog.
- **isValueChanged**: Read-only property, true when at leas one the internal parameters differs from the reference value. The reference value can be: the initial value, last applied preset, or last value loaded from JSON.
- **count**: Read-only property indicating the number of parameters in the list.

## Static Methods
* There are no static methods

## Constructors
- `ParameterList(QObject *parent = nullptr)`: Creates an empty parameter list with default values.

- `ParameterList(const QString& name, QObject *parent = nullptr)`: Creates a parameter list with a specified name.

- `ParameterList(const QJsonObject &schemaList, const QJsonObject& valueList, QObject *parent = nullptr)`: Creates a parameter list by loading configuration from JSON schema and JSON values objects.

## Properties management methods
- `name()`: Returns the name of the parameter list.
- `setName(const QString& value)`: Sets the parameter list name.
- `bindableName()`: Returns a bindable property for the name.

* `label()`: Returns the label of the parameter list.
- `setLabel(const QString& value)`: Sets the parameter list label.
- `bindableLabel()`: Returns a bindable property for the label.

- `description()`: Returns the description text of the parameter list.
- `setDescription(const QString& value)`: Sets the parameter list description.
- `bindableDescription()`: Returns a bindable property for the description.

- `tooltip()`: Returns the tooltip text for the parameter list.
- `setTooltip(const QString& value)`: Sets the tooltip text.
- `bindableTooltip()`: Returns a bindable property for the tooltip.

- `readOnly()`: Returns whether the parameter list is marked as read-only.
- `setReadOnly(bool value)`: Sets the read-only flag.
- `bindableReadOnly()`: Returns a bindable property for readOnly.

- `visible()`: Returns whether the parameter list is visible.
- `setVisible(bool value)`: Sets the visible flag.
- `bindableVisible()`: Returns a bindable property for visible.

- `isValueChanged()`: Returns true if at least one parameter in the list currently differs from its reference value.
- `bindableIsValueChanged()`: Returns a bindable property for isValueChanged.
    
- `count()`: Returns the number of parameters in the list.
-  `bindableCount():` Returns a bindable property for count.

## List management methods
- `append(Parameter *parameter)`: Adds an existing Parameter to the list, returns true on success.
- `append(const QJsonObject& schema, const QJsonObject& value)`: Creates and adds a Parameter from JSON objects, returns true on success.

- `emplace(const QVariant& initialValue, const QString& name, const QString& description = {})`: Creates a new Parameter with the given properties and adds it to the list, returning the pointer to the created Parameter.
- `emplace(const QJsonObject& schema, const QJsonObject& value)`: Creates a new Parameter from JSON objects and adds it to the list, returns pointer to created Parameter.

- `removeParameter(Parameter* parameter)`: Removes the Parameter specified by the pointer from the list .
- `removeParameter(const QString& name)`: Removes the Parameter with the given name from the list.

- `clear()`: Removes all Parameters from the list.

- `isEmpty()`: Returns true if the list contains no Parameters.

## Access methods
- `parameter(int index)`: Returns the Parameter at the specified index, or nullptr if index is invalid.
- `parameter(const QString& name)`: Returns the Parameter with the specified name, or nullptr if not found.

- `indexOf(Parameter* parameter)`: Returns the index of the specified Parameter, or -1 if not found.
- `indexOf(const QString& name)`: Returns the index of the Parameter with the given name, or -1 if not found.

- `contains(Parameter* parameter)`: Returns true if the list contains the specified Parameter.
- `contains(const QString& name)`: Returns true if the list contains a Parameter with the given name.

- `parameters()`: Returns a QList containing all Parameter pointers in the list.

## Convenience methods
- `value(const QString& name)`: Returns the value of the Parameter with the given name, or invalid QVariant if not found.

- `setValue(const QString& name, const QVariant& value)`: Sets the value of the Parameter with the given name, returns true on success.

- `applyPreset(const QString& presetName)`: Applies the specified preset to all Parameters. where the preset name is available. Parameters without the preset name are left unchanged.

## Iterator methods
- `begin()`: Returns an iterator to the beginning of the parameter list.
- `end()`: Returns an iterator to the end of the parameter list.

- `cbegin()`: Returns a const iterator to the beginning of the parameter list.
- `cend()`: Returns a const iterator to the end of the parameter list.

- `rbegin()`: Returns a reverse iterator to the beginning (end) of the parameter list.
- `rend()`: Returns a reverse iterator to the end (beginning) of the parameter list.

- `crbegin()`: Returns a const reverse iterator to the beginning (end) of the parameter list.
- `crend()`: Returns a const reverse iterator to the end (beginning) of the parameter list.

## Serialization methods
- `toJsonValues()`: Returns a QJsonObject containing all parameter names and their current values for configuration storage.

- `toJsonSchema()`: Returns a QJsonObject containing the schema definitions of all parameters (properties, ranges, presets, etc.).

- `valuesFromJson(const QJsonObject& json)`: Loads parameter values from a JSON object, returns true on success.

- `schemaFromJson(const QJsonObject& json)`: Updates parameter configurations  from a JSON schema object, returns true on success.

## Operators
- `operator<<(Parameter& param)`: Stream insertion operator for adding a Parameter reference to the list.

- `operator<<(Parameter* param)`: Stream insertion operator for adding a Parameter pointer to the list.

## Signals
- `nameChanged(const QString& value)`: Emitted when the parameter list name is changed.
- `nameEdited(const QString &oldName, const QString &newName)`: emitted when the parameter list name is edited, passing both the old and new name.

- **labelChanged(const QString& value):** Emitted when the parameter list label is changed.

- `descriptionChanged(const QString& value)`: Emitted when the parameter list description is modified.

- `tooltipChanged(const QString& value)`: Emitted when the parameter list tooltip is changed.

* `readOnlyChanged(bool value)`: Emitted when the readOnly flag is changed.

- `visibleChanged(bool value)`: Emitted when the visible flag is changed.

* `isValueChangedChanged(bool value)`: Emitted when the aggregate isValueChanged state changes, i.e. when the list goes from "no parameter changed" to "at least one parameter changed", or back.

- `countChanged(int count)`: Emitted when the number of parameters in the list changes.

- `parameterAdded(const QtNoid::App::Parameter* parameter)`: Emitted when a parameter is added to the list.

- `parameterRemoved(QtNoid::App::Parameter* parameter)`: Emitted when a parameter is removed from the list.

- `parameterRenameError(const QString& oldName, const QString& newName)`: Emitted when a parameter rename operation fails due to name conflicts.

- `writeAttemptedWhileReadOnly(const QString &parameterName)`: Emitted when `setValue()` is called while the list is read-only; carries the name of the parameter whose modification was blocked.

[⬆ Back to QtNoidApp](QtNoidApp.md)
[← Back to README](../../README.md)
