# Class: Parameter
This class is a generic application parameter class with value storage, range validation, presets, and Qt property binding. Every property is bindable and fires a signal when changed. The class content can be exported in 2 types of JSON objects:
 * **Value** that contains just the name and the value
 * **Schema** that contains all the other properties that define the object, but not the value.

A single Parameter has the following properties:
- **value**: This is a QVariant object that represents the parameter value.
- **isValid**: Read only property true when the parameter has a name, a valid value, and (if a range is configured) the value falls within range.
- **isValueChanged**: Read-only property, true when the current value differs from the reference value (initial value, last applied preset, or last value loaded from JSON).
- **unit**: This is a QString that represents the unit of measure.
- **min**, **max**, **range**: These properties can be used to enforce limitations to the range of the value. Any time a bigger or smaller value is set it is automatically clipped to the max or to the min.
- **presets**: The parameter can have a list of default values, which are stored in a QVariantMap with preset name and preset value. This can be used for quick configuration change such as for light or dark theme.
- **name**: This is a string that represents the parameter name as it is saved in a JSON object.
- **label:** this can be an alias for the name or its translation in local language. Since it is not tied to the parameter name, it is the preferred one for the user interface. 
- **description**: This is the parameter description
- **tooltip**: This is the parameter tooltip
- **readOnly**: When true, the parameter value (only the value) cannot be modified. Trying to change it fires the signal writeAttemptedWhileReadOnly().
- **visible**: use this flag to show or hide the parameter in a dialog.

## Static Methods
* There are no static methods.
## Constructors
- `Parameter(QObject *parent = nullptr)`: Creates an empty parameter with default values and no initial configuration.

- `Parameter(const QVariant& initialValue, QObject *parent = nullptr)`: Creates a parameter with a specified initial value.

- `Parameter(const QVariant& initialValue, const QString &name, QObject *parent = nullptr)`: Creates a parameter with initial value and name.

- `Parameter(const QVariant& initialValue, const QString &name, const QString &description, QObject *parent = nullptr)`: Creates a parameter with initial value, name, and description.

- `Parameter(const QJsonObject& schema, const QJsonObject& value, QObject *parent = nullptr)`: Creates a parameter by loading configuration from the JSON schema and value from the JSON value object.

## Support methods
- `uniqueId()`: Returns an integer that represents the object unique ID of the object.

## Properties management methods
* `value()`: Returns the current value of the parameter as a QVariant
- `setValue(const QVariant& val)`: Sets the parameter value, with range validation if configured.
- `bindableValue()`: Returns a bindable property for the value, enabling Qt's property binding system.

* `isValid()`: Read only property that returns true if the object meets all of the following conditions: 
	- it has a name, 
	- the QVariant value is valid, 
	- if a range is configured, the value falls within that range.
* `bindableIsValid()`: Returns a bindable property for the read only property.

* `isValueChanged()`: Returns true if the object's current value is different from the internal reference value. The internal reference value is either: the initial value of the object or the value of last preset applied or from the latest JSON object loaded.
* `bindableIsValueChanged()`: Returns a bindable property for the read only property.

- `min()`: Returns the minimum allowed value for the parameter
- `setMin(const QVariant& val)`: Sets the minimum allowed value for range validation
- `bindableMin()`: Returns a bindable property for the minimum value

- `max()`: Returns the maximum allowed value for the parameter
- `setMax(const QVariant& val)`: Sets the maximum allowed value for range validation
- `bindableMax()`: Returns a bindable property for the maximum value

- `range()`: Returns a pair containing the current min and max values.
- `setRange(const QVariant& min, const QVariant& max)`: Sets both minimum and maximum values for range validation.
- `setRange(const std::pair<QVariant, QVariant>& newRange)`: Sets the range using a pair of min/max values.
- `rangeIsValid()`: Returns true if the current range configuration is valid (min <= max).

- `presets()`: Returns the map of all available preset values  as name and value pairs.
- `setPresets(const QVariantMap& presets)`: Sets the complete map of preset values.
- `clearPresets()`: Removes all preset values from the parameter.
- `preset(const QString& name)`: Returns the value of a specific preset by name.
- `setPreset(const QString& name, const QVariant& value)`: Adds or updates a single preset.
- `setPreset(const std::pair<QString, QVariant>& preset)`: Adds or updates a single preset.
- `removePreset(const QString& name)`: Removes a specific preset by name.
- `applyPreset(const QString& name)`: Sets the parameter value to the specified preset value, if the preset name exists.
- `bindablePresets()`: Returns a bindable property for the presets map.

- `name()`: Returns the parameter name as displayed in dialogs and used in JSON serialization.
- `setName(const QString& value)`: Sets the parameter name.
- `bindableName()`: Returns a bindable property for the parameter name.

- `label()`: Returns the parameter label to be used for dialogs with multilanguage translation instead of name.
- `setLabel(const QString& value)`: Sets the parameter label.
- `bindableLabel()`: Returns a bindable property for the parameter label.

- `description()`: Returns the parameter description text.
- `setDescription(const QString& value)`: Sets the parameter description.
- `bindableDescription()`: Returns a bindable property for the description.

- `unit()`: Returns the unit of measure string for the parameter.
- `setUnit(const QString& value)`: Sets the unit of measure.
- `bindableUnit()`: Returns a bindable property for the unit.

- `tooltip()`: Returns the tooltip text shown in UI elements.
- `setTooltip(const QString& value)`: Sets the tooltip text.
- `bindableTooltip()`: Returns a bindable property for the tooltip.

- `readOnly()`: Returns true if the parameter value cannot be modified.
- `setReadOnly(bool value)`: Sets the read-only state of the parameter.
- `bindableReadOnly()`: Returns a bindable property for the read-only state.

- `visible()`: Returns true if the parameter should be visible in dialogs.
- `setVisible(bool value)`: Sets the visibility state of the parameter.
- `bindableVisible()`: Returns a bindable property for the visibility state.


## Serialization / Deserialization methods
- `toJsonValue()`: Returns a QJsonObject containing only the parameter name and current value. This is used to save the configuration in a configuration file.

- `toJsonSchema()`: Returns a QJsonObject containing all parameter properties (min, max, presets, description, etc.) except the current value. This is the definition of the object.

- `fromJson(const QJsonObject& schema, const QJsonObject& value)`: Restores the parameter from both schema and value JSON objects, reconstructing the complete parameter state.

- `valueFromJson(const QJsonObject& json)`: Loads only the parameter value from a JSON object, leaving other properties unchanged. This alse update the m_initialValue property so the isValueChanged becomes false as a result.

- `schemaFromJson(const QJsonObject& json)`: Update the configuration of current Parameter object (min, max, presets, description and so on), from the JSON schema object.


## Signals
- `valueChanged(const QVariant &newValue)`: Emitted when the parameter value changes.

- `minChanged(const QVariant &min)`: Emitted when the minimum value constraint is modified.

- `maxChanged(const QVariant &max)`: Emitted when the maximum value constraint is modified.

- `rangeChanged(const QVariant &min, const QVariant &max)`: Emitted when either minimum or maximum values change.

- `presetsChanged(const QVariantMap &presets)`: Emitted when the presets map is modified (added, removed, or cleared).

- `nameChanged(const QString &newName)`: Emitted when the parameter name is changed.

- `nameEdited(const QString &oldName, const QString &newName)`: Emitted during name editing, providing both old and new names. This simplify the rename of object collections, like the class ParameterList.

- `labelChanged(const QString &value)`: Emitted when the parameter label is changed.

- `descriptionChanged(const QString &value)`: Emitted when the parameter description is modified.

- `unitChanged(const QString &value)`: Emitted when the unit of measure is changed.

- `tooltipChanged(const QString &value)`: Emitted when the tooltip text is modified.

- `readOnlyChanged(bool value)`: Emitted when the read-only state is toggled.

- `visibleChanged(bool value)`: Emitted when the visibility state is changed.

- `writeAttemptedWhileReadOnly(const QString &parameterName)`: Emitted when an attempt is made to modify a read-only parameter.

- `isValidChanged(bool value)`: Emitted when the isValid state is changed.

- `isValueChangedChanged(bool value)`: Emitted when the value is changed hence the isValueChanged flag changes his status.

## Slots
- `setValue(const QVariant& newValue)`: Slot that can be connected to external signals to update the parameter value.



[⬆ Back to QtNoidApp](QtNoidApp.md)
[← Back to README](../../README.md)
