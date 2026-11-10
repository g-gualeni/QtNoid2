# QtNoidUiCommon
This is a collection of functionality that each desktop application should use.
This Library contains:

- [**Core**](QtNoidUiCommonCore.md): collection of static methods to save and restore the content of UI widgets to and from a `QJsonObject`, for example a combo box's items and current selection.

- [**RecentFiles**](QtNoidUiCommonRecentFiles.md): `QObject` that manages the list of the most recently used files, keeps a `QMenu` in sync with it.

## CMake
```
find_package: QtNoidUiCommon

target_link_libraries: QtNoid::QtNoidUiCommon
```

## Header

```cpp
#include "QtNoidUiCommon/QtNoidUiCommon"
```

## Namespace

```cpp
using namespace QtNoid::UiCommon;
```

[← Back to README](../../README.md)
