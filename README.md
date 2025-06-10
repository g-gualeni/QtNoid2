# QtNoid2
A modular Qt library providing essential utilities 
and functionality to simplify the development of **Qt 
Widget Applications**.

The name is inspired by Spacenoids, humans living in space, 
in the Gundam Universe.

This is a collection of different modules that implement common 
practices I have re-implemented many times when creating applications.

In an effort to avoid repeating myself, I created 
this library.

This is **QtNoid2**, the second iteration of the project, and  
the first public release.

&nbsp;

## Modules in the library
* **[QtNoidCore](./Core/QtNoidCore.md)**
  This is a simple class to control the library life cycle.
  
* **[QtNoidCommon](./Common/QtNoidCommon.md)**
  This is a collection of class (Text, Scale, 
  File) to help on general tasks.

&nbsp;

* ***More modules are in coming soon.***

&nbsp;


## Quick Start
Download latest version of QtNoid2 from the Releases 
section on GitHub.
Unzip the content in a folder on your computer and 
that's it. If you point find_packages() to that 
folder using HINTS you can use the library in your 
project. 

### Integrate the library in CMakeLists.txt
If the library is not installed in a system path 
but on a custom folder, CMake cannot find it unless 
you specify the lib path in the the HINTS parameter 
of **find_package()** command.
Insert the path to QtNoid2 instead of `<LIB Path>` in
the code example below.
If you are creating a portable project, use a switch
as in the example below:

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

Iinclude the convenient header of the library you need:
 - QtNoidCore/QtNoidCore
 - QtNoidCommon/QtNoidCommon
 
and so on, and you can access to all class included
in the library.

In this example we simply print the library version
using **Core::buildInfo()**.

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





