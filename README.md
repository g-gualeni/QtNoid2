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

&nbsp;

## Modules in the library
* **[QtNoidCore](./Core/QtNoidCore.md)**
  This is a simple class to control the library life cycle.
  
* **[QtNoidCommon](./Common/QtNoidCommon.md)**
  This is a simple class to control the library life cycle.

&nbsp;

* ***More modules are in coming soon.***

&nbsp;


## Quick Start
Download latest version of QtNoid2 from the Releases section on GitHub.
Unzip the content in a folder on your computer and that's it.

### Point the library in CMakeLists.txt
Since the library is not installed in a system path but
on a custom folder, add the HINTS parameter to the **find_package()** 
command specifing the path to QtNoid2 instead of `<LIB Path>`.
In oder to support multiple OS use a switch as in the example below:

```cmake
if(WIN32)
  find_package(QtNoidCore QtNoidCommon REQUIRED 
        HINTS "<LIB Path>/QtNoid2")
elseif(APPLE)
  find_package(QtNoidCore QtNoidCommon REQUIRED 
        HINTS "<LIB Path>/QtNoid2")
endif()

target_link_libraries(BasicUsage PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  QtNoid::QtNoidCore
  QtNoid::QtNoidCommon
)
```


### Integrating the library in a console application

In the code we can include the convenient header
of the library and that's it.

```cpp
include <QCoreApplication>
#include <QDebug>
#include <QtNoidCore/QtNoidCore>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << QtNoid::Core::buildInfo();

    return a.exec();
}
```





