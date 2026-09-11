# QtNoid2
A modular Qt library providing essential utilities and functionality to simplify the development of **Qt Widget Applications**.
The name is inspired by Spacenoids, humans living in space, in the Gundam Universe.
This is a collection of different modules that implement common practices I have re-implemented many times when creating applications.
In an effort to avoid repeating myself, I created this library.
This is **QtNoid2**, because it is the second iteration of the project, and  the first public release.

# Modules in the library
* **[QtNoid::Core](QtNoidCore/doc/QtNoidCore.md):**
  This is a simple class to control the library life cycle.  
* **[QtNoid::Common](QtNoidCommon/doc/QtNoidCommon.md)**:
  This is a collection of class to help on general tasks. 
	* [**QtNoid::Common::File**](QtNoidCommon/doc/QtnoidCommonFile.md): methods to list and manage files, rename, read and write text files. 
	* [**QtNoid::Common::Scale**](QtNoidCommon/doc/QtNoidCommonScale.md): this class convert numbers to a human readable format using suffix like kilo, Mega, and so on.
	* [**QtNoid::Common::Text**](QtNoidCommon/doc/QtNoidCommonText.md): methods to clean-up text, tokenize, convert to camel case or snake case.
* **[QtNoidApp](QtNoidApp.md)**:
    This is a collection of class to simplify the implementation of common feature of a Desktop Application. Main classes are:
    * [**Core**](QtNoidApp/doc/QtNoidAppCore.md): class to manage the application initial configuration file.
    * [**Config**](QtNoidApp/doc/QtNoidAppConfig.md): container class to manage a list of parameters with binding support and serialization.
    * [**Development**](QtNoidApp/doc/QtNoidAppDevelopment.md): API collection to simplify the application development and not intended to be customer facing.
* **[QtNoidJson](QtNoidJson/doc/QtNoidJson.md)**:
    This lib contains the class Yaml2Json that simplify the creation of simple
    JSON doc, starting from a quick textual description with a few rules.
    This is still under development.
    
* ***More modules are coming soon.***


# Quick Start
Download latest version of QtNoid2 from the Releases section on GitHub.
Unzip the content in a folder on your computer and that's it. If you point find_packages() to that folder using HINTS you can use the library in your project. 

## Integrate the library in CMakeLists.txt
If the library is not installed in a system path but on a custom folder, CMake cannot find it unless you specify the lib path in the the HINTS parameter of **find_package()** command.
Insert the path to QtNoid2 instead of `<LIB Path>` in the code example below.

```cmake
find_package(QtNoidApp REQUIRED HINTS "<LIB Path>/QtNoid2")
find_package(QtNoidCommon REQUIRED HINTS "<LIB Path>/QtNoid2")
find_package(QtNoidCore REQUIRED HINTS "<LIB Path>/QtNoid2")
find_package(QtNoidJson REQUIRED HINTS "<LIB Path>/QtNoid2")

target_link_libraries(BasicUsage PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  QtNoid::QtNoidApp
  QtNoid::QtNoidCore
  QtNoid::QtNoidCommon
  QtNoid::QtNoidJson
)
```

## Integrating the library in a console application
Include the convenient header of the library you need:
 - QtNoidCore/QtNoidCore
 - QtNoidCommon/QtNoidCommon
 - ...
 
And you can access to all class included in the library.

In this example we simply print the library version using **Core::buildInfo()**.

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


# See also
**[Examples](Examples/Examples.md)**: Sample Qt applications showing how to use each module are available in the folder.



