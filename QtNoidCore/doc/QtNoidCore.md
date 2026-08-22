# QtNoid::Core
This is a simple class to control the library life cycle.
It provides static methods to access project version and build information.
## Class: Core
### Non Static Methods
* None
### Static Methods

- `static QString version()`: Returns the current version of the QtNoid library as a QString.
- `static QString buildInfo()`: Returns detailed information about the current 
  library build as a Formatted string containing:
  - Project name and version
  - Qt version used for compilation
  - Build date and time
  - Project copyright information

## CMake
```
find_package: QtNoidCore

target_link_libraries: QtNoid::QtNoidCore
```

## Header

```cpp
#include "QtNoidCore/QtNoidCore"
```

## Namespace

```cpp
using namespace QtNoid;
```

## Examples
- [**QtNoid::Core Basic Usage**](../../Examples/CoreBasicUsage/doc/CoreBasicUsage.md): use this example to understand how the library can be integrated in your project



[← Back to README](../../README.md)

