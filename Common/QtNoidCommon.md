# QtNoidCore
This is a simple class to control the library life cycle.
It provides static methods to access project version and build information.


### Header

```cpp
#include "QtNoidCore/core.h"
```

### Namespace

```cpp
namespace QtNoid
```

### Static Methods


- `static QString version()`: Returns the current version of the QtNoid library as a QString.
- `static QString buildInfo()`: Returns detailed information about the current 
  library build as a Formatted string containing:
  - Project name and version
  - Qt version used for compilation
  - Build date and time
  - Project copyright information


[← Back to Main Page](./../README.md)

