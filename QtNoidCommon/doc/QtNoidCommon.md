# QtNoidCommon
This module contains:
- **[File](File.md):** collection of methods to list and manage files, rename, read and write text files. 
- **[Scale](Scale.md):** convert numbers to a human readable format.
- **[Text](Text.md):** clean up, tokenize convert to camel case or snake case methods.
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
**[CommonBasicUsage](../../Examples/CommonBasicUsage/doc/CommonBasicUsage.md)**
This simple application shows how to use: 
`QtNoid::Common::File::listPathRecursively()` 
`QtNoid::Common::File::listSubPathRecursively`


[← Back to Main Page](README.md)