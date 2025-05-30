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
* **QtNoidCore**
  This is a simple class to control the library life cycle.

More modules are in coming soon.


## Quick Start

```cpp
#include <QtNoidCore/QtNoidCore>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    return app.exec();
}



