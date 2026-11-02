# Class: ConfigGlobal
`ConfigGlobal` is a non-instantiable singleton holder: it owns a single, lazily-created [**ConfigFile**](QtNoidAppConfigFile.md) instance, available as the global **appConfig** macro defined in `configglobal.h`:

```cpp
#define appConfig (QtNoid::App::ConfigGlobal::instance())
```

The intended usage is via **appConfig**, nothing else.
The underlying `ConfigFile` is created the first time `appConfig` (or `ConfigGlobal::instance()`) is used, with its fileName defaulting to `Core::filePathAsAppSibling()` - a JSON file named after the application executable, next to it. Because it's a `ConfigFile`, it loads automatically when created and saves automatically when the application terminates (when the global instance is destroyed). Everything `Config`/`ConfigFile` offers - pages, parameters, binding, JSON round trip, `load()`/`save()`, `fileLoaded`/`fileLoadError`/`fileSaveError` - is available through `appConfig`.

## CMake
```
find_package: QtNoidApp

target_link_libraries: QtNoid::QtNoidApp
```

## Header
```cpp
#include "QtNoidApp/QtNoidApp"
```

## Static Methods
- `ConfigGlobal::instance()`: Returns the single, lazily-created `ConfigFile*` instance. Equivalent to, and more conveniently used as, the `appConfig` macro.

## Constructors
- `ConfigGlobal` can't be constructed: its default constructor, copy constructor, destructor, and assignment operator are all deleted. The class only exists to provide the static `instance()` accessor.


[⬆ Back to QtNoidApp](QtNoidApp.md)

[← Back to README](../../README.md)
