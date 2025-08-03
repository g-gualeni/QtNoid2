# QtNoidSettings
This module contains:
- [**Settings**](#class-settings): collection of methods to simplify the management of the application
settings.



&nbsp;

## CMake
```
find_package: QtNoidSettings

target_link_libraries: QtNoid::QtNoidSettings
```

&nbsp;

## Header

```cpp
#include "QtNoidSettings/QtNoidSettings"
```

&nbsp;

## Namespace

```cpp
namespace QtNoid::Settings
```

&nbsp;

## Class: Settings
This class contains a set of helper methods to work with the application settings


### Static Methods
- `QString File::autoNamingNextName(const QString &fileName)`: splits the file
  name in tokens, finds the last number block if present and add 1 while trying 
  to replicate the numbering scheme.This means that 1 becomes 2, 001 becomes 002
  and (1) becomes (2). The fileName can be just the file name or a full path. If
  there are no number blocks it adds ` 001` right before the first "."


### Static Methods

### Non Static Methods


[⬆ Back to top](#qtnoidsettings)

&nbsp;


[← Back to Main Page](./../README.md)

