# QtNoid2
A modular Qt library providing essential utilities 
and functionality to simplify the development of Qt 
Widget Applications.

The name is inspired by Spacenoids, human people living in space, 
in the Gundam Universe.

This is a collection of different modules that implement common 
practices I have re-implemented many times when creating applications.

Trying to not repeat myself I created this library.

This **QtNoid2** because it is the second iteration of the same project, 
and the first public version.


## Modules in the library
* **[QtNoidCore](./Core/QtNoidCore.md)**
  This is a simple class to control the library life cycle.

* ***More modules are in coming soon.***


## Quick Start

### CMakeLists.txt
If the library is not installed in a system path but
on a project library folder, add the HINTS parameter
to the find_package() command:

```cmake
if(WIN32)
  find_package(QtNoidCore REQUIRED HINTS "<LIB Path>QtNoid2")
elseif(APPLE)
  find_package(QtNoidCore REQUIRED HINTS "/Users/QtQb/GitHub/Libs/QtNoid2")
endif()

target_link_libraries(BasicUsage PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  QtNoid::QtNoidCore
)
```

### Integrating the library in the code

In the code we can include the convenient header
of the library and that's it.

```cpp
#include <QtNoidCore/QtNoidCore>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    return app.exec();
}
```





