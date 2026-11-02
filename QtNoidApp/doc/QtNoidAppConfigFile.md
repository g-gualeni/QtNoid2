# Class: ConfigFile
This class extends [Config](QtNoidAppConfig.md) with the ability to load from, and save to, a JSON file on disk, so it can be used directly as a persisted application configuration. Everything a `Config` can do (pages, parameters, binding, serialization to/from `QJsonObject`) is still available; `ConfigFile` only adds the file name and the load/save operations around it.

## Static Methods
* There are no static methods

## Constructors
- `ConfigFile(QObject *parent = nullptr)`: Creates an empty configuration with no fileName set. `isValid()` returns false, and `load()`/`save()` do nothing until a fileName is set with `setFileName()`.

- `ConfigFile(const QString &fileName, QObject *parent = nullptr)`: Creates a configuration using `fileName` (or, if `fileName` is empty, `Core::filePathAsAppSibling()`), then immediately calls `load()`. It's fine if the file doesn't exist yet or can't be read - `load()` simply fails (see **Save/Load methods** below) and the configuration starts empty; the constructor doesn't check `load()`'s return value.

## Destructors
- `~ConfigFile()`: Calls `save()` before the object is torn down, so changes made during the object's lifetime are automatically persisted. If `isValid()` is false (no fileName), `save()` does nothing.

## FileName property management methods
- `fileName()`: Returns the file path used to load and save the configuration.
- `setFileName(const QString& value)`: Sets the file path. Does not load or save by itself.
- `bindableFileName()`: Returns a bindable property for fileName.

## Save/Load methods
- `isValid() const`: Returns true if fileName is not empty. Does not check that the file actually exists or is readable/writable.

- `load()`: Returns false, and emits `fileLoadError(fileName(), errorString)`, if: `isValid()` is false; the file can't be opened for reading; its content isn't valid JSON; its content is valid JSON but not a JSON object; or the content is a JSON object that doesn't match the structure `valuesFromJson()` expects. On success, the previous in-memory content is discarded (`clear()`) before loading the file's pages and parameters, and `fileLoaded(fileName())` is emitted.

- `save()`: Returns false, and emits `fileSaveError(fileName(), errorString)`, if `isValid()` is false (no fileName), or if the file can't be opened for writing (e.g. permissions, an invalid path). Otherwise (re)writes the whole file with `toJsonValues()`, as `QJsonDocument::toJson()` produces it (pretty-printed, not `QJsonDocument::Compact`). This includes the case where the configuration is currently empty: saving an empty `ConfigFile` legitimately writes an empty configuration to the file, so `save()`'s return value always matches what actually happens to the file on disk - it never silently fails while still touching the file, nor the other way around.

## Signals
- `fileNameChanged(const QString& value)`: Emitted when fileName changes.

- `fileLoaded(const QString& fileName)`: Emitted at the end of a successful `load()`.

- `fileLoadError(const QString& fileName, const QString& errorString)`: Emitted by `load()` whenever it fails, with a human-readable reason. Never emitted by `save()`.

- `fileSaveError(const QString& fileName, const QString& errorString)`: Emitted by `save()` whenever it fails, with a human-readable reason.


[⬆ Back to QtNoidApp](QtNoidApp.md)

[← Back to README](../../README.md)
