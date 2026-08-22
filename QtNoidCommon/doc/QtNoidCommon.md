# QtNoid::Common
This module contains:
- **[QtNoid::Common::File](File.md):** collection of methods to list and manage files, rename, read and write text files. 
- **[QtNoid::Common::Scale](Scale.md):** convert numbers to a human readable format.
- **[QtNoid::Common::Text](Text.md):** clean up, tokenize convert to camel case or snake case methods.
## CMake
```
find_package: QtNoidCommon

target_link_libraries: QtNoid::QtNoidCommon
```
## Header

```cpp
#include "QtNoidCommon/QtNoidCommon"
```
## Namespace

```cpp
using namespace QtNoid::Common;
```

## Examples
- **[QtNoid::Common Basic Usage](../../Examples/CommonBasicUsage/doc/CommonBasicUsage.md)**


[← Back to README](../../README.md)