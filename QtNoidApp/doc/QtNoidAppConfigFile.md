# Class: ConfigFile
This class extends [`Config`](QtNoidAppConfig.md) with the ability to load from, and save to, a JSON file on disk, so it can be used directly as a persisted application configuration. Everything a `Config` can do (pages, parameters, binding, serialization to/from `QJsonObject`) is still available; `ConfigFile` only adds the file name and the load/save operations around it.

## Static Methods
* There are no static methods

## Constructors
- `ConfigFile(QObject *parent = nullptr)`: Creates an empty configuration with no fileName set. 
- `ConfigFile(const QString &fileName, QObject *parent = nullptr)`: Creates a configuration using `fileName` (or, if `fileName` is empty, `Core::filePathAsAppSibling()`), then immediately calls `load()`. 

## Destructors
- `~ConfigFile()`: Calls `save()` before the object is torn down, so changes made during the object's lifetime are automatically persisted.

## Properties management methods
- `isValid()`: Returns true if fileName is not empty. Does not check that the file actually exists or is readable/writable.
- `fileName()`: Returns the file path used to load and save the configuration.
- `setFileName(const QString& value)`: Sets the file path. Does not load or save by itself.
- `bindableFileName()`: Returns a bindable property for fileName.

## Save/Load methods
- `load()`: Returns false, and emits `fileLoadError(fileName(), errorString)`, if there is an error.
- `save()`: Returns true on success, false there is an error. Saving an empty object creates an empty file.
## Signals
- `fileNameChanged(const QString& value)`: Emitted when fileName changes.
- `fileLoaded(const QString& fileName)`: Emitted at the end of a successful `load()`.
- `fileLoadError(const QString& fileName, const QString& errorString)`: Emitted by `load()` whenever it fails, with a human-readable reason.


[⬆ Back to QtNoidApp](QtNoidApp.md)

[← Back to README](../../README.md)
