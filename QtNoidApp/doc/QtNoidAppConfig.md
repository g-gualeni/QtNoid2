# Class: Config
This class is a container for managing multiple ParametersPage instances with support for binding and serialization. It provides a comprehensive API for adding, removing, accessing, and managing ParametersPage objects, along with JSON serialization capabilities for both values and schema definitions, plus a set of convenience methods to read and write individual parameter values without going through a page object first. The class supports Qt's property binding system and provides STL-compatible iterators for efficient traversal.
A Config has the following properties:
- **name**: String identifier for the configuration.
- **label:** This can be an alias for the name or its translation in local language. Since it is not tied to the configuration name, it is the preferred one for the user interface.
- **description**: Descriptive text explaining the purpose of the configuration.
- **tooltip**: Tooltip text for UI elements.
- **isValueModified**: Read-only property, true when at least one of the contained pages currently differs from its reference value. Can also be forced via `setValueModified()`, which cascades to every contained ParametersPage.
- **isSchemaModified**: Read-only property, true when either this configuration's own properties (name, label, description, tooltip) differ from their reference schema, or at least one contained ParametersPage reports its own `isSchemaModified()` as true, or the flag was manually forced via `setSchemaModified(true)`. Unlike `isValueModified`, this never reacts to a value-only change.
- **count**: Read-only property indicating the number of pages in the configuration.

## Static Methods
* There are no static methods

## Constructors
- `Config(QObject *parent = nullptr)`: Creates an empty configuration with default values.

- `Config(const QString& name, QObject *parent = nullptr)`: Creates a configuration with a specified name.

- `Config(const QJsonObject &schemaConfig, const QJsonObject& valueConfig, QObject *parent = nullptr)`: Creates a configuration by loading from JSON schema and JSON values objects. The name is resolved once, preferring the schema's unique top-level key and falling back to the value's, then delegates to `schemaFromJson()` and `valuesFromJson()`.

## Destructors
- `~Config()`: Destroys the object, emitting `aboutToBeDestroyed(this)` first, while the object is still fully intact.

## Support methods
- `uniqueId()`: Returns an integer that represents the object unique ID of the object.

## Properties management methods
- `name()`: Returns the name of the configuration.
- `setName(const QString& value)`: Sets the configuration name.
- `bindableName()`: Returns a bindable property for the name.

- `label()`: Returns the label of the configuration.
- `setLabel(const QString& value)`: Sets the configuration label.
- `bindableLabel()`: Returns a bindable property for the label.

- `description()`: Returns the description text of the configuration.
- `setDescription(const QString& value)`: Sets the configuration description.
- `bindableDescription()`: Returns a bindable property for the description.

- `tooltip()`: Returns the tooltip text for the configuration.
- `setTooltip(const QString& value)`: Sets the tooltip text.
- `bindableTooltip()`: Returns a bindable property for the tooltip.

- `isValueModified()`: Returns true if at least one of the contained pages has at least one parameter that currently differs from its reference value.
- `bindableIsValueModified()`: Returns a bindable property for isValueModified.
- `setValueModified(bool modified)`: Cascades to every contained ParametersPage's own `setValueModified()`. With `true`, forces every page's (and so every parameter's) `isValueModified()` to report `true` regardless of whether any value actually changed. With `false`, clears every page's override and accepts each one's current state as its own new reference (equivalent to calling `setValueModified(false)` on every page right after a save): `isValueModified()` goes back to `false`, and a later `resetValueModifications()` restores to these new baselines, not to the ones captured before. Unlike `setSchemaModified()`, this has no effect of its own at the configuration level — it only forwards to the contained pages.
- `resetValueModifications()`: Cascades to every contained ParametersPage's own `resetValueModifications()` (which in turn resets every one of its Parameters), making `isValueModified()` become `false` again. Does not affect any page's or parameter's schema.

- `isSchemaModified()`: Returns true if this configuration's own properties (name, label, description, tooltip) differ from their reference schema, if at least one contained ParametersPage's `isSchemaModified()` is true, or if the flag was manually forced via `setSchemaModified(true)`.
- `bindableIsSchemaModified()`: Returns a bindable property for isSchemaModified.
- `setSchemaModified(bool modified)`: With `true`, manually forces `isSchemaModified()` to report `true` regardless of whether any tracked property actually differs from its baseline — for a change that falls outside the tracked properties but should still count as "modified" (mirrors `QTextDocument::setModified(true)`). With `false`, clears that manual override and accepts the configuration's *own* current properties as the new reference schema (same effect internally as the baseline capture done at construction/load time) — mirrors calling `QTextDocument::setModified(false)` right after a save. Note: `setSchemaModified(false)` only clears Config's own override and re-baselines Config's own properties; it does not force the aggregate to `false` if a contained page is independently reporting `isSchemaModified()`.
- `resetSchemaModifications()`: Restores this configuration's own properties to their reference schema, cascades `resetSchemaModifications()` to every contained page (which in turn cascades to every one of its Parameters), and clears any manual override set via `setSchemaModified(true)` — so `isSchemaModified()` becomes `false` again. Does not affect any page's or parameter's `value()`/`isValueModified()`.

- `count()`: Returns the number of ParametersPage objects in the configuration.
- `bindableCount()`: Returns a bindable property for count.

## Page management methods
- `append(ParametersPage *page)`: Adds an existing ParametersPage to the configuration, returns true on success.
- `append(const QJsonObject& schema, const QJsonObject& value)`: Creates and adds a ParametersPage from JSON objects, returns true on success.

- `emplace(const QString& name, const QString& description = {})`: Creates a new ParametersPage with the given properties and adds it to the configuration, returning the pointer to the created page.
- `emplace(const QJsonObject& schema, const QJsonObject& values)`: Creates a new ParametersPage from JSON objects and adds it to the configuration, returns pointer to the created page.

- `remove(ParametersPage* page)`: Removes the page specified by the pointer from the configuration. The page itself is not deleted and stays parented to the Config; it is simply no longer tracked.
- `remove(const QString& pageName)`: Removes the page with the given name from the configuration.

- `clear()`: Removes all pages from the configuration.

- `isEmpty()`: Returns true if the configuration contains no pages.

## Access methods
- `page(int index)`: Returns the ParametersPage at the specified index, or nullptr if index is invalid.
- `page(const QString& pageName)`: Returns the ParametersPage with the specified name, or nullptr if not found.

- `indexOf(ParametersPage* page)`: Returns the index of the specified page, or -1 if not found.
- `indexOf(const QString& pageName)`: Returns the index of the page with the given name, or -1 if not found.

- `contains(ParametersPage* page)`: Returns true if the configuration contains the specified page.
- `contains(const QString& pageName)`: Returns true if the configuration contains a page with the given name.

- `pages()`: Returns a QList containing all ParametersPage pointers in the configuration.

## Convenience methods for nested access
These methods read and write a single parameter's value without the caller having to look up the containing page first. Every one of them takes a `pageName` that defaults to `"Settings"`, and the `saveValue()`/`addRecentFile()`/`saveComboBoxTextItems()` family creates the page and/or the parameter on demand if it doesn't exist yet.

- `parametersCount(const QString& pageName = "Settings")`: Returns the number of parameters in the given page, or 0 if the page does not exist.
- `parameter(const QString& paramName, const QString& pageName = "Settings")`: Returns the Parameter with the given name in the given page, or nullptr if either the page or the parameter does not exist.

**Save methods:** Store a value into a parameter, creating the page or the object if not present. This is used to save an application parameter and it works also the first time when the specific parameter is not available. There are a specific method for each value type.
- `saveValue(const QString& paramName, bool value, const QString& pageName = "Settings")`: This is the method that saves a bool value.
- `saveValue(const QString& paramName, int value, const QString& pageName = "Settings")`: Same as above, for an int value.
- `saveValue(const QString& paramName, double value, const QString& pageName = "Settings")`: Same as above, for a double value.
- `saveValue(const QString& paramName, const QString& value, const QString& pageName = "Settings")`: Same as above, for a QString value.
- `saveValue(const QString& paramName, const QStringList& value, const QString& pageName = "Settings")`: Same as above, for a QStringList value.
- `saveValue(const QString& paramName, const QVariant& value, const QString& pageName = "Settings")`: Same as above, for a QVariant value.
- `saveValue(const QString& paramName, const QByteArray& value, const QString& pageName = "Settings")`: Same as above, for a QByteArray value. This is used for the Window's geometry of a UI dialog.

**Restore methods:** Recover a parameter from a specific page and value, automatically using the defaultValue if the parameter is not present.
- `restoreAsBool(const QString& paramName, bool defaultValue, const QString& pageName = "Settings")`: Returns the parameter's value as a bool.
- `restoreAsInt(const QString& paramName, int defaultValue, const QString& pageName = "Settings")`: Returns the parameter's value as an int.
- `restoreAsDouble(const QString& paramName, double defaultValue, const QString& pageName = "Settings")`: Returns the parameter's value as a double.
- `restoreAsString(const QString& paramName, const QString& defaultValue, const QString& pageName = "Settings")`: Returns the parameter's value as a QString.
- `restoreAsStringList(const QString& paramName, const QStringList& defaultValue, const QString& pageName = "Settings")`: Returns the parameter's value as a QStringList.
- `restoreAsVariant(const QString& paramName, const QVariant& defaultValue, const QString& pageName = "Settings")`: Returns the parameter's value as a QVariant.
- `restoreAsByteArray(const QString& paramName, const QByteArray defaultValue, const QString& pageName = "Settings")`: Returns the parameter's value as a QByteArray. This is used to restore the UI Windows geometry.

## RecentFiles methods
A small helper layer built on top of `saveValue()`/`restoreAsStringList()`, dedicated to maintaining a most-recently-used file list in a single QStringList parameter.

- `restoreRecentFiles(const QStringList defaultValue = {}, const QString& paramName = "RecentFiles", const QString& pageName = "Settings")`: Returns the stored recent-files list, or `defaultValue` if the page or the parameter does not exist.
- `clearRecentFiles(const QString& paramName = "RecentFiles", const QString& pageName = "Settings")`: Empties the recent-files list, if the parameter exists.
- `addRecentFile(QString filePath, int max = 10, const QString& paramName = "RecentFiles", const QString& pageName = "Settings")`: Adds `filePath` to the front of the recent-files list, creating the page and/or the parameter if needed. An existing occurrence of the same path is removed first (so it moves to the front instead of being duplicated), and the list is then truncated to at most `max` entries. Returns false if `filePath` is empty or the page could not be created.

## ComboBox methods
A small helper layer to save and restore the text items (and the selected one) of a `QComboBox`, storing them as two parameters: the items list, and the current selection under `paramName + "Current"`.

- `restoreComboBoxTextItems(QComboBox* cbo, const QString& paramName, const QStringList& defaultValue, const QString& pageName = "Settings")`: Clears `cbo` and repopulates it from the stored items (or `defaultValue` if not found), then restores the previously selected text, if any.
- `saveComboBoxTextItems(QComboBox* cbo, const QString& paramName, const QString& pageName = "Settings")`: Saves all of `cbo`'s current text items and its current selection.

## Iterator methods
- `begin()`: Returns an iterator to the beginning of the pages.
- `end()`: Returns an iterator to the end of the pages.

- `cbegin()`: Returns a const iterator to the beginning of the pages.
- `cend()`: Returns a const iterator to the end of the pages.

- `rbegin()`: Returns a reverse iterator to the beginning (end) of the pages.
- `rend()`: Returns a reverse iterator to the end (beginning) of the pages.

- `crbegin()`: Returns a const reverse iterator to the beginning (end) of the pages.
- `crend()`: Returns a const reverse iterator to the end (beginning) of the pages.

## Serialization methods
- `toJsonValues()`: Returns a QJsonObject containing, for every page, the parameter names and their current values, for configuration storage.

- `toJsonSchema()`: Returns a QJsonObject containing the configuration's own properties (label, description, tooltip) and, for every page, the schema definitions of its parameters.

- `valuesFromJson(const QJsonObject& json)`: Loads values into existing pages (or creates new ones) from a JSON object, returns true on success.

- `schemaFromJson(const QJsonObject& json)`: Updates the configuration's own properties and the pages' schema definitions from a JSON schema object, returns true on success. Note: the reference schema is only re-captured when the configuration's name was previously unset (i.e. on a first load) — calling this again on an already-named Config does **not** re-baseline, so `isSchemaModified()` keeps comparing against the original baseline. This differs from `Parameter::schemaFromJson()`/`ParametersPage::schemaFromJson()`, which always re-baseline unconditionally; it's a known inconsistency, not a documented guarantee.

## Operators
- `operator<<(ParametersPage& page)`: Stream insertion operator for adding a ParametersPage reference to the configuration.

- `operator<<(ParametersPage* page)`: Stream insertion operator for adding a ParametersPage pointer to the configuration.

## Signals
- `nameChanged(const QString& value)`: Emitted when the configuration name is changed.

- `labelChanged(const QString& value)`: Emitted when the configuration label is changed.

- `descriptionChanged(const QString& value)`: Emitted when the configuration description is modified.

- `tooltipChanged(const QString& value)`: Emitted when the configuration tooltip is changed.

* `isValueModifiedChanged(bool value)`: Emitted when the aggregate isValueModified state changes, i.e. when the configuration goes from "no page changed" to "at least one page changed", or back.

* `isSchemaModifiedChanged(bool value)`: Emitted when the aggregate isSchemaModified state changes, i.e. when the configuration's own schema, any contained page's schema, or the manual override set via `setSchemaModified()`, goes from "nothing modified" to "something modified", or back.

- `countChanged(int count)`: Emitted when the number of pages in the configuration changes.

- `pageAdded(const QtNoid::App::ParametersPage* parametersPage)`: Emitted when a page is added to the configuration.

- `pageRemoved(QtNoid::App::ParametersPage* parametersPage)`: Emitted when a page is removed from the configuration.

- `pageRenameError(const QString& oldName, const QString& newName)`: Emitted when a page rename operation fails due to name conflicts.

- `aboutToBeDestroyed(QtNoid::App::Config *config)`: Emitted from the destructor, before the object is torn down, with the object still fully intact. Useful for an owner that needs to react when a Config is destroyed: a slot connected to this signal can still safely call `config->isValueModified()`, `config->isSchemaModified()`, `config->uniqueId()`, etc. on the live pointer.


[⬆ Back to QtNoidApp](QtNoidApp.md)

[← Back to README](../../README.md)
